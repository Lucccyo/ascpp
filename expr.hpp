#pragma once
#include <variant>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "sign.hpp"
#include "parser.hpp"
#include "interval.hpp"

// arithmetic expressions
struct N { int value; };
struct Var { std::string name; };
struct Add;
struct Neg;
struct Mul;
using AExpr = std::variant<
  N, Var,
  std::unique_ptr<Neg>,
  std::unique_ptr<Add>,
  std::unique_ptr<Mul>
>;
struct Neg { AExpr e; };
struct Add { AExpr e1, e2; };
struct Mul { AExpr e1, e2; };

// boolean expressions
struct True {};
struct False {};
struct Gt;
struct Eq;
using BExpr = std::variant<
  True, False,
  std::unique_ptr<Gt>,
  std::unique_ptr<Eq>
>;
struct Gt  { AExpr e1, e2; };
struct Eq  { AExpr e1, e2; };

// statements
struct Skip {};
struct Assign { std::string name; AExpr e; };
struct Seq;
struct Ifthenelse;
struct While;
using Ast = std::variant<
  Skip,
  std::unique_ptr<Assign>,
  std::unique_ptr<Seq>,
  std::unique_ptr<Ifthenelse>,
  std::unique_ptr<While>
>;
struct Seq { Ast s1, s2; };
struct Ifthenelse { BExpr b; Ast s1, s2; };
struct While { BExpr b; Ast s; };

Ast g_parse_ast(const std::vector<Token>& tokens);

std::string pp_ast(const Ast& e);

using Store = std::unordered_map<std::string, Interval>;

Store store_of_ast (Store store, const Ast& ast);
Store get_store (const Ast& ast);

std::string store_to_string(const Store& store);
