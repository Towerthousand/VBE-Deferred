#include "SceneMain/SceneMain.hpp"

int main() {
    WINDOW_TITLE = "Deferred Test";
    Game game;
    SceneMain* sc = new SceneMain();
    sc->addTo(&game);
    game.run();
    return 42;
}
