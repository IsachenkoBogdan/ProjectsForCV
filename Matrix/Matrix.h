#include <iostream>
#include <vector>
class BigInteger {
 private:
  static const size_t base = 1000000000;
 public:
  std::vector<int64_t> number;
  bool sign = true;
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
  num.sign = true;
  BigInteger den = denominator;
  den.sign = true;
  BigInteger ans = 1;
  while (den!=0 && num!=0) {
    while(den.number[0]%2==0 and num.number[0]%2==0 and(den.number[0]!=0 or num.number[0]!=0)){
      den/=2;
      num/=2;
      ans*=2;
    }
    while(den.number[0]%2==0 and den.number[0]!=0) den/=2;
    while(num.number[0]%2==0 and num.number[0]!=0) num/=2;
    if(den>=num) den-=num;
    else num-=den;
  }
  num = ans*((den==0)?num:den);
  if(num == 0) num = numerator;
  numerator /= num;
  denominator /= num;
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

std::istream& operator>>(std::istream& in, Rational& input) {
  BigInteger s;
  in >> s;
  input = Rational(s);
  return in;
}

template<unsigned V>
struct Sqrt {
  template<size_t L, size_t R>
  struct SqrtHelperFunc {
    static const size_t M = (L + R + 1) / 2;
    static const size_t N = R * R > V ? L : R;
    static const size_t value = SqrtHelperFunc<(L + 1 >= R) ? (N) : (M * M < V ? M : L),
                                               L + 1 >= R ? N : M * M < V ? R : M>::value;
  };
  template<size_t M>
  struct SqrtHelperFunc<M, M> {
    static const size_t value = M;
  };
  static const size_t value = SqrtHelperFunc<0, V>::value;
};

template<int N, int K>
struct IsPrimeHelper {
  static const bool value = (N % K != 0 && IsPrimeHelper<N, K - 1>::value);
};

template<int N>
struct IsPrimeHelper<N, 1> {
  static const bool value = true;
};

template<int N>
struct IsPrime {
  static const bool value = IsPrimeHelper<N, Sqrt<N>::value>::value;
};

template<int N>
class Residue {
 public:
  size_t value;
  Residue<N>(int64_t value = 0) : value((value % N + N) % N) {}
  
  Residue<N>& operator+=(const Residue<N>& second) {
    value = (value + second.value) % N;
    return *this;
  }
  Residue<N> operator+(const Residue<N>& second) const {
    Residue<N> copy = *this;
    return copy += second;
  }
  
  Residue<N>& operator-=(const Residue<N>& second) {
    value = (value - second.value + N) % N;
    return *this;
  }
  Residue<N> operator-(const Residue<N> second) const {
    Residue<N> copy = *this;
    return copy -= second;
  }
  Residue<N> operator-() const {
    return Residue<N>(-value);
  }
  
  Residue<N>& operator*=(const Residue<N>& second) {
    value = (value * second.value) % N;
    return *this;
  }
  Residue<N> operator*(const Residue<N>& second) const {
    Residue<N> copy = *this;
    return copy *= second;
  }
  
  Residue<N> powForResidue(int power) const {
    if (power == 0) return 1;
    if (power % 2 == 0) {
      Residue<N> sqrt = powForResidue(power / 2);
      return sqrt * sqrt;
    } else {
      return powForResidue(power - 1) * *this;
    }
  }
  
  Residue<N>& operator/=(const Residue<N>& second) {
    static_assert(IsPrime<N>::value, "Non-prime division is not possible(Error)");
    value = (*this * second.powForResidue(N - 2)).value;
    return *this;
  }
  Residue<N> operator/(const Residue<N>& second) const {
    Residue<N> copy = *this;
    return copy /= second;
  }
  
  bool operator==(const Residue<N>& second) const {
    return value == second.value;
  }
  bool operator!=(const Residue<N>& second) const {
    return value != second.value;
  }
  
  Residue<N>& operator++() {
    return (*this += 1);
  }
  Residue<N> operator++(int) {
    return (++(*this) - 1);
  }
  template<int L>
  friend std::ostream& operator<<(std::ostream& out, const Residue<L>& value);
};

template<int N>
std::ostream& operator<<(std::ostream& out, const Residue<N>& element) {
  out << element.value;
  return out;
}

template<size_t M, size_t N, typename Field = Rational>
class Matrix {
 private:
  std::vector<std::vector<Field>> table;
 public:
  //конструкторы
  Matrix<M, N, Field>();
  Matrix<M, N, Field>(int64_t);
  Matrix<M, N, Field>(std::vector<std::vector<Field>>&);
  Matrix<M, N, Field>(std::vector<std::vector<int64_t>>&);
  template<class t>
  Matrix(const std::initializer_list<std::initializer_list<t>>& tab);
  // сравнения
  template<size_t M1, size_t N1, typename Field1>
  bool operator==(const Matrix<M1, N1, Field1>&) const;
  template<size_t M1, size_t N1, typename Field1>
  bool operator!=(const Matrix<M1, N1, Field1>&) const;
  // сумма
  Matrix<M, N, Field>& operator+=(const Matrix<M, N, Field>&);
  Matrix<M, N, Field> operator+(const Matrix<M, N, Field>) const;
  // разность
  Matrix<M, N, Field>& operator-=(const Matrix<M, N, Field>&);
  Matrix<M, N, Field> operator-(const Matrix<M, N, Field>&) const;
  // умножение на элемент поля
  Matrix<M, N, Field> operator*=(const Field&);
  Matrix<M, N, Field> operator*(const Field&) const;
  // умножение на матрицу
  Matrix<M, N, Field>& operator*=(const Matrix<M, N, Field>&);
  // различные характеристики матрицы
  Field det() const;
  size_t rank() const;
  Field trace() const;
  // транспонирование и обращение матриц
  Matrix<N, M, Field> transposed() const;
  void invert();
  Matrix<N, M, Field> inverted() const;
  // элементарные преобразования матрицы
  void rowDivision(size_t, const Field);
  void rowDifference(size_t, size_t, const Field);
  Field GaussMatrixMethod();
  // доступ к элементам
  std::vector<Field> getRow(size_t) const;
  std::vector<Field> getColumn(size_t) const;
  std::vector<Field>& operator[](size_t index);
  const std::vector<Field>& operator[](size_t index) const;
};

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field>::Matrix(): table(std::vector<std::vector<Field>>(M, std::vector<Field>(N, 0))) {
  for (size_t i = 0; i < std::min(M, N); ++i) {
    table[i][i] = 1;
  }
}

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field>::Matrix(int64_t number): table(std::vector<std::vector<Field>>(M, std::vector<Field>(N, number))) {
}

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field>::Matrix(std::vector<std::vector<Field>>& second): table(second) {
}

template<size_t M, size_t N, typename Field>
template<class t>
Matrix<M, N, Field>::Matrix(const std::initializer_list<std::initializer_list<t>>& second) {
  std::vector<Field> row;
  for (auto& element: second) {
    for (auto& elem: element) {
      row.push_back(elem);
    }
    table.push_back(row);
    row.clear();
  }
}

template<size_t M, size_t N, typename Field>
template<size_t M1, size_t N1, typename Field1>
bool Matrix<M, N, Field>::operator==(const Matrix<M1, N1, Field1>& second) const {
  if (!std::is_same<Matrix<M, N, Field>, Matrix<M1, N1, Field1>>::value)
    return false;
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < N; ++j) {
      if (table[i][j] != second[i][j])
        return false;
    }
  }
  return true;
}

template<size_t M, size_t N, typename Field>
template<size_t M1, size_t N1, typename Field1>
bool Matrix<M, N, Field>::operator!=(const Matrix<M1, N1, Field1>& second) const {
  return !(*this == second);
}

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field>& Matrix<M, N, Field>::operator+=(const Matrix<M, N, Field>& second) {
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < N; ++j) {
      table[i][j] += second[i][j];
    }
  }
  return *this;
}

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field> Matrix<M, N, Field>::operator+(const Matrix<M, N, Field> second) const {
  return second += *this;
}

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field>& Matrix<M, N, Field>::operator-=(const Matrix<M, N, Field>& second) {
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < N; ++j) {
      table[i][j] -= second[i][j];
    }
  }
  return *this;
}

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field> Matrix<M, N, Field>::operator-(const Matrix<M, N, Field>& second) const {
  Matrix<M, N, Field> copy = *this;
  return copy -= second;
}

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field> Matrix<M, N, Field>::operator*=(const Field& second) {
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < N; ++j) {
      table[i][j] *= second;
    }
  }
  return *this;
}

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field> Matrix<M, N, Field>::operator*(const Field& second) const {
  Matrix<M, N, Field> copy = *this;
  return copy *= second;
}

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field> operator*(const Field& number, const Matrix<M, N, Field>& second) {
  return second * number;
}

template<size_t M, size_t N, size_t K, typename Field>
Matrix<M, K, Field> operator*(const Matrix<M, N, Field>& first, const Matrix<N, K, Field>& second) {
  Matrix<M, K, Field> result(0);
  for (size_t m = 0; m < M; ++m) {
    for (size_t k = 0; k < K; ++k) {
      for (size_t n = 0; n < N; ++n) {
        result[m][k] += first[m][n] * second[n][k];
      }
    }
  }
  return result;
}

template<size_t M, size_t N, typename Field>
Matrix<M, N, Field>& Matrix<M, N, Field>::operator*=(const Matrix<M, N, Field>& second) {
  static_assert(M == N, "An attempt to multiply matrices of the wrong size(Error)\n");
  return *this = *this * second;
}

template<size_t M, size_t N, typename Field>
Field Matrix<M, N, Field>::det() const {
  static_assert(N == M, "Tried to get the determinant of a non-square matrix(Error)\n");
  Matrix<M, N, Field> copy = *this;
  return copy.GaussMatrixMethod();
}

template<size_t M, size_t N, typename Field>
size_t Matrix<M, N, Field>::rank() const {
  Matrix<M, N, Field> copy = *this;
  copy.GaussMatrixMethod();
  size_t rank = 0;
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < N; ++j) {
      if (copy[i][j] != 0) {
        ++rank;
        j = N; // выйдем за границы
      }
    }
  }
  return rank;
}

template<size_t M, size_t N, typename Field>
Field Matrix<M, N, Field>::trace() const {
  static_assert(N == M, "Tried to get the trace of a non-square matrix(Error)\n");
  Field sum(0);
  for (size_t i = 0; i < N; ++i) {
    sum += table[i][i];
  }
  return sum;
}

template<size_t M, size_t N, typename Field>
Matrix<N, M, Field> Matrix<M, N, Field>::transposed() const {
  Matrix<N, M, Field> result;
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < M; ++j) {
      result[i][j] = table[j][i];
    }
  }
  return result;
}

template<size_t M, size_t N, typename Field>
void Matrix<M, N, Field>::invert() {
  static_assert(N == M, "Tried to invert a non-square matrix(Error)\n");
  std::vector<std::vector<Field>> abc(N, std::vector<Field>(2 * N, 0));
  Matrix<N, 2 * N, Field> matrixWithE(abc);
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < N; ++j) {
      matrixWithE[i][j] = table[i][j];
    }
  }
  for (size_t i = 0; i < N; ++i)
    matrixWithE[i][i + N] = Field(1);
  //приписали справа единичную матрицу
  matrixWithE.GaussMatrixMethod();
  for (size_t i = 0; i < N; ++i) {
    for (size_t j = 0; j < N; ++j) {
      table[i][j] = matrixWithE[i][j + N];
    }
  }
}

template<size_t M, size_t N, typename Field>
Matrix<N, M, Field> Matrix<M, N, Field>::inverted() const {
  Matrix<N, M, Field> copy = *this;
  copy.invert();
  return copy;
}

template<size_t M, size_t N, typename Field>
void Matrix<M, N, Field>::rowDivision(size_t rowNumber, const Field denominator) {
  for (size_t i = 0; i < N; ++i) {
    table[rowNumber][i] /= denominator;
  }
}

template<size_t M, size_t N, typename Field>
void Matrix<M, N, Field>::rowDifference(size_t firstRowNumber, size_t secondRowNumber, const Field quantity) {
  for (size_t i = 0; i < N; ++i) {
    table[firstRowNumber][i] -= table[secondRowNumber][i] * quantity;
  }
}

template<size_t M, size_t N, typename Field>
Field Matrix<M, N, Field>::GaussMatrixMethod() {
  Field det = 1;
  for (size_t rowNumber = 0; rowNumber < N; ++rowNumber) {
    size_t startIsNotNullRow = SIZE_MAX;
    for (size_t j = rowNumber; j < M; ++j) {
      if (table[j][rowNumber] != 0){
        startIsNotNullRow = j;
        break;
      }
    }
    det = (startIsNotNullRow == SIZE_MAX) ? Field(0) : det * table[startIsNotNullRow][rowNumber];
    if (startIsNotNullRow == SIZE_MAX) continue;
    std::swap(table[rowNumber], table[startIsNotNullRow]);
    std::cerr << "1";
    rowDivision(rowNumber, table[rowNumber][rowNumber]);
    std::cerr << "2";
    for (size_t j = 0; j < M; ++j) {
      if(j!=rowNumber) rowDifference(j, rowNumber, table[j][rowNumber]);
    }
    std::cerr << "3";
  }
  std::cerr << "0";
  return det;
}

template<size_t M, size_t N, typename Field>
std::vector<Field> Matrix<M, N, Field>::getRow(size_t rowNumber) const {
  return table[rowNumber];
}

template<size_t M, size_t N, typename Field>
std::vector<Field> Matrix<M, N, Field>::getColumn(size_t columnNumber) const {
  std::vector<Field> result(M);
  for (size_t i = 0; i < M; ++i) {
    result[i] = table[i][columnNumber];
  }
  return result;
}

template<size_t M, size_t N, typename Field>
std::vector<Field>& Matrix<M, N, Field>::operator[](size_t index) {
  return table[index];
}

template<size_t M, size_t N, typename Field>
const std::vector<Field>& Matrix<M, N, Field>::operator[](size_t index) const {
  return table[index];
}

template<size_t N, typename Field = Rational>
using SquareMatrix = Matrix<N, N, Field>;