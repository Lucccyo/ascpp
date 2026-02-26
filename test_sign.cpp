#include "sign.hpp"
#include <iostream>
#include <cassert>

void test_add() {
  // BOT row
  assert(add(Sign::BOT, Sign::BOT) == Sign::BOT);
  assert(add(Sign::BOT, Sign::ZER) == Sign::BOT);
  assert(add(Sign::BOT, Sign::POS) == Sign::BOT);
  assert(add(Sign::BOT, Sign::NEG) == Sign::BOT);
  assert(add(Sign::BOT, Sign::TOP) == Sign::BOT);
  // ZER row
  assert(add(Sign::ZER, Sign::BOT) == Sign::BOT);
  assert(add(Sign::ZER, Sign::ZER) == Sign::ZER);
  assert(add(Sign::ZER, Sign::POS) == Sign::POS);
  assert(add(Sign::ZER, Sign::NEG) == Sign::NEG);
  assert(add(Sign::ZER, Sign::TOP) == Sign::TOP);
  // POS row
  assert(add(Sign::POS, Sign::BOT) == Sign::BOT);
  assert(add(Sign::POS, Sign::ZER) == Sign::POS);
  assert(add(Sign::POS, Sign::POS) == Sign::POS);
  assert(add(Sign::POS, Sign::NEG) == Sign::TOP);
  assert(add(Sign::POS, Sign::TOP) == Sign::TOP);
  // NEG row
  assert(add(Sign::NEG, Sign::BOT) == Sign::BOT);
  assert(add(Sign::NEG, Sign::ZER) == Sign::NEG);
  assert(add(Sign::NEG, Sign::POS) == Sign::TOP);
  assert(add(Sign::NEG, Sign::NEG) == Sign::NEG);
  assert(add(Sign::NEG, Sign::TOP) == Sign::TOP);
  // TOP row
  assert(add(Sign::TOP, Sign::BOT) == Sign::BOT);
  assert(add(Sign::TOP, Sign::ZER) == Sign::TOP);
  assert(add(Sign::TOP, Sign::POS) == Sign::TOP);
  assert(add(Sign::TOP, Sign::NEG) == Sign::TOP);
  assert(add(Sign::TOP, Sign::TOP) == Sign::TOP);
  std::cout << "[ADD OK]\n";
};

void test_join() {
  // BOT row
  assert(join(Sign::BOT, Sign::BOT) == Sign::BOT);
  assert(join(Sign::BOT, Sign::ZER) == Sign::ZER);
  assert(join(Sign::BOT, Sign::POS) == Sign::POS);
  assert(join(Sign::BOT, Sign::NEG) == Sign::NEG);
  assert(join(Sign::BOT, Sign::TOP) == Sign::TOP);
  // ZER row
  assert(join(Sign::ZER, Sign::BOT) == Sign::ZER);
  assert(join(Sign::ZER, Sign::ZER) == Sign::ZER);
  assert(join(Sign::ZER, Sign::POS) == Sign::TOP);
  assert(join(Sign::ZER, Sign::NEG) == Sign::TOP);
  assert(join(Sign::ZER, Sign::TOP) == Sign::TOP);
  // POS row
  assert(join(Sign::POS, Sign::BOT) == Sign::POS);
  assert(join(Sign::POS, Sign::ZER) == Sign::TOP);
  assert(join(Sign::POS, Sign::POS) == Sign::POS);
  assert(join(Sign::POS, Sign::NEG) == Sign::TOP);
  assert(join(Sign::POS, Sign::TOP) == Sign::TOP);
  // NEG row
  assert(join(Sign::NEG, Sign::BOT) == Sign::NEG);
  assert(join(Sign::NEG, Sign::ZER) == Sign::TOP);
  assert(join(Sign::NEG, Sign::POS) == Sign::TOP);
  assert(join(Sign::NEG, Sign::NEG) == Sign::NEG);
  assert(join(Sign::NEG, Sign::TOP) == Sign::TOP);
  // TOP row
  assert(join(Sign::TOP, Sign::BOT) == Sign::TOP);
  assert(join(Sign::TOP, Sign::ZER) == Sign::TOP);
  assert(join(Sign::TOP, Sign::POS) == Sign::TOP);
  assert(join(Sign::TOP, Sign::NEG) == Sign::TOP);
  assert(join(Sign::TOP, Sign::TOP) == Sign::TOP);
  std::cout << "[JOIN OK]\n";
};

void test_negate() {
  assert(negate(Sign::BOT) == Sign::BOT);
  assert(negate(Sign::ZER) == Sign::ZER);
  assert(negate(Sign::POS) == Sign::NEG);
  assert(negate(Sign::NEG) == Sign::POS);
  assert(negate(Sign::TOP) == Sign::TOP);
  std::cout << "[NEGATE OK]\n";
};

int main() {
  test_add();
  test_join();
  test_negate();
  return 0;
};
