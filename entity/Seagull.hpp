#include "../utility/RandomGenerator.hpp"
#include "threepp/core/Object3D.hpp"
#include "threepp/math/MathUtils.hpp"
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <mutex>
#include <stdint.h>
#include <stop_token>
#include <thread>

class Seagull {
private:
  std::shared_ptr<threepp::Object3D> model;
  std::mutex positionMutex;
  std::function<void()> behaviour;
  double xPos;
  double yPos;
  double zPos;
  uint8_t hungerLevel;
  int32_t speed;
  double xDirection;
  double yDirection;
  double zDirection;
  RandomGenerator randomizer;
  std::jthread movementThread;

  void run(std::stop_token stop_token) {
    while (!stop_token.stop_requested()) {
      std::unique_lock<std::mutex> lock(positionMutex);
      auto currentBehaviour = behaviour;
      lock.unlock();
      currentBehaviour();
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
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

public:
  explicit Seagull(double xPos = 0, double yPos = 0, double zPos = 0,
                   double xDir = 0, double yDir = 0, double zDir = 0)
      : xPos(xPos), yPos(yPos), zPos(zPos), xDirection(xDir), yDirection(yDir),
        zDirection(zDir) {
    this->behaviour = []() {
      /* does nothing. Use setBehaviour to set the seagulls behaviour.*/
    };
    float len = sqrtf(xDirection * xDirection + yDirection * yDirection +
                      zDirection * zDirection);
    if (len > 0) {
      xDirection /= len;
      yDirection /= len;
      zDirection /= len;
    }
    this->movementThread =
        std::jthread([this](std::stop_token token) { run(token); });
  };

  enum class Behaviour { FlyCircularly };

  double getXPos() {
    std::lock_guard<std::mutex> lock(positionMutex);
    return this->xPos;
  }
  double getYPos() {
    std::lock_guard<std::mutex> lock(positionMutex);
    return this->yPos;
  }
  double getZPos() {
    std::lock_guard<std::mutex> lock(positionMutex);
    return this->zPos;
  }

  void setBehaviour(Behaviour b) {
    switch (b) {
    case Behaviour::FlyCircularly:
      behaviour = [this] { flyCircularly(); };
      break;
    }
  }

  uint8_t getHungerLevel() { return this->hungerLevel; }

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
