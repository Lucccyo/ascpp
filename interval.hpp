#include <string>
#include <optional>
#include <vector>

struct Bound {
  enum class Kind {NEG_INF, FINITE, POS_INF};
  Kind kind;
  int finite_value;

  static Bound neg_inf() {return {Kind::NEG_INF, 0};}
  static Bound pos_inf() {return {Kind::POS_INF, 0};}
  static Bound finite(int v) {return {Kind::FINITE, v};}

  bool is_neg_inf() const {return kind == Kind::NEG_INF;}
  bool is_pos_inf() const {return kind == Kind::POS_INF;}
  bool is_finite() const {return kind == Kind::FINITE;}
  bool is_positive() const {
    return is_pos_inf() || (is_finite() && finite_value > 0);
  }
};

struct Interval {
  Bound lower_bound;
  Bound higher_bound;
  bool is_bot;
};

inline const Interval BOT = {Bound::neg_inf(), Bound::pos_inf(), true};
inline const Interval TOP = {Bound::neg_inf(), Bound::pos_inf(), false};

bool is_bottom(const Interval& i);

bool is_top(const Interval& i);

Interval join(const Interval& i1, const Interval& i2);

Interval add(const Interval& i1, const Interval& i2);

Interval neg(const Interval& i);

Interval mul(const Interval& i1, const Interval& i2);

Interval square(const Interval& i);

Interval widen(const Interval& i1, const Interval& i2);

std::string interval_to_string(const Interval& i);

std::string bound_to_string(const Bound& b);
