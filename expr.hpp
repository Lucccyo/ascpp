#pragma once
#include <variant>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "sign.hpp"
#include "parser.hpp"

struct Int { int value; };
struct Val { std::string name; };
struct Add;
struct Neg;

using Expr = std::variant<
  Int,
  Val,
  std::unique_ptr<Add>,
  std::unique_ptr<Neg>
>;

struct Add { Expr left, right; };
struct Neg { Expr expr; };

Expr parse_expr(const std::vector<Token>& tokens);

Sign sign_of_expr (const Expr& e);
