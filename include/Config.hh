#ifndef _CONFIG_HH_
#define _CONFIG_HH_
/**
 * @file   Config.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  tree-based configuration store
 * @section Licence
 *   |LICENCE|
 *
 */
#include <string>
#include <boost/thread.hpp>
#include "detail/Parsers.hh"
#include "Singleton.hh"

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
   * @param intputStream the source
   * @param format the source format
   */
  void
  load(std::istream& inputStream, Format format = FORMAT_INFO) {
    using boost::property_tree::read_json;
    using boost::property_tree::read_ini;
    using boost::property_tree::read_xml;
    using boost::property_tree::read_info;

    switch (format) {
    case FORMAT_JSON:
      read_json(inputStream, store_);
      break;
    case FORMAT_INI:
      read_ini(inputStream, store_);
      break;
    case FORMAT_XML:
      read_xml(inputStream, store_);
    case FORMAT_INFO:
    default:
      read_info(inputStream, store_);
    }
  }


  /**
   * @brief save the config
   * @param outputStream the output stream
   * @param format the source format
   */
  void
  save(std::ostream& output, Format format = FORMAT_INFO) {
    using boost::property_tree::write_json;
    using boost::property_tree::write_ini;
    using boost::property_tree::write_xml;
    using boost::property_tree::write_info;

    switch (format) {
    case FORMAT_JSON:
      write_json(output, store_);
      break;
    case FORMAT_INI:
      write_ini(output, store_);
      break;
    case FORMAT_XML:
      write_xml(output, store_);
    case FORMAT_INFO:
    default:
      write_info(output, store_);
    }
  }

  /**
   * @brief insert or update node value
   * @param key configuration key
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
   * @brief read a node value
   * @param key configuration key
   * @return the node value
   * @throw ptree_error if a key or data is invalid
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
   * @throw a boost exception if the given path is unknown
   */
  ConfigStore
  get_child(const std::string& path) const {
    boost::shared_lock<boost::shared_mutex> lock(mutex_);
    ConfigStore subConfig;

    try {
      subConfig= store_.get_child(path);
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

#endif /* _CONFIG_HH_ */
