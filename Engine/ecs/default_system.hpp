#pragma once
#include "ecs.hpp"
#include "../render/Sprite.hpp"


class RenderSystem : public System {
public:
    void update(float delta_time) override {
        std::vector<Sprite*> sorted_sprites;
        
        auto Ents = get_all_with_component<Sprite>();

        for (auto& entity : Ents) {
            Sprite& sprite = entity->get_component<Sprite>();

            sprite.updateVBO();  // Keep geometry up to date
            sorted_sprites.push_back(&sprite);
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
