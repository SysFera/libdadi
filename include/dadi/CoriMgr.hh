/**
 * @file   CoriMgr.hh
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  Collectors of Resources Information)
 * @section License
 *   |LICENSE|
 *
 *
 */


#ifndef _CORIMGR_HH_
#define _CORIMGR_HH_

#include <string>
#include <vector>
#include <list>
#include "dadi/ICori.hh"
#include "dadi/Attributes.hh"
#include "dadi/Registry.hh"


namespace dadi {

/**
 * @class CoriMgr
 * @brief Abstract the Cori metrics providers
 */
class CoriMgr {
public:
 /**
  * @brief default constructor
  */
  CoriMgr();
  /**
   * @brief destructor
   */
  ~CoriMgr();

  /**
   * @brief list Cori plugins available
   * @return list of plugins names
   */
  std::list<std::string>
  listPlugins();


  /**
   * @brief list metrics available
   * @return Attributes object containing all metrics availables
   */
  Attributes
  listMetrics();

  /**
   * @brief takes a Cori request and return metrics
   * @param filter Cori request
   * @return
   */
  Attributes
  getMetrics(const std::string& filter);

private:
  std::vector<ICori *> plugins; /**< ICori plugins loaded */
};

} /* namespace dadi */


#endif  /* _CORIMGR_HH_ */
