// Copyright [2019] <Copyright R.X.Cao>
#ifndef LIB_BIG_NUMBER_H_
#define LIB_BIG_NUMBER_H_

#include <cstdint>
#include <vector>
#include <string>

namespace utils {
namespace bignum {

inline uint32_t _number(char ch) {
  return ch - '0';
}

inline char _charactor(uint32_t num) {
  return '0' + num;
}

class BigNum {
 public:
  explicit BigNum(const std::string &num);
  ~BigNum();
  BigNum& operator+(const BigNum &num);
  BigNum& operator-(const BigNum &num);
  BigNum& operator*(const BigNum &num);
  BigNum& operator/(const BigNum &num);
  bool operator<(const BigNum &num);
  bool operator>(const BigNum &num);
  bool operator==(const BigNum &num);
  std::string toString();

 private:
  bool IsVaild();
  void BitCarry();
  // NOTE: Only useable for data's length whitch are equal.
  bool CompareGreatPerChar(const std::vector<char> &num);

  bool is_negetive_;
  std::vector<char> data_;
};

}  // namespace bignum
}  // namespace utils

#endif  // LIB_BIG_NUMBER_H_
