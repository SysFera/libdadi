/**
 * @file   Attributes.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  Dadi attributes implementation
 * @section Licence
 *   |LICENCE|
 *
 */

#ifndef _ATTRIBUTES_HH_
#define _ATTRIBUTES_HH_

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

  /**
 * @brief count
 */


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
   * @brief get the list of values associated to path and store it
   * in a sequence
   * @param path path to attribute
   * @throw dadi::UnknownAttributeError
   * @throw dadi::InvalidAttributeError
   */
  template<class Sequence>
  Sequence
  getAttrList(const std::string& path) const {
    Sequence seq;

    try {
      BOOST_FOREACH(const boost::property_tree::ptree::value_type& v,
                    pt.get_child(path)) {
        seq.push_back(v.second.data());
      }
    } catch (const boost::property_tree::ptree_bad_path& e) {
      BOOST_THROW_EXCEPTION(UnknownAttributeError() << errinfo_msg(e.what()));
    } catch (const boost::property_tree::ptree_bad_data& e) {
      BOOST_THROW_EXCEPTION(InvalidAttributeError() << errinfo_msg(e.what()));
    }

    return seq;
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
   * @brief update a node to attributes (or create it if it doesn't exist)
   * @param path path to attribute
   * @param value attribute new value
   */
  template<typename T> void
  putAttr(const std::string& path, T value) {
    pt.put(path, value);
  }

  /**
   * @brief add a new node to attributes
   * @param path path to attribute
   * @param value attribute new value
   */
  template<typename T> void
  addAttr(const std::string& path, T value) {
    pt.add(path, value);
  }

  /**
   * @brief deserialize attributes
   * @param[in] data serialized attributes
   * @param format XML by default
   */
  void
  loadAttr(const std::string& data, int format = FORMAT_XML);

  /**
   * @brief serialize attributes
   * @param format XML by default
   * @return serialized attribute
   */
  std::string
  saveAttr(int format = 0) const;

  /**
   * @brief swap  attributes
   * @param from another attribute
   *
   */
  void
  swap(Attributes& from);

  /* operators */
  /**
   * @brief copy operator
   * @param other copied object
   * @return new copied object
   */
  Attributes&
  operator=(const Attributes& other);
  /**
   * @brief comparison operator
   * @param other object object to compare
   * @return boolean
   */
  bool
  operator==(const Attributes& other) const;
  /**
   * @brief comparison operator
   * @param other object to compare
   * @return boolean
   */
  bool
  operator!=(const Attributes& other) const;
  /**
   * @brief merge 2 sets of attributes
   * @param other object to merge
   * @return
   */
  void
  merge(Attributes& other);

private:
  boost::property_tree::ptree pt; /**< property tree holding attributes */
};

} /* namespace dadi */

#endif /* _ATTRIBUTES_HH_ */
