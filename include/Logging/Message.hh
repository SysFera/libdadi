#ifndef _MESSAGE_HH_
#define _MESSAGE_HH_

#include <map>
#include <string>


namespace dadi {

typedef std::map<std::string, std::string> StringMap;

class Message {
public:
  enum Priority {
    PRIO_TRACE=1,
    PRIO_DEBUG,
    PRIO_INFORMATION,
    PRIO_WARNING,
    PRIO_ERROR,
    PRIO_FATAL
  };

  /**
   * @brief default constructor
   */
  Message();
  Message(const std::string& src,
          const std::string& txt,
          Priority prio);
  Message(const std::string& src,
          const std::string& txt,
          Priority prio,
          const std::string& file,
          int line);

  void setSource(const std::string& src);
  const std::string& getSource() const;

  void setText(const std::string& txt);
  const std::string& getText() const;

  void setPriority(Priority prio);
  Priority getPriority() const;

  void setTimestamp(long timestamp);
  long getTimestamp() const;

  void setFile(const std::string& file);
  const std::string& getFile() const;

  void setLine(int line);
  int getLine() const;

  // write access
  std::string& operator[](const std::string& key);
  // read access
  const std::string& operator[](const std::string& key) const;

private:
  std::string src_;
  std::string txt_;
  Priority prio_;
  long timestamp_;
  std::string file_;
  int line_;
  mutable StringMap tags_;
};

} /* namespace dadi */

#endif /* _MESSAGE_HH_ */
