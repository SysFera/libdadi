#ifndef _SHARED_LIBRARY_IMPL_WIN32_HH_
#define _SHARED_LIBRARY_IMPL_WIN32_HH_
/**
 * @file   SharedLibraryImpl_win32.hh
 * @author hguemar <hguemar@sysfera.com>
 * @brief  Win32 implementation of class SharedLibrary
 * based on LoadLibrary calls
 * @section
 *   <licence>
 *
 */

#include <string>

namespace dadi {

// TODO: untested
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

#endif /* _SHARED_LIBRARY_IMPL_WIN32_HH_ */
