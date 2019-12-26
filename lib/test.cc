#include "big_number.h"

#include <iostream>

namespace bignum = utils::bignum;

int main(void) {
  bignum::BigNum b1("12322");
  bignum::BigNum b2("234");
  bignum::BigNum b = b1 + b2;

  std::cout << b.toString();
}

