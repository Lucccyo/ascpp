#include <iostream>
#include <string>
#include "expr.hpp"
#include "sign.hpp"
#include "parser.hpp"

int main() {
  std::string str = "x = 2; y = -4; while x > 5 do y = y + 1; x = x + 1";
  std::vector<Token> l = lex(str);
  std::cout << '\n';
  Stmt stmt = g_parse_stmt(l);
  std::cout << pp_stmt(stmt) << '\n';
  return 0;
};
