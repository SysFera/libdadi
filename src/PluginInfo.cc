#include "PluginInfo.hh"

PluginInfo::PluginInfo()
  : factory(NULL) {}


PluginInfo::PluginInfo(const PluginInfo& other)
  : uuid(other.uuid), name(other.name), version(other.version),
    interface(other.interface), path(other.path),
    factory(other.factory), sPtr(other.sPtr) {}

PluginInfo&
PluginInfo::operator=(const PluginInfo& other) {
  PluginInfo tmp(other);
  std::swap(tmp.uuid, this->uuid);
  std::swap(tmp.name, this->name);
  std::swap(tmp.version, this->version);
  std::swap(tmp.interface, this->interface);
  std::swap(tmp.path, this->path);
  std::swap(tmp.sPtr, this->sPtr);

  return *this;
}


PluginInfo::operator bool()
{
  return (!name.empty() &&
          !interface.empty() &&
          !path.empty() &&
          factory);
}


bool
PluginInfo::operator==(const PluginInfo& other) {
  if (this->uuid == other.uuid) {
    return true;
  } else {
    return false;
  }
}

bool
PluginInfo::operator!=(const PluginInfo& other) {
  return !(*this == other);
}

