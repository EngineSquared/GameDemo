#include "SoftBodyBox.hpp"

#include "SoftBodyCollision.hpp"
#include "SoftBodyNode.hpp"
#include "SoftBodySpring.hpp"
#include "Transform.hpp"

void Objects::SoftBodyBox::RotateFromPitchYawRoll(ES::Engine::Registry &registry, float pitch, float yaw, float roll)
{
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(pitch), glm::vec3(1, 0, 0));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(yaw), glm::vec3(0, 1, 0));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(roll), glm::vec3(0, 0, 1));

    for (auto entity : entities)
    {
        auto &transform = registry.GetRegistry().get<ES::Plugin::Object::Component::Transform>(entity);
        glm::vec3 position = transform.position;
        glm::vec3 rotatedPosition = glm::vec3(rotationMatrix * glm::vec4(position, 1));
        transform.position = rotatedPosition;
    }
}

glm::vec3 Objects::SoftBodyBox::GetMiddlePoint(ES::Engine::Registry &registry) const
{
    glm::vec3 middlePoint(0.f);

    for (auto entity : entities)
    {
        auto &transform = registry.GetRegistry().get<ES::Plugin::Object::Component::Transform>(entity);
        middlePoint += transform.position;
    }

    middlePoint /= entities.size();

    return middlePoint;
}

glm::vec3 Objects::SoftBodyBox::GetVelocity(ES::Engine::Registry &registry) const
{
    glm::vec3 velocity(0.f);

    for (auto entity : entities)
    {
        auto &node = registry.GetRegistry().get<ES::Plugin::Physics::Component::SoftBodyNode>(entity);
        velocity += node.velocity;
    }

    velocity /= entities.size();

    return velocity;
}

static ES::Engine::Entity CreateParticle(ES::Engine::Registry &registry, const glm::vec3 &position, float mass = 1.f,
                                         float damping = 0.999f, float friction = 10.f, float elasticity = 0.1f)
{
    ES::Engine::Entity entity = registry.CreateEntity();

    registry.GetRegistry().emplace<ES::Plugin::Object::Component::Transform>(entity, position);
    registry.GetRegistry().emplace<ES::Plugin::Physics::Component::SoftBodyNode>(entity, mass, damping, friction, elasticity);

    return entity;
}

static ES::Engine::Entity CreateSpring(ES::Engine::Registry &registry, ES::Engine::Entity nodeA, ES::Engine::Entity nodeB,
                                       float stiffness = 1, float damping = 0.99f, float restLength = 1)
{
    ES::Engine::Entity entity = registry.CreateEntity();

    registry.GetRegistry().emplace<ES::Plugin::Physics::Component::SoftBodySpring>(entity, nodeA, nodeB, stiffness,
                                                                                   damping, restLength);

    return entity;
}

static ES::Engine::Entity CreateBoxFakeMesh(ES::Engine::Registry &registry, std::array<ES::Engine::Entity, 3> particles)
{
    ES::Engine::Entity entity = registry.CreateEntity();
    Objects::SoftBodyBoxMesh mesh {particles};

    registry.GetRegistry().emplace<Objects::SoftBodyBoxMesh>(entity, mesh);

    return entity;
}

static std::vector<ES::Engine::Entity> CreateBoxParticles(ES::Engine::Registry &registry, const glm::vec3 &position,
                                                          const glm::vec3 &size)
{
    std::vector<ES::Engine::Entity> entities;
    entities.reserve(8);

    // Create particles for each corner of the box
    entities.push_back(CreateParticle(registry, position + glm::vec3(-size.x, -size.y, -size.z) / 2.f));
    entities.push_back(CreateParticle(registry, position + glm::vec3(size.x, -size.y, -size.z) / 2.f));
    entities.push_back(CreateParticle(registry, position + glm::vec3(size.x, -size.y, size.z) / 2.f));
    entities.push_back(CreateParticle(registry, position + glm::vec3(-size.x, -size.y, size.z) / 2.f));
    entities.push_back(CreateParticle(registry, position + glm::vec3(-size.x, size.y, -size.z) / 2.f));
    entities.push_back(CreateParticle(registry, position + glm::vec3(size.x, size.y, -size.z) / 2.f));
    entities.push_back(CreateParticle(registry, position + glm::vec3(size.x, size.y, size.z) / 2.f));
    entities.push_back(CreateParticle(registry, position + glm::vec3(-size.x, size.y, size.z) / 2.f));

    float stiffness = 1000.f;
    float damping = 0.95f;

    // Create springs for the bottom face
    CreateSpring(registry, entities[0], entities[1], stiffness, damping, size.x);
    CreateSpring(registry, entities[1], entities[2], stiffness, damping, size.z);
    CreateSpring(registry, entities[2], entities[3], stiffness, damping, size.x);
    CreateSpring(registry, entities[3], entities[0], stiffness, damping, size.z);

    // Create springs for the top face
    CreateSpring(registry, entities[4], entities[5], stiffness, damping, size.x);
    CreateSpring(registry, entities[5], entities[6], stiffness, damping, size.z);
    CreateSpring(registry, entities[6], entities[7], stiffness, damping, size.x);
    CreateSpring(registry, entities[7], entities[4], stiffness, damping, size.z);

    // Create vertical springs
    CreateSpring(registry, entities[0], entities[4], stiffness, damping, size.y);
    CreateSpring(registry, entities[1], entities[5], stiffness, damping, size.y);
    CreateSpring(registry, entities[2], entities[6], stiffness, damping, size.y);
    CreateSpring(registry, entities[3], entities[7], stiffness, damping, size.y);

    // 6 Face diagonal springs, mesh-like
    CreateSpring(registry, entities[0], entities[2], stiffness, damping, glm::sqrt(size.x * size.x + size.z * size.z));
    CreateSpring(registry, entities[1], entities[3], stiffness, damping, glm::sqrt(size.x * size.x + size.z * size.z));
    CreateSpring(registry, entities[4], entities[6], stiffness, damping, glm::sqrt(size.x * size.x + size.z * size.z));
    CreateSpring(registry, entities[5], entities[7], stiffness, damping, glm::sqrt(size.x * size.x + size.z * size.z));
    CreateSpring(registry, entities[0], entities[5], stiffness, damping, glm::sqrt(size.x * size.x + size.y * size.y));
    CreateSpring(registry, entities[1], entities[4], stiffness, damping, glm::sqrt(size.x * size.x + size.y * size.y));
    CreateSpring(registry, entities[2], entities[7], stiffness, damping, glm::sqrt(size.x * size.x + size.y * size.y));
    CreateSpring(registry, entities[3], entities[6], stiffness, damping, glm::sqrt(size.x * size.x + size.y * size.y));
    CreateSpring(registry, entities[0], entities[7], stiffness, damping, glm::sqrt(size.z * size.z + size.y * size.y));
    CreateSpring(registry, entities[1], entities[6], stiffness, damping, glm::sqrt(size.z * size.z + size.y * size.y));
    CreateSpring(registry, entities[2], entities[5], stiffness, damping, glm::sqrt(size.z * size.z + size.y * size.y));
    CreateSpring(registry, entities[3], entities[4], stiffness, damping, glm::sqrt(size.z * size.z + size.y * size.y));

    // Create fake meshes
    CreateBoxFakeMesh(registry, {entities[0], entities[1], entities[2]});
    CreateBoxFakeMesh(registry, {entities[2], entities[3], entities[0]});
    CreateBoxFakeMesh(registry, {entities[4], entities[5], entities[6]});
    CreateBoxFakeMesh(registry, {entities[6], entities[7], entities[4]});
    CreateBoxFakeMesh(registry, {entities[0], entities[4], entities[5]});
    CreateBoxFakeMesh(registry, {entities[5], entities[1], entities[0]});
    CreateBoxFakeMesh(registry, {entities[2], entities[6], entities[7]});
    CreateBoxFakeMesh(registry, {entities[7], entities[3], entities[2]});
    CreateBoxFakeMesh(registry, {entities[0], entities[2], entities[7]});
    CreateBoxFakeMesh(registry, {entities[7], entities[4], entities[0]});
    CreateBoxFakeMesh(registry, {entities[1], entities[5], entities[6]});
    CreateBoxFakeMesh(registry, {entities[6], entities[2], entities[1]});

    return entities;
}

ES::Engine::Entity Objects::CreateSoftBodyBox(ES::Engine::Registry &registry, const glm::vec3 &position, const glm::vec3 &size)
{
    auto particles = CreateBoxParticles(registry, position, size);
    ES::Engine::Entity entity = registry.CreateEntity();

    registry.GetRegistry().emplace<Objects::SoftBodyBox>(entity, particles);

    return entity;
}
