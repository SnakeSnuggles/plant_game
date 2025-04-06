#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <iostream>

template<typename T>
class Manager {
private:
    static int current_id;
public:
    static int get() {
        return current_id++;
    }

    // Get a unique ID for a specific component type T
    template <typename U>
    static int get() {
        static int component_id = current_id++;
        return component_id;
    }
};

template<typename T>
int Manager<T>::current_id = 0;

struct EntityTag {};
struct ComponentTag {};

using Entity_Manager = Manager<EntityTag>;
using Component_Manager = Manager<ComponentTag>;

class Entity {
private:
    unsigned int ID;
    std::unordered_map<std::type_index, std::shared_ptr<void>> components;

public:
    static std::vector<std::shared_ptr<Entity>> Entities;
    static std::shared_ptr<Entity> create() {
        auto e = std::make_shared<Entity>();
        Entities.push_back(e);
        return e;
    }

    Entity() : ID(Entity_Manager::get()) {
        Entities.push_back(std::make_shared<Entity>(*this));
    }

    // Add a specific component to the entity
    template <typename T>
    void add_component(const T& component) {
        components[typeid(T)] = std::make_shared<T>(component);
    }

    // Check if the entity has a specific component
    template <typename T>
    bool has_component() const {
        //std::cout << (components.find(typeid(T)) != components.end()) << "\n";
        return components.find(typeid(T)) != components.end();
    }

    // Get the specific component
    template <typename T>
    T& get_component() {
        return *std::static_pointer_cast<T>(components.at(typeid(T)));
    }

    unsigned int get_id() const {
        return ID;
    }
};

// Initialize the static member of Entity
std::vector<std::shared_ptr<Entity>> Entity::Entities;

class System {
public:
    virtual void update() = 0;
    virtual ~System() = default;
};

class System_Manager {
private:
    static std::vector<std::shared_ptr<System>> systems;

public:
    static void add_system(std::shared_ptr<System> system) {
        systems.push_back(system);
    }

    static void update() {
        for (auto& sys : systems) {
            sys->update();
        }
    }
};

// Initialize the static member of System_Manager
std::vector<std::shared_ptr<System>> System_Manager::systems = {};

class Component {};
