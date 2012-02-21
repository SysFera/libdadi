/**
 * @file   CoriMgr.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  Collectors of Resources Information)
 * @section Licence
 *   |LICENCE|
 *
 *
 */

#include <string>
#include <vector>
#include "dadi/ICori.hh"
#include "dadi/Attributes.hh"
#include "dadi/Registry.hh"


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
