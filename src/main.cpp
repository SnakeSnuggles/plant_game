#include "../Engine/engine.hpp"


class sine_move : public System {
    private:
        std::shared_ptr<Entity> entity_;
    public:
        sine_move(std::shared_ptr<Entity> ent): entity_(ent) {}
        void update() {
            float time = glfwGetTime();
            entity_->get_component<Sprite>().x = sin(time);
            //std::cout << sprite_->x << "\n";
        } 
};
class RenderSystem : public System {
public:
    void update() override {
        std::vector<Sprite*> sorted_sprites;

        for (auto& entity : Entity::Entities) {

            // Check if the entity has a Sprite component
            if (entity->has_component<Sprite>()) {
                Sprite& sprite = entity->get_component<Sprite>();

                sprite.updateVBO();  // Keep geometry up to date
                sorted_sprites.push_back(&sprite);
            }
        }

        std::sort(sorted_sprites.begin(), sorted_sprites.end(), [](Sprite* a, Sprite* b) {
            return a->z < b->z;  // Sort based on z-index for rendering order
        });

        for (auto sprite : sorted_sprites) {
            sprite->shader.use();  
            glActiveTexture(GL_TEXTURE0 + sprite->texture.ID);  
            sprite->texture.bind();
            sprite->shader.setInt("texturea", sprite->texture.ID); 

            sprite->vao.draw();
        }
    }
};

int main() {
    App game{"plant game"};
    
    auto fellow = Entity::create();
    
    // Create Sprite and add it as a component to the entity
    Sprite sprite1{"char.png", 0.5f, 0.0f, 0.0f, 0.0f};
    fellow->add_component(sprite1);

    // Check if the entity has a Sprite component

    // Retrieve the Sprite component and use it
    System_Manager::add_system(std::make_shared<RenderSystem>());
    System_Manager::add_system(std::make_shared<sine_move>(fellow));
    // Run the game
    game.run();
}
