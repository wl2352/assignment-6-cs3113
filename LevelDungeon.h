#include "Scene.h"

class LevelDungeon : public Scene {
public:
    int ENEMY_COUNT = 1;
    int DEAD_ENEMIES = 0;
    int level_width = 23;
    int level_height = 15;
    bool win = false;
    bool gameover = false;

    ~LevelDungeon();

    void initialise() override;
    void update(float delta_time) override;
    void render(ShaderProgram* program) override;
    void level_passed()               { win = true; };
    bool const is_passed()      const { return win; };
    bool const game_over()      const { return gameover; };
    int const get_edge()        const { return level_width; };
};