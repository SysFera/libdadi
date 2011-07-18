#include <Logging/Message.hh>

namespace dadi {

Message::Message()
  : prio_(PRIO_FATAL), line_(0) {}


Message::Message(const std::string& src,
                 const std::string& txt,
                 Priority prio)
  : src_(src), txt_(txt), prio_(prio) {}

Message::Message(const std::string& src,
                 const std::string& txt,
                 Priority prio,
                 const std::string& file,
                 int line)
  : src_(src), txt_(txt), prio_(prio), file_(file), line_(line) {}

void
Message::setSource(const std::string& src) {
  src_ = src;
}

const std::string&
Message::getSource() const {
  return src_;
}

void
Message::setText(const std::string& txt) {
  txt_ = txt;
}

const std::string&
Message::getText() const {
  return txt_;
}

void
Message::setPriority(Priority prio) {
  prio_ = prio;
}

Message::Priority
Message::getPriority() const {
  return prio_;
}

void
Message::setTimestamp(long timestamp) {
  timestamp_ = timestamp;
}

long
Message::getTimestamp() const {
  return timestamp_;
}

void
Message::setFile(const std::string& file) {
  file_ = file;
}

const std::string&
Message::getFile() const {
  return file_;
}

void
Message::setLine(int line) {
  line_ = line;
}

int
Message::getLine() const {
  return line_;
}

std::string&
Message::operator[](const std::string& key) {
  return tags_[key];
}

const std::string&
Message::operator[](const std::string& key) const {
  return tags_[key];
}

} /* namespace dadi */

