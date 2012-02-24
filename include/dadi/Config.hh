/**
 * @file   Config.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @author Benjamin Depardon <benjamin.depardon@sysfera.com>
 * @brief  tree-based configuration store
 * @section License
 *   |LICENSE|
 *
 */

#ifndef _CONFIG_HH_
#define _CONFIG_HH_

#include <string>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>
#include "dadi/detail/Parsers.hh"
#include "dadi/Singleton.hh"
#include "dadi/Exception/Parameters.hh"
#include "dadi/Exception/Attributes.hh"

namespace dadi {

/**
 * @class Config
 * @brief thread-safe singleton configuration store using the CRTP idiom
 */
class Config : public Singleton<Config> {
public:
  /**
   * @brief configuration is actually stored in a std::map
   */
  typedef ConfigStore::size_type size_type;
  friend class Singleton<Config>;

  /**
   * @brief load the config
   * @param inputStream the source
   * @param format the source format
   * @throw ParsingAttributeError when an error occured while reading the file
   */
  void
  load(std::istream& inputStream, Format format = FORMAT_INFO) {
    using boost::property_tree::read_json;
    using boost::property_tree::read_ini;
    using boost::property_tree::read_xml;
    using boost::property_tree::read_info;

    try {
      switch (format) {
      case FORMAT_JSON:
        read_json(inputStream, store_);
        break;
      case FORMAT_INI:
        read_ini(inputStream, store_);
        break;
      case FORMAT_XML:
        read_xml(inputStream, store_);
        break;
      case FORMAT_INFO:
      default:
        read_info(inputStream, store_);
      }
    } catch (const boost::property_tree::file_parser_error& e) {
      BOOST_THROW_EXCEPTION(ParsingAttributeError() << errinfo_msg(e.what()));
    }
  }


  /**
   * @brief save the config
   * @param output the output stream
   * @param format the source format
   * @throw ParsingAttributeError when an error occured while writing the file
   */
  void
  save(std::ostream& output, Format format = FORMAT_INFO) {
    using boost::property_tree::write_json;
    using boost::property_tree::write_ini;
    using boost::property_tree::write_xml;
    using boost::property_tree::write_info;

    try {
      switch (format) {
      case FORMAT_JSON:
        write_json(output, store_);
        break;
      case FORMAT_INI:
        write_ini(output, store_);
        break;
      case FORMAT_XML:
        write_xml(output, store_);
        break;
      case FORMAT_INFO:
      default:
        write_info(output, store_);
      }
    } catch (const boost::property_tree::file_parser_error& e) {
      BOOST_THROW_EXCEPTION(ParsingAttributeError() << errinfo_msg(e.what()));
    }
  }

  /**
   * @brief insert or update node value
   * @param key configuration key
   * @param value configuration value
   * @throw UnknownParameterError
   * @throw InvalidParameterError
   */
  template <typename valueType>
  void
  put(const std::string& key, const valueType& value) {
    boost::unique_lock<boost::shared_mutex> lock(mutex_);

    try {
      store_.template put(key, value);
    } catch (const boost::property_tree::ptree_bad_path& e) {
      BOOST_THROW_EXCEPTION(UnknownParameterError() << errinfo_msg(e.what()));
    } catch (const boost::property_tree::ptree_bad_data& e) {
      BOOST_THROW_EXCEPTION(InvalidParameterError() << errinfo_msg(e.what()));
    }
  }

  /**
   * @brief insert node value
   * @param key configuration key
   * @param value configuration value
   * @throw UnknownParameterError if a key is invalid
   * @throw InvalidParameterError if a data is invalid
   */
  template <typename valueType>
  void
  add(const std::string& key, const valueType& value) {
    boost::unique_lock<boost::shared_mutex> lock(mutex_);

    try {
      store_.template add(key, value);
    } catch (const boost::property_tree::ptree_bad_path& e) {
      BOOST_THROW_EXCEPTION(UnknownParameterError() << errinfo_msg(e.what()));
    } catch (const boost::property_tree::ptree_bad_data& e) {
      BOOST_THROW_EXCEPTION(InvalidParameterError() << errinfo_msg(e.what()));
    }
  }

  /**
   * @brief insert a node value
   * @param key configuration key
   * @param seq sequence of values to insert
   * @throw UnknownParameterError if a key is invalid
   * @throw InvalidParameterError if a data is invalid
   */
  template <class Sequence>
  void
  add_seq(const std::string& key, const Sequence& seq) {
    boost::shared_lock<boost::shared_mutex> lock(mutex_);

    try {
      BOOST_FOREACH(const typename Sequence::value_type& v, seq) {
        store_.template add(key, v);
      }
    } catch (const boost::property_tree::ptree_bad_path& e) {
      BOOST_THROW_EXCEPTION(UnknownParameterError() << errinfo_msg(e.what()));
    } catch (const boost::property_tree::ptree_bad_data& e) {
      BOOST_THROW_EXCEPTION(InvalidParameterError() << errinfo_msg(e.what()));
    }
  }


  /**
   * @brief read a node value
   * @param key configuration key
   * @return the node value
   * @throw UnknownParameterError if a key is invalid
   * @throw InvalidParameterError if a data is invalid
   */
  template <typename valueType>
  valueType
  get(const std::string& key) const {
    boost::shared_lock<boost::shared_mutex> lock(mutex_);
    valueType result;

    try {
      result = store_.get<valueType>(key);
    } catch (const boost::property_tree::ptree_bad_path& e) {
      BOOST_THROW_EXCEPTION(UnknownParameterError() << errinfo_msg(e.what()));
    } catch (const boost::property_tree::ptree_bad_data& e) {
      BOOST_THROW_EXCEPTION(InvalidParameterError() << errinfo_msg(e.what()));
    }

    return result;
  }

  /**
   * @brief read a node value
   * @param key configuration key
   * @return the node value
   * @throw UnknownParameterError if a key is invalid
   * @throw InvalidParameterError if a data is invalid
   */
  template <class Sequence>
  Sequence
  get_seq(const std::string& key) const {
    boost::shared_lock<boost::shared_mutex> lock(mutex_);
    Sequence result;

    try {
      BOOST_FOREACH(typename ConfigStore::value_type& v,
                    store_.get_child(key)) {
        result.push_back(v.second.data());
      }
    } catch (const boost::property_tree::ptree_bad_path& e) {
      BOOST_THROW_EXCEPTION(UnknownParameterError() << errinfo_msg(e.what()));
    } catch (const boost::property_tree::ptree_bad_data& e) {
      BOOST_THROW_EXCEPTION(InvalidParameterError() << errinfo_msg(e.what()));
    }

    return result;
  }

  /**
   * @brief read a node value without throwing exception
   * @param key configuration key
   * @return a boost optional object containing the node value
   */
  template <typename valueType>
  boost::optional<valueType>
  get_optional(const std::string& key) const {
    boost::shared_lock<boost::shared_mutex> lock(mutex_);
    return store_.get_optional<valueType>(key);
  }

  /**
   * @brief Get a subtree of the configuration settings
   * @param path configuration key
   * @return the subtree of the given path
   * @throw an UnknownParameterError exception if the given path is unknown
   */
  ConfigStore
  get_child(const std::string& path) const {
    boost::shared_lock<boost::shared_mutex> lock(mutex_);
    ConfigStore subConfig;

    try {
      subConfig = store_.get_child(path);
    } catch (const boost::property_tree::ptree_bad_path& e) {
      BOOST_THROW_EXCEPTION(UnknownParameterError() << errinfo_msg(e.what()));
    }

    return subConfig;
  }


  /**
   * @brief Get a subtree of the configuration settings
   * @param path configuration key
   * @return the subtree of the given path
   */
  boost::optional<ConfigStore & >
  get_child_optional(const std::string& path) const {
    boost::shared_lock<boost::shared_mutex> lock(mutex_);
    return store_.get_child_optional(path);
  }

  /**
   * @brief clear the configuration completely
   */
  void
  clear() {
    boost::unique_lock<boost::shared_mutex> lock(mutex_);
    store_.clear();
  }

private:
  mutable boost::shared_mutex mutex_; /**< R/W lock */
  mutable ConfigStore store_; /**< actual configuration store */
};
}

#endif  /* _CONFIG_HH_ */
