#ifndef _SINGLETON_HH_
#define _SINGLETON_HH_
/**
 * @file   Singleton.hh
 * @author hguemar <hguemar@sysfera.com>
 * @brief  defines thread-safe singleton
 *
 */

#include <boost/scoped_ptr.hpp>
#include <boost/thread/once.hpp>
#include <boost/utility.hpp>

namespace dadi {

  /**
   * @class Singleton
   * @brief template thread-safe singleton
   */
  template <typename T>
  class Singleton : private boost::noncopyable {
  public:
    /**
     * @brief get singleton instance
     * @return return reference to singleton
     */
    static T& instance() {
      boost::call_once(init, flag);
      return *instancePtr;
    }

    /**
     * @brief initialize instance
     */
    static void init() {
      instancePtr.reset(new T());
    }

  private:
    static boost::scoped_ptr<T> instancePtr; /**< singleton instance */
    static boost::once_flag flag; /**< ensure thread-safety */
  };

  template<typename T> boost::scoped_ptr<T>
  Singleton<T>::instancePtr(0);

  template<typename T>boost::once_flag
  Singleton<T>::flag = BOOST_ONCE_INIT;

} /* namespace dadi */

#endif /* _SINGLETON_HH_ */
