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
    read_xml(ss, pt);
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
    write_xml(ss, pt);
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


} /* namespace dadi */
