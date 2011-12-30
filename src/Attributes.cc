/**
 * @file   Attributes.cc
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  dadi attributes implementation
 * @section Licence
 *   |LICENCE|
 *
 */

#include <dadi/Attributes.hh>
#include <list>
#include <sstream>
#include <string>
#include <boost/format.hpp>
#include <boost/algorithm/string/erase.hpp>

namespace dadi {

Attributes::Attributes() {}

Attributes::Attributes(const Attributes& other) : pt(other.pt) {}

Attributes::Attributes(const std::string& data, int format) {
  loadAttr(data, format);
}

void
Attributes::loadAttr(const std::string& data, int format) {
  using boost::property_tree::ptree;
  std::istringstream ss(data);

  switch (format) {
  case FORMAT_JSON:
    read_json(ss, pt);
    break;
  case FORMAT_INI:
    read_ini(ss, pt);
    break;
  case FORMAT_XML:
  default:
    read_xml(ss, pt, boost::property_tree::xml_parser::trim_whitespace);
  }
}

std::string
Attributes::saveAttr(int format) const {
  std::ostringstream ss;

  switch (format) {
  case FORMAT_JSON:
    write_json(ss, pt);
    break;
  case FORMAT_INI:
    write_ini(ss, pt);
    break;
  case FORMAT_XML:
  default:
    boost::property_tree::xml_writer_settings<char> w(' ', 2);
    write_xml(ss, pt, w);
  }

  return ss.str();
}

void
Attributes::swap(Attributes& from) {
  (this->pt).swap(from.pt);
}
Attributes&
Attributes::operator=(const Attributes& other) {
  Attributes tmp(other);
  swap(tmp);
}

bool
Attributes::operator==(const Attributes& other) const {
  return ((this->pt) == (other.pt));
}

bool
Attributes::operator!=(const Attributes& other) const {
  return ((this->pt) != (other.pt));
}

namespace {
template<typename T>
/**
 * @class ptree_applier
 * @brief helper class that applies a given functor to
 * two boost::property_tree::ptree.
 * The template parameter is a functor applied to both ptree
 * (add, diff, merge etc...)
 */
class ptree_applier : public boost::noncopyable {
public:
  /**
   * @brief constructor
   * @param one first ptree
   * @param two second ptree
   */
  ptree_applier(boost::property_tree::ptree& one,
       boost::property_tree::ptree& two)
    : one_(one), two_(two) {
    method_.reset(one_);
  }

  /**
   * @brief return resulting ptree
   * @return ptree
   */
  boost::property_tree::ptree
  operator() () {
    boost::property_tree::ptree::path_type empty;
    traverse_recursive(empty, two_);
    return method_.getResult();
  }

  /**
   * @brief recursive method that applies method_
   * @param childPath
   * @param child
   */
  void
  traverse_recursive(boost::property_tree::ptree::path_type& childPath,
                     const boost::property_tree::ptree& child) {
    using boost::property_tree::ptree;

    method_(childPath, child);
    for (ptree::const_iterator it = child.begin(); it != child.end(); ++it) {
      ptree::path_type curPath = childPath / ptree::path_type(it->first);
       traverse_recursive(curPath, it->second);
    }
  }

  boost::property_tree::ptree& one_; /*< first ptree */
  boost::property_tree::ptree& two_; /*< second ptree */
  T method_; /*< functor applied to the ptree */
};

/**
 * @class ptree_merge
 * @brief helper class that merge two boost::property::ptree
 */
class ptree_merge {
public:
  /**
   * @brief default constructor
   */
  ptree_merge() {}
  /**
   * @brief constructor
   * @param pt initialize resulting ptree
   */
  ptree_merge(boost::property_tree::ptree& pt) : res_(pt) {}

  /**
   * @brief reset resulting ptree
   * @param pt initialize resulting ptree
   */
  void
  reset(boost::property_tree::ptree& pt) {
    boost::property_tree::ptree tmp(pt);
    res_.swap(tmp);
  }

  /**
   * @brief merge a node into resulting ptree
   * @param path node to merge path
   * @param child node to merge
   */
  void
  operator()(const boost::property_tree::ptree::path_type& path,
             const boost::property_tree::ptree& child) {
    using boost::property_tree::ptree;
    using boost::algorithm::erase_tail_copy;

    /* check if this is a final node */
    if (child.empty()) {
      /* if same node exists don't add it */
      std::string currentPath = path.dump();
      size_t pos = currentPath.length() - currentPath.find_last_of('.');
      std::string parentPath = erase_tail_copy(currentPath, pos);
      bool exists(false);
      // check if new parent node exists before
      boost::optional<ptree&> parent = res_.get_child_optional(parentPath);
      if (parent) {
        BOOST_FOREACH(ptree::value_type& v, parent.get()) {
          std::string p =
          boost::str(boost::format("%1%.%2%") % parentPath % v.first);
          if ((p == path.dump()) && v.second.data() == child.data()) {
            exists = true;
          }
        }
      }

      if (!exists) {
        res_.add(path, child.data());
      }
    }
  }

  /**
   * @brief get resulting ptree
   * @return ptree
   */
  boost::property_tree::ptree
  getResult() {
    return res_;
  }

private:
  boost::property_tree::ptree res_; /*< resulting ptree */
};

} /* namespace */


void
Attributes::merge(Attributes& other) {
  ptree_applier<ptree_merge> c(this->pt, other.pt);
  pt = c();
}


} /* namespace dadi */
