/**
* Author: William Liburd
* Assignment: Platformer
* Date due: 2023-12-02, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define FIXED_TIMESTEP 0.0166666f
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8
#define LEVEL1_LEFT_EDGE 5.0f
#define LEVEL1_RIGHT_EDGE 9.0f
#define LOG(argument) std::cout << argument << '\n'


#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "cmath"
#include <ctime>
#include <vector>
#include "Entity.h"
#include "Map.h"
#include "Utility.h"
#include "Scene.h"
#include "MainMenu.h"
#include "LevelA.h"
#include "LevelGrey.h"
#include "LevelBrown.h"
#include "LevelDungeon.h"

// ����� CONSTANTS ����� //
const int   WINDOW_WIDTH = 640,
WINDOW_HEIGHT = 480;

const float BG_RED = 132.0f,
BG_BLUE = 198.0f,
BG_GREEN = 105.0f,
BG_OPACITY = 1.0f;

const int   VIEWPORT_X = 0,
VIEWPORT_Y = 0,
VIEWPORT_WIDTH = WINDOW_WIDTH,
VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const char  V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
F_SHADER_PATH[] = "shaders/fragment_textured.glsl";
const char FONT_FILEPATH[] = "sprites/font1.png";

const float MILLISECONDS_IN_SECOND = 1000.0;

// ����� GLOBAL VARIABLES ����� //
Scene* g_current_scene;
MainMenu* g_main_menu;
LevelA* g_level_a;
LevelGrey* g_level_grey;
LevelBrown* g_level_brown;
LevelDungeon* g_level_dungeon;

Scene* g_levels[5];

SDL_Window* g_display_window;
bool g_game_is_running = true;

ShaderProgram g_shader_program;
glm::mat4 g_view_matrix, g_projection_matrix;

float g_previous_ticks = 0.0f;
float g_accumulator = 0.0f;
int secs = 0;
int lives = 3;
int cooldown = 400;
int curr_weapon_slot = 1; // This can either be 1 or 2 to represent the which weapon slot is being used
bool is_paused = false;
bool is_attacking = false;

//bool is_main = false;

GLuint g_text_texture_id;

void switch_to_scene(Scene* scene)
{
    g_current_scene = scene;
    g_current_scene->initialise();
}

void initialise()
{
    // ����� VIDEO ����� //
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    g_display_window = SDL_CreateWindow("Hello, Scenes!",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);

#ifdef _WINDOWS
    glewInit();
#endif

    // ����� GENERAL ����� //
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    glUseProgram(g_shader_program.get_program_id());

    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);

    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);

    glUseProgram(g_shader_program.get_program_id());

    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);

    // ����� LEVEL A SETUP ����� //
    g_main_menu = new MainMenu();
    g_level_a = new LevelA();
    g_level_grey = new LevelGrey();
    g_level_brown = new LevelBrown();
    g_level_dungeon = new LevelDungeon();
    //switch_to_scene(g_level_a);

    g_levels[0] = g_main_menu;
    g_levels[1] = g_level_a;
    g_levels[2] = g_level_grey;
    g_levels[3] = g_level_brown;
    g_levels[4] = g_level_dungeon;

    // Start at level A
    switch_to_scene(g_levels[0]);

    // ����� BLENDING ����� //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void process_input()
{
    if (g_current_scene != g_main_menu) g_current_scene->m_state.player->set_movement(glm::vec3(0.0f));

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // ����� KEYSTROKES ����� //
        switch (event.type) {
            // ����� END GAME ����� //
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            g_game_is_running = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_q:
                // Quit the game with a keystroke
                g_game_is_running = false;
                break;

            case SDLK_SPACE:
                // ����� ATTACK ����� //
                // do attack
                if (cooldown == 400 && !is_attacking) {
                    // do attack
                    is_attacking = true;
                    g_current_scene->m_state.player->attack();
                }
                break;

            case SDLK_RETURN:
                if (g_current_scene == g_main_menu) {
                    g_current_scene->start_game();
                }
                break;

            case SDLK_1:
                g_current_scene->m_state.player->set_equipped_weapon(
                    g_current_scene->m_state.player->get_first_weapon()
                );
                curr_weapon_slot = 1;
                LOG(curr_weapon_slot);
                break;

            case SDLK_2:
                g_current_scene->m_state.player->set_equipped_weapon(
                    g_current_scene->m_state.player->get_second_weapon()
                );
                curr_weapon_slot = 2;
                LOG(curr_weapon_slot);
                break;

            case SDLK_g:
                g_current_scene->m_state.player->set_equipped_weapon(FISTS);
                if (curr_weapon_slot == 1) {
                    g_current_scene->m_state.player->set_first_weapon(FISTS);
                }
                else {
                    g_current_scene->m_state.player->set_second_weapon(FISTS);
                }

            case SDLK_p:
                is_paused = !is_paused;
                break;

            default:
                break;
            }

        default:
            break;
        }
    }

    // ����� KEY HOLD ����� //
    const Uint8* key_state = SDL_GetKeyboardState(NULL);

    if (key_state[SDL_SCANCODE_LEFT] && g_current_scene != g_main_menu)
    {
        g_current_scene->m_state.player->move_left();
        g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->LEFT];
    }
    else if (key_state[SDL_SCANCODE_RIGHT] && g_current_scene != g_main_menu)
    {
        g_current_scene->m_state.player->move_right();
        g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->RIGHT];
    }
    else if (key_state[SDL_SCANCODE_UP] && g_current_scene != g_main_menu)
    {
        g_current_scene->m_state.player->move_up();
        g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->UP];
    }
    else if (key_state[SDL_SCANCODE_DOWN] && g_current_scene != g_main_menu)
    {
        g_current_scene->m_state.player->move_down();
        g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->DOWN];
    }

    // ����� NORMALISATION ����� //
    if ((g_current_scene != g_main_menu) && (glm::length(g_current_scene->m_state.player->get_movement()) > 1.0f))
    {
        g_current_scene->m_state.player->set_movement(glm::normalize(g_current_scene->m_state.player->get_movement()));
    }
}

void update()
{
    // ����� DELTA TIME / FIXED TIME STEP CALCULATION ����� //
    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float delta_time = ticks - g_previous_ticks;
    g_previous_ticks = ticks;

    delta_time += g_accumulator;

    if (delta_time < FIXED_TIMESTEP)
    {
        g_accumulator = delta_time;
        return;
    }

    while (delta_time >= FIXED_TIMESTEP) {
        // ����� UPDATING THE SCENE (i.e. map, character, enemies...) ����� //
        g_current_scene->update(FIXED_TIMESTEP);

        delta_time -= FIXED_TIMESTEP;
    }

    g_accumulator = delta_time;


    // ����� PLAYER CAMERA ����� //
    g_view_matrix = glm::mat4(1.0f);

    if (g_current_scene == g_main_menu) {
        if (g_current_scene->is_start()) switch_to_scene(g_level_a);
    }
    else {
        if (g_current_scene->m_state.player->get_position().x > LEVEL1_LEFT_EDGE) {
            g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-g_current_scene->m_state.player->get_position().x, -g_current_scene->m_state.player->get_position().y, 0));
        }

        else {
            g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-5, -g_current_scene->m_state.player->get_position().y, 0));
        }

        if (cooldown <= 0) {
            is_attacking = false;
            g_current_scene->m_state.player->stop_attack();
            cooldown = 400;
        }
        else if ((0 < cooldown <= 400) && (is_attacking)) {
            cooldown -= 1;
        }

        // LOG(g_current_scene->m_state.map->get_entrance());

        // LOG(g_current_scene->m_state.player->get_position().x);
        // Forward level progression
        if (g_current_scene == g_level_a) {
            if (g_current_scene->m_state.map->get_goal_to_grey() ) switch_to_scene(g_level_grey);
            if (g_current_scene->m_state.map->get_goal_to_brown() ) switch_to_scene(g_level_brown);
            if (g_current_scene->m_state.player->colliding_top() && g_current_scene->m_state.map->get_entrance()) switch_to_scene(g_level_dungeon);
        }

        if (g_current_scene == g_level_dungeon) {
            if (g_current_scene->m_state.player->colliding_top() && g_current_scene->m_state.map->get_entrance()) switch_to_scene(g_level_a);
        }

        if (g_current_scene == g_level_grey) {
            // pass to next level based on parameters
        }

        /*if (g_current_scene == g_level_a && g_current_scene->m_state.player->get_position().x > g_level_a->get_edge()) switch_to_scene(g_level_dungeon);
        if (g_current_scene == g_level_dungeon && g_current_scene->m_state.player->get_position().x > g_level_dungeon->get_edge()) switch_to_scene(g_level_b);
        if (g_current_scene == g_level_b && g_current_scene->m_state.player->get_position().x > g_level_b->get_edge()) g_current_scene->m_state.player->set_winner();*/

        // TODO: Backwards level regression

        /*if ((g_current_scene->m_state.player->is_colliding_other_x()) || (g_current_scene->m_state.player->is_colliding_other_y()))
        {
            switch_to_scene(g_level_a);
            lives -= 1;
        }*/
        if (g_current_scene->m_state.player->get_health() == 0) g_current_scene->m_state.player->deactivate();
    }
}

void render()
{
    g_shader_program.set_view_matrix(g_view_matrix);

    glClear(GL_COLOR_BUFFER_BIT);
    GLuint g_text_texture_id = Utility::load_texture(FONT_FILEPATH);

    if (g_current_scene != g_main_menu) {
        if (!g_current_scene->m_state.player->is_active()) {
            // LOG(secs);
            secs += 1;
            // LOSER
            Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("YOU LOSE!!"), 0.50f, 0.00f, glm::vec3(2.5f, -2.0f, 0.0f));
            //g_game_is_running = false;
        }
        if (g_current_scene->m_state.player->is_winner()) {
            secs += 1;
            // WINNER
            Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("YOU WIN!!"), 0.50f, 0.00f, glm::vec3(g_current_scene->m_state.player->get_position().x - 4.0f,
                g_current_scene->m_state.player->get_position().y + 2.0f,
                g_current_scene->m_state.player->get_position().z));
            //g_game_is_running = false;
        }

        if (secs == 75) g_game_is_running = false;
    }

    // ����� RENDERING THE SCENE (i.e. map, character, enemies...) ����� //
    g_current_scene->render(&g_shader_program);

    if (g_current_scene == g_main_menu) {
        Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("Press Enter to Start"), 0.45f, 0.0f, glm::vec3(-2.5f, 0.0f, 0.0f));
        Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("WASD to move"), 0.25f, 0.0f, glm::vec3(-2.5f, -0.9f, 0.0f));
        Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("SPACE to attack"), 0.25f, 0.0f, glm::vec3(-2.5f, -1.2f, 0.0f));
        Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("1 or 2 to equip weapon"), 0.25f, 0.0f, glm::vec3(-2.5f, -1.5f, 0.0f));
        Utility::draw_text(&g_shader_program, g_text_texture_id, std::string("G to destroy item"), 0.25f, 0.0f, glm::vec3(-2.5f, -1.8f, 0.0f));
    }

    SDL_GL_SwapWindow(g_display_window);
}

void shutdown()
{
    SDL_Quit();

    // ����� DELETING LEVEL A DATA (i.e. map, character, enemies...) ����� //
    delete g_main_menu;
    delete g_level_a;
    delete g_level_grey;
    delete g_level_brown;
    delete g_level_dungeon;
}

// ����� GAME LOOP ����� //
int main(int argc, char* argv[])
{
    initialise();

    while (g_game_is_running)
    {
        process_input();
        if (!is_paused) update();

        //if (g_current_scene->m_state.next_scene_id >= 0) switch_to_scene(g_levels[g_current_scene->m_state.next_scene_id]);

        render();
    }

    shutdown();
    return 0;
}