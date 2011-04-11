#ifndef _SINGLETON_HH_
#define _SINGLETON_HH_

#include <boost/scoped_ptr.hpp>
#include <boost/thread/once.hpp>
#include <boost/utility.hpp>

namespace diet {

  // thread-safe singleton
  template <typename T>
  class Singleton : private boost::noncopyable {
  public:
    static T& instance() {
      boost::call_once(init, flag);
      return *instancePtr;
    }

    static void init() {
      instancePtr.reset(new T());
    }

  protected:
    Singleton() {};
    ~Singleton() {};

  private:
    static boost::scoped_ptr<T> instancePtr;
    static boost::once_flag flag;
  };

  template<typename T> boost::scoped_ptr<T>
  Singleton<T>::instancePtr(0);

  template<typename T>boost::once_flag
  Singleton<T>::flag = BOOST_ONCE_INIT;
}

#endif /* _SINGLETON_HH_ */
