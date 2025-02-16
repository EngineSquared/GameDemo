#include "SoftBodyBox.hpp"

#include "SoftBodyCollision.hpp"
#include "SoftBodyNode.hpp"
#include "SoftBodySpring.hpp"
#include "Transform.hpp"

void Objects::SoftBodyBox::RotateFromPitchYawRoll(ES::Engine::Core &core, float pitch, float yaw, float roll)
{
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(pitch), glm::vec3(1, 0, 0));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(yaw), glm::vec3(0, 1, 0));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(roll), glm::vec3(0, 0, 1));

    for (auto entity : entities)
    {
        auto &transform = core.GetRegistry().get<ES::Plugin::Object::Component::Transform>(entity);
        glm::vec3 position = transform.position;
        glm::vec3 rotatedPosition = glm::vec3(rotationMatrix * glm::vec4(position, 1));
        transform.position = rotatedPosition;
    }
}

glm::vec3 Objects::SoftBodyBox::GetMiddlePoint(ES::Engine::Core &core) const
{
    glm::vec3 middlePoint(0.f);

    for (auto entity : entities)
    {
        auto &transform = core.GetRegistry().get<ES::Plugin::Object::Component::Transform>(entity);
        middlePoint += transform.position;
    }

    middlePoint /= entities.size();

    return middlePoint;
}

glm::vec3 Objects::SoftBodyBox::GetVelocity(ES::Engine::Core &core) const
{
    glm::vec3 velocity(0.f);

    for (auto entity : entities)
    {
        auto &node = core.GetRegistry().get<ES::Plugin::Physics::Component::SoftBodyNode>(entity);
        velocity += node.velocity;
    }

    velocity /= entities.size();

    return velocity;
}

static ES::Engine::Entity CreateParticle(ES::Engine::Core &core, const glm::vec3 &position, float mass = 1.f,
                                         float damping = 0.999f, float friction = 8.f, float elasticity = 0.02f)
{
    ES::Engine::Entity entity = core.CreateEntity();

    core.GetRegistry().emplace<ES::Plugin::Object::Component::Transform>(entity, position);
    core.GetRegistry().emplace<ES::Plugin::Physics::Component::SoftBodyNode>(entity, mass, damping, friction, elasticity);

    return entity;
}

static ES::Engine::Entity CreateSpring(ES::Engine::Core &core, ES::Engine::Entity nodeA, ES::Engine::Entity nodeB,
                                       float stiffness = 3.f, float damping = 0.99f, float restLength = 1)
{
    ES::Engine::Entity entity = core.CreateEntity();

    core.GetRegistry().emplace<ES::Plugin::Physics::Component::SoftBodySpring>(entity, nodeA, nodeB, stiffness,
                                                                                   damping, restLength);

    return entity;
}

static ES::Engine::Entity CreateBoxFakeMesh(ES::Engine::Core &core, std::array<ES::Engine::Entity, 3> particles)
{
    ES::Engine::Entity entity = core.CreateEntity();
    Objects::SoftBodyBoxMesh mesh {particles};

    core.GetRegistry().emplace<Objects::SoftBodyBoxMesh>(entity, mesh);

    return entity;
}

static std::vector<ES::Engine::Entity> CreateBoxParticles(ES::Engine::Core &core, const glm::vec3 &position,
                                                          const glm::vec3 &size)
{
    std::vector<ES::Engine::Entity> entities;
    entities.reserve(8);

    // Create particles for each corner of the box
    entities.push_back(CreateParticle(core, position + glm::vec3(-size.x, -size.y, -size.z) / 2.f));
    entities.push_back(CreateParticle(core, position + glm::vec3(size.x, -size.y, -size.z) / 2.f));
    entities.push_back(CreateParticle(core, position + glm::vec3(size.x, -size.y, size.z) / 2.f));
    entities.push_back(CreateParticle(core, position + glm::vec3(-size.x, -size.y, size.z) / 2.f));
    entities.push_back(CreateParticle(core, position + glm::vec3(-size.x, size.y, -size.z) / 2.f));
    entities.push_back(CreateParticle(core, position + glm::vec3(size.x, size.y, -size.z) / 2.f));
    entities.push_back(CreateParticle(core, position + glm::vec3(size.x, size.y, size.z) / 2.f));
    entities.push_back(CreateParticle(core, position + glm::vec3(-size.x, size.y, size.z) / 2.f));

    float stiffness = 1000.f;
    float damping = 0.95f;

    // Create springs for the bottom face
    CreateSpring(core, entities[0], entities[1], stiffness, damping, size.x);
    CreateSpring(core, entities[1], entities[2], stiffness, damping, size.z);
    CreateSpring(core, entities[2], entities[3], stiffness, damping, size.x);
    CreateSpring(core, entities[3], entities[0], stiffness, damping, size.z);

    // Create springs for the top face
    CreateSpring(core, entities[4], entities[5], stiffness, damping, size.x);
    CreateSpring(core, entities[5], entities[6], stiffness, damping, size.z);
    CreateSpring(core, entities[6], entities[7], stiffness, damping, size.x);
    CreateSpring(core, entities[7], entities[4], stiffness, damping, size.z);

    // Create vertical springs
    CreateSpring(core, entities[0], entities[4], stiffness, damping, size.y);
    CreateSpring(core, entities[1], entities[5], stiffness, damping, size.y);
    CreateSpring(core, entities[2], entities[6], stiffness, damping, size.y);
    CreateSpring(core, entities[3], entities[7], stiffness, damping, size.y);

    // 6 Face diagonal springs, mesh-like
    CreateSpring(core, entities[0], entities[2], stiffness, damping, glm::sqrt(size.x * size.x + size.z * size.z));
    CreateSpring(core, entities[1], entities[3], stiffness, damping, glm::sqrt(size.x * size.x + size.z * size.z));
    CreateSpring(core, entities[4], entities[6], stiffness, damping, glm::sqrt(size.x * size.x + size.z * size.z));
    CreateSpring(core, entities[5], entities[7], stiffness, damping, glm::sqrt(size.x * size.x + size.z * size.z));
    CreateSpring(core, entities[0], entities[5], stiffness, damping, glm::sqrt(size.x * size.x + size.y * size.y));
    CreateSpring(core, entities[1], entities[4], stiffness, damping, glm::sqrt(size.x * size.x + size.y * size.y));
    CreateSpring(core, entities[2], entities[7], stiffness, damping, glm::sqrt(size.x * size.x + size.y * size.y));
    CreateSpring(core, entities[3], entities[6], stiffness, damping, glm::sqrt(size.x * size.x + size.y * size.y));
    CreateSpring(core, entities[0], entities[7], stiffness, damping, glm::sqrt(size.z * size.z + size.y * size.y));
    CreateSpring(core, entities[1], entities[6], stiffness, damping, glm::sqrt(size.z * size.z + size.y * size.y));
    CreateSpring(core, entities[2], entities[5], stiffness, damping, glm::sqrt(size.z * size.z + size.y * size.y));
    CreateSpring(core, entities[3], entities[4], stiffness, damping, glm::sqrt(size.z * size.z + size.y * size.y));

    // Create fake meshes
    CreateBoxFakeMesh(core, {entities[0], entities[1], entities[2]});
    CreateBoxFakeMesh(core, {entities[2], entities[3], entities[0]});
    CreateBoxFakeMesh(core, {entities[4], entities[5], entities[6]});
    CreateBoxFakeMesh(core, {entities[6], entities[7], entities[4]});
    CreateBoxFakeMesh(core, {entities[0], entities[4], entities[5]});
    CreateBoxFakeMesh(core, {entities[5], entities[1], entities[0]});
    CreateBoxFakeMesh(core, {entities[2], entities[6], entities[7]});
    CreateBoxFakeMesh(core, {entities[7], entities[3], entities[2]});
    CreateBoxFakeMesh(core, {entities[0], entities[2], entities[7]});
    CreateBoxFakeMesh(core, {entities[7], entities[4], entities[0]});
    CreateBoxFakeMesh(core, {entities[1], entities[5], entities[6]});
    CreateBoxFakeMesh(core, {entities[6], entities[2], entities[1]});

    return entities;
}

ES::Engine::Entity Objects::CreateSoftBodyBox(ES::Engine::Core &core, const glm::vec3 &position, const glm::vec3 &size)
{
    auto particles = CreateBoxParticles(core, position, size);
    ES::Engine::Entity entity = core.CreateEntity();

    core.GetRegistry().emplace<Objects::SoftBodyBox>(entity, particles);

    return entity;
}
