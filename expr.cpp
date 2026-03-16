#include <iostream>
#include "expr.hpp"

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
         tokens[i].type == TokenType::Minus ||
         tokens[i].type == TokenType::Mul)) {
    TokenType op = tokens[i].type;
    i++;
    AExpr right = parse_factor(tokens, i);
    switch (op) {
      case TokenType::Plus:
        left = AExpr { std::make_unique<Add>(Add{std::move(left), std::move(right)})}; break;
      case TokenType::Minus:
        left = AExpr{ std::make_unique<Add>(Add{ std::move(left), std::make_unique<Neg>(Neg{std::move(right)}) }) }; break;
      case TokenType::Mul:
        left = AExpr { std::make_unique<Mul>(Mul{std::move(left), std::move(right)})}; break;
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

Ast parse_ast(const std::vector<Token>& tokens, size_t& i);

Ast parse_ast_factor(const std::vector<Token>& tokens, size_t& i) {
  switch (tokens[i].type) {
    case TokenType::Skip:
      i++;
      return Ast {Skip{}};
    case TokenType::Identifier: {
      std::string name = tokens[i].text;
      i++;
      if (tokens[i].type != TokenType::Equal) {
        throw std::runtime_error("Error, '=' expected");
      }
      i++;
      AExpr inner = parse_aexpr(tokens, i);
      return Ast {std::make_unique<Assign>(Assign{name, std::move(inner)})};
    }
    case TokenType::If: {
      i++;
      BExpr cond = parse_bexpr(tokens, i);
      if (tokens[i].type != TokenType::Then) {
        throw std::runtime_error("Error, 'then' expected");
      }
      i++;
      Ast s1 = parse_ast(tokens, i);
      Ast s2;
      if (tokens[i].type != TokenType::Else) {
        s2 = Ast{Skip{}};
      } else {
        i++;
        s2 = parse_ast(tokens, i);
      }
      return Ast {std::make_unique<Ifthenelse>(Ifthenelse{std::move(cond), std::move(s1), std::move(s2)})};
    }
    case TokenType::While: {
      i++;
      BExpr cond = parse_bexpr(tokens, i);
      if (tokens[i].type != TokenType::Do) {
        throw std::runtime_error("Error, 'do' expected");
      }
      i++;
      Ast s = parse_ast(tokens, i);
      return Ast {std::make_unique<While>(While{std::move(cond), std::move(s)})};
    }
    default: throw std::runtime_error("Unexpected token in parse_ast"); break;
  }
}

Ast parse_ast(const std::vector<Token>& tokens, size_t& i) {
  Ast left = parse_ast_factor(tokens, i);
  while (i < tokens.size() && tokens[i].type == TokenType::Semi_colon) {
    i++;
    Ast right = parse_ast_factor(tokens, i);
    left = Ast {std::make_unique<Seq>(Seq{std::move(left), std::move(right)})};
  }
  return left;
}

Ast g_parse_ast(const std::vector<Token>& tokens) {
  size_t i = 0;
  return parse_ast(tokens, i);
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
  std::string operator()(const std::unique_ptr<Mul>& e) {
    std::cout << "Mul = (" << pp_aexpr(e->e1) << "; " << pp_aexpr(e->e2) << ")";
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

struct VisitAst {
  std::string operator()(const Skip&) {
    std::cout << "Skip";
    return "";
  }
  std::string operator()(const std::unique_ptr<Seq>& e) {
    std::cout << "Seq = (" << pp_ast(e->s1) << "; " << pp_ast(e->s2) << ")";
    return "";
  }
  std::string operator()(const std::unique_ptr<Assign>& e) {
    std::cout << "Assign = ( name = " << e->name << ", " << pp_aexpr(e->e) << ")";
    return "";
  }
  std::string operator()(const std::unique_ptr<Ifthenelse>& e) {
    std::cout << "IfthenElse = (" << pp_bexpr(e->b) << "; " << pp_ast(e->s1) << "; " << pp_ast(e->s2) << ")";
    return "";
  }
  std::string operator()(const std::unique_ptr<While>& e) {
    std::cout << "While = (" << pp_bexpr(e->b) << "; " << pp_ast(e->s) << ")";
    return "";
  }
};

std::string pp_aexpr(const AExpr& e) {
  return std::visit(VisitAExpr(), e);
}

std::string pp_bexpr(const BExpr& e) {
  return std::visit(VisitBExpr(), e);
}

std::string pp_ast(const Ast& s) {
  return std::visit(VisitAst(), s);
}






Store join_stores(const Store& s1, const Store& s2) {
  Store tmp = {};
  for (const auto& [name, interval] : s1) {
    auto iterator = s2.find(name);
    if (iterator == s2.end()) {
      tmp[name] = interval;
    } else {
      tmp[name] = join(interval, iterator->second);
    }
  }
  for (const auto& [name, interval] : s2) {
    auto iterator = tmp.find(name);
    if (iterator == tmp.end()) {
      tmp[name] = interval;
    }
  }
  return tmp;
}

bool is_included_in(const Interval& i1, const Interval& i2) {
  if (is_bottom(i2)) {return is_bottom(i1);}
  if (is_bottom(i1)) {return true;}
  bool ok_lower =
    i2.lower_bound.is_neg_inf()
    || (i1.lower_bound.is_finite()
        && i1.lower_bound.finite_value >= i2.lower_bound.finite_value);
  bool ok_higher = 
    i2.higher_bound.is_pos_inf()
    || (i1.higher_bound.is_finite()
        && i1.higher_bound.finite_value <= i2.higher_bound.finite_value);
  return ok_lower && ok_higher;
}

bool store_included_in(const Store& s1, const Store& s2) {
  for (const auto& [name, interval] : s1) {
    auto iterator = s2.find(name);
    if (iterator == s2.end()) { return false; }
    bool r = is_included_in(interval, iterator->second);
    if (!r) {return false;}
  }
  return true;
}

Store widen_stores(const Store& s1, const Store& s2) {
  Store tmp = {};
  for (const auto& [name, interval] : s1) {
    auto iterator = s2.find(name);
    if (iterator == s2.end()) {
      tmp[name] = interval;
    } else {
      tmp[name] = widen(interval, iterator->second);
    }
  }
  for (const auto& [name, interval] : s2) {
    auto iterator = tmp.find(name);
    if (iterator == tmp.end()) {
      tmp[name] = interval;
    }
  }
  return tmp;
}





Interval interval_of_aexpr(Store store, const AExpr& e);

struct Visit_interval_aexpr {
  Store store;
  Visit_interval_aexpr(Store s) : store(s) {}

  Interval operator()(const N& s) {
    return {Bound::finite(s.value), Bound::finite(s.value), false};
  }
  Interval operator()(const Var& s) {
    auto iterator = store.find(s.name);
    if (iterator == store.end()) { return BOT; }
    return iterator->second;
  }
  Interval operator()(const std::unique_ptr<Add>& s) {
    return add(interval_of_aexpr(store, s->e1), interval_of_aexpr(store, s->e2));
  }
  Interval operator()(const std::unique_ptr<Neg>& s) {
    return neg(interval_of_aexpr(store, s->e));
  }
  Interval operator()(const std::unique_ptr<Mul>& s) {
    const Var* v1 = std::get_if<Var>(&s->e1);
    const Var* v2 = std::get_if<Var>(&s->e2);
    if (v1 && v2 && v1->name == v2->name) {
      return square(interval_of_aexpr(store, s->e1));
    }
    return mul(interval_of_aexpr(store, s->e1), interval_of_aexpr(store, s->e2));
  }
};

Interval interval_of_aexpr(Store store, const AExpr& e) {
  return std::visit(Visit_interval_aexpr{store}, e);
}

Store store_of_ast (Store store, const Ast& ast);

struct Visit_interval_ast {
  Store store;
  Visit_interval_ast(Store s) : store(s) {}

  Store operator()(const Skip&) {
    return store;
  }
  Store operator()(const std::unique_ptr<Assign>& s) {
    store[s->name] = interval_of_aexpr(store, s->e);
    return store;
  }
  Store operator()(const std::unique_ptr<Seq>& s) {
    Store s1_store = store_of_ast(store, s->s1);
    return store_of_ast(s1_store, s->s2);
  }
  Store operator()(const std::unique_ptr<Ifthenelse>& s) {
    Store then_store = store_of_ast(store, s->s1);
    Store else_store = store_of_ast(store, s->s2);
    return join_stores(then_store, else_store);
  }
  Store operator()(const std::unique_ptr<While>& s) {
    Store s1 = store_of_ast(store, s->s);
    Store s_wide = widen_stores(store, s1);
    Store s_next;
    while(true) {
      s_next = store_of_ast(s_wide, s->s);
      if (store_included_in(s_next, s_wide)) {
        break;
      }
      s_wide = widen_stores(s_wide, s_next);
    }
    return s_wide; // fixpoint
  }
};

Store store_of_ast (Store store, const Ast& ast) {
  return std::visit(Visit_interval_ast{store}, ast);
}

Store get_store (const Ast& ast) {
  return store_of_ast({}, ast);
}

std::string store_to_string(const Store& store) {
  std::string res = "";
  for (const auto& [name, interval] : store) {
    res = res
      + name
      + " -> "
      + interval_to_string(interval)
      + "\n";
  }
  return res;
}