#ifndef _SHARED_LIBRARY_IMPL_POSIX_HH_
#define _SHARED_LIBRARY_IMPL_POSIX_HH_
/**
 * @file   SharedLibraryImpl_posix.hh
 * @author hguemar <hguemar@sysfera.com>
 * @brief  Posix implementation of class SharedLibrary
 * based on dlopen/dlclose/dlsym calls
 * @section
 *   <licence>
 *
 */

#include <string>

namespace dadi {

class SharedLibraryImpl {
public:
  SharedLibraryImpl();
  explicit SharedLibraryImpl(const std::string& path);
  ~SharedLibraryImpl();

  void loadImpl(const std::string& path);
  void unloadImpl();

  bool isLoadedImpl() const;
  bool hasSymbolImpl(const std::string& symbol);
  void *symbolImpl(const std::string& symbol);

  const std::string& pathImpl() const;

private:
  std::string path_;
  void *handle_;
};

} /* namespace dadi */

#endif /* _SHARED_LIBRARY_IMPL_POSIX_HH_ */

