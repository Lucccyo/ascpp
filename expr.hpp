#pragma once
#include <variant>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "sign.hpp"
#include "parser.hpp"

// arithmetic expressions
struct N { int value; };
struct Var { std::string name; };
struct Add;
struct Neg;
using AExpr = std::variant<
  N, Var,
  std::unique_ptr<Neg>,
  std::unique_ptr<Add>
>;
struct Neg { AExpr e; };
struct Add { AExpr e1, e2; };

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
using Stmt = std::variant<
  Skip,
  std::unique_ptr<Assign>,
  std::unique_ptr<Seq>,
  std::unique_ptr<Ifthenelse>,
  std::unique_ptr<While>
>;
struct Seq { Stmt s1, s2; };
struct Ifthenelse { BExpr b; Stmt s1, s2; };
struct While { BExpr b; Stmt s; };

Stmt g_parse_stmt(const std::vector<Token>& tokens);

std::string pp_stmt(const Stmt& e);
// Sign sign_of_expr (const Expr& e);
