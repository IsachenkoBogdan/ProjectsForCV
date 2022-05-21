#include <iostream>
#include <vector>
class BigInteger {
 private:
  static const size_t base = 1000000000;
  std::vector<int64_t> number;
  bool sign = true;
 public:
  BigInteger() = default;
  BigInteger(const int64_t);
  BigInteger(const std::string&);
  
  std::string toString() const;
  
  friend bool operator==(const BigInteger&, const BigInteger&);
  friend bool operator>(const BigInteger&, const BigInteger&);
  
  BigInteger& operator+=(const BigInteger&);
  BigInteger& operator-=(const BigInteger&);
  BigInteger& operator/=(const BigInteger&);
  BigInteger& operator*=(const BigInteger&);
  BigInteger& operator%=(const BigInteger&);
  
  explicit operator bool() const;
  
  BigInteger operator-() const;
  BigInteger& operator++();
  const BigInteger operator++(int);
  BigInteger& operator--();
  const BigInteger operator--(int);
 
 private:
  void cleanZeros();
  void addZeros(size_t);
};

BigInteger::BigInteger(int64_t c) : sign(c >= 0) {
  if (c == 0) {
    number = std::vector<int64_t>(1, 0);
  }
  c = std::abs(c);
  while (c != 0) {
    number.push_back(c % base);
    c /= base;
  }
}

BigInteger::BigInteger(const std::string& str) {
  number.clear();
  sign = true;
  int lastIndex = 0;
  int powerOfTen = std::to_string(base).length() - 1;
  if (str.length() != 0) {
    if (str[0] == '-') {
      lastIndex = 1;
      sign = false;
    }
    for (int i = str.length() - lastIndex; i > 0; i -= powerOfTen) {
      number.push_back(std::stoi(
          i <= powerOfTen ?
          str.substr(lastIndex, i) :
          str.substr(i - powerOfTen + lastIndex, powerOfTen)));
    }
    cleanZeros();
  } else {
    number = std::vector<int64_t>(1, 0);
  }
}

void BigInteger::cleanZeros() {
  while (number.size() > 1 && number.back() == 0) {
    number.pop_back();
  }
}
void BigInteger::addZeros(size_t n) {
  number.reserve(n);
  for (size_t i = n - number.size(); i != 0; --i) {
    number.push_back(0);
  }
}
std::string BigInteger::toString() const {
  std::string s;
  size_t powerOfTen = std::to_string(base).length() - 1;
  if (!sign) {
    s.push_back('-');
  }
  s += std::to_string(number[number.size() - 1]);
  for (int64_t i = static_cast<int64_t>(number.size()) - 2; i >= 0; --i) {
    s += ((powerOfTen > std::to_string(number[i]).length()) ?
          std::string(powerOfTen - std::to_string(number[i]).length(), '0') :
          "") + std::to_string(number[i]);
  }
  return s;
}

bool operator==(const BigInteger& left, const BigInteger& right) {
  if (left.sign != right.sign || left.number.size() != right.number.size()) {
    return false;
  }
  for (size_t i = 0; i < left.number.size(); ++i) {
    if (left.number[i] != right.number[i]) {
      return false;
    }
  }
  return true;
}

bool operator>(const BigInteger& left, const BigInteger& right) {
  if (!left.sign && right.sign) {
    return false;
  }
  if (left.sign && !right.sign) {
    return true;
  }
  if (left.number.size() == right.number.size()) {
    for (size_t i = right.number.size(); i > 0; --i) {
      if (left.number[i - 1] != right.number[i - 1]) {
        return (left.sign ? (left.number[i - 1] > right.number[i - 1]) : (left.number[i - 1] < right.number[i - 1]));
      }
    }
  }
  return
      (left.sign ?
       (left.number.size() > right.number.size()) :
       (left.number.size() > right.number.size()));
}

bool operator!=(const BigInteger& left, const BigInteger& right) {
  return !(left == right);
}

bool operator<=(const BigInteger& left, const BigInteger& right) {
  return !(left > right);
}

bool operator>=(const BigInteger& left, const BigInteger& right) {
  return !(right > left);
}

bool operator<(const BigInteger& left, const BigInteger& right) {
  return right > left;
}

BigInteger& BigInteger::operator+=(const BigInteger& right) {
  if (!right.sign && sign) {
    return *this -= (-right);
  }
  if (right.sign && !sign) {
    sign = !sign;
    *this -= right;
    sign = !sign;
    return *this;
  }
  addZeros(std::max(number.size(), right.number.size()));
  size_t sum, balance = 0;
  for (size_t pos = 0; pos <= right.number.size() - 1; ++pos) {
    sum = number[pos] + right.number[pos] + balance;
    balance = sum / base;
    number[pos] = sum % base;
  }
  if (balance != 0) {
    number.push_back(balance);
  }
  cleanZeros();
  return *this;
}

BigInteger operator+(const BigInteger& left, const BigInteger& right) {
  BigInteger answer = left;
  return answer += right;
}

BigInteger& BigInteger::operator-=(const BigInteger& right) {
  if (*this == right) {
    return *this = 0;
  }
  BigInteger copy = right;
  if (!copy.sign && sign) {
    copy.sign = true;
    return *this += copy;
  }
  if (copy.sign && !sign) {
    copy -= *this;
    copy.sign = false;
    return *this = copy;
  }
  addZeros(std::max(number.size(), copy.number.size()));
  copy.addZeros(std::max(number.size(), copy.number.size()));
  if (sign) {
    if (*this > copy) {
      for (size_t i = 0; i < copy.number.size(); ++i) {
        if (number[i] >= copy.number[i]) {
          number[i] -= copy.number[i];
        } else {
          number[i] += base - copy.number[i];
          number[i + 1] -= 1;
        }
      }
      cleanZeros();
      return *this;
    }
    copy -= *this;
    *this = copy;
    sign = !sign;
    return *this;
  }
  sign = !sign;
  copy.sign = !copy.sign;
  *this -= copy;
  sign = !sign;
  return *this;
}

BigInteger operator-(const BigInteger& left, const BigInteger& right) {
  BigInteger answer = left;
  return answer -= right;
}

BigInteger& BigInteger::operator*=(const BigInteger& right) {
  BigInteger answer = 0;
  answer.addZeros(2 * std::max(number.size(), 2 * right.number.size()));
  for (size_t i = 0; i < number.size(); ++i)
    for (size_t j = 0, result; j < right.number.size(); ++j) {
      result = number[i] * right.number[j] + answer.number[i + j];
      answer.number[i + j + 1] += result / base;
      answer.number[i + j] = result % base;
    }
  answer.cleanZeros();
  answer.sign = (right.sign == sign);
  if (-answer == 0) {
    answer.sign = true;
  }
  return *this = answer;
}

BigInteger operator*(const BigInteger& left, const BigInteger& right) {
  BigInteger answer = left;
  return answer *= right;
}

BigInteger& BigInteger::operator/=(const BigInteger& right) {
  BigInteger copy = right;
  copy.sign = true;
  BigInteger result, digit;
  result.number.resize(number.size());
  for (long long i = static_cast<long long>(number.size()) - 1; i >= 0; --i) {
    digit *= base;
    digit.number[0] = number[i];
    digit.cleanZeros();
    size_t searchRes = base, pos_left = 0, pos_right = base;
    while (pos_left <= pos_right) {
      size_t middle = (pos_left + pos_right) / 2;
      BigInteger t = copy * middle;
      if (t <= digit) {
        searchRes = middle;
        pos_left = middle + 1;
      } else {
        pos_right = middle - 1;
      }
    }
    result.number[i] = searchRes;
    digit -= copy * searchRes;
  }
  result.sign = (sign == right.sign);
  result.cleanZeros();
  if (-result == 0) {
    result.sign = true;
  }
  return *this = result;
}

BigInteger operator/(const BigInteger& left, const BigInteger& right) {
  BigInteger answer = left;
  return answer /= right;
}

BigInteger& BigInteger::operator%=(const BigInteger& right) {
  return *this -= (*this / right) * right;
}

BigInteger operator%(const BigInteger& left, const BigInteger& right) {
  BigInteger answer = left;
  return answer %= right;
}

BigInteger BigInteger::operator-() const {
  BigInteger copy(*this);
  if (copy != 0) {
    copy.sign = !copy.sign;
  }
  return copy;
}

BigInteger& BigInteger::operator++() {
  return (*this += 1);
}

const BigInteger BigInteger::operator++(int) {
  return (*this += 1) - 1;
}

BigInteger& BigInteger::operator--() {
  return *this -= 1;
}

const BigInteger BigInteger::operator--(int) {
  return (*this -= 1) + 1;
}

std::ostream& operator<<(std::ostream& os, const BigInteger& output) {
  os << output.toString();
  return os;
}

BigInteger::operator bool() const {
  return !(*this == 0);
}
std::istream& operator>>(std::istream& in, BigInteger& input) {
  std::string s;
  in >> s;
  input = BigInteger(s);
  return in;
}
class Rational;
bool operator<(const Rational& left, const Rational& right);
class Rational {
 private:
  BigInteger numerator;
  BigInteger denominator;
 
 public:
  Rational() = default;
  Rational(const BigInteger& left, const BigInteger& right = 1) : numerator(left), denominator(right) {
    reduce();
  };
  Rational(const int64_t& left, const int64_t& right = 1) : numerator(left), denominator(right) {
    reduce();
  };
  
  void reduce();
  std::string toString();
  std::string asDecimal(size_t) const;
  
  friend bool operator==(const Rational&, const Rational&);
  friend bool operator>(const Rational&, const Rational&);
  
  Rational& operator+=(const Rational&);
  Rational& operator-=(const Rational&);
  Rational& operator/=(const Rational&);
  Rational& operator*=(const Rational&);
  
  explicit operator bool() const;
  explicit operator double() const;
  
  Rational operator-() const;
  Rational& operator++();
  Rational operator++(int);
  Rational& operator--();
  Rational operator--(int);
};
void Rational::reduce() {
  BigInteger num = numerator;
  BigInteger den = denominator;
  while (den) {
    std::swap(num %= den, den);
  }
  numerator /= num;
  denominator /= num;
  if (denominator < 0) {
    denominator *= -1;
    numerator *= -1;
  }
}

std::string Rational::toString() {
  reduce();
  if (denominator == 1) {
    return numerator.toString();
  }
  return numerator.toString() + '/' + denominator.toString();
}

std::string Rational::asDecimal(size_t precision = 0) const {
  BigInteger pow = 1;
  for (size_t i = 0; i < precision; ++i) {
    pow *= 10;
  }
  BigInteger res = (numerator / denominator);
  std::string answer;
  BigInteger new_numerator = numerator;
  BigInteger new_denominator = denominator;
  if (res == 0 && (*this < 0)) {
    answer = "-";
    new_numerator *= -1;
    res *= -1;
  }
  if (precision == 0) {
    return answer + res.toString();
  }
  answer += res.toString() + '.';
  std::string part = (((new_numerator * pow) / new_denominator) - res * pow).toString();
  if (part == "0") {
    answer += std::string(precision, '0');
  } else if (static_cast<int64_t>(precision) - static_cast<int64_t>(part.length()) > 0) {
    answer += std::string(precision - part.length(), '0') + part;
  } else {
    answer += part;
  }
  return answer;
}

bool operator==(const Rational& left, const Rational& right) {
  return left.numerator * right.denominator == left.denominator * right.numerator;
}

bool operator>(const Rational& left1, const Rational& right1) {
  return left1.numerator * right1.denominator > left1.denominator * right1.numerator;
}

bool operator<(const Rational& left, const Rational& right) {
  return right > left;
}

bool operator!=(const Rational& left, const Rational& right) {
  return !(left == right);
}

bool operator>=(const Rational& left, const Rational& right) {
  return !(right > left);
}

bool operator<=(const Rational& left, const Rational& right) {
  return !(left > right);
}

Rational::operator bool() const {
  return !(*this == 0);
}

Rational& Rational::operator+=(const Rational& right) {
  numerator = numerator * right.denominator + denominator * right.numerator;
  denominator *= right.denominator;
  reduce();
  return *this;
}

Rational& Rational::operator-=(const Rational& right) {
  numerator = numerator * right.denominator - denominator * right.numerator;
  denominator *= right.denominator;
  reduce();
  return *this;
}

Rational& Rational::operator/=(const Rational& right) {
  numerator *= right.denominator;
  denominator *= right.numerator;
  reduce();
  return *this;
}

Rational& Rational::operator*=(const Rational& right) {
  numerator *= right.numerator;
  denominator *= right.denominator;
  reduce();
  return *this;
}

Rational Rational::operator-() const {
  Rational copy = *this;
  copy.reduce();
  copy.numerator *= -1;
  return copy;
}

Rational operator+(const Rational& left, const Rational& right) {
  Rational answer(left);
  answer += right;
  return answer;
}

Rational operator-(const Rational& left, const Rational& right) {
  Rational answer(left);
  answer -= right;
  return answer;
}

Rational operator/(const Rational& left, const Rational& right) {
  Rational answer(left);
  answer /= right;
  return answer;
}

Rational operator*(const Rational& left, const Rational& right) {
  Rational answer(left);
  answer *= right;
  return answer;
}

Rational& Rational::operator++() {
  return *this += 1;
}

Rational Rational::operator++(int) {
  return (*this += 1) - 1;
}

Rational& Rational::operator--() {
  return *this -= 1;
}

Rational Rational::operator--(int) {
  return (*this -= 1) + 1;
}

Rational::operator double() const {
  return atof(asDecimal(20).c_str());
}
