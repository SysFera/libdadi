/**
 * @file   PurgeStrategy.cc
 * @author Haïkel Guémar <haikel.guemar@sysfera.com>
 * @brief  defines log files purge strategies
 * @section License
 *   |LICENSE|
 *
 */

#include "dadi/Logging/PurgeStrategy.hh"
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

namespace dadi {

namespace bfs = boost::filesystem;

boost::format PurgeStrategy::regexTpl("");

PurgeStrategy::PurgeStrategy() {}

PurgeStrategy::~PurgeStrategy() {}

void
PurgeStrategy::list(const std::string& basename,
                    std::vector<std::string>& paths) {
  const bfs::path& logFile = bfs::absolute(basename);
  const bfs::path& logDir = logFile.parent_path();
  const bfs::directory_iterator dir(logDir);
  bfs::directory_iterator it(logDir), end;

  regexTpl.clear();
  regexTpl % logFile.native();
  boost::regex reg(regexTpl.str(), boost::regex::perl);

  for (; it != end; ++it) {
    const bfs::path& p = it->path();
    const std::string& path = p.string();
    if (boost::regex_match(path, reg)) {
      paths.push_back(path);
    }
  }
}

void
PurgeStrategy::sort(std::vector<std::string>& paths) {}

/*****************************************************************************/
PurgeByCountStrategy::PurgeByCountStrategy(unsigned int count)
  : count_(count) {}

PurgeByCountStrategy::~PurgeByCountStrategy() {}

// functor: sort files by age from the newest to the oldest
class SortFilesByAge {
public:
  bool
  operator()(const std::string& path1, const std::string& path2) {
    std::time_t t1(bfs::last_write_time(path1));
    std::time_t t2(bfs::last_write_time(path2));
    return (t1 < t2);
  }
};

void
PurgeByCountStrategy::sort(std::vector<std::string>& paths) {
  std::sort(paths.begin(), paths.end(), SortFilesByAge());
}

void
PurgeByCountStrategy::purge(const std::string& path) {
  std::vector<std::string> archives;
  list(path, archives);
  // no archives to delete, return early
  if (archives.size() < count_) {
    return;
  }

  sort(archives);
  std::vector<std::string>::const_iterator it = archives.begin();
  it += count_;
  for (; it != archives.end(); ++it) {
    bfs::remove(*it);
  }
}

} /* namespace dadi */
