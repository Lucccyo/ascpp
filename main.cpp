#include <iostream>
#include <string>
#include "expr.hpp"
#include "sign.hpp"

int main() {
  std::string str = "0 + 0";
  Expr e = parse_expr(lex(str));
  Sign sign = sign_of_expr(e);
  std::string s = to_string(sign);
  std::cout << s << "\n";
  return 0;
};
