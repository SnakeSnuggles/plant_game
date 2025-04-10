#include "../Engine/engine.hpp"

struct Position {
    float x, y, z;
};
struct Speed {float speed = 1.0;};
class sine_move : public System {
    private:
        std::shared_ptr<Entity> entity_;
    public:
        sine_move(std::shared_ptr<Entity> ent): entity_(ent) {}
        void update(float delta_time) override {
            float time = glfwGetTime();
            entity_->get_component<Position>().x = sin(time);
            //std::cout << sprite_->x << "\n";
        } 
};
class move_to_pos : public System {
    void update(float delta_time) override {
        auto ents = get_all_with_component<Sprite, Position>();

        for(auto ent : ents) {
            Sprite& sprite = ent->get_component<Sprite>();
            Position& pos = ent->get_component<Position>();
            sprite.x = pos.x;
            sprite.y = pos.y;
            sprite.z = pos.z;
        }

    }
};
class player_inputs : public System {
    private:
        GLFWwindow* window;
        std::shared_ptr<Entity> entity;
    public:
        player_inputs(GLFWwindow* win,std::shared_ptr<Entity> ent) : window(win), entity(ent){}
        void update(float delta_time) override {
            glm::vec3 direction{0.0f,0.0f,0.0f};
            float speed = entity->get_component<Speed>().speed;
            if(glfwGetKey(window,GLFW_KEY_W)) {
                direction.y += 1.0f;
            }
            if(glfwGetKey(window,GLFW_KEY_S)) {
                direction.y -= 1.0f;
            }
            if(glfwGetKey(window,GLFW_KEY_D)) {
                direction.x += 1.0f;
            }
            if(glfwGetKey(window,GLFW_KEY_A)) {
                direction.x -= 1.0f;
            }
            if(glm::length(direction) > 0.0f) {
                direction = glm::normalize(direction);
            }

            entity->get_component<Position>().x += direction.x * speed * delta_time;
            entity->get_component<Position>().y += direction.y * speed * delta_time;
            
        }
};

class delta : public System {
    public:
        void update(float delta_time) override {
//            std::cout << delta_time << "\n";
            glfwGetTime();
        }
};

class time_rotate : public System {
    private:
        std::shared_ptr<Entity> entity_;
        bool done = false;
    public:
        time_rotate(std::shared_ptr<Entity> ent): entity_(ent) {}
        void update(float delta_time) override {
            if(done) {}
                //return;

            float time = glfwGetTime();

            entity_->get_component<Sprite>().set_rotation(glm::vec3(0,time * 200,0));
            std::cout << "time: " << time << "\n";
            done = true;
        }
};

int main() {
    App game{"plant game"};
    
    auto fellow2 = Entity::create();
    auto player = Entity::create();
    
    // Create Sprite and add it as a component to the entity


    Sprite sprite2{"wall.jpg", 0.5f};
    Sprite sprite1{"char.png", 0.5f};

    player->add_component(sprite1);
    player->add_component(Position{0.5,0.0,0.0});
    player->add_component(Speed{1.0f});

    fellow2->add_component(sprite2);
    fellow2->add_component(Position{0.5,0.0,0.0});

    // Check if the entity has a Sprite component

    // Retrieve the Sprite component and use it
    System_Manager::add_system(std::make_shared<move_to_pos>());
    System_Manager::add_system(std::make_shared<RenderSystem>());
    System_Manager::add_system(std::make_shared<player_inputs>(game.get_window(), player));
    System_Manager::add_system(std::make_shared<delta>());
    System_Manager::add_system(std::make_shared<time_rotate>(player));
    
    // Run the game
    game.run();
}
