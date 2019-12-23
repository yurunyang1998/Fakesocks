// Copyright [2019] <Copyright R.X.Cao>

#include "big_number.h"  // NOLINT

namespace utils {
namespace bignum {

BigNum::BigNum(const std::string &num)
  : is_negetive_(false) {
  if (num[0] == '-') {
    is_negetive_ = true;
    data_.resize(num.size() - 1);
    for (uint32_t i = 1; i < num.size(); i++) {
      data_[i] = num[i];
    }
  } else {
    data_.resize(num.size());
    for (uint32_t i = 0; i < num.size(); i++) {
      data_[i] = num[i];
    }
  }
}

BigNum::~BigNum() { }

bool BigNum::operator<(const BigNum &num) {
  if (this->is_negetive_ ^ num.is_negetive_) {
    if (this->is_negetive_) {
      return true;
    }
    return false;
  }

  if (this->is_negetive_) {
    if (this->data_.size() == num.data_.size()) {
      return !CompareGreatPerChar(num.data_);
    } else if (this->data_.size() > num.data_.size()) {
      return true;
    }
  }

  return false;
}

bool BigNum::operator>(const BigNum &num) {
  return !(*this < num);
}

bool BigNum::operator==(const BigNum &num) {
  if (!(this->is_negetive_ ^ num.is_negetive_)
      && data_.size() == num.data_.size()) {
    for (uint32_t i = 0; i < data_.size(); i++) {
      if (data_[i] != num.data_[i]) {
        return false;
      }
    }

    return true;
  }

  return false;
}

BigNum BigNum::operator+(const BigNum &num) {
  if (this->is_negetive_ ^ num.is_negetive_) {
    return *this - num;
  }
}

bool BigNum::IsVaild() {
  for (auto &i : data_) {
    if (!(i >= '0' && i <= '9')) {
      return false;
    }
  }

  return true;
}

bool BigNum::CompareGreatPerChar(const std::vector<char> &num) {
  for (uint32_t i = 0; i < num.size(); i++) {
    if (_number(data_[i]) > _number(num[i])) {
      return false;
    }
  }

  return true;
}

}  // namespace bignum
}  // namespace utils

