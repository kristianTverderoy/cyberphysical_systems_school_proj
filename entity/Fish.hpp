#pragma once
#include "threepp/geometries/SphereGeometry.hpp"
#include "threepp/materials/MeshBasicMaterial.hpp"
#include "threepp/objects/Mesh.hpp"

#include <cstdint>
#include <cstdlib>
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
  int32_t xLocation;
  int32_t yLocation;
  int32_t zLocation;
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
    int32_t xDirectionLength = rand() % 101;
    int32_t yDirectionLength = rand() % 101;
    int32_t zDirectionLength = rand() % 101;

    const int xPosOrNegDirection = rand() % 2;
    const int yPosOrNegDirection = rand() % 2;
    int zPosOrNegDirection = rand() % 2;

    if (xPosOrNegDirection == 1) {
      xDirectionLength = -xDirectionLength;
    }
    if (yPosOrNegDirection == 1) {
      yDirectionLength = -yDirectionLength;
    }
    if (zPosOrNegDirection == 1) {
      zDirectionLength = -zDirectionLength;
    }
    if ((zLocation + (zDirectionLength / 100) * zPosOrNegDirection) > 0) {
      zPosOrNegDirection = -zPosOrNegDirection;
    }

    this->xLocation += (xDirectionLength / 100) * xPosOrNegDirection;
    this->yLocation += (yDirectionLength / 100) * yPosOrNegDirection;
    this->zLocation += (zDirectionLength / 100) * zPosOrNegDirection;

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
  // TODO: This function doesnt use the biasees. Need to make that work.
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
    int32_t xDirectionLength = rand() % 101;
    int32_t yDirectionLength = rand() % 101;
    int32_t zDirectionLength = rand() % 101;

    // Let 1 == negative, and 2 == positive
    const int xPosOrNegDirection = rand() % 2;
    const int yPosOrNegDirection = rand() % 2;
    int zPosOrNegDirection = rand() % 2;

    if (xPosOrNegDirection == 1) {
      xDirectionLength = -xDirectionLength;
    }
    if (yPosOrNegDirection == 1) {
      yDirectionLength = -yDirectionLength;
    }
    if (zPosOrNegDirection == 1) {
      zDirectionLength = -zDirectionLength;
    }
    if ((zLocation + (zDirectionLength / 100) * zPosOrNegDirection) > 0) {
      zPosOrNegDirection = -zPosOrNegDirection;
    }

    this->xLocation += (xDirectionLength / 100) * xPosOrNegDirection;
    this->yLocation += (yDirectionLength / 100) * yPosOrNegDirection;
    this->zLocation += (zDirectionLength / 100) * zPosOrNegDirection;
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
