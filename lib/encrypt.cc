// Copyright [2019] <Copyright R.X.Cao>
#include <fstream>
#include <cmath>

#include "encrypt.h"  // NOLINT

namespace utils {
namespace encrypt {

char *encrypt_data(const char *buff, uint64_t len) {
  return nullptr;
}

namespace inner {

point_t get_G_point(const ECC_para_t &parameter) {
  point_t g_point;
  int64_t x, y;
  x = get_random_number(0, 100000000);
  do {
    x++;
    y = floor(sqrt(x * x * x + parameter.a * x + parameter.b));
  } while ((y * y) == (x * x * x + parameter.a * x + parameter.b));

  g_point.x = x;
  g_point.y = y;
  return g_point;
}

void initialized() {
  ECC_para_t eccpara;
  point_t g_p = get_G_point(eccpara);
}

}  // namespace inner

}  // namespace encrypt
}  // namespace utils

#if defined DEBUG

#include <iostream>

namespace util = utils::encrypt::inner;

int main() {
  util::ECC_para_t p;
  util::point_t g_p = util::get_G_point(p);
  std::cout << g_p.x << " " << g_p.y << std::endl;
  return 0;
}

#endif
