
#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <stdint.h>
#include <string>

/**
 * Represents a fish's internal state.
 * Has functions for moving the fish randomly, or explicitly.
 */
class Fish {

private:
  int32_t xLocation;
  int32_t yLocation;
  int32_t zLocation;

public:
  int32_t getX() { return xLocation; }
  int32_t getY() { return yLocation; }
  int32_t getZ() { return zLocation; }

  /**
   * Updates the fish's location by a random value from -1 to 1 in x,y and z
   * directions. Move is accurate to the 100th decimal. Meaning the fish can
   * move in this range of numbers -1.000 -> 1.000
   */
  void randomMove() {
    int32_t xDirection = rand() % 101;
    int32_t yDirection = rand() % 101;
    int32_t zDirection = rand() % 101;

    const int xPosOrNegDirection = rand() % 2;
    const int yPosOrNegDirection = rand() % 2;
    const int zPosOrNegDirection = rand() % 2;

    this->xLocation += (xDirection / 100) * xPosOrNegDirection;
    this->yLocation += (yDirection / 100) * yPosOrNegDirection;
    this->zLocation += (zDirection / 100) * zPosOrNegDirection;
  }

  /**
   * Updates the fish's location by a random value from -1 to 1 in x,y and z
   * directions. The fish moves with higher probability towards the direction of
   * highest bias. Move is accurate to the 100th decimal. Meaning the fish can
   * move in this range of numbers -1.000 -> 1.000
   */
  void biasedRandomMove(float const xBias, float const yBias,
                        float const zBias) {
    if (xBias < -1 || xBias > 1 || yBias < -1 || yBias > 1 || zBias < -1 ||
        zBias > 1) {
      throw std::invalid_argument(
          "One or more bias values were off. They should be between -1 and 1, "
          "but they were as follows: xBias: " +
          std::to_string(xBias) + ", yBias: " + std::to_string(yBias) +
          ", zBias: " + std::to_string(zBias) + ".");
    }
    const int32_t xDirectionLength = rand() % 101;
    const int32_t yDirectionLength = rand() % 101;
    const int32_t zDirectionLength = rand() % 101;

    const int xPosOrNegDirection = rand() % 2;
    const int yPosOrNegDirection = rand() % 2;
    const int zPosOrNegDirection = rand() % 2;

    this->xLocation += (xDirectionLength / 100) * xPosOrNegDirection;
    this->yLocation += (yDirectionLength / 100) * yPosOrNegDirection;
    this->zLocation += (zDirectionLength / 100) * zPosOrNegDirection;
  }

  /**
   * Moves the fish by an explicit amount in the given directions.
   */
  void explicitDeltaMove(const int32_t x, const int32_t y, const int32_t z) {
    this->xLocation += x;
    this->yLocation += y;
    this->zLocation += z;
  }
};
