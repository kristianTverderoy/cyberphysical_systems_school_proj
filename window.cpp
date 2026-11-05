#include "threepp/geometries/BoxGeometry.hpp"
#include "threepp/geometries/CircleGeometry.hpp"
#include "threepp/loaders/OBJLoader.hpp"
#include "threepp/materials/MeshPhongMaterial.hpp"
#include "threepp/materials/MeshPhysicalMaterial.hpp"
#include "threepp/materials/interfaces.hpp"
#include "threepp/threepp.hpp"

using namespace threepp;

auto createBox(const Vector3 &pos, const Color &color) {
  auto geometry = BoxGeometry::create();
  auto material = MeshPhongMaterial::create();
  material->color = color;

  auto box = Mesh::create(geometry, material);
  box->position.copy(pos);

  return box;
}

auto createSphere(const Vector3 &pos, const Color &color) {
  auto geometry = SphereGeometry::create();
  auto material = MeshPhongMaterial::create();
  material->color = color;
  auto ball = Mesh::create(geometry, material);
  ball->position.copy(pos);
  return ball;
}

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

  Canvas canvas{"Demo"};
  GLRenderer renderer{canvas};

  auto scene = Scene::create();
  auto camera = PerspectiveCamera::create(75, canvas.aspect(), 0.1f, 100.f);
  camera->position.z = 10;

  OrbitControls controls{*camera, canvas};

  auto light = HemisphereLight::create();
  light->groundColor = Color::yellow;
  scene->add(light);

  auto plane = createPlane();
  scene->add(plane);

  auto group = Group::create();
  group->add(createBox({-1, 0, 0}, Color::red));
  group->add(createBox({1, 0, 0}, Color::green));
  group->add(createSphere({0, 0, 0}, Color::lightseagreen));
  scene->add(group);

  canvas.onWindowResize([&camera, &renderer](WindowSize size) {
    camera->aspect = size.aspect();
    camera->updateProjectionMatrix();
    renderer.setSize(size);
  });

  Clock clock;
  canvas.animate([&] {
    const auto dt = clock.getDelta();
    group->rotation.y += 10.f * dt;

    renderer.render(*scene, *camera);
  });
}
