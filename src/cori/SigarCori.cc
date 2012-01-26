/**
 * @file   SigarCori.cc
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief
 * @section Licence
 *   |LICENCE|
 *
 */

#include <dadi/ICori.hh>
#include <bitset>
#include <sstream>
#include <string>
#include <boost/foreach.hpp>
#include <boost/scoped_ptr.hpp>
#include <sigar.h>

namespace dadi {

class SigarCori : public ICori {
protected:
  virtual void do_init();
  virtual Attributes do_listMetrics();
  virtual Attributes do_getMetrics(const std::string& filter);

private:
  void get_uptime(Attributes& pt);
  void get_mem(Attributes& pt, std::bitset<8>& mask);
  void get_swap(Attributes& pt, std::bitset<8>& mask);
  void get_cpu(Attributes& pt, std::bitset<8>& mask);
  void get_loadavg(Attributes& pt, unsigned int time);
  sigar_t *handle;
};

void
SigarCori::do_init() {
  assert(SIGAR_OK == sigar_open(&handle));
}

Attributes
SigarCori::do_listMetrics() {
  Attributes a(piPtr_->metadata);
  return a;
}

Attributes
SigarCori::do_getMetrics(const std::string& filter) {
  Attributes ft, pt;
  std::istringstream iss(filter);
  std::list<std::string> keys;
  std::bitset<8> mem_mask(0);
  std::bitset<8> swap_mask(0);
  std::bitset<8> cpu_mask(0);

  ft.loadAttr(filter);
  std::list<std::string> req =
    ft.getAttrList<std::list<std::string> >("diet.cori");
  BOOST_FOREACH(std::string &v, req)
  keys.push_back(v);

  std::list<std::string>::const_iterator it = keys.begin();
  for (; it != keys.end(); ++it) {
    if (*it == "uptime") {
      get_uptime(pt);
    }
    if (*it == "ram.total") {
      mem_mask.set(2);
    }
    if (*it == "ram.used") {
      mem_mask.set(1);
    }
    if (*it == "ram.free") {
      mem_mask.set(0);
    }
    if (*it == "swap.total") {
      swap_mask.set(2);
    }
    if (*it == "swap.used") {
      swap_mask.set(1);
    }
    if (*it == "swap.free") {
      swap_mask.set(0);
    }
    if (*it == "cpu.freq") {
      cpu_mask.set(0);
    }
  }

  get_mem(pt, mem_mask);
  get_swap(pt, swap_mask);
  get_cpu(pt, cpu_mask);
  get_loadavg(pt, 5);

  return pt;
}

void
SigarCori::get_uptime(Attributes& pt) {
  sigar_uptime_t res;
  int status = sigar_uptime_get(handle, &res);
  pt.putAttr("diet.cori.uptime", res.uptime);
}

void
SigarCori::get_mem(Attributes& pt, std::bitset<8>& mask) {
  sigar_mem_t res;
  int status = sigar_mem_get(handle, &res);
  if (mask.test(2)) {
    pt.putAttr("diet.cori.ram.total", res.total);
  }
  if (mask.test(1)) {
    pt.putAttr("diet.cori.ram.used", res.used);
  }
  if (mask.test(0)) {
    pt.putAttr("diet.cori.ram.free", res.free);
  }
}

void
SigarCori::get_swap(Attributes& pt, std::bitset<8>& mask) {
  sigar_swap_t res;
  int status = sigar_swap_get(handle, &res);
  if (mask.test(2)) {
    pt.putAttr("diet.cori.swap.total", res.total);
  }
  if (mask.test(1)) {
    pt.putAttr("diet.cori.swap.used", res.used);
  }
  if (mask.test(0)) {
    pt.putAttr("diet.cori.swap.free", res.free);
  }
}

void
SigarCori::get_cpu(Attributes& pt, std::bitset<8>& mask) {
  sigar_cpu_info_list_t cpuinfolist;
  int status = sigar_cpu_info_list_get(handle, &cpuinfolist);

  sigar_cpu_info_t res = cpuinfolist.data[0];
  pt.putAttr("diet.cori.cpu.core_number", res.total_cores * res.cores_per_socket);
  pt.putAttr("diet.cori.cpu.freq", res.mhz);
  sigar_cpu_info_list_destroy(handle, &cpuinfolist);
}

void
SigarCori::get_loadavg(Attributes& pt, unsigned int time = 1) {
  sigar_loadavg_t res;
  int status = sigar_loadavg_get(handle, &res);
  int index(0);

  switch(time) {
  case 15:
    index = 2;
    break;
  case 5:
    index = 1;
    break;
  case 1:
  default:
    index = 0;
  }
  pt.putAttr("diet.cori.loadavg", res.loadavg[index]);
}

} /* namespace dadi */

int
create_plugin_instance(void **instance) {
  try {
    *instance = new dadi::SigarCori;
  } catch (const std::bad_alloc& e) {
    return 1;
  }

  return PLUGIN_OK;
}


