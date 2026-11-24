
#include <cstdint>
#include <cstdlib>
#include <stdint.h>
class Seagull {
private:
  int32_t xPos;
  int32_t yPos;
  int32_t zPos;
  uint8_t hungerLevel;

public:
  int32_t getXPos() { return this->xPos; }
  int32_t getYPos() { return this->yPos; }
  int32_t getZPos() { return this->zPos; }
  uint8_t getHunderLevel() { return this->hungerLevel; }

  // TODO: This random move will probably look very weird, in regards to how a
  // seagul actually moves.
  /**
   * Makes the seagull move in a random direction in only the x and y axis.
   */
  void randomMoveWhileFlying() {
    int32_t xDirection = rand() % 101;
    int32_t yDirection = rand() % 101;

    const int xPosOrNegDirection = rand() % 2;
    const int yPosOrNegDirection = rand() % 2;

    this->xPos += (xDirection / 100) * xPosOrNegDirection;
    this->yPos += (yDirection / 100) * yPosOrNegDirection;
  }

  void diveForFishIfHungry() {
    if (hungerLevel <= 50) {
      const uint8_t diveSpeed = 2;
      zPos -= diveSpeed;
    }
  }
};
