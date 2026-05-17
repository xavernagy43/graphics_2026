#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"

#include <obj/model.h>

typedef struct {
    float minX, maxX;
    float minY, maxY;
    float minZ, maxZ;
} AABB;

typedef struct Scene
{
    Model enviroment;
    GLuint texture_enviroment;
    Material material;
    Model house;
    GLuint texture_house;
    Model moon;
    GLuint texture_moon;
    Model character;
    GLuint texture_character;
    Model wood_trunk;
    GLuint texture_wood_trunk;
    Model dead_tree;
    GLuint texture_dead_tree;
    Model tent;
    GLuint texture_tent;
    Model fireplace;
    GLuint texture_fireplace;
    //Model skeleton;
    //GLuint texture_skeleton;
    Model scroll;
    GLuint texture_scroll;
    float scroll_animation_phase;
    bool scroll_collected[8];
    int preview;
    bool flashlight_enabled;
    bool fog_enabled;
    float brightness;
    GLuint help;
} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene* scene);

/**
 * Set the lighting of the scene.
 */
void set_lighting(const Scene* scene);

/**
 * Set the current material.
 */
void set_material(const Material* material);

/**
 * Update the scene.
 */
void update_scene(Scene* scene);

/**
 * Render the scene objects.
 */
void render_scene(const Scene* scene, const Camera* camera);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin();

// Render the moon with its own settings.
void render_moon(const Scene* scene);

// Set the moon's light properties.
void set_moon_light();

// Render the house with its own settings.
void render_house(const Scene* scene);

// Render the camera-attached flashlight character.
void render_flashlight(const Scene* scene);

// Render multiple wood trunks with varying positions and rotations.
void render_wood_trunks(const Scene* scene);

// Render multiple dead trees with varying positions and rotations.
void render_trees(const Scene* scene);

// Render the scroll object in the scene.
void render_scrolls(const Scene* scene);

// Check whether a world position is blocked by a static scene obstacle.
bool scene_is_position_blocked(const Scene* scene, float x, float y, float z);

// Collect a scroll when the camera moves through it.
bool scene_collect_scroll_if_touching(Scene* scene, float x, float y, float z);

// Render the fireplace object in the scene.
void render_fireplace(const Scene* scene);

#endif /* SCENE_H */
