// Copyright [2019] <Copyright R.X.Cao>
#ifndef LIB_ENCRYPT_H_
#define LIB_ENCRYPT_H_

#include <cstdint>
#include <random>
#include <vector>

namespace utils {
namespace encrypt {

const char pubkey_filename[] = "key.pub";
const char prikey_filename[] = "key.pri";

const uint64_t low_bound = 13999999;
const uint64_t high_bound = 899929999;

extern char *encrypt_data(const char *buff, uint64_t len);

namespace inner {

template <typename T>
T get_random_number(T low, T high) {
  std::random_device rd{};
  std::mt19937_64 gen{rd()};

  std::uniform_int_distribution<> dis(low, high);
  return dis(gen);
}

template <typename T>
bool _sieve_num(T num) {
  std::vector<bool> primer_array(num + 1, false);
  for (uint64_t i = 2; i < num + 1; i++) {
    if (i % 2) {
      primer_array[i] = true;
    }
  }

  for (uint64_t i = 3; i <= sqrt(num + 1); i++) {
    if (primer_array[i]) {
      for (uint64_t j = i + i; j < num + 1; j += i) {
        primer_array[i] = false;
      }
    }
  }

  return primer_array[num];
}

template <typename T>
bool is_primer(T num) {
  if (num < 0) {
    return false;
  }
  if (num < 10000000) {
    return _sieve_num(num);
  }

  uint64_t n_sqrt = floor(sqrt(num));
  for (uint64_t i = 5; i <= n_sqrt; i += 6) {
    if (num % i == 0 || num % (i + 2) == 0) {
      return false;
    }
  }

  return true;
}

typedef struct ecc_parameter {
  ecc_parameter() {
    a = get_random_number(low_bound, high_bound);
    b = get_random_number(low_bound, high_bound);
    p = get_random_number(low_bound, high_bound);

    while (((4 * (a * a * a) + 27 * (b * b)) / p == 0)
        && is_primer(p) == false) {
      a++;
      b++;
      p--;
    }
  }
  uint64_t a;
  uint64_t b;
  uint64_t p;
} ECC_para_t;

typedef struct _Point {
  int64_t x;
  int64_t y;
} point_t;

bool is_primer();
point_t get_G_point(const ECC_para_t &parameter);

void intialized();
}  // namespace inner

}  // namespace encrypt
}  // namespace utils

#endif  // LIB_ENCRYPT_H_
