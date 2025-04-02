#include "../Engine/engine.h"

int main() {
    App game;
    Sprite sprite1{"char.png", 0.5, 0, 0, 0.16};
    Sprite sprite3{"char.png", 1.0, 0, 0, 0.5};
    Sprite sprite2{"wall.jpg", 0.5, 0, 0, 0.0};

    game.run();
}
