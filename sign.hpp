#pragma once
#include <string>

enum class Sign {
  TOP,
  POS,
  ZER,
  NEG,
  BOT,
};

std::string to_string (Sign s);

Sign add (Sign x, Sign y);

Sign join (Sign x, Sign y);

Sign negate (Sign x);
