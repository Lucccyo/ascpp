#include <iostream>
#include "sign.hpp"

int main() {
  // run_test();
  std::string s = to_string(join(Sign::POS, Sign::NEG));
  std::cout << s << "\n";
  return 0;
};
