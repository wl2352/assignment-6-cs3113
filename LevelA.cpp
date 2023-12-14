#include "LevelA.h"
#include "Utility.h"

#define LOG(argument) std::cout << argument << '\n'
#define LEVEL_WIDTH 23
#define LEVEL_HEIGHT 15

unsigned int LEVEL_DATA[] =
{
    000, 000, 000, 000, 000, 000, 000, 126, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
    000, 99, 100, 100, 101, 000, 000, 126, 000, 000, 000, 000, 000, 000, 000, 000, 48, 49, 50, 51, 000, 000, 000,
    000, 000, 126, 126, 000, 000, 000, 126, 000, 000, 000, 000, 000, 000, 000, 000, 60, 61, 62, 63, 000, 000, 000,
    000, 000, 111, 112, 000, 000, 000, 126, 000, 000, 000, 000, 000, 000, 000, 000, 72, 84, 74, 75, 000, 000, 000,
    126, 126, 123, 124, 126, 126, 126, 126, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 002, 002, 000, 000,
    126, 000, 43, 000, 43, 43, 43, 000, 43, 43, 43, 43, 43, 001, 000, 43, 000, 000, 000, 12, 13, 14, 000,
    126, 000, 000, 000, 000, 000, 000, 000, 43, 000, 000, 000, 000, 000, 000, 43, 000, 000, 000, 24, 25, 26, 000,
    126, 000, 000, 000, 000, 000, 000, 000, 43, 000, 000, 000, 000, 000, 000, 43, 000, 000, 000, 36, 37, 38, 000,
    126, 126, 126, 000, 000, 126, 126, 126, 43, 000, 000, 000, 000, 000, 000, 43, 000, 000, 52, 53, 54, 55, 000,
    000, 000, 000, 001, 000, 000, 000, 126, 43, 000, 000, 000, 000, 000, 000, 43, 000, 000, 64, 65, 66, 67, 000,
    000, 000, 000, 000, 000, 001, 001, 126, 43, 000, 000, 000, 000, 000, 000, 43, 000, 000, 76, 77, 78, 79, 000,
    000, 000, 000, 001, 001, 000, 001, 001, 43, 000, 000, 000, 000, 000, 000, 43, 000, 000, 000, 000, 43, 000, 000,
    000, 000, 000, 000, 000, 000, 000, 000, 43, 000, 000, 000, 000, 000, 002, 000, 43, 43, 43, 43, 000, 000, 000,
    000, 000, 000, 001, 000, 000, 000, 126, 43, 000, 000, 000, 000, 000, 001, 000, 000, 000, 000, 000, 000, 000, 000,
    000, 000, 000, 000, 000, 000, 000, 126, 43, 000, 000, 000, 000, 000, 002, 000, 000, 000, 000, 000, 000, 000, 000
};
/*
000, 000, 000, 000, 000, 000, 000, 126, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000,
000, 099, 100, 100, 101, 000, 000, 126, 000, 000, 000, 000, 000, 000, 000, 000, 048, 049, 050, 051, 000, 000, 000,
000, 000, 126, 126, 000, 000, 000, 126, 000, 000, 000, 000, 000, 000, 000, 000, 060, 061, 062, 063, 000, 000, 000,
000, 000, 111, 112, 000, 000, 000, 126, 000, 000, 000, 000, 000, 000, 000, 000, 072, 084, 074, 075, 000, 000, 000,
126, 126, 123, 124, 126, 126, 126, 126, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 002, 002, 000, 000,
000, 000, 043, 000, 043, 043, 043, 000, 043, 043, 043, 043, 043, 001, 000, 043, 000, 000, 000, 012, 013, 014, 000,
000, 000, 000, 000, 000, 000, 000, 000, 043, 000, 000, 000, 000, 000, 000, 043, 000, 000, 000, 024, 025, 026, 000,
000, 000, 000, 000, 000, 000, 000, 000, 043, 000, 000, 000, 000, 000, 000, 043, 000, 000, 000, 036, 037, 038, 000,
126, 126, 126, 000, 000, 126, 126, 126, 043, 000, 000, 000, 000, 000, 000, 043, 000, 000, 052, 053, 054, 055, 000,
000, 000, 000, 001, 000, 000, 000, 126, 043, 000, 000, 000, 000, 000, 000, 043, 000, 000, 064, 065, 066, 067, 000,
000, 000, 000, 000, 000, 001, 001, 126, 043, 000, 000, 000, 000, 000, 000, 043, 000, 000, 076, 077, 078, 079, 000,
000, 000, 000, 001, 001, 000, 001, 001, 043, 000, 000, 000, 000, 000, 000, 043, 000, 000, 000, 000, 043, 000, 000,
000, 000, 000, 000, 000, 000, 000, 000, 043, 000, 000, 000, 000, 000, 003, 000, 043, 043, 043, 043, 000, 000, 000,
000, 000, 000, 001, 000, 000, 000, 126, 043, 000, 000, 000, 000, 000, 001, 000, 000, 000, 000, 000, 000, 000, 000,
000, 000, 000, 000, 000, 000, 000, 126, 043, 000, 000, 000, 000, 000, 003, 000, 000, 000, 000, 000, 000, 000, 000
*/

LevelA::~LevelA()
{
    delete[] m_state.enemies;
    delete[] m_state.weapons;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void LevelA::initialise()
{
    GLuint map_texture_id = Utility::load_texture("assets/images/kenney_tiny_town.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_DATA, map_texture_id, 1.0f, 12, 11);
    m_state.map->set_level(1);

    // Code from main.cpp's initialise()
     // Existing
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(10.0f, -5.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->set_speed(2.5f);
    m_state.player->set_health(100.0f);
    m_state.player->set_first_weapon(FISTS);
    m_state.player->set_second_weapon(FISTS);
    // m_state.player->set_equipped_weapon(FISTS);
    // m_state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.player->m_texture_id = Utility::load_texture("assets/images/drake.png");

    // Walking
    m_state.player->m_walking[m_state.player->LEFT] = new int[8] { 43, 42, 41, 40, 39, 38, 37, 36 };
    m_state.player->m_walking[m_state.player->RIGHT] = new int[8] { 36, 37, 38, 39, 40, 41, 42, 43 };
    m_state.player->m_walking[m_state.player->UP] = new int[8] { 36, 37, 38, 39, 40, 41, 42, 43 };
    m_state.player->m_walking[m_state.player->DOWN] = new int[8] { 43, 42, 41, 40, 39, 38, 37, 36 };

    m_state.player->m_animation_indices = m_state.player->m_walking[m_state.player->RIGHT];  // start player looking left
    m_state.player->m_animation_frames = 4;
    m_state.player->m_animation_index = 0;
    m_state.player->m_animation_time = 0.0f;
    m_state.player->m_animation_cols = 9;
    m_state.player->m_animation_rows = 5;
    m_state.player->set_height(0.8f);
    m_state.player->set_width(0.8f);

    // Jumping
    m_state.player->m_jumping_power = 5.0f;

    /**
     Enemies' stuff */
    GLuint basic_enemy_texture_id = Utility::load_texture("assets/images/monster.png");
    GLuint quick_enemy_texture_id = Utility::load_texture("assets/images/tile_0109.png");

    m_state.enemies = new Entity[ENEMY_COUNT];
    m_state.enemies[0].set_entity_type(ENEMY);
    m_state.enemies[0].set_ai_type(GUARD);
    m_state.enemies[0].set_ai_state(IDLE);
    m_state.enemies[0].m_texture_id = basic_enemy_texture_id;
    m_state.enemies[0].set_position(glm::vec3(8.0f, -3.0f, 0.0f));
    m_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_state.enemies[0].set_speed(1.0f);

    m_state.enemies[1].set_entity_type(ENEMY);
    m_state.enemies[1].set_ai_type(GUARD);
    m_state.enemies[1].set_ai_state(IDLE);
    m_state.enemies[1].m_texture_id = quick_enemy_texture_id;
    m_state.enemies[1].set_position(glm::vec3(12.0f, -10.0f, 0.0f));
    m_state.enemies[1].set_movement(glm::vec3(0.0f));
    m_state.enemies[1].set_speed(1.5f);
    m_state.enemies[1].set_scale(0.5);

    m_state.enemies[2].set_entity_type(SPAWNED);
    m_state.enemies[3].set_entity_type(SPAWNED);

    /*
    Weapons' Stuff */
    GLuint bat_texture_id = Utility::load_texture("assets/images/tile_0107.png"); // PLACEHOLDER --> FIND TEXTURE FOR BAT

    m_state.weapons = new Entity[WEAPON_COUNT];
    m_state.weapons[0].set_entity_type(WEAPON);
    m_state.weapons[0].set_weapon_type(BAT);
    m_state.weapons[0].set_position(glm::vec3(3.5f, -11.5f, 0.0f));
    m_state.weapons[0].m_texture_id = bat_texture_id;
    m_state.weapons[0].set_scale(0.5f);
    m_state.weapons[0].set_movement(glm::vec3(0.0f));
    m_state.weapons[0].set_speed(2.5f);


    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    m_state.bgm = Mix_LoadMUS("assets/audio/Gothamlicious.mp3");
    Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(1.0f);

    m_state.jump_sfx = Mix_LoadWAV("assets/audio/jump.wav");
}

void LevelA::update(float delta_time)
{
    //gameover = false;
    GLuint quick_enemy_texture_id = Utility::load_texture("assets/images/tile_0109.png");
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map);
    //LOG(ENEMY_COUNT);
    //LOG(m_state.player->get_velocity().y);
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        if (m_state.enemies[i].is_active()) {
            m_state.enemies[i].update(delta_time, m_state.player, 0, 0, m_state.map);
            if (m_state.enemies[i].check_collision(m_state.player) && !m_state.player->attacking()) {
                // LOG("reached A");
                m_state.player->deactivate();
                end_game();
            }
            if (m_state.enemies[i].check_collision(m_state.player) && m_state.player->attacking()) {
                m_state.enemies[i].take_damage(m_state.player);
            }

            if (m_state.enemies[i].get_health() <= 0.0f && m_state.enemies[i].is_active()) {
                m_state.enemies[i].deactivate();
                DEAD_ENEMIES += 1;
            }
           /* LOG("=======");
            LOG(m_state.enemies[i].get_entity_type());
            LOG(m_state.enemies[i].get_health());
            LOG("=======");*/
        }

        if (!m_state.enemies[0].is_active() && !m_state.enemies[1].is_active() && m_state.enemies[2].get_entity_type() == SPAWNED) {
            // Spawn 3rd enemy
            LOG("REACHED HERE OMG FOR ENEMY 3");
            m_state.enemies[2].set_entity_type(ENEMY);
            m_state.enemies[2].set_ai_type(GUARD);
            m_state.enemies[2].set_ai_state(IDLE);
            m_state.enemies[2].m_texture_id = quick_enemy_texture_id;
            m_state.enemies[2].set_position(glm::vec3(12.0f, -10.0f, 0.0f));
            m_state.enemies[2].set_movement(glm::vec3(0.0f));
            m_state.enemies[2].set_speed(1.5f);
            m_state.enemies[2].set_scale(0.5);
        }

        if (!m_state.enemies[2].is_active() && m_state.enemies[3].get_entity_type() == SPAWNED) {
            // Spawn 3rd enemy
            LOG("REACHED HERE OMG FOR ENEMY 3");
            m_state.enemies[3].set_entity_type(ENEMY);
            m_state.enemies[3].set_ai_type(GUARD);
            m_state.enemies[3].set_ai_state(IDLE);
            m_state.enemies[3].m_texture_id = quick_enemy_texture_id;
            m_state.enemies[3].set_position(glm::vec3(9.0f, -9.0f, 0.0f));
            m_state.enemies[3].set_movement(glm::vec3(0.0f));
            m_state.enemies[3].set_speed(1.5f);
            m_state.enemies[3].set_scale(0.5);
        }
        LOG(DEAD_ENEMIES);
        //LOG(m_state.enemies[i].get_health());
    }

    for (int i = 0; i < WEAPON_COUNT; i++)
    {
        if (m_state.weapons[i].is_equipped()) m_state.weapons[i].follow_player(m_state.player);
        else {
            m_state.weapons[i].update(delta_time, m_state.player, 0, 0, m_state.map);
            if (m_state.player->check_collision(&m_state.weapons[i])) {
                // Check for empty slots
                if (m_state.player->get_first_weapon() == FISTS) {
                    m_state.player->set_first_weapon(m_state.weapons[i].get_weapon_type());
                    // If the equipped weapon is a fist right after picking up a new weapon, equip the new weapon
                    if (m_state.player->get_equipped_weapon() == FISTS) {
                        m_state.player->set_equipped_weapon(m_state.player->get_first_weapon(), &m_state.weapons[i]);
                        m_state.weapons[i].equip();
                        LOG(m_state.weapons[i].is_equipped());
                    }
                }
                else if (m_state.player->get_second_weapon() == FISTS) {
                    m_state.player->set_second_weapon(m_state.weapons[i].get_weapon_type());
                    // If the equipped weapon is a fist right after picking up a new weapon, equip the new weapon
                    if (m_state.player->get_equipped_weapon() == FISTS) {
                        m_state.player->set_equipped_weapon(m_state.player->get_second_weapon(), &m_state.weapons[i]);
                        m_state.weapons[i].equip();
                    }
                }
            }
        }
        
        // LOG(m_state.player->get_health());

        /*LOG("====================");
        LOG("Current equipped weapon:");
        LOG(m_state.player->get_equipped_weapon());
        LOG("Is equipped (1) for True, (0) for False:");
        LOG(m_state.weapons[i].is_equipped());
        LOG("Current weapon position:");
        LOG(m_state.weapons[i].get_position().x);
        LOG(m_state.weapons[i].get_position().y);
        LOG("---------");
        LOG("Current player position:");
        LOG(m_state.player->get_position().x);
        LOG(m_state.player->get_position().y);
        LOG("====================");
        LOG("");*/
    }

    if (m_state.player->get_health() <= 0.0f) {
        //LOG("reached");
        m_state.player->deactivate();
        end_game();
    }
    if (DEAD_ENEMIES >= ENEMY_COUNT) {
        // 13, 27, 41, 55, 69, 83, 97, 111 
        level_passed();

    }
    /*LOG("\n================");
    LOG(m_state.player->get_health());
    LOG(gameover);
    LOG("================");*/
}


void LevelA::render(ShaderProgram* program)
{
    m_state.map->render(program);
    m_state.player->render(program);
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].render(program);
    }
    for (int i = 0; i < WEAPON_COUNT; i++)
    {
        m_state.weapons[i].render(program);
    }
}