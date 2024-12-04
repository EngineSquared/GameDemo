#include "RaylibRenderer.hpp"

#include "BoxCollider3D.hpp"
#include "Camera3D.hpp"
#include "RealTimeProvider.hpp"
#include "RealTimeUpdater.hpp"
#include "SoftBodyCollision.hpp"
#include "SoftBodyNode.hpp"
#include "SoftBodySpring.hpp"
#include "Transform.hpp"
#include "VelocityIntegration.hpp"
#include "Physics.hpp"
#include "SoftBodyBox.hpp"

void Raylib::SoftBodySpringRenderer(ES::Engine::Registry &registry)
{
    // View of node springs
    auto springView = registry.GetRegistry().view<ES::Plugin::Physics::Component::SoftBodySpring>();

    for (auto entity : springView)
    {
        auto &spring = springView.get<ES::Plugin::Physics::Component::SoftBodySpring>(entity);
        auto &nodeA = registry.GetRegistry().get<ES::Plugin::Physics::Component::SoftBodyNode>(spring.entityNodeA);
        auto &nodeB = registry.GetRegistry().get<ES::Plugin::Physics::Component::SoftBodyNode>(spring.entityNodeB);
        auto &transformA = registry.GetRegistry().get<ES::Plugin::Object::Component::Transform>(spring.entityNodeA);
        auto &transformB = registry.GetRegistry().get<ES::Plugin::Object::Component::Transform>(spring.entityNodeB);

        Vector3 rbAvec = {transformA.position.x, transformA.position.y, transformA.position.z};
        Vector3 rbBvec = {transformB.position.x, transformB.position.y, transformB.position.z};

        DrawLine3D(rbAvec, rbBvec, RED);
    }
}

void Raylib::BoxColliderRenderer(ES::Engine::Registry &registry)
{
    // View of box colliders
    auto boxColliderView =
        registry.GetRegistry()
            .view<ES::Plugin::Physics::Component::BoxCollider3D, ES::Plugin::Object::Component::Transform>();

    for (auto entity : boxColliderView)
    {
        auto &boxCollider = boxColliderView.get<ES::Plugin::Physics::Component::BoxCollider3D>(entity);
        auto &transform = boxColliderView.get<ES::Plugin::Object::Component::Transform>(entity);

        Vector3 position = {transform.position.x, transform.position.y, transform.position.z};
        Vector3 size = {boxCollider.size.x, boxCollider.size.y, boxCollider.size.z};

        DrawCubeV(position, size, GRAY);
    }
}

void Raylib::FakeMeshRenderer(ES::Engine::Registry &registry)
{
    // View of fake meshes
    auto fakeMeshView = registry.GetRegistry().view<Objects::SoftBodyBoxMesh>();

    for (auto entity : fakeMeshView)
    {
        auto &fakeMesh = fakeMeshView.get<Objects::SoftBodyBoxMesh>(entity);

        if (fakeMesh.entities.size() < 3)
        {
            continue;
        }

        auto &transformA = registry.GetRegistry().get<ES::Plugin::Object::Component::Transform>(fakeMesh.entities[0]);
        auto &transformB = registry.GetRegistry().get<ES::Plugin::Object::Component::Transform>(fakeMesh.entities[1]);
        auto &transformC = registry.GetRegistry().get<ES::Plugin::Object::Component::Transform>(fakeMesh.entities[2]);

        Vector3 positionA = {transformA.position.x, transformA.position.y, transformA.position.z};
        Vector3 positionB = {transformB.position.x, transformB.position.y, transformB.position.z};
        Vector3 positionC = {transformC.position.x, transformC.position.y, transformC.position.z};

        DrawTriangle3D(positionA, positionB, positionC, GREEN);
        DrawTriangle3D(positionC, positionB, positionA, GREEN);
    }
}

void Raylib::GlobalRenderer(ES::Engine::Registry &registry)
{
    SoftBodySpringRenderer(registry);
    BoxColliderRenderer(registry);
    FakeMeshRenderer(registry);

    DrawGrid(50, 1.0f);
}