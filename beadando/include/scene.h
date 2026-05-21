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
    GLuint end;
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
void set_moon_light(const Scene* scene);

// Set the flashlight properties.
void set_flashlight(const Camera* camera, bool enabled, float brightness);

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

// Check if a point is inside an axis-aligned bounding box.
bool point_inside_aabb(const AABB* box, float x, float y, float z);

// Check if the camera is close enough to collect a scroll, and mark it as collected.
bool scene_collect_scroll_if_touching(Scene* scene, float x, float y, float z);

// Compute the shadow position on the ground for an object based on the light source position and the object's position.
void compute_shadow_position(const vec3* light_pos, float obj_x, float obj_y, float obj_z, float ground_z, float* shadow_x, float* shadow_y);

// Render the shadow of a model on the ground based on the light source position and the model's position, scale, and rotation.
void render_shadow_for_model(const vec3* light_pos, float obj_x, float obj_y, float obj_z, float obj_scale, float obj_rotation, const Model* model);

// Render shadows for all relevant objects in the scene based on the current light source and camera position.
void render_shadows(const Scene* scene, const Camera* camera);

#endif /* SCENE_H */
