#include "threepp/animation/AnimationMixer.hpp"
#include "threepp/cameras/PerspectiveCamera.hpp"
#include "threepp/canvas/Canvas.hpp"
#include "threepp/constants.hpp"
#include "threepp/controls/OrbitControls.hpp"
#include "threepp/core/Clock.hpp"
#include "threepp/geometries/PlaneGeometry.hpp"
#include "threepp/lights/HemisphereLight.hpp"
#include "threepp/loaders/AsyncGroup.hpp"
#include "threepp/loaders/GLTFLoader.hpp"
#include "threepp/loaders/ModelLoader.hpp"
#include "threepp/loaders/OBJLoader.hpp"
#include "threepp/loaders/TextureLoader.hpp"
#include "threepp/materials/MeshLambertMaterial.hpp"
#include "threepp/materials/MeshPhongMaterial.hpp"
#include "threepp/objects/Mesh.hpp"
#include "threepp/renderers/GLRenderer.hpp"
#include "threepp/scenes/Scene.hpp"
#include <iostream>
#include <memory>

using namespace threepp;

// auto createBox(const Vector3 &pos, const Color &color) {
//   auto geometry = BoxGeometry::create();
//   auto material = MeshPhongMaterial::create();
//   material->color = color;
//
//   auto box = Mesh::create(geometry, material);
//   box->position.copy(pos);
//
//   return box;
// }

// auto createSphere(const Vector3 &pos, const Color &color) {
//   auto geometry = SphereGeometry::create();
//   auto material = MeshPhongMaterial::create();
//   material->color = color;
//   auto ball = Mesh::create(geometry, material);
//   ball->position.copy(pos);
//   return ball;
// }

auto createPlane() {
  auto planeGeometry = PlaneGeometry::create(5, 5);
  auto planeMaterial = MeshLambertMaterial::create();
  planeMaterial->color = Color::gray;
  planeMaterial->side = Side::Double;

  auto plane = Mesh::create(planeGeometry, planeMaterial);
  plane->position.y = -1;
  plane->rotateX(math::degToRad(90));

  return plane;
}

int main() {

  Canvas canvas{"Project"};
  GLRenderer renderer{canvas};
  renderer.shadowMap().enabled = true;

  auto scene = Scene::create();
  auto camera = PerspectiveCamera::create(75, canvas.aspect(), 0.1f, 100.f);
  camera->position.z = 10;

  OrbitControls controls{*camera, canvas};

  auto light = HemisphereLight::create();
  light->groundColor = Color::yellow;
  scene->add(light);

  auto plane = createPlane();
  scene->add(plane);
  // TextureLoader textureLoader;
  // auto texture = textureLoader.load(
  //     "./models/",
  //     ColorSpace::sRGB);

  GLTFLoader loader;
  auto result = loader.load("./3d/models/seagull.glb");
  if (!result) {
    std::cerr << "Failed to load model" << "\n";
    return 1;
  }
  auto &model = result->scene;
  model->traverseType<Mesh>([](Mesh &m) {
    m.castShadow = true;
    m.receiveShadow = true;
  });
  scene->add(model);

  AnimationMixer mixer(*model);
  std::cout << "Animations found: " << result->animations.size() << "\n";
  if (!result->animations.empty()) {
    mixer.clipAction(result->animations[0])->play();
  } else {
    std::cerr << "No animation in file\n";
  }

  canvas.onWindowResize([&camera, &renderer](WindowSize size) {
    camera->aspect = size.aspect();
    camera->updateProjectionMatrix();
    renderer.setSize(size);
  });

  Clock clock;
  canvas.animate([&] {
    const auto dt = clock.getDelta();
    mixer.update(dt);
    // group->rotation.y += 1.f * dt;

    renderer.render(*scene, *camera);
  });
}
