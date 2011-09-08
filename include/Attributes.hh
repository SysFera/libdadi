#ifndef _ATTRIBUTES_HH_
#define _ATTRIBUTES_HH_
/**
 * @file   Attributes.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  Dadi attributes implementation
 * @section Licence
 *   |LICENCE|
 *
 */

#include <list>
#include <string>
#include <boost/any.hpp>
#include <boost/foreach.hpp>
#include "Exception/Attributes.hh"
#include "detail/Parsers.hh"

namespace dadi {

/**
 * @class Attributes
 * @brief base class with attributes
 *
 * stupid class that wraps a boost::property_tree::ptree
 * and (de)serialize it in xml format (easy to parse)
 */
class Attributes {
public:
  /**
   * @brief default constructor
   */
  Attributes();
  /**
   * @brief copy constructor
   */
  Attributes(const Attributes& other);
  /**
   * @brief constructor from serialized data
   * @param data serialized data
   * @param format XML by default
   */
  Attributes(const std::string& data, int format = FORMAT_XML);

  /* accessors */
  /**
   * @brief get value associated to path
   * @param path attribute path
   * @return expected value
   * @throw dadi::UnknownAttributeError
   * @throw dadi::InvalidAttributeError
   */
  template<typename T> T
  getAttr(const std::string& path) const {
    try {
      return pt.get<T>(path);
    } catch (const boost::property_tree::ptree_bad_path& e) {
      BOOST_THROW_EXCEPTION(UnknownAttributeError() << errinfo_msg(e.what()));
    } catch (const boost::property_tree::ptree_bad_data& e) {
      BOOST_THROW_EXCEPTION(InvalidAttributeError() << errinfo_msg(e.what()));
    }
  }

  /**
   * @brief get attributes list and store it in standard container
   * @param path attribute path
   * @param container container path list of values
   * @throw dadi::UnknownAttributeError
   * @throw dadi::InvalidAttributeError
   */
  template<typename T, template <typename Element> class Container >
  void
  getAttrList(const std::string& path, Container<T>& container) const {
    container.clear();

    try {
      BOOST_FOREACH(boost::property_tree::ptree::value_type& v,
                    pt.get_child(path)) {
        container.push_back(v.second.get_value<T>());
      }
    } catch (const boost::property_tree::ptree_bad_path& e) {
      BOOST_THROW_EXCEPTION(UnknownAttributeError() << errinfo_msg(e.what()));
    } catch (const boost::property_tree::ptree_bad_data& e) {
      BOOST_THROW_EXCEPTION(InvalidAttributeError() << errinfo_msg(e.what()));
    }
  }


  /**
   * @brief get the list of values associated to path and store it in a std::list
   * @param path path to attribute
   * @param container container path list of values
   * @throw dadi::UnknownAttributeError
   * @throw dadi::InvalidAttributeError
   */
  template<typename T>
  std::list<T>
  getAttrList(const std::string& path) const {
      std::list<T> container;
      getAttrList(path, container);
      return container;
  }

  /**
   * @brief get value associated to path or send default value
   *        does not throw any exception
   * @param path path to attribute
   * @param default_value default value to return
   * @return expected value or default
   */
  template<typename T> T
  getAttr(const std::string& path, T default_value) const {
    return pt.get(path, default_value);
  }

  /* modifiers */
  /**
   * @brief add value to attributes
   * @param path path to attribute
   * @param value attribute new value
   */
  template<typename T> void
  putAttr(const std::string& path, T value) {
    pt.put(path, value);
  }

  /**
   * @brief deserialize attributes
   * @param[in] data serialized attributes
   * @param format XML by default
   */
  void loadAttr(const std::string& data, int format = FORMAT_XML);

  /**
   * @brief serialize attributes
   * @param format XML by default
   * @return serialized attribute
   */
  std::string saveAttr(int format = 0) const;

  /* operators */
  /**
   * @brief copy operator
   * @param other copied object
   * @return new copied object
   */
  Attributes& operator=(const Attributes& other);
  /**
   * @brief comparison operator
   * @param other object object to compare
   * @return boolean
   */
  bool operator==(const Attributes& other) const;
  /**
   * @brief comparison operator
   * @param other object object to compare
   * @return boolean
   */
  bool operator!=(const Attributes& other) const;

private:
  boost::property_tree::ptree pt; /**< property tree holding attributes */
};

} /* namespace dadi */

#endif /* _ATTRIBUTES_HH_ */
