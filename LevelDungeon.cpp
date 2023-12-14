#include "LevelDungeon.h"
#include "Utility.h"

#define LEVEL_WIDTH 23
#define LEVEL_HEIGHT 15
#define LOG(argument) std::cout << argument << '\n'

unsigned int LEVELDUNGEON_DATA[] =
{
    000, 000, 000, 000, 000, 000, 000, 22, 23, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    000, 000, 000, 000, 000, 000, 13, 000, 00, 15, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    000, 000, 000, 000, 000, 000, 13, 00, 000, 15, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    000, 000, 000, 000, 000, 000, 13, 000, 000, 15, 000, 000, 000, 001, 002, 002, 003, 000, 000, 000, 000, 000, 000,
    000, 001, 002, 002, 002, 002, 17, 00, 000, 16, 002, 002, 002, 17, 000, 000, 15, 000, 000, 000, 000, 000, 000,
    000, 13, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 15, 000, 000, 000, 000, 000, 000,
    001, 13, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 16, 002, 002, 002, 002, 003, 000,
    13, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 15, 000,
    13, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 15, 000,
    13, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 15, 000,
    13, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 15, 000,
    002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 15, 000,
    57, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 59, 000,
    57, 40, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 59, 000,
    57, 40, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 59, 000
};

/*
000, 000, 000, 000, 000, 000, 000, 022, 023, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
000, 000, 000, 000, 000, 000, 013, 000, 042, 015, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
000, 000, 000, 000, 000, 000, 013, 042, 000, 015, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
000, 000, 000, 000, 000, 000, 013, 000, 000, 015, 000, 000, 000, 001, 002, 002, 003, 000, 000, 000, 000, 000, 000,
000, 001, 002, 002, 002, 002, 017, 042, 000, 016, 002, 002, 002, 017, 000, 000, 015, 000, 000, 000, 000, 000, 000,
000, 013, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 015, 000, 000, 000, 000, 000, 000,
001, 013, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 016, 002, 002, 002, 002, 003, 000,
013, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 015, 000,
013, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 015, 000,
013, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 015, 000,
013, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 015, 000,
002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 002, 015, 000,
000, 000, 057, 040, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 059, 000,
000, 000, 057, 040, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 059, 000,
000, 000, 057, 040, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 059, 000
*/

LevelDungeon::~LevelDungeon()
{
    delete[] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void LevelDungeon::initialise()
{
    m_state.next_scene_id = -1;

    GLuint map_texture_id = Utility::load_texture("assets/images/kenney_tiny_dungeon.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELDUNGEON_DATA, map_texture_id, 1.0f, 12, 11);
    m_state.map->set_level(2);

    // Code from main.cpp's initialise()
    /**
     George's Stuff
     */
     // Existing
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(9.0f, -6.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->set_speed(2.5f);
    m_state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.player->m_texture_id = Utility::load_texture("assets/images/drake.png");

    // Walking
    m_state.player->m_walking[m_state.player->LEFT] = new int[8] { 43, 42, 41, 40, 39, 38, 37, 36 };
    m_state.player->m_walking[m_state.player->RIGHT] = new int[8] { 36, 37, 38, 39, 40, 41, 42, 43 };
    m_state.player->m_walking[m_state.player->UP] = new int[4] { 2, 6, 10, 14 };
    m_state.player->m_walking[m_state.player->DOWN] = new int[4] { 0, 4, 8, 12 };

    m_state.player->m_animation_indices = m_state.player->m_walking[m_state.player->RIGHT];  // start George looking left
    m_state.player->m_animation_frames = 4;
    m_state.player->m_animation_index = 0;
    m_state.player->m_animation_time = 0.0f;
    m_state.player->m_animation_cols = 9;
    m_state.player->m_animation_rows = 5;
    m_state.player->set_height(0.8f);
    m_state.player->set_width(0.8f);

    // Jumping
    m_state.player->m_jumping_power = 7.0f;

    /**
     Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture("assets/images/monster.png");

    m_state.enemies = new Entity[ENEMY_COUNT];
    m_state.enemies[0].set_entity_type(BRUTE);
    m_state.enemies[0].set_ai_type(GUARD);
    m_state.enemies[0].set_ai_state(IDLE);
    m_state.enemies[0].m_texture_id = enemy_texture_id;
    m_state.enemies[0].set_position(glm::vec3(14.0f, -6.0f, 0.0f));
    m_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_state.enemies[0].set_speed(0.6f);
    m_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));


    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    m_state.bgm = Mix_LoadMUS("assets/audio/Gothamlicious.mp3");
    Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(1.0f);

    m_state.jump_sfx = Mix_LoadWAV("assets/audio/jump.wav");
}

void LevelDungeon::update(float delta_time)
{
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map);
    //LOG(ENEMY_COUNT);

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].update(delta_time, m_state.player, 0, 0, m_state.map);
        if (m_state.player->is_colliding_other_x()) {
            //LOG("reached");
            m_state.player->take_damage(&m_state.enemies[i]);
            //LOG(m_state.player->get_health());
        }
        if (m_state.player->is_colliding_other_y()) {
            m_state.enemies[i].take_damage(m_state.player);
        }

        if (m_state.enemies[i].get_health() <= 0.0f && m_state.enemies[i].is_active()) {
            m_state.enemies[i].deactivate();
            DEAD_ENEMIES += 1;
        }
    }
    if (m_state.player->get_health() <= 0.0f && m_state.player->is_active()) {
        m_state.player->deactivate();
        gameover = true;
    }
    if (DEAD_ENEMIES == ENEMY_COUNT) {
        // 13, 27, 41, 55, 69, 83, 97, 111 
        //LOG("reached");
        LEVELDUNGEON_DATA[13] = 0;
        LEVELDUNGEON_DATA[27] = 0;
        LEVELDUNGEON_DATA[41] = 0;
        LEVELDUNGEON_DATA[55] = 0;
        LEVELDUNGEON_DATA[69] = 0;
        LEVELDUNGEON_DATA[83] = 0;
        LEVELDUNGEON_DATA[97] = 0;
        LEVELDUNGEON_DATA[111] = 0;
    }
}

void LevelDungeon::render(ShaderProgram* program)
{
    m_state.map->render(program);
    m_state.player->render(program);

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].render(program);
    }
}