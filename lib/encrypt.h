// Copyright [2019] <Copyright R.X.Cao>
#ifndef LIB_ENCRYPT_H_
#define LIB_ENCRYPT_H_

#include <cstdint>

namespace utils {
namespace encrypt {

const char pubkey_filename[] = "key.pub";
const char prikey_filename[] = "key.pri";

const uint64_t low_bound = 13999999;
const uint64_t high_bound = 899929999;

extern char *encrypt_data(const char *buff, uint64_t len);

namespace inner {

void generate_primer_number();
bool is_primer();

void intialized();
}  // namespace inner

}  // namespace encrypt
}  // namespace utils

#endif  // LIB_ENCRYPT_H_
