#include <iostream>
#include <cstring>

class String {
 private:
  size_t size = 0;
  size_t capacity = 1;
  char* str = nullptr;
 
 public:
  String() = default;
  String(const char c) : size(1), capacity(1), str(new char{c}) {}
  String(const char* c) : size(strlen(c)), capacity(2 * size), str(new char[capacity]) {
    memcpy(str, c, size);
  }
  String(const size_t size, char c = '\0') : size(size), capacity(2 * size), str(new char[capacity]) {
    memset(str, c, size);
  }
  String(const String& s) : String(s.size, '\0') {
    memcpy(str, s.str, size);
  }
  
  String& operator=(const String& s) {
    if (*this == s) return *this;
    String copy = s;
    swap(copy);
    return *this;
  }
  
  void swap(String& s);
  size_t length() const;
  void push_back(char c);
  void pop_back();
  String substr(size_t start, size_t count) const;
  size_t find(const String& substring) const;
  size_t rfind(const String& substring) const;
  const char& front() const { return str[0]; }
  const char& back() const { return str[size - 1]; }
  char& front() { return str[0]; }
  char& back() { return str[size - 1]; }
  bool empty() const { return (size == 0); }
  void clear();
  
  String& operator+=(const String& right);
  const char& operator[](size_t index) const { return str[index]; }
  char& operator[](size_t index) { return str[index]; }
  bool operator==(const String& right);
  
  ~String() { delete[] str; }
};

std::ostream& operator<<(std::ostream& out, const String& s) {
  for (size_t i = 0; i < s.length(); ++i) {
    out << s[i];
  }
  return out;
}

std::istream& operator>>(std::istream& in, String& s) {
  s.clear();
  char c;
  do {
    c = in.get();
    if (std::isspace(c) || in.eof()) {
      break;
    }
    s.push_back(c);
  } while (true);
  return in;
}

String operator+(const String& left, const String& right) {
  String answer(left);
  answer += right;
  return answer;
}

bool String::operator==(const String& right) {
  return !strcmp(str, right.str);
}

String String::substr(size_t start, size_t count) const {
  String answer(count);
  for (size_t i = start; i < start + count; ++i) {
    answer.str[i - start] = str[i];
  }
  return answer;
}

size_t String::find(const String& substring) const {
  for (size_t i = 0; i + substring.length() <= size; ++i) {
    if (substr(i, substring.length()) == substring) {
      return i;
    }
  }
  return length();
}

size_t String::rfind(const String& substring) const {
  for (size_t i = size - 1; static_cast<int>(i - substring.length() + 1) >= 0; --i) {
    if (substr(i, substring.length()) == substring) {
      return i;
    }
  }
  return length();
}

void String::pop_back() {
  --size;
  if (2 * size < capacity) {
    capacity /= 2;
    char* copy = new char[capacity];
    memcpy(copy, str, size);
    delete[] str;
    str = copy;
  }
}

size_t String::length() const {
  return size;
}

void String::push_back(const char c) {
  ++size;
  if (size >= capacity) {
    capacity *= 2;
    char* copy = new char[capacity];
    memcpy(copy, str, size);
    delete[] str;
    str = copy;
  }
  str[size - 1] = c;
}

void String::swap(String& s) {
  std::swap(size, s.size);
  std::swap(str, s.str);
  std::swap(capacity, s.capacity);
}

void String::clear() {
  delete[] str;
  str = new char[0];
  size = 0;
  capacity = 1;
}

String& String::operator+=(const String& right) {
  size_t len = right.length();
  for (size_t i = 0; i < len; ++i) {
    push_back(right[i]);
  }
  return *this;
}