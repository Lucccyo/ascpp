#include "sign.hpp"
#include <iostream>

std::string to_string (Sign s) {
  switch(s) {
    case Sign::TOP: return "TOP";
    case Sign::POS: return "POS";
    case Sign::ZER: return "ZER";
    case Sign::NEG: return "NEG";
    case Sign::BOT: return "BOT";
  };
  return "";
};

// add
Sign add (Sign x, Sign y) {
  if (x == Sign::BOT || y == Sign::BOT) return Sign::BOT;
  if (x == Sign::TOP || y == Sign::TOP) return Sign::TOP;
  if (x == Sign::ZER && y == Sign::ZER) return Sign::ZER;
  if (x == Sign::POS && (y == Sign::POS || y == Sign::ZER)) return Sign::POS;
  if (y == Sign::POS && (x == Sign::POS || x == Sign::ZER)) return Sign::POS;
  if (x == Sign::POS && y == Sign::NEG) return Sign::TOP;
  if (y == Sign::POS && x == Sign::NEG) return Sign::TOP;
  return Sign::NEG;
};



// Union
Sign join (Sign x, Sign y) {
  if (x == Sign::BOT && y == Sign::BOT) return Sign::BOT;
  if (x == Sign::ZER && (y == Sign::ZER || y == Sign::BOT)) return Sign::ZER;
  if (y == Sign::ZER && (x == Sign::ZER || x == Sign::BOT)) return Sign::ZER;
  if (x == Sign::POS && (y == Sign::BOT || y == Sign::POS)) return Sign::POS;
  if (y == Sign::POS && (x == Sign::BOT || x == Sign::POS)) return Sign::POS;
  if (x == Sign::NEG && (y == Sign::BOT || y == Sign::NEG)) return Sign::NEG;
  if (y == Sign::NEG && (x == Sign::BOT || x == Sign::NEG)) return Sign::NEG;
  return Sign::TOP;
};


Sign negate (Sign x) {
  switch (x) {
    case Sign::BOT: return Sign::BOT;
    case Sign::ZER: return Sign::ZER;
    case Sign::POS: return Sign::NEG;
    case Sign::NEG: return Sign::POS;
    case Sign::TOP: return Sign::TOP;
  };
  return Sign::BOT;
};
