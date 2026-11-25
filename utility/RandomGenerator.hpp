#pragma once
#include <random>

class RandomGenerator {
private:
  std::mt19937 rng;

public:
  RandomGenerator() : rng(std::random_device{}()) {}

  double getRandomDouble(double min, double max) {
    std::uniform_real_distribution<double> dist(min, max);
    return dist(rng);
  }

  int getRandomInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
  }
};
