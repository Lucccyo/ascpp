#include <iostream>
#include <string>
#include "expr.hpp"
#include "sign.hpp"
#include "parser.hpp"

int main() {
  std::string str = R"(
  n = n * n + 4;
  i = 1;
  while 10 > i do
    if i > 5 then
      n = n + 1
    else
      n = n + 2;
    i = i + 1
  )";
  Store store = {{"n", TOP}};
  std::vector<Token> l = lex(str);
  std::cout << '\n';
  Ast ast = g_parse_ast(l);
  std::cout << pp_ast(ast) << '\n' << '\n';
  std::cout << store_to_string(store_of_ast(store, ast)) << '\n';
  return 0;
};
