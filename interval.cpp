#include "interval.hpp"
#include <iostream>
#include <algorithm>

bool is_bottom(const Interval& i) {
  return i.is_bot;
}

bool is_top(const Interval& i) {
  return !i.is_bot && i.lower_bound.is_neg_inf() && i.higher_bound.is_pos_inf();
 }

Bound min_bound(const Bound& b1, const Bound& b2) {
  if (b1.is_neg_inf() || b2.is_neg_inf()) {return Bound::neg_inf();}
  if (b1.is_pos_inf()) {return b2;}
  if (b2.is_pos_inf()) {return b1;}
  return Bound::finite(std::min(b1.finite_value, b2.finite_value));
}

Bound max_bound(const Bound& b1, const Bound& b2) {
  if (b1.is_pos_inf() || b2.is_pos_inf()) {return Bound::pos_inf();}
  if (b1.is_neg_inf()) {return b2;}
  if (b2.is_neg_inf()) {return b1;}
  return Bound::finite(std::max(b1.finite_value, b2.finite_value));
}

Interval join(const Interval& i1, const Interval& i2) {
  if (is_bottom(i1)) {return i2;}
  if (is_bottom(i2)) {return i1;}
  return {min_bound(i1.lower_bound, i2.lower_bound), max_bound(i1.higher_bound, i2.higher_bound), false};
}

Bound add_lower(const Bound& b1, const Bound& b2) {
  if (b1.is_neg_inf() || b2.is_neg_inf()) {return Bound::neg_inf();}
  return Bound::finite(b1.finite_value + b2.finite_value);
}

Bound add_upper(const Bound& b1, const Bound& b2) {
  if (b1.is_pos_inf() || b2.is_pos_inf()) {return Bound::pos_inf();}
  return Bound::finite(b1.finite_value + b2.finite_value);
}

Interval add(const Interval& i1, const Interval& i2) {
  if (is_bottom(i1) || is_bottom(i2)) {return BOT;}
  return {add_lower(i1.lower_bound, i2.lower_bound), add_upper(i1.higher_bound, i2.higher_bound), false};
}

Bound neg_bound(const Bound& b) {
  if (b.is_neg_inf()) {return Bound::pos_inf();}
  if (b.is_pos_inf()) {return Bound::neg_inf();}
  return Bound::finite(-b.finite_value);
}

Interval neg(const Interval& i) {
  if (is_bottom(i)) {return i;}
  return {neg_bound(i.higher_bound), neg_bound(i.lower_bound), false};
}



Bound mul_bound(const Bound& b1, const Bound& b2) {
  if (b1.is_finite() && b1.finite_value == 0) return Bound::finite(0);
  if (b2.is_finite() && b2.finite_value == 0) return Bound::finite(0);
  if (!b1.is_finite() || !b2.is_finite()) {
    return (b1.is_positive() == b2.is_positive())
      ? Bound::pos_inf()
      : Bound::neg_inf();
  }
  return Bound::finite(b1.finite_value * b2.finite_value);
}

bool contains_zero(const Interval& i) {
  bool lower_ok = i.lower_bound.is_neg_inf()
    || (i.lower_bound.is_finite() && i.lower_bound.finite_value <= 0);
  bool higher_ok = i.higher_bound.is_pos_inf()
    || (i.higher_bound.is_finite() && i.higher_bound.finite_value >= 0);
  return lower_ok && higher_ok;
}

Bound square_bound(const Bound& b) {
  if (!b.is_finite()) return Bound::pos_inf();
  return Bound::finite(b.finite_value * b.finite_value);
}

Interval square(const Interval& i) {
  if (is_bottom(i)) return BOT;
  Bound sq_lo = square_bound(i.lower_bound);
  Bound sq_hi = square_bound(i.higher_bound);
  if (contains_zero(i)) {
    return {Bound::finite(0), max_bound(sq_lo, sq_hi), false};
  } else {
    return {min_bound(sq_lo, sq_hi), max_bound(sq_lo, sq_hi), false};
  }
}

Interval mul(const Interval& i1, const Interval& i2) {
  Bound ac = mul_bound(i1.lower_bound, i2.lower_bound);
  // std::cout << "ac" << bound_to_string(ac) << '\n';
  Bound ad = mul_bound(i1.lower_bound, i2.higher_bound);
  // std::cout << "ad" << bound_to_string(ad) << '\n';
  Bound bc = mul_bound(i1.higher_bound, i2.lower_bound);
  // std::cout << "bc" << bound_to_string(bc) << '\n';
  Bound bd = mul_bound(i1.higher_bound, i2.higher_bound);
  // std::cout << "bd" << bound_to_string(bd) << '\n';
  // Interval mul = {min_bound(min_bound(ac, ad), min_bound(bc, bd)), max_bound(max_bound(ac, ad), max_bound(bc, bd)), false};
  // std::cout << "mul interval" << interval_to_string(mul) << '\n';
  return {min_bound(min_bound(ac, ad), min_bound(bc, bd)), max_bound(max_bound(ac, ad), max_bound(bc, bd)), false};
}

Bound widen_lower(const Bound& b1, const Bound& b2) {
  if (b1.is_neg_inf() || b2.is_neg_inf()) { return Bound::neg_inf(); }
  if (b2.finite_value < b1.finite_value) {
    return Bound::neg_inf(); }
  else { return b1; }
}

Bound widen_higher(const Bound& b1, const Bound& b2) {
  if (b1.is_pos_inf() || b2.is_pos_inf()) { return Bound::pos_inf(); }
  if (b2.finite_value > b1.finite_value) { return Bound::pos_inf(); }
  else { return b1; }
}

Interval widen(const Interval& i1, const Interval& i2) {
  if (is_bottom(i1)) { return i2; }
  if (is_bottom(i2)) { return i1; }
  return { widen_lower(i1.lower_bound, i2.lower_bound), widen_higher(i1.higher_bound, i2.higher_bound), false};
}

std::string bound_to_string(const Bound& b) {
  if (b.is_neg_inf()) {return "-inf";}
  if (b.is_pos_inf()) {return "+inf";}
  return std::to_string(b.finite_value);
}

std::string interval_to_string(const Interval& i) {
  return "["
    + bound_to_string(i.lower_bound)
    + "; "
    + bound_to_string(i.higher_bound)
    + "]";
}