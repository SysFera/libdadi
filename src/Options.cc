/**
 * @file   Options.cc
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  options parsing implementation
 *
 */

#include "Options.hh"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <typeinfo>

#include <boost/format.hpp>
#include <boost/foreach.hpp>
namespace dadi {

#define foreach BOOST_FOREACH
template<>
po::typed_value<std::vector<std::string> >*
set_multitoken(po::typed_value<std::vector<std::string> > *option) {
  return option->multitoken();
}

/* class OptionsGroup implementation */
OptionsGroup&
OptionsGroup::operator=(const OptionsGroup& other) {
  // avoids auto-affectation
  if (this != &other) {
    OptionsGroup temp(other);
    std::swap(temp.options_, this->options_);
  }

  return *this;
}

void
OptionsGroup::addSwitch(const std::string& name, const std::string& desc) {
  options_->add_options()(name.c_str(), desc.c_str());
}

std::ostream&
operator<< (std::ostream& os, const OptionsGroup& opt) {
  if (!opt.hidden_) {
    os << *(opt.options_);
  }
  return os;
}

/* class Options implementation */
void
Options::addGroup(OptionsGroup& group) {
  groups_.push_back(group);
}

void
Options::setPositional(const std::string& name, int count) {
  pos_.add(name.c_str(), count);
}

void
Options::parseCommandLine(int& argc, char* argv[]) {
  if (!PreCmdHook_.empty()) {
    PreCmdHook_(argc, argv);
  }

  boost::shared_ptr<po::options_description> options =
    groups(OptionsGroup::CMD);

  try {
    store(po::command_line_parser(argc, argv)
          .options(*options).positional(pos_)
          .run(), vm_);
  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    boost::shared_ptr<po::options_description> opt = default_.get();
    const po::option_description *help = opt->find_nothrow("help", true);
    if (help) {
      help->semantic()->notify(true);
    }
  }
}

void
Options::parseConfigFile(const std::string& file, bool unregistered) {
  boost::shared_ptr<po::options_description> options =
    groups(OptionsGroup::CFG);

  std::ifstream ifs(file.c_str());

  po::parsed_options parsed_file(parse_config_file(ifs,
                                                   *options,
                                                   unregistered));

  // store the registred options
  store(parsed_file, vm_);

  dadi::Config& store_ = dadi::Config::instance();
  // store the unregistred options
  foreach(po::option unregoption, parsed_file.options) {
    if (unregoption.unregistered) {
      if (unregoption.value.size() == 1) {
        dadi::setProperty(unregoption.string_key, unregoption.value.back());
      } else {
        dadi::setProperty(unregoption.string_key, unregoption.value);
      }
    }
  }

  // store(parse_config_file(ifs, *options, unregistered), vm_);
}

void
Options::parseEnv(const Func1& name_mapper) {
  boost::shared_ptr<po::options_description> options =
    groups(OptionsGroup::ENV);
  store(parse_environment(*options, name_mapper), vm_);
}

void
Options::notify() {
  int size = vm_.size();

  for (po::variables_map::iterator it = vm_.begin(); it != vm_.end();
       ++it) {
    if (it->second.defaulted()) {
      size--;
    }
  }

  try {
    po::notify(vm_);
  } catch (const po::required_option& e) {
    /* this is quite hairy but safe
       instead of just printing help message, we execute our callback
       which allows user to customize the output */
    if (vm_.count("help") || (0 == size)) {
      boost::shared_ptr<po::options_description> opt = default_.get();
      const po::option_description *help = opt->find_nothrow("help", true);
      if (help) {
        help->semantic()->notify(true);
      }
    }
  }
}

int
Options::count(const std::string& key) const {
  return (vm_.count(key));
}

void
Options::setName(const std::string& name) {
  prog_ = name;
}

void
Options::setUsage(const std::string& usage) {
  usage_ = usage;
}

void
Options::setPreCommandHook(const ProcessHook& PreCmdHook) {
  PreCmdHook_ = PreCmdHook;
}


boost::shared_ptr<po::options_description>
Options::groups(OptionsGroup::GroupType type) {
  boost::shared_ptr<po::options_description> options(
    new po::options_description);

  if (type == default_.type_) {
    options->add(*(default_.get()));
  }

  std::vector<OptionsGroup>::iterator it = groups_.begin();
  for (; it != groups_.end(); ++it) {
    if (type == it->type_) {
      options->add(*(it->get()));
    }
  }
  return options;
}


std::ostream&
operator<< (std::ostream& os, const Options& opt) {
  os << boost::format("Usage: \n%1% %2%\n") % opt.prog_ % opt.usage_
     << "\n"
     << opt.default_;

  std::ostream_iterator<OptionsGroup> it(os, "\n");
  std::copy(opt.groups_.begin(), opt.groups_.end(), it);

  return os;
}

/* Display help message and exit. */
void
help(const dadi::Options& opt) {
  std::cout << opt;
  exit(EXIT_SUCCESS);
}


} /* namespace dadi */
