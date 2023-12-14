#pragma once
#include "Map.h"

enum Weapon { FISTS, BAT, AXE, KATANA, LEGENDARY };
enum EntityType { PLATFORM, PLAYER, ENEMY, BRUTE, WEAPON, SPAWNED };
enum AIType { WALKER, GUARD, WINGER };
enum AIState { WALKING, IDLE, ATTACKING };

class Entity
{
private:
    bool m_is_active = true;
    bool is_chill = true;
    bool winner = false;

    // ����� ANIMATION ����� //
    int* m_animation_right = NULL, // move to the right
        * m_animation_left = NULL, // move to the left
        * m_animation_up = NULL, // move upwards
        * m_animation_down = NULL; // move downwards

    // ����� PHYSICS (GRAVITY) ����� //
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    glm::vec3 m_acceleration;

    // ����� TRANSFORMATIONS ����� //
    float     m_speed;
    glm::vec3 m_movement;
    glm::vec3 m_curr_scale = glm::vec3(1.0f, 1.0f, 0.0f);
    glm::vec3 w_curr_scale;
    glm::mat4 m_model_matrix;
    glm::mat4 w_model_matrix;

    // ----- DAMAGE ----- //
    int level = 0;
    float damage;
    float health;
    float radius = 0.0f;
    int cooldown = 100;
    bool is_equip = false;
    bool is_attacking = false;
    bool is_hit = false;
    Weapon equipped_weapon = FISTS;
    Weapon first_weapon;
    Weapon second_weapon;
    Weapon m_weapon_type;       // USED FOR WEAPON ENTITY

    // ����� ENEMY AI ����� //
    EntityType m_entity_type;
    AIType     m_ai_type;
    AIState    m_ai_state;

    float m_width = 1;
    float m_height = 1;


public:
    // ����� STATIC VARIABLES ����� //
    static const int    SECONDS_PER_FRAME = 4;
    static const int    LEFT = 0,
        RIGHT = 1,
        UP = 2,
        DOWN = 3;

    // ����� ANIMATION ����� //
    int** m_walking = new int* [4]
        {
            m_animation_left,
                m_animation_right,
                m_animation_up,
                m_animation_down
        };

    int m_animation_frames = 0,
        m_animation_index = 0,
        m_animation_cols = 0,
        m_animation_rows = 0;

    int* m_animation_indices = NULL;
    float   m_animation_time = 0.0f;

    // ----- CIRCLE MOTION ----- //
    const float RADIUS = 0.5f;      // radius of your circle
    const float ROT_SPEED = 0.008f;  // rotational speed
    float       g_angle = 0.0f;     // current angle
    float       g_x_coord = RADIUS, // current x and y coordinates
                g_y_coord = 0.0f;

    // ����� PHYSICS (JUMPING) ����� //
    bool  m_is_jumping = false;
    float m_jumping_power = 0;

    // ����� PHYSICS (COLLISIONS) ����� //
    bool m_collided_top = false;
    bool m_collided_bottom = false;
    bool m_collided_left = false;
    bool m_collided_right = false;

    bool m_collided_enemy_x = false;
    bool m_collided_enemy_y = false;

    GLuint    m_texture_id;

    // ����� METHODS ����� //
    Entity();
    ~Entity();

    void draw_sprite_from_texture_atlas(ShaderProgram* program, GLuint texture_id, int index);
    void update(float delta_time, Entity* player, Entity* objects, int object_count, Map* map); // Now, update should check for both objects in the game AND the map
    void render(ShaderProgram* program);

    bool const check_collision(Entity* other) const;
    void const check_collision_y(Entity* collidable_entities, int collidable_entity_count);
    void const check_collision_x(Entity* collidable_entities, int collidable_entity_count);
    void take_damage(Entity* other);
    void do_attack(Entity* other);
    void follow_player(Entity* player);

    // Overloading our methods to check for only the map
    void const check_collision_y(Map* map);
    void const check_collision_x(Map* map);

    void move_left() { m_movement.x = -1.0f; };
    void move_right() { m_movement.x = 1.0f; };
    void move_up() { m_movement.y = 1.0f; };
    void move_down() { m_movement.y = -1.0f; };

    void ai_activate(Entity* player);
    void ai_walk();
    void ai_guard(Entity* player);
    void ai_winger(Entity* player);

    void activate() { m_is_active = true; };
    void deactivate() { m_is_active = false; };

    // ������GETTERS ����� //
    EntityType const get_entity_type()    const { return m_entity_type; };
    AIType     const get_ai_type()        const { return m_ai_type; };
    AIState    const get_ai_state()       const { return m_ai_state; };
    Weapon     const get_equipped_weapon() const { return equipped_weapon; };
    Weapon     const get_first_weapon()   const { return first_weapon; };
    Weapon     const get_second_weapon()  const { return second_weapon; };
    Weapon     const get_weapon_type()    const { return m_weapon_type; };      // USED FOR WEAPON ENTITY
    glm::vec3  const get_position()       const { return m_position; };
    glm::vec3  const get_movement()       const { return m_movement; };
    glm::vec3  const get_velocity()       const { return m_velocity; };
    glm::vec3  const get_acceleration()   const { return m_acceleration; };
    float      const get_jumping_power()  const { return m_jumping_power; };
    float      const get_speed()          const { return m_speed; };
    int        const get_width()          const { return m_width; };
    int        const get_height()         const { return m_height; };
    float      const get_damage()         const { return damage; };
    float      const get_health()         const { return health; };
    bool       const is_winner()          const { return winner; };
    bool       const is_active()         const { return m_is_active; };
    bool       const is_colliding_x()     const { return m_collided_left || m_collided_right; };
    bool       const is_colliding_y()     const { return m_collided_top || m_collided_bottom; };
    bool       const colliding_bottom()     const { return m_collided_bottom; };
    bool       const colliding_top()        const { return m_collided_top; };
    bool       const is_colliding_other_x() const { return m_collided_enemy_x; };
    bool       const is_colliding_other_y() const { return m_collided_enemy_y; };
    bool       const is_equipped()          const { return is_equip; };
    int        const get_level()            const { return level; };
    bool       attacking()                  const { return is_attacking; };

    // ����� SETTERS ����� //
    void const set_entity_type(EntityType new_entity_type) { m_entity_type = new_entity_type; set_entity_info(); };
    void const set_ai_type(AIType new_ai_type) { m_ai_type = new_ai_type; };
    void const set_ai_state(AIState new_state) { m_ai_state = new_state; };
    void const set_equipped_weapon(Weapon weapon, Entity* weapon_model);
    void const set_equipped_weapon(Weapon weapon) { equipped_weapon = weapon; };
    void const set_first_weapon(Weapon weapon) { first_weapon = weapon; };
    void const set_second_weapon(Weapon weapon) { second_weapon = weapon; };
    void const set_weapon_type(Weapon weapon) { m_weapon_type = weapon; };          // USED FOR WEAPON ENTITY
    void const set_position(glm::vec3 new_position) { m_position = new_position; };
    void const set_movement(glm::vec3 new_movement) { m_movement = new_movement; };
    void const set_velocity(glm::vec3 new_velocity) { m_velocity = new_velocity; };
    void const set_speed(float new_speed) { m_speed = new_speed; };
    void const set_jumping_power(float new_jumping_power) { m_jumping_power = new_jumping_power; };
    void const set_acceleration(glm::vec3 new_acceleration) { m_acceleration = new_acceleration; };
    void const set_width(float new_width) { m_width = new_width; };
    void const set_height(float new_height) { m_height = new_height; };
    void const set_health(float new_health) { health = new_health; };
    void const set_scale(float scale) { m_curr_scale = glm::vec3(1.0f * scale, 1.0f * scale, 0.0f); };
    void const set_winner() { winner = true; };
    void const set_entity_info();
    void const set_level(int new_level) { level = new_level; };
    void const equip() { is_equip = true; };
    void const unequip() { is_equip = false; };
    void const attack() { is_attacking = true; };
    void const stop_attack() { is_attacking = false; };
};