#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION
#define LOG(argument) std::cout << argument << '\n'

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Entity.h"

Entity::Entity()
{
    // ����� PHYSICS ����� //
    m_position = glm::vec3(0.0f);
    m_velocity = glm::vec3(0.0f);
    m_acceleration = glm::vec3(0.0f);

    // ����� TRANSLATION ����� //
    m_movement = glm::vec3(0.0f);
    m_speed = 0;
    m_model_matrix = glm::mat4(1.0f);

    // ----- SCALE ----- //
    m_curr_scale = glm::vec3(1.0f, 1.0f, 0.0f);
}

Entity::~Entity()
{
    delete[] m_animation_up;
    delete[] m_animation_down;
    delete[] m_animation_left;
    delete[] m_animation_right;
    delete[] m_walking;
}

void Entity::draw_sprite_from_texture_atlas(ShaderProgram* program, GLuint texture_id, int index)
{
    // Step 1: Calculate the UV location of the indexed frame
    float u_coord = (float)(index % m_animation_cols) / (float)m_animation_cols;
    float v_coord = (float)(index / m_animation_cols) / (float)m_animation_rows;

    // Step 2: Calculate its UV size
    float width = 1.0f / (float)m_animation_cols;
    float height = 1.0f / (float)m_animation_rows;

    // Step 3: Just as we have done before, match the texture coordinates to the vertices
    float tex_coords[] =
    {
        u_coord, v_coord + height, u_coord + width, v_coord + height, u_coord + width, v_coord,
        u_coord, v_coord + height, u_coord + width, v_coord, u_coord, v_coord
    };

    float vertices[] =
    {
        -0.5, -0.5, 0.5, -0.5,  0.5, 0.5,
        -0.5, -0.5, 0.5,  0.5, -0.5, 0.5
    };

    // Step 4: And render
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());

    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}

void const Entity::set_entity_info() {
    if (m_entity_type == ENEMY) {
        damage = 1000.0f;
        health = 10.0f;
    }
    else if (m_entity_type == PLAYER) {
        damage = 1.0f;
        health = 1.0f;
    }
    else if (m_entity_type == BRUTE) {
        damage = 20.0f;
        health = 20.0f;
        set_scale(1.5f);
    }
}

void Entity::ai_activate(Entity* player)
{
    switch (m_ai_type)
    {
    case WALKER:
        ai_walk();
        break;

    case GUARD:
        ai_guard(player);
        break;

    case WINGER:
        ai_winger(player);
        break;

    default:
        break;
    }
}

void Entity::ai_walk()
{
    m_movement = glm::vec3(-1.0f, 0.0f, 0.0f);
}

void Entity::ai_guard(Entity* player)
{
    switch (m_ai_state) {
    case IDLE:
        if (glm::distance(m_position, player->get_position()) < 3.0f) m_ai_state = WALKING;
        break;

    case WALKING:
        if (m_position.x > player->get_position().x) {
            m_movement = glm::vec3(-1.0f, 0.0f, 0.0f);
        }

        else if (m_position.y > player->get_position().y) {
            m_movement = glm::vec3(0.0f, -1.0f, 0.0f);
        }
        else {
            m_movement = glm::vec3(1.0f, 1.0f, 0.0f);
        }
        break;

    case ATTACKING:
        break;

    default:
        break;
    }
}

void Entity::ai_winger(Entity* player) {
    switch (m_ai_state) {
    case IDLE:
        if (glm::distance(m_position, player->get_position()) < 3.0f) m_ai_state = ATTACKING;
        break;

    case ATTACKING:
        // Fly in a circular motion
        // 1. Setting up transformation logic
        g_angle += ROT_SPEED;     // increment g_angle by ROT_SPEED

        // 2. Calculate x,y using trigonometry
        g_x_coord = RADIUS * glm::cos(g_angle);
        g_y_coord = RADIUS * glm::sin(g_angle);
        m_position.x += g_x_coord;
        m_position.y += g_y_coord;
        is_chill = false;
        break;

    default:
        break;
    }
}

void Entity::follow_player(Entity* player) {
    m_model_matrix = glm::mat4(1.0f);
    m_model_matrix = glm::translate(m_model_matrix, glm::vec3(player->m_position.x + 0.2f, player->m_position.y + 0.1f, 0.0f));
}

void Entity::take_damage(Entity* other) {
    if (!is_hit && cooldown == 100) {
        health -= other->get_damage();
        is_hit = true;
    }
}

const void Entity::set_equipped_weapon(Weapon weapon, Entity* weapon_model) {
    equipped_weapon = weapon;
    switch (equipped_weapon)
    {
    case FISTS:
        // w_model_matrix = glm::mat4(1.0f);
        // w_curr_scale = glm::vec3(1.2f, 1.2f, 0.0f);
        // weapon_model->set_scale(1.2f);
        radius = 0.2f;
        damage = 3.0f;
        break;
    case BAT:
        // w_model_matrix = glm::mat4(1.0f);
        // w_curr_scale = glm::vec3(1.5f, 1.5f, 0.0f);
        radius = 0.5f;
        weapon_model->set_scale(radius);
        weapon_model->m_movement = glm::vec3(m_position.x + 0.02f, m_position.y, 0.0f);
        weapon_model->equip();
        damage = 5.0f;
        break;
    case AXE:
        // w_model_matrix = glm::mat4(1.0f);
        // w_curr_scale = glm::vec3(1.5f, 1.5f, 0.0f);
        radius = 0.5f;
        weapon_model->set_scale(radius);
        weapon_model->m_movement = glm::vec3(m_position.x + 0.02f, m_position.y, 0.0f);
        weapon_model->equip();
        damage = 8.0f;
        break;
    case KATANA:
        // w_model_matrix = glm::mat4(1.0f);
        // w_curr_scale = glm::vec3(1.8f, 1.8f, 0.0f);
        radius = 0.8f;
        weapon_model->set_scale(radius);
        weapon_model->m_movement = glm::vec3(m_position.x + 0.02f, m_position.y, 0.0f);
        weapon_model->equip();
        damage = 10.0f;
        break;
    case LEGENDARY:
        // w_model_matrix = glm::mat4(1.0f);
        // w_curr_scale = glm::vec3(2.0f, 2.0f, 0.0f);
        radius = 1.0f;
        weapon_model->set_scale(radius);
        weapon_model->m_movement = glm::vec3(m_position.x + 0.02f, m_position.y, 0.0f);
        weapon_model->equip();
        damage = 15.0f;
        break;
    default:
        break;
    }
}


void Entity::update(float delta_time, Entity* player, Entity* objects, int object_count, Map* map)
{
    if (!m_is_active) return;

    m_collided_top = false;
    m_collided_bottom = false;
    m_collided_left = false;
    m_collided_right = false;
    m_collided_enemy_x = false;
    m_collided_enemy_y = false;

    if (m_entity_type == ENEMY || m_entity_type == BRUTE) ai_activate(player);

    if (m_animation_indices != NULL)
    {
        if (glm::length(m_movement) != 0)
        {
            m_animation_time += delta_time;
            float frames_per_second = (float)1 / SECONDS_PER_FRAME;

            if (m_animation_time >= frames_per_second)
            {
                m_animation_time = 0.0f;
                m_animation_index++;

                if (m_animation_index >= m_animation_frames)
                {
                    m_animation_index = 0;
                }
            }
        }
    }

    m_velocity.x = m_movement.x * m_speed;
    m_velocity.y = m_movement.y * m_speed;
    // m_velocity += m_acceleration * delta_time;

    // We make two calls to our check_collision methods, one for the collidable objects and one for
    // the map.
    if (is_chill) {
        m_position.y += m_velocity.y * delta_time;
        check_collision_y(objects, object_count);
        if (is_chill) {
            check_collision_y(map);
        }


        m_position.x += m_velocity.x * delta_time;
        check_collision_x(objects, object_count);
        if (is_chill) {
            check_collision_x(map);
        }

        /*if (m_is_jumping)
        {
            m_is_jumping = false;

            m_velocity.y += m_jumping_power;
        }*/
    }

    if (m_entity_type == BRUTE && get_health() < 20.0f && get_health() >= 10.0f) {
        set_scale(0.75f);
    }
    if (m_entity_type == BRUTE && get_health() < 10.0f && get_health() > 0.0f) {
        set_scale(0.25f);
    }
    if (m_entity_type == BRUTE && get_health() <= 0.0f) {
        set_scale(0.0f);
    }
    
    if (cooldown <= 0) {
        is_hit = false;
        cooldown = 100;
    }
    else if ((0 < cooldown <= 100) && (is_hit)) {
        cooldown -= 1;
    }
   
    m_model_matrix = glm::mat4(1.0f);
    m_model_matrix = glm::translate(m_model_matrix, m_position);
    m_model_matrix = glm::scale(m_model_matrix, m_curr_scale);
    
    
}

void const Entity::check_collision_y(Entity* collidable_entities, int collidable_entity_count)
{
    for (int i = 0; i < collidable_entity_count; i++)
    {
        Entity* collidable_entity = &collidable_entities[i];

        if (check_collision(collidable_entity))
        {
            float y_distance = fabs(m_position.y - collidable_entity->get_position().y);
            float y_overlap = fabs(y_distance - (m_height / 2.0f) - (collidable_entity->get_height() / 2.0f));
            if (m_velocity.y > 0) {
                m_position.y -= y_overlap;
                m_velocity.y = 0;
                m_collided_top = true;
                LOG("reached");
            }
            else if (m_velocity.y < 0) {
                m_position.y += y_overlap;
                m_velocity.y = 0;
                m_collided_bottom = true;
                if (collidable_entity->get_entity_type() == PLAYER || collidable_entity->get_entity_type() == ENEMY || collidable_entity->get_entity_type() == BRUTE) {
                    m_collided_enemy_y = true;
                }
                else {
                    m_collided_enemy_y = false;
                }
            }
        }
    }
}

void const Entity::check_collision_x(Entity* collidable_entities, int collidable_entity_count)
{
    for (int i = 0; i < collidable_entity_count; i++)
    {
        Entity* collidable_entity = &collidable_entities[i];

        if (check_collision(collidable_entity))
        {
            float x_distance = fabs(m_position.x - collidable_entity->get_position().x);
            float x_overlap = fabs(x_distance - (m_width / 2.0f) - (collidable_entity->get_width() / 2.0f));
            if (m_velocity.x > 0) {
                m_position.x -= x_overlap;
                m_velocity.x = 0;
                m_collided_right = true;
                if (collidable_entity->get_entity_type() == PLAYER || collidable_entity->get_entity_type() == ENEMY || collidable_entity->get_entity_type() == BRUTE) {
                    m_collided_enemy_x = true;
                }
                else {
                    m_collided_enemy_x = false;
                }
            }
            else if (m_velocity.x < 0) {
                m_position.x += x_overlap;
                m_velocity.x = 0;
                m_collided_left = true;
                if (collidable_entity->get_entity_type() == PLAYER || collidable_entity->get_entity_type() == ENEMY || collidable_entity->get_entity_type() == BRUTE) {
                    m_collided_enemy_x = true;
                }
                else {
                    m_collided_enemy_x = false;
                }
            }
        }
    }
}


void const Entity::check_collision_y(Map* map)
{
    // Probes for tiles above
    glm::vec3 top = glm::vec3(m_position.x, m_position.y + (m_height / 2), m_position.z);
    glm::vec3 top_left = glm::vec3(m_position.x - (m_width / 2), m_position.y + (m_height / 2), m_position.z);
    glm::vec3 top_right = glm::vec3(m_position.x + (m_width / 2), m_position.y + (m_height / 2), m_position.z);

    // Probes for tiles below
    glm::vec3 bottom = glm::vec3(m_position.x, m_position.y - (m_height / 2), m_position.z);
    glm::vec3 bottom_left = glm::vec3(m_position.x - (m_width / 2), m_position.y - (m_height / 2), m_position.z);
    glm::vec3 bottom_right = glm::vec3(m_position.x + (m_width / 2), m_position.y - (m_height / 2), m_position.z);

    float penetration_x = 0;
    float penetration_y = 0;

    // If the map is solid, check the top three points
    if (map->is_solid(top, &penetration_x, &penetration_y) && m_velocity.y > 0)
    {
        m_position.y -= penetration_y;
        m_velocity.y = 0;
        m_collided_top = true;
    }
    else if (map->is_solid(top_left, &penetration_x, &penetration_y) && m_velocity.y > 0)
    {
        m_position.y -= penetration_y;
        m_velocity.y = 0;
        m_collided_top = true;
    }
    else if (map->is_solid(top_right, &penetration_x, &penetration_y) && m_velocity.y > 0)
    {
        m_position.y -= penetration_y;
        m_velocity.y = 0;
        m_collided_top = true;
    }

    // And the bottom three points
    if (map->is_solid(bottom, &penetration_x, &penetration_y) && m_velocity.y < 0)
    {
        m_position.y += penetration_y;
        m_velocity.y = 0;
        m_collided_bottom = true;
    }
    else if (map->is_solid(bottom_left, &penetration_x, &penetration_y) && m_velocity.y < 0)
    {
        m_position.y += penetration_y;
        m_velocity.y = 0;
        m_collided_bottom = true;
    }
    else if (map->is_solid(bottom_right, &penetration_x, &penetration_y) && m_velocity.y < 0)
    {
        m_position.y += penetration_y;
        m_velocity.y = 0;
        m_collided_bottom = true;

    }
}

void const Entity::check_collision_x(Map* map)
{
    // Probes for tiles; the x-checking is much simpler
    glm::vec3 left = glm::vec3(m_position.x - (m_width / 2), m_position.y, m_position.z);
    glm::vec3 right = glm::vec3(m_position.x + (m_width / 2), m_position.y, m_position.z);

    float penetration_x = 0;
    float penetration_y = 0;

    if (map->is_solid(left, &penetration_x, &penetration_y) && m_velocity.x < 0)
    {
        m_position.x += penetration_x;
        m_velocity.x = 0;
        m_collided_left = true;
    }
    if (map->is_solid(right, &penetration_x, &penetration_y) && m_velocity.x > 0)
    {
        m_position.x -= penetration_x;
        m_velocity.x = 0;
        m_collided_right = true;
    }
}



void Entity::render(ShaderProgram* program)
{
    program->set_model_matrix(m_model_matrix);

    if (m_animation_indices != NULL)
    {
        draw_sprite_from_texture_atlas(program, m_texture_id, m_animation_indices[m_animation_index]);
        return;
    }

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float tex_coords[] = { 0.0,  1.0, 1.0,  1.0, 1.0, 0.0,  0.0,  1.0, 1.0, 0.0,  0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    glVertexAttribPointer(program->get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->get_position_attribute());
    glVertexAttribPointer(program->get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, tex_coords);
    glEnableVertexAttribArray(program->get_tex_coordinate_attribute());

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->get_position_attribute());
    glDisableVertexAttribArray(program->get_tex_coordinate_attribute());
}


bool const Entity::check_collision(Entity* other) const
{
    if (other == this) return false;
    // If either entity is inactive, there shouldn't be any collision
    if (!m_is_active || !other->m_is_active) return false;

    float x_distance = fabs((m_position.x + radius) - (other->m_position.x + radius)) - ((m_width + other->m_width) / 2.0f);
    float y_distance = fabs((m_position.y + radius) - (other->m_position.y + radius)) - ((m_height + other->m_height) / 2.0f);

    return x_distance < 0.0f && y_distance < 0.0f;
}