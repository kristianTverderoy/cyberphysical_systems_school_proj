#include "../utility/RandomGenerator.hpp"
#include "threepp/core/Object3D.hpp"
#include "threepp/math/MathUtils.hpp"
#include "threepp/objects/Mesh.hpp"
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <stdint.h>

class Seagull {
private:
  std::shared_ptr<threepp::Object3D> model;
  double xPos;
  double yPos;
  double zPos;
  uint8_t hungerLevel;
  int32_t speed;
  double xDirection;
  double yDirection;
  double zDirection;
  RandomGenerator randomizer;

public:
  double getXPos() { return this->xPos; }
  double getYPos() { return this->yPos; }
  double getZPos() { return this->zPos; }
  uint8_t getHunderLevel() { return this->hungerLevel; }

  Seagull(double xPos = 0, double yPos = 0, double zPos = 0, double xDir = 0,
          double yDir = 0, double zDir = 0)
      : xPos(xPos), yPos(yPos), zPos(zPos), xDirection(xDir), yDirection(yDir),
        zDirection(zDir) {
    float len = sqrtf(xDirection * xDirection + yDirection * yDirection +
                      zDirection * zDirection);
    if (len > 0) {
      xDirection /= len;
      yDirection /= len;
      zDirection /= len;
    }
  };

  // TODO: This random move will probably look very weird, in regards to how a
  // seagul actually moves.
  /**
   * Makes the seagull move in a random direction in only the x and y axis.
   */
  void randomMoveWhileFlying() {
    double xDirection = randomizer.getRandomDouble(0, 100);
    double yDirection = randomizer.getRandomDouble(0, 100);

    const int xPosOrNegDirection = rand() % 2;
    const int yPosOrNegDirection = rand() % 2;

    this->xPos += (xDirection / 100) * xPosOrNegDirection;
    this->yPos += (yDirection / 100) * yPosOrNegDirection;
  }

  void flyCircularly() {
    const double deltaTheta = 0.01;
    const double cosDelta = cosf(deltaTheta);
    const double sinDelta = sinf(deltaTheta);

    if (xDirection == 0 && zDirection == 0) {
      xDirection = 0.0;
      zDirection = 1.0;
    }
    double newX = xDirection * cosDelta - zDirection * sinDelta;
    double newZ = xDirection * sinDelta + zDirection * cosDelta;

    double length = sqrtf(newX * newX + newZ * newZ);
    xDirection = newX / length;
    zDirection = newZ / length;

    const double stepSize = 0.1;
    xPos += xDirection * stepSize;
    zPos += zDirection * stepSize;

    if (model) {
      double angle = atan2(xDirection, zDirection);
      this->model->rotation.y = angle + 90.0;
    }
  }

  void diveForFishIfHungry() {
    if (hungerLevel <= 50) {
      const uint8_t diveSpeed = 2;
      zPos -= diveSpeed;
    }
  }

  void setXPos(double x) { this->xPos = x; }
  void setModel(std::shared_ptr<threepp::Object3D> obj) { this->model = obj; }
  std::shared_ptr<threepp::Object3D> getModel() { return this->model; }
};
