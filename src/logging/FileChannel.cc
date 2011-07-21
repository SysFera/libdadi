#include <Logging/FileChannel.hh>
#include <boost/assign/list_of.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>
#include <boost/thread/locks.hpp>
#include <Logging/Message.hh>

namespace dadi {

namespace io = boost::iostreams;
typedef boost::lock_guard<boost::mutex> Lock;

const std::string FileChannel::ATTR_PATH = "path";
const std::string FileChannel::ATTR_COMPRESSION_MODE = "compression_mode";
const std::string FileChannel::ATTR_ARCHIVE = "archive";
const std::string FileChannel::ATTR_ROTATE = "rotate";
const std::string FileChannel::ATTR_ROTATE_SIZE = "rotate.size";
const std::string FileChannel::ATTR_ROTATE_TIME = "rotate.time";
const std::string FileChannel::ATTR_ROTATE_INTERVAL = "rotate.interval";
const long DEFAULT_ROT_SIZE(1024*1024);
const std::string DEFAULT_ROT_INTERVAL("24:00:00");
const boost::regex FileChannel::regex1(
  "\\s*" // should be trimmed but safer
  "(?(?=.*,.*)" // conditional base on lookahead assertion
  "([0-6]|sunday|monday|tuesday|wednesday|thursday|friday|saturday),)"
  "((?:\\d{2}:?){1,3})", // hh[:mm[:ss[:]]] beware side-effect last ":"
  boost::regex::perl);

std::map<std::string, int> FileChannel::attrMap =
  boost::assign::map_list_of("", 0)
  ("none", 0)
  ("bzip2", FileChannel::COMP_BZIP2)
  ("bz2", FileChannel::COMP_BZIP2)
  ("gzip", FileChannel::COMP_GZIP)
  ("gz", FileChannel::COMP_GZIP)
  ("zlib", FileChannel::COMP_ZLIB)
  ("z", FileChannel::COMP_ZLIB)
  ("number", FileChannel::AR_NUMBER)
  ("timestamp", FileChannel::AR_TIMESTAMP)
  ("size", FileChannel::ROT_SIZE)
  ("interval", FileChannel::ROT_INTERVAL)
  ("time", FileChannel::ROT_TIME)
  ("utc", 0)
  ("local", 1);

FileChannel::FileChannel() : cMode_(0), aMode_(0) {}

FileChannel::FileChannel(const std::string& path)
  : path_(path), cMode_(0), aMode_(0) {}

FileChannel::~FileChannel() {}

void
FileChannel::open() {
  Lock lock(mutex_);

  if (path_.empty()) {
    try {
      path_ = getAttr<std::string>(FileChannel::ATTR_PATH);
    } catch (...) {
      throw std::string("missing path name");
    }
  }

  if (!out_) {
    cMode_ = attrMap[
      getAttr<std::string>(FileChannel::ATTR_COMPRESSION_MODE, "")];
    switch(cMode_) {
    case FileChannel::COMP_ZLIB:
      out_.push(io::zlib_compressor());
      break;
    case FileChannel::COMP_GZIP:
      out_.push(io::gzip_compressor());
      break;
    case FileChannel::COMP_BZIP2:
      out_.push(io::bzip2_compressor());
      break;
    case FileChannel::COMP_NONE:
    default:
      break;
    }

    out_.push(io::file_sink(path_));
  }

  setArchiveStrategy();
  setRotateStrategy();
}

void
FileChannel::close() {}

void
FileChannel::log(const Message& msg) {
  open();
  /* since mutex_ is not a recursive one, we wait that open()
     ends before locking it */
  Lock lock(mutex_);

  // FIXME: compressors are not flushable-friendly
  out_ << msg.getText() << std::endl;

  if (pRotateStrategy_ && pArchiveStrategy_ &&
      pRotateStrategy_->mustRotate(path_)) {
    out_.pop();
    pArchiveStrategy_->archive(path_);
    out_.push(io::file_sink(path_));
    purge();
  }

}

long
FileChannel::getLastWriteTime() const {
  if (boost::filesystem::exists(path_)) {
    std::time_t last_write = boost::filesystem::last_write_time(path_);
    return last_write;
  } else {
    return -1;
  }
}

long
FileChannel::getSize() const {
  if (boost::filesystem::exists(path_)) {
    return boost::filesystem::file_size(path_);
  } else {
    return 0;
  }
}

const std::string&
FileChannel::getPath() const {
  return path_;
}

void
FileChannel::setArchiveStrategy() {
  if (!pArchiveStrategy_) {
    aMode_ = attrMap[getAttr<std::string>(FileChannel::ATTR_ARCHIVE, "")];
    if (FileChannel::AR_NUMBER == aMode_) {
      pArchiveStrategy_.reset(new ArchiveByNumberStrategy);
    }
    if (FileChannel::AR_TIMESTAMP == aMode_) {
      pArchiveStrategy_.reset(new ArchiveByTimestampStrategy);
    }
  }
}

class Weekday {
public:
  unsigned int operator() (const std::string& str) {
    std::map<std::string, unsigned int>::iterator it = dMap.find(str);
    if (dMap.end() != it) {
      return it->second;
    } else {
      return dMap[""];
    }
  }

private:
  static std::map<std::string, unsigned int> dMap;
};

std::map<std::string, unsigned int> Weekday::dMap =
  boost::assign::map_list_of("", 0)
  ("0", 0)("1", 1)("2", 2)("3", 3)("4", 4)("5", 5)("6", 6)
  ("sunday", 0)
  ("monday", 1)
  ("tuesday", 2)
  ("wednesday", 3)
  ("friday", 4)
  ("monday", 5)
  ("sunday", 6);

void
FileChannel::setRotateStrategy() {
  using boost::posix_time::time_duration;
  using boost::posix_time::duration_from_string;

  if (!pRotateStrategy_) {
    rMode_ = attrMap[getAttr<std::string>(FileChannel::ATTR_ROTATE, "")];
    if (FileChannel::ROT_SIZE == rMode_) {
      // by default: 1Mo
      long sz = getAttr<long>(FileChannel::ATTR_ROTATE_SIZE, DEFAULT_ROT_SIZE);
      pRotateStrategy_.reset(new RotateBySizeStrategy(sz));
    }
    if (FileChannel::ROT_INTERVAL == rMode_) {
      // by default: 1 day
      std::string s(getAttr<std::string>(FileChannel::ATTR_ROTATE_INTERVAL,
                                  DEFAULT_ROT_INTERVAL));
      // FIXME: we should catch an exception when user sets a badly formatted
      // duration
      time_duration td(duration_from_string(s));
      pRotateStrategy_.reset(new RotateByIntervalStrategy(td));
    }
    if (FileChannel::ROT_TIME == rMode_) {
      std::string s(getAttr<std::string>(FileChannel::ATTR_ROTATE_INTERVAL, ""));
      boost::smatch res;
      if (boost::regex_match(s, res, regex1)) {
        std::cout << "nb: " << res.size() << "\n";
        std::cout << res[1] << " | " << res[2] << "\n";
        bool utc = getAttr<bool>(FileChannel::ATTR_ROTATE_TIME, true);
        time_duration td(duration_from_string(res[2].str()));
        unsigned int day = Weekday()(res[1].str());

        pRotateStrategy_.reset(new RotateByTimeStrategy(td, utc, day));
      } else {
        pRotateStrategy_.reset(new RotateByTimeStrategy);
      }
    }
  }
}


// TODO: implement purgatory and cleanse logs from evil spirits
void
FileChannel::purge() {}

}
