#include <iostream>
#include "expr.hpp"

std::unordered_map<std::string, Sign> sign_table;

AExpr parse_factor(const std::vector<Token>& tokens, size_t& i) {
  switch (tokens[i].type) {
    case TokenType::Minus: {
      i++;
      AExpr inner = parse_factor(tokens, i);
      return AExpr { std::make_unique<Neg>(Neg{std::move(inner)})};
    }
    case TokenType::Integer: {
      int value = std::stoi(tokens[i].text);
      i++;
      return AExpr {N {value}};
    }
    case TokenType::Identifier: {
      std::string name = tokens[i].text;
      i++;
      return AExpr {Var {name}};
    }
    default: throw std::runtime_error("Unexpected token in factor");
  }
}

AExpr parse_aexpr(const std::vector<Token>& tokens, size_t& i) {
  AExpr left = parse_factor(tokens, i);
  while (i < tokens.size() &&
        (tokens[i].type == TokenType::Plus ||
         tokens[i].type == TokenType::Minus)) {
    TokenType op = tokens[i].type;
    i++;
    AExpr right = parse_factor(tokens, i);
    switch (op) {
      case TokenType::Plus:
        left = AExpr { std::make_unique<Add>(Add{std::move(left), std::move(right)})}; break;
      case TokenType::Minus:
        left = AExpr{ std::make_unique<Add>(Add{ std::move(left), std::make_unique<Neg>(Neg{std::move(right)}) }) }; break;
      default: throw std::runtime_error("Unexpected token in aexpr"); break;
    }
  }
  return left;
}

BExpr parse_bexpr(const std::vector<Token>& tokens, size_t& i) {
  switch (tokens[i].type) {
    case TokenType::True:
      i++;
      return BExpr {True{}};
    case TokenType::False:
      i++;
      return BExpr {False{}};
    default: break;
  }
  AExpr left = parse_aexpr(tokens, i);
  TokenType op = tokens[i].type;
  i++;
  AExpr right = parse_aexpr(tokens, i);
  switch (op) {
    case TokenType::Greater_than:
      return BExpr { std::make_unique<Gt>(Gt{std::move(left), std::move(right)})};
    case TokenType::Equal:
      return BExpr { std::make_unique<Eq>(Eq{std::move(left), std::move(right)})};
    default: throw std::runtime_error("Unexpected token in parse_bexpr"); break;
  }
}

Stmt parse_stmt(const std::vector<Token>& tokens, size_t& i);

Stmt parse_stmt_factor(const std::vector<Token>& tokens, size_t& i) {
  switch (tokens[i].type) {
    case TokenType::Skip:
      i++;
      return Stmt {Skip{}};
    case TokenType::Identifier: {
      std::string name = tokens[i].text;
      i++;
      if (tokens[i].type != TokenType::Equal) {
        throw std::runtime_error("Error, '=' expected");
      }
      i++;
      AExpr inner = parse_aexpr(tokens, i);
      return Stmt {std::make_unique<Assign>(Assign{name, std::move(inner)})};
    }
    case TokenType::If: {
      i++;
      BExpr cond = parse_bexpr(tokens, i);
      if (tokens[i].type != TokenType::Then) {
        throw std::runtime_error("Error, 'then' expected");
      }
      i++;
      Stmt s1 = parse_stmt(tokens, i);
      Stmt s2;
      if (tokens[i].type != TokenType::Else) {
        s2 = Stmt{Skip{}};
      } else {
        s2 = parse_stmt(tokens, i);
      }
      return Stmt {std::make_unique<Ifthenelse>(Ifthenelse{std::move(cond), std::move(s1), std::move(s2)})};
    }
    case TokenType::While: {
      i++;
      BExpr cond = parse_bexpr(tokens, i);
      if (tokens[i].type != TokenType::Do) {
        throw std::runtime_error("Error, 'do' expected");
      }
      i++;
      Stmt s = parse_stmt(tokens, i);
      return Stmt {std::make_unique<While>(While{std::move(cond), std::move(s)})};
    }
    default: throw std::runtime_error("Unexpected token in parse_stmt"); break;
  }
}

Stmt parse_stmt(const std::vector<Token>& tokens, size_t& i) {
  Stmt left = parse_stmt_factor(tokens, i);
  while (i < tokens.size() && tokens[i].type == TokenType::Semi_colon) {
    i++;
    Stmt right = parse_stmt_factor(tokens, i);
    left = Stmt {std::make_unique<Seq>(Seq{std::move(left), std::move(right)})};
  }
  return left;
}

Stmt g_parse_stmt(const std::vector<Token>& tokens) {
  size_t i = 0;
  return parse_stmt(tokens, i);
}



std::string pp_aexpr(const AExpr& e);
std::string pp_bexpr(const BExpr& e);

struct VisitAExpr {
  std::string operator()(const N& e) {
    std::cout << "N = " << e.value;
    return "";
  }
  std::string operator()(const Var& e) {
    std::cout << "Var = " << e.name;
    return "";
  }
  std::string operator()(const std::unique_ptr<Add>& e) {
    std::cout << "Add = (" << pp_aexpr(e->e1) << "; " << pp_aexpr(e->e2) << ")";
    return "";
  }
  std::string operator()(const std::unique_ptr<Neg>& e) {
    std::cout << "Neg = (" << pp_aexpr(e->e) << ")";
    return "";
  }
};

struct VisitBExpr {
  std::string operator()(const True&) {
    std::cout << "True";
    return "";
  }
  std::string operator()(const False&) {
    std::cout << "False";
    return "";
  }
  std::string operator()(const std::unique_ptr<Gt>& e) {
    std::cout << "Gt = (" << pp_aexpr(e->e1) << "; " << pp_aexpr(e->e2) << ")";
    return "";
  }
  std::string operator()(const std::unique_ptr<Eq>& e) {
    std::cout << "Eq = (" << pp_aexpr(e->e1) << "; " << pp_aexpr(e->e2) << ")";
    return "";
  }
};

struct VisitStmt {
  std::string operator()(const Skip&) {
    std::cout << "Skip";
    return "";
  }
  std::string operator()(const std::unique_ptr<Seq>& e) {
    std::cout << "Seq = (" << pp_stmt(e->s1) << "; " << pp_stmt(e->s2) << ")";
    return "";
  }
  std::string operator()(const std::unique_ptr<Assign>& e) {
    std::cout << "Assign = ( name = " << e->name << ", " << pp_aexpr(e->e) << ")";
    return "";
  }
  std::string operator()(const std::unique_ptr<Ifthenelse>& e) {
    std::cout << "IfthenElse = (" << pp_bexpr(e->b) << "; " << pp_stmt(e->s1) << "; " << pp_stmt(e->s2) << ")";
    return "";
  }
  std::string operator()(const std::unique_ptr<While>& e) {
    std::cout << "While = (" << pp_bexpr(e->b) << "; " << pp_stmt(e->s) << ")";
    return "";
  }
};

std::string pp_aexpr(const AExpr& e) {
  return std::visit(VisitAExpr(), e);
}

std::string pp_bexpr(const BExpr& e) {
  return std::visit(VisitBExpr(), e);
}

std::string pp_stmt(const Stmt& s) {
  return std::visit(VisitStmt(), s);
}

// struct VisitExpr {
//   Sign operator()(const N& e) {
//     if (e.value < 0) return Sign::NEG;
//     if (e.value > 0) return Sign::POS;
//     return Sign::ZER;
//   }
//   Sign operator()(const Var& e) {
//     return sign_table[e.value];
//   }
//   Sign operator()(const std::unique_ptr<Add>& e) {
//     return add(sign_of_expr(*e->e1), sign_of_expr(*e->e2));
//   }
//   Sign operator()(const std::unique_ptr<Neg>& e) {
//     return negate(sign_of_expr(*e->e));
//   }
// };

// Sign sign_of_expr(const Expr& e) {
//   return std::visit(VisitExpr(), e);
// }
