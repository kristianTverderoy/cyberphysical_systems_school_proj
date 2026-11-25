#pragma once
#include "../utility/RandomGenerator.hpp"
#include "threepp/geometries/SphereGeometry.hpp"
#include "threepp/materials/MeshBasicMaterial.hpp"
#include "threepp/objects/Mesh.hpp"
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <stdint.h>
#include <string>

using namespace threepp;
/**
 * Represents a fish's internal state.
 * Has functions for moving the fish randomly, or explicitly.
 */
class Fish {

private:
  std::shared_ptr<threepp::Mesh> mesh;
  double xLocation;
  double yLocation;
  double zLocation;
  RandomGenerator randomizer;
  void updateMesh() {
    this->mesh->position.set(this->xLocation, this->yLocation, this->zLocation);
  }

public:
  explicit Fish(int32_t xPos = 0, int32_t yPos = 0, int32_t zPos = 0) {
    auto geometry = SphereGeometry::create(0.2f);
    auto material = MeshBasicMaterial::create();
    material->color = Color::yellow;
    this->mesh = Mesh::create(geometry, material);
    this->mesh->position.set(xPos, yPos, zPos);
  }
  int32_t getX() { return xLocation; }
  int32_t getY() { return yLocation; }
  int32_t getZ() { return zLocation; }

  /**
   * Updates the fish's location by a random value from -1 to 1 in x,y and z
   * directions. Move is accurate to the 100th decimal. Meaning the fish can
   * move in this range of numbers -1.000 -> 1.000
   */
  void randomMove() {
    double xDirectionLength = randomizer.getRandomDouble(-100, 100);
    double yDirectionLength = randomizer.getRandomDouble(-100, 100);
    double zDirectionLength = randomizer.getRandomDouble(-100, 100);
    if ((yLocation + (yDirectionLength / 100.0)) > 0) {
      yDirectionLength = -yDirectionLength;
    }
    this->xLocation += (xDirectionLength / 2000.0);
    this->yLocation += (yDirectionLength / 2000.0);
    this->zLocation += (zDirectionLength / 2000.0);
    updateMesh();
  }

  std::shared_ptr<Mesh> getMesh() { return this->mesh; }

  /**
   * Updates the fish's location by a random value from -1 to 1 in x,y and z
   * directions. The fish moves with higher probability towards the direction of
   * highest bias. Move is accurate to the 100th decimal. Meaning the fish can
   * move in this range of numbers -1.000 -> 1.000
   *
   * @input all values should be between -1 and 1.
   */
  void biasedRandomMove(double const xBias, double const yBias,
                        double const zBias) {
    if (!(xBias >= -1.0 && xBias <= 1.0) || !(yBias >= -1.0 && yBias <= 1.0) ||
        !(zBias >= -1.0 && zBias <= 1.0)) {
      throw std::invalid_argument(
          "One or more bias values were off. They should be between -1 and 1, "
          "but they were as follows: xBias: " +
          std::to_string(xBias) + ", yBias: " + std::to_string(yBias) +
          ", zBias: " + std::to_string(zBias) + ".");
    }

    // Step lengths are always positive here; the bias decides the sign
    double xDirectionLength = randomizer.getRandomDouble(0, 100);
    double yDirectionLength = randomizer.getRandomDouble(0, 100);
    double zDirectionLength = randomizer.getRandomDouble(0, 100);

    // bias -1 -> 0% positive, 0 -> 50%, +1 -> 100%
    if (randomizer.getRandomDouble(0, 1) >= (1.0 + xBias) / 2.0) {
      xDirectionLength = -xDirectionLength;
    }
    if (randomizer.getRandomDouble(0, 1) >= (1.0 + yBias) / 2.0) {
      yDirectionLength = -yDirectionLength;
    }
    if (randomizer.getRandomDouble(0, 1) >= (1.0 + zBias) / 2.0) {
      zDirectionLength = -zDirectionLength;
    }

    // Same constraint as your randomMove: don't let y go above 0
    if ((yLocation + (yDirectionLength / 100.0)) > 0) {
      yDirectionLength = -yDirectionLength;
    }

    this->xLocation += (xDirectionLength / 2000.0);
    this->yLocation += (yDirectionLength / 2000.0);
    this->zLocation += (zDirectionLength / 2000.0);
    updateMesh();
  }

  /**
   * Moves the fish by an explicit amount in the given directions.
   */
  void explicitDeltaMove(const int32_t x, const int32_t y, const int32_t z) {
    this->xLocation += x;
    this->yLocation += y;
    this->zLocation += z;
    updateMesh();
  }

  /**
   * Sets the fish's coordinates to the given coordinates.
   */
  void setPosition(const float x, const float y, const float z) {
    this->xLocation = x;
    this->yLocation = y;
    this->zLocation = z;
  }
};
