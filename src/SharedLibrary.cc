#include "SharedLibrary.hh"
#if defined(WIN32)
#include "SharedLibraryImpl_win32.hh"
#else /* WIN32 */
#include "SharedLibraryImpl_posix.hh"
#endif

SharedLibrary::SharedLibrary() {}

SharedLibrary::SharedLibrary(const std::string& path)
  : pimpl(new SharedLibraryImpl(path)) {}

SharedLibrary::~SharedLibrary() {}

void
SharedLibrary::load(const std::string& path) {
  pimpl->loadImpl(path);
}

void
SharedLibrary::unload() {
  pimpl->unloadImpl();
}

bool
SharedLibrary::isLoaded() const {
  return pimpl->isLoadedImpl();
}

bool
SharedLibrary::hasSymbol(const std::string& symbol) {
  return pimpl->hasSymbolImpl(symbol);
}


void *
SharedLibrary::symbol(const std::string& symbol) {
  return pimpl->symbolImpl(symbol);
}

const std::string&
SharedLibrary::path() const {
 return pimpl->pathImpl();
}

std::string
SharedLibrary::prefix() {
#ifdef __WIN32__
  return "";
#else /* __WIN32__ */
  return "lib";
#endif /* __WIN32__ */
}

std::string
SharedLibrary::suffix() {
#ifdef __WIN32__
  return ".dll";
#else /* __WIN32__ */
  #ifdef __APPLE__
    return ".dylib";
  #else /* __APPLE__ */
    return ".so";
  #endif /* __APPLE__ */
#endif /* __WIN32__ */
}

