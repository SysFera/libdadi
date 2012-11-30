/**
 * @file   PilgrimCori.cc
 * @brief Implementation of a Cori plugin for Pilgrim
 */

#include <bitset>
#include <sstream>
#include <string>
#include <boost/foreach.hpp>
#include <boost/scoped_ptr.hpp>
#include <sigar.h>
#include <curl/curl.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "dadi/ICori.hh"

// curl callback to get the data
size_t get_data_cb(void *buffer, size_t size, size_t nmemb, void *userp) {
  std::stringstream *pbuffer = reinterpret_cast<std::stringstream *>(userp);
  pbuffer->write((char*)buffer, size * nmemb);
  return size * nmemb;
}

// http get an url. returns http body + http response code
std::pair<std::string, int> get_url(std::string url) {
  std::stringstream buffer;
  long http_code = 0;
  CURL *hcurl;
  hcurl = curl_easy_init();
  if(hcurl) {
    curl_easy_setopt(hcurl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(hcurl, CURLOPT_WRITEFUNCTION, get_data_cb);
    curl_easy_setopt(hcurl, CURLOPT_WRITEDATA, &buffer);
    CURLcode retcode = curl_easy_perform(hcurl);
    if(retcode != CURLE_OK)
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(retcode) << std::endl;
    retcode = curl_easy_getinfo(hcurl, CURLINFO_RESPONSE_CODE, &http_code);
    if(retcode != CURLE_OK)
      std::cerr << "curl_easy_getinfo() failed: " << curl_easy_strerror(retcode) << std::endl;
    curl_easy_cleanup(hcurl);
  }
  std::pair<std::string, long> result;
  result.first = buffer.str();
  result.second = http_code;
  return result;
}

std::list<std::string> predict_transfers(std::string &url, std::list<std::string> transfers) {
  std::string request = url;
  std::string separator("?");
  BOOST_FOREACH(std::string &t, transfers) {
    request += separator + "transfer=" + t;
    separator = "&";
  }
  std::pair<std::string, long> response = get_url(request);
  std::list<std::string> results;
  if (response.second / 100 == 2) {
    boost::property_tree::ptree pt;
    std::stringstream ss(response.first);
    boost::property_tree::read_json(ss, pt);
    BOOST_FOREACH(boost::property_tree::ptree::value_type& c1, pt) {
      std::string result;
      bool first = true;
      BOOST_FOREACH(boost::property_tree::ptree::value_type& c2, c1.second) {
        if (not first) result += ",";
        first = false;
        result += c2.second.data();
      }
      results.push_back(result);
    }
  }
  return results;
}

namespace dadi {

class PilgrimCori : public ICori {
protected:
  virtual void
  do_init();
  virtual Attributes
  do_listMetrics();
  virtual Attributes
  do_getMetrics(const std::string& filter);
};

void
PilgrimCori::do_init() {
  // #TODO perhaps: ping or GET the root of the pilgrim server URL to make sure it is listening
}

Attributes
PilgrimCori::do_listMetrics() {
  Attributes a(piPtr_->metadata);
  return a;
}

Attributes
PilgrimCori::do_getMetrics(const std::string& filter) {
  Attributes ft, pt;
  ft.loadAttr(filter);
  std::list<std::string> req;
  try {
    req = ft.getAttrList<std::list<std::string> >("diet.pilgrim.metrics.metric");
  } catch (const dadi::UnknownAttributeError& e) {}
  BOOST_FOREACH(std::string &v, req) {
    if (v == "predict_transfers") {
      std::string url = piPtr_->metadata.getAttr<std::string>("predict_transfers_url");
      std::list<std::string> transfers = ft.getAttrList<std::list<std::string> >("transfers.transfer");
      std::list<std::string> results = predict_transfers(url, transfers);
      BOOST_FOREACH(std::string &r, results) {
        pt.addAttr<std::string>("transfers.transfer", r);
      }
    }
  }
  return pt;
}


} /* namespace dadi */

int
create_plugin_instance(void **instance) {
  try {
    *instance = new dadi::PilgrimCori;
  } catch (const std::bad_alloc& e) {
    return 1;
  }

  return PLUGIN_OK;
}
