/**
 * @file   CoriMgr.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  Collectors of Resources Information)
 * @section Licence
 *   |LICENCE|
 *
 *
 */

#include <dadi/Attributes.hh>
#include <dadi/Registry.hh>

#include <list>
#include <string>
#include <dadi/ICori.hh>

namespace dadi {

class CoriMgr {
public:
  CoriMgr();
  ~CoriMgr();

  std::list<std::string>
  listPlugins();

  Attributes
  listMetrics();

  Attributes
  getMetrics(const std::string& filter);

private:
  std::vector<ICori *> plugins;
};

} /* namespace dadi */
