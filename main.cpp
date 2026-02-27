#include <iostream>
#include "expr.hpp"

int main() {
  Expr e = std::make_unique<Add>(Add{ Expr{Int{5}}, std::make_unique<Neg>(Neg{ Expr{Int{5}} })});

  Sign sign = sign_of_expr(e);
  std::string s = to_string(sign);
  std::cout << s << "\n";
  return 0;
};
