#include "entity/Fish.hpp"
#include "entity/FishEntity.hpp"
#include "threepp/animation/AnimationMixer.hpp"
#include "threepp/cameras/PerspectiveCamera.hpp"
#include "threepp/canvas/Canvas.hpp"
#include "threepp/constants.hpp"
#include "threepp/controls/OrbitControls.hpp"
#include "threepp/core/Clock.hpp"
#include "threepp/geometries/BoxGeometry.hpp"
#include "threepp/geometries/PlaneGeometry.hpp"
#include "threepp/geometries/SphereGeometry.hpp"
#include "threepp/helpers/CameraSensor.hpp"
#include "threepp/helpers/LidarSensor.hpp"
#include "threepp/lights/HemisphereLight.hpp"
#include "threepp/loaders/AsyncGroup.hpp"
#include "threepp/loaders/GLTFLoader.hpp"
#include "threepp/loaders/ModelLoader.hpp"
#include "threepp/loaders/OBJLoader.hpp"
#include "threepp/loaders/TextureLoader.hpp"
#include "threepp/materials/MeshBasicMaterial.hpp"
#include "threepp/materials/MeshLambertMaterial.hpp"
#include "threepp/materials/MeshPhongMaterial.hpp"
#include "threepp/objects/Mesh.hpp"
#include "threepp/renderers/GLRenderer.hpp"
#include "threepp/scenes/Scene.hpp"
#include <iostream>
#include <memory>

using namespace threepp;

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
  auto &seagullModel = result->scene;
  seagullModel->scale.set(0.1f, 0.1f, 0.1f);
  seagullModel->traverseType<Mesh>([](Mesh &m) {
    m.castShadow = true;
    m.receiveShadow = true;
  });
  scene->add(seagullModel);

  std::vector<std::shared_ptr<Fish>> fishList;
  for (int i = 0; i < 5; i++) {
    auto fish = std::make_shared<Fish>();
    fish->setPosition(2.0f * static_cast<float>(i) - 4.0f, -0.5f, -1.0f);
    scene->add(fish->getMesh());
    fishList.push_back(fish);
  }

  auto sensorGeometry = SphereGeometry::create(0.05f);
  auto sensorMat = MeshBasicMaterial::create();
  sensorMat->color = Color::red;
  auto sensor = Mesh::create(sensorGeometry, sensorMat);
  sensor->position.set(0, 0.5f, 2.0f);
  seagullModel->add(sensor);

  auto cameraSensor =
      std::make_shared<threepp::CameraSensor>(60.f, 640, 480, 0.1f, 100.f);
  seagullModel->add(cameraSensor);

  AnimationMixer mixer(*seagullModel);
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
    if (cameraSensor) {
      cameraSensor->capture(renderer, *scene);
      auto image = cameraSensor->image();
    }
    if (fishList.size() > 0) {
      for (const auto &fish : fishList) {
        fish->biasedRandomMove(0, 0, 0.7);
        // fish->randomMove();
      }
    }

    renderer.render(*scene, *camera);
  });
}
