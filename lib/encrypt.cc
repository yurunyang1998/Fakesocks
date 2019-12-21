// Copyright [2019] <Copyright R.X.Cao>
#include <fstream>
#include <random>
#include <cmath>
#include <vector>

#include "encrypt.h"  // NOLINT

namespace utils {
namespace encrypt {

char *encrypt_data(const char *buff, uint64_t len) {
  return nullptr;
}

namespace inner {

bool _sieve_num(uint64_t num) {
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

bool is_primer(uint64_t num) {
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

uint64_t get_random_number(uint64_t low, uint64_t high) {
  std::random_device rd{};
  std::mt19937_64 gen{rd()};

  std::uniform_int_distribution<> dis(low, high);
  return dis(gen);
}

void generate_primer_number(uint64_t *q, uint64_t *p) {
  *p = get_random_number(low_bound, high_bound);
  while (is_primer(*p) == false) {
    (*p)--;
  }
  *q = get_random_number(low_bound, high_bound);
  while (is_primer(*q) == false) {
    (*q)++;
  }
}

void initialized() {
  uint64_t p, q;
  generate_primer_number(&q, &p);
  uint64_t m = (p - 1) * (q - 1);
  uint64_t e = get_random_number(1, m);
  if (e < m / 2) {
    while (is_primer(e) == false) {
      e++;
    }
  } else {
    while (is_primer(e) == false) {
      e--;
    }
  }


}

}  // namespace inner

}  // namespace encrypt
}  // namespace utils

#if defined DEBUG

#include <iostream>

namespace util = utils::encrypt::inner;

int main() {
  uint64_t q, p;
  util::generate_primer_number(&q, &p);
  std::cout << q << " " << p;
  return 0;
}

#endif
