#ifndef _OPTIONS_HH_
#define _OPTIONS_HH_

/**
 * @file Options.hh
 * @brief defines a class to handle command line options.
 * @author Ibrahima Cisse (ibrahima.cisse@sysfera.com)
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @section Licence
 *   |LICENCE|
 *
 * @example simple-commandline/main.cc
 * this is a simple example on how to use Options class to handle
 * command-line options
 *
 * @example simple-configfile/main.cc
 * this is a simple example on how to use Options class to handle
 * configuration files
 *
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <boost/function.hpp>
#include <boost/program_options.hpp>

#include "Config.hh"

namespace dadi {

namespace po = boost::program_options;

/**
 * @typedef Func1
 * @brief functor:
 * - args: std::string
 * - returns: std::string
 */
typedef boost::function1<std::string, std::string> Func1;
/**
 * @typedef ProcessHook
 * @brief command-line pre-processing hook functor:
 * - args: int&, char ** (argc, argv)
 * - returns: void
 */
typedef boost::function2<void, int&, char**> ProcessHook;

/**
 * @brief multitoken helper
 * @param option
 * @return
 */
template<typename T>
po::typed_value<T>*
set_multitoken(po::typed_value<T> *option) {
  return option;
}

/**
 * @class OptionsGroup
 * @brief group of options
 */
class OptionsGroup {
public:
  /**
   * @enum GroupType
   * @brief define group visibility
   */
  typedef enum {
    CMD = 0,
    CFG,
    ENV,
  } GroupType;

  /**
   * @brief default constructor
   */
  OptionsGroup()
    : options_(new po::options_description("")),
      hidden_(false), type_(CMD) {}

  /**
   * @param desc group description
   * @param hidden visibility
   * @param type type
   * @brief constructor
   */
  explicit OptionsGroup(const std::string& desc, bool hidden, GroupType type)
    : options_(new po::options_description(desc)),
      hidden_(hidden),
      type_(type) {}

  /**
   * @param other
   * @brief copy constructor
   */
  OptionsGroup(const OptionsGroup& other)
    : options_(other.options_),
      hidden_(other.hidden_),
      type_(other.type_) {}

  /**
   * @brief destructor
   */
  ~OptionsGroup() {}

  /**
   * @brief operator=
   */
  OptionsGroup& operator=(const OptionsGroup& other);

  /**
   * @brief add new switch
   * @param name  switch name
   * @param desc  description (displayed by help)
   * @brief add a new option without argument (a.k.a switch)
   */
  void
  addSwitch(const std::string& name, const std::string& desc);

  /**
   * @class Wrapper
   * @brief wraps a boost::function0<void> as a boost::function1<void, bool>
   *      in order to fool boost::program_options
   */
  class Wrapper {
  public:
    /**
     * @brief constructor
     * @param f functor or function that return nothing and takes no argyments
     */
    explicit Wrapper(boost::function0<void>& f) : f_(f) {}

    /**
     * @brief wrapper
     */
    void
    operator()(const bool& res) {
      if (res) {
        f_();
      }
    }

  private:
    boost::function0<void>& f_;
  };

  /**
   * @brief add new option
   * @param name  option name
   * @param desc  description (as shown by help)
   * @param userFunc callback function
   * @return option
   */
  void
  addSwitch(const std::string& name,
            const std::string& desc,
            boost::function0<void>& userFunc) {
    po::typed_value<bool> *option = po::value<bool>();

    Wrapper w(userFunc);
    option->implicit_value(true)->zero_tokens()->notifier(w);
    options_->add_options()(name.c_str(), option, desc.c_str());
  }

  /**
   * @brief add a new option with argument
   * @param name  option name
   * @param desc  description (displayed by help)
   * @param value argument store
   * @param required (default: false)
   * @return option
   */
  template<class T>
  po::typed_value<T>*
  addOption(const std::string& name,
            const std::string& desc,
            T& value,
            bool required = false) {
    po::typed_value<T> *option = po::value(&value);

    if (required) {
      option->required();
    }
    set_multitoken<T>(option);
    options_->add_options()
      (name.c_str(), option, desc.c_str());

    return option;
  }

  /**
   * @brief add new option
   * @param name  option name
   * @param desc  description (as shown by help)
   * @param userFunc callback function
   * @param required (default: false)
   * @return option
   */
  template<typename T>
  po::typed_value<T>*
  addOption(const std::string& name,
            const std::string& desc,
            boost::function1<void, T>& userFunc,
            bool required = false) {
    po::typed_value<T> *option = po::value<T>();

    if (required) {
      option->required();
    }
    option->notifier(userFunc);
    set_multitoken(option);
    options_->add_options()
      (name.c_str(), option, desc.c_str());
    return option;
  }


  /**
   * @brief get access to the underlying object
   * @return boost::shared_ptr<po::description>
   */
  boost::shared_ptr<po::options_description>
  get() {
    return options_;
  }

  /**
   * @brief print group into a std::ostream
   */
  friend std::ostream&
  operator<<(std::ostream& os, const OptionsGroup& opt);

  friend class Options;

private:
  boost::shared_ptr<po::options_description> options_;
  bool hidden_;
  GroupType type_;
};


/**
 * @class Options
 * @brief process environment, configuration file and command line options
 */
class Options {
public:
  /**
   * @brief default constructor
   * use default store
   */
  Options() : store_(Config::instance()) {}

  /**
   * @param store configuration store
   * @brief constructor
   */
  explicit Options(Config& store) : store_(store) {}

  /**
   * @brief destructor
   */
  ~Options() {}

  /**
   * @brief add options group
   * @param group options group
   */
  void addGroup(OptionsGroup& group);

  /**
   * @brief add new switch
   * @param name  switch name
   * @param desc  description (displayed by help)
   * @brief add a new option without argument (a.k.a switch)
   */
  void
  addSwitch(const std::string& name, const std::string& desc);

  /**
   * @brief add new option
   * @param name  option name
   * @param desc  description (as shown by help)
   * @param userFunc callback function
   */
  void
  addSwitch(const std::string& name,
            const std::string& desc,
            boost::function0<void>& userFunc) {
    default_.addSwitch(name, desc, userFunc);
  }

  /**
   * @brief add a new option with argument
   * @param name  option name
   * @param desc  description (displayed by help)
   * @param value argument store
   * @param required (default: false)
   * @return option
   */
  template<class T>
  po::typed_value<T>*
  addOption(const std::string& name,
            const std::string& desc,
            T& value, bool required = false) {
    return default_.addOption(name, desc, required);
  }


  /**
   * @brief add new option
   * @param name  option name
   * @param desc  description (as shown by help)
   * @param userFunc callback function
   * @param required (default: false)
   * @return option
   */
  template<typename T>
  po::typed_value<T>*
  addOption(const std::string& name,
            const std::string& desc,
            boost::function1<void, T>& userFunc,
            bool required = false) {
    return default_.addOption(name, desc, userFunc, required);
  }

  /**
   * @brief sets a previously defined option as positional
   * @param key option key
   * @param count maximum number of arguments (-1: unlimited)
   */
  void setPositional(const std::string& key, int count=-1);

  /**
   * @brief parse command line
   * @param argc
   * @param argv
   */
  void parseCommandLine(int& argc, char *argv[]);

  /**
   * @brief parse configuration file
   * @param file configuration file path
   * @param unregistered allow unregistered option
   */
  void parseConfigFile(const std::string& file, bool unregistered = false);

  /**
   * @brief parse environment variables
   * @param userFunc callback
   */
  void parseEnv(const Func1& userFunc);

  /**
   * @brief call user-defined callbacks
   */
  void notify();

  /**
   * @brief count options with key
   * @param key
   */
  int count(const std::string& key) const;

  /**
   * @brief check if we have any option or not
   */
  bool empty() const { return vm_.empty(); }

  /**
   * @brief get the value associated to a key
   */
  template<class T>
  const T & get(const std::string& key) const {
    return(vm_[key.c_str()].template as<T>());
  }

  /**
   * @brief set program name
   * @param name program name
   */
  void setName(const std::string& name);

  /**
   * @brief set usage
   * @param usage set usage line displayed by help
   */
  void setUsage(const std::string& usage);

  /**
   * @brief set a command parsing pre hook
   * @param PreCmdHook
   */
  void setPreCommandHook(const ProcessHook& PreCmdHook);

  /**
   * @brief print options into a std::ostream
   */
  friend std::ostream&
  operator<<(std::ostream& os, const Options& opt);

private:
  /**
   * @brief get managed groups by type
   * @param visibility
   * @return boost::shared_ptr<po::optionsgroup>
   */
  boost::shared_ptr<po::options_description>
  groups(OptionsGroup::GroupType type);

  ProcessHook PreCmdHook_;
  std::string prog_;
  std::string usage_;
  mutable po::variables_map vm_;
  OptionsGroup default_;
  std::vector<OptionsGroup> groups_;
  po::positional_options_description pos_;
  Config& store_;
};


/**
 * @brief default help handler
 * @param opt Options object
 */
void
help(const dadi::Options& opt);


/**
 * @brief simple helper that sets ConfigStore[key] = value
 * @param key property key
 * @param value value
 */
template<typename T>
void
setProperty(std::string key, const T& value) {
  dadi::Config& store_ = dadi::Config::instance();
  store_[key] = value;
#ifdef NDEBUG
  std::cerr << key << ": " << value << "\n";
#endif
}

/**
 * @brief A helper function to simplify the display of vector
 * @param os: an ostream to write data in
 * @param v: a vector to print
 */
template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
  std::copy(v.begin(), v.end(), std::ostream_iterator<T>(os, " "));
  return os;
}


} /* namespace dadi */



// alias to commonly used specialization
/** alias to setProperty std::string specialization */
#define setPropertyString setProperty<std::string >
/** alias to setProperty std::vector<std::string> specialization */
#define setPropertyStringList setProperty<std::vector<std::string> >

#endif /* _OPTIONS_HH_ */
