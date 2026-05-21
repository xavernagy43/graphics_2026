#include "scene.h"

#include <SDL2/SDL.h>
#include <obj/load.h>
#include <obj/draw.h>
#include <math.h>
#include <stdio.h>
#include <GL/glu.h>

typedef struct ScrollInstance {
    float x;
    float y;
    float z;
    float rotation;
    float scale;
} ScrollInstance;

static const ScrollInstance scroll_instances[] = {
    { -12.0f,  14.0f, -1.0f,  30.0f, 0.40f },
    {  18.0f,  10.0f, -1.0f, -20.0f, 0.40f },
    { -20.0f,  -8.0f, -1.0f,  45.0f, 0.40f },
    {   8.0f, -16.0f, -1.0f, -35.0f, 0.40f },
    {  22.0f, -12.0f, -1.0f,  15.0f, 0.40f },
    { -26.0f,   4.0f, -1.0f, -10.0f, 0.40f },
    {  14.0f,  22.0f, -1.0f,  25.0f, 0.40f },
    {  -4.0f, -22.0f, -1.0f,  60.0f, 0.40f },
};

static const AABB scene_obstacles[] = {
    // House
    { -5.5f, 5.5f, -5.5f, 5.5f, -2.0f, 3.0f },
    // Tent
    { 18.5f, 21.5f, 18.5f, 21.5f, -2.0f, 3.0f },
    // Skeleton
    //{ 14.0f, 16.0f, 14.0f, 16.0f, -2.0f, 3.0f },
    // Fireplace
    { 21.0f, 23.5f, 21.0f, 23.5f, -2.0f, 3.0f },
    // Wood trunks
    { -19.0f, -17.0f, -19.0f, -17.0f, -2.0f, 3.0f },
    { -15.0f, -13.0f, -23.0f, -21.0f, -2.0f, 3.0f },
    { -21.0f, -19.0f, -11.0f, -9.0f, -2.0f, 3.0f },
    { -13.0f, -11.0f, -17.0f, -15.0f, -2.0f, 3.0f },
    { -17.0f, -15.0f, -27.0f, -25.0f, -2.0f, 3.0f },
    { 13.0f, 15.0f, -19.0f, -17.0f, -2.0f, 3.0f },
    { 17.0f, 19.0f, -13.0f, -11.0f, -2.0f, 3.0f },
    { 21.0f, 23.0f, -21.0f, -19.0f, -2.0f, 3.0f },
    // Dead trees
    { -24.0f, -20.0f, 20.0f, 24.0f, -2.0f, 3.0f },
    { -18.0f, -16.0f, 24.0f, 28.0f, -2.0f, 3.0f },
    { -8.0f, -4.0f, 22.0f, 26.0f, -2.0f, 3.0f },
    { 8.0f, 12.0f, 26.0f, 30.0f, -2.0f, 3.0f },
    { 24.0f, 28.0f, 20.0f, 24.0f, -2.0f, 3.0f },
    { -33.0f, -31.0f, 7.0f, 9.0f, -2.0f, 3.0f },
    { -17.0f, -15.0f, 1.0f, 3.0f, -2.0f, 3.0f },
    { -13.0f, -11.0f, 7.0f, 9.0f, -2.0f, 3.0f },
    { 11.0f, 13.0f, 5.0f, 7.0f, -2.0f, 3.0f },
    { 17.0f, 19.0f, 1.0f, 3.0f, -2.0f, 3.0f },
    { 21.0f, 23.0f, 7.0f, 9.0f, -2.0f, 3.0f },
    { -11.0f, -9.0f, 17.0f, 19.0f, -2.0f, 3.0f },
    { 9.0f, 11.0f, 19.0f, 21.0f, -2.0f, 3.0f },
    { 15.0f, 17.0f, 13.0f, 15.0f, -2.0f, 3.0f },
};

bool point_inside_aabb(const AABB* box, float x, float y, float z)
{
    return x >= box->minX && x <= box->maxX
        && y >= box->minY && y <= box->maxY
        && z >= box->minZ && z <= box->maxZ;
}

bool scene_is_position_blocked(const Scene* scene, float x, float y, float z)
{
    (void)scene;
    for (size_t i = 0; i < sizeof(scene_obstacles) / sizeof(scene_obstacles[0]); ++i) {
        if (point_inside_aabb(&scene_obstacles[i], x, y, z)) {
            return true;
        }
    }
    return false;
}

bool scene_collect_scroll_if_touching(Scene* scene, float x, float y, float z)
{
    (void)z;
    for (size_t i = 0; i < sizeof(scroll_instances) / sizeof(scroll_instances[0]); ++i) {
        if (scene->scroll_collected[i]) {
            continue;
        }

        float dx = x - scroll_instances[i].x;
        float dy = y - scroll_instances[i].y;
        float distance2 = dx * dx + dy * dy;
        if (distance2 < 0.85f * 0.85f) {
            scene->scroll_collected[i] = true;
            return true;
        }
    }
    return false;
}

void init_scene(Scene* scene)
{
    //Terrain
    load_model(&(scene->enviroment), "assets/models/grass.obj");
    scene->texture_enviroment = load_texture("assets/textures/grasstexture.png");

    //House
    load_model(&(scene->house), "assets/models/newhouse.obj");
    scene->texture_house = load_texture("assets/textures/newhouse.jpg");

    //Moon
    load_model(&(scene->moon), "assets/models/moon.obj");
    scene->texture_moon = load_texture("assets/textures/moon.jpg");

    //Character
    load_model(&(scene->character), "assets/models/flashlight.obj");
    scene->texture_character = load_texture("assets/textures/flashlight.jpg");

    //Wood trunk
    load_model(&(scene->wood_trunk), "assets/models/woodtrunk.obj");
    scene->texture_wood_trunk = load_texture("assets/textures/woodtrunk.jpg");

    // Dead tree
    load_model(&(scene->dead_tree), "assets/models/deadtree.obj");
    scene->texture_dead_tree = load_texture("assets/textures/deadtree.jpg");

    // Tent
    load_model(&(scene->tent), "assets/models/tent.obj");
    scene->texture_tent = load_texture("assets/textures/tent.jpg");

    // Fireplace
    load_model(&(scene->fireplace), "assets/models/fireplace_new.obj");
    scene->texture_fireplace = load_texture("assets/textures/fireplace.jpg");

    // Skeleton
    //load_model(&(scene->skeleton), "assets/models/skeleton.obj");
    //scene->texture_skeleton = load_texture("assets/textures/skeleton.jpg");

    // Help
    scene->help = load_texture("assets/textures/help.jpg");

    // End
    scene->end = load_texture("assets/textures/end.jpg");

    // Scroll
    load_model(&(scene->scroll), "assets/models/scroll.obj");
    scene->texture_scroll = load_texture("assets/textures/scroll.jpg");

    scene->scroll_animation_phase = 0.0f;
    scene->preview = 0;
    for (size_t i = 0; i < sizeof(scene->scroll_collected) / sizeof(scene->scroll_collected[0]); ++i) {
        scene->scroll_collected[i] = false;
    }

    scene->material.ambient.red = 0.2;
    scene->material.ambient.green = 0.2;
    scene->material.ambient.blue = 0.2;

    scene->material.diffuse.red = 0.8;
    scene->material.diffuse.green = 0.8;
    scene->material.diffuse.blue = 0.8;

    scene->material.specular.red = 1.0;
    scene->material.specular.green = 1.0;
    scene->material.specular.blue = 1.0;

    scene->material.shininess = 60.0;

    scene->flashlight_enabled = false;
    scene->fog_enabled = false;
    scene->brightness = 1.0f;
}

void set_lighting(const Scene* scene)
{
    float ambient_light[] = {
        0.0f,
        0.0f,
        0.0f,
        1.0f
    };
    float diffuse_light[] = {
        1.0f * scene->brightness,
        1.0f * scene->brightness,
        1.0f * scene->brightness,
        1.0f
    };
    float specular_light[] = {
        0.0f,
        0.0f,
        0.0f,
        1.0f
    };
    float position[] = { 0.0f, 0.0f, 10.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void set_moon_light(const Scene* scene)
{
    float ambient_light[] = {
        0.03f * scene->brightness,
        0.03f * scene->brightness,
        0.05f * scene->brightness,
        1.0f
    };
    float diffuse_light[] = {
        0.22f * scene->brightness,
        0.22f * scene->brightness,
        0.28f * scene->brightness,
        1.0f
    };
    float specular_light[] = {
        0.12f * scene->brightness,
        0.12f * scene->brightness,
        0.15f * scene->brightness,
        1.0f
    };
    float position[] = { 0.0f, -70.0f, 50.0f, 1.0f };
    float spot_direction[] = { 0.0f, 0.8f, -0.6f };

    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT1, GL_POSITION, position);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0f);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1.5f);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.005f);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0002f);
    glEnable(GL_LIGHT1);
}

void set_material(const Material* material)
{
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue
    };

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue
    };

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void update_scene(Scene* scene)
{
    float seconds = (float)SDL_GetTicks() / 1000.0f;
    scene->scroll_animation_phase = seconds * 1.4f;
}

void render_moon(const Scene* scene)
{
    glPushMatrix();

    Material moon_material = scene->material;

    moon_material.ambient.red = 0.30f;
    moon_material.ambient.green = 0.30f;
    moon_material.ambient.blue = 0.34f;

    moon_material.diffuse.red = 0.95f;
    moon_material.diffuse.green = 0.95f;
    moon_material.diffuse.blue = 1.0f;

    moon_material.specular.red = 0.85f;
    moon_material.specular.green = 0.85f;
    moon_material.specular.blue = 0.95f;

    moon_material.shininess = 80.0f;

    set_material(&moon_material);

    float emission_color[] = { 0.12f, 0.12f, 0.15f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission_color);

    glTranslatef(0.0f, -70.0f, 50.0f);
    glScalef(3.0f, 3.0f, 3.0f);

    glBindTexture(GL_TEXTURE_2D, scene->texture_moon);
    draw_model(&(scene->moon));

    // Glow
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.9f, 0.9f, 1.0f, 0.12f);

    GLUquadric* quad = gluNewQuadric();
    gluSphere(quad, 3.35f, 32, 32);
    gluDeleteQuadric(quad);

    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);


    float no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_emission);

    glPopMatrix();
}

void render_house(const Scene* scene)
{
    glPushMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);
    
    Material house_material = scene->material;
    house_material.ambient.red = 0.2f;
    house_material.ambient.green = 0.2f;
    house_material.ambient.blue = 0.2f;

    house_material.diffuse.red = 1.0f;
    house_material.diffuse.green = 1.0f;
    house_material.diffuse.blue = 1.0f;

    house_material.specular.red = 0.1f;
    house_material.specular.green = 0.1f;
    house_material.specular.blue = 0.1f;
    house_material.shininess = 5.0f;

    set_material(&house_material);

    float emission_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission_color);

    glTranslatef(0.0f, 0.0f, -1.0f);
    glScalef(1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, scene->texture_house);
    draw_model(&(scene->house));

    float no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_emission);

    glPopMatrix();
}


void set_flashlight(const Camera* camera, bool enabled, float brightness)
{
    if (!enabled) {
        glDisable(GL_LIGHT2);
        return;
    }

    float position[] = {
        (float)camera->position.x,
        (float)camera->position.y,
        (float)camera->position.z,
        1.0f
    };

    double yaw = degree_to_radian(camera->rotation.z);
    double pitch = degree_to_radian(camera->rotation.x);
    float direction[] = {
        (float)(cos(pitch) * cos(yaw)),
        (float)(cos(pitch) * sin(yaw)),
        (float)sin(pitch)
    };

    float ambient_light[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float diffuse_light[] = {
        1.0f * brightness,
        1.0f * brightness,
        0.95f * brightness,
        1.0f
    };
    float specular_light[] = {
        0.5f * brightness,
        0.5f * brightness,
        0.45f * brightness,
        1.0f
    };

    glLightfv(GL_LIGHT2, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT2, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT2, GL_POSITION, position);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, direction);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 20.0f);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 15.0f);
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.01f);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.0005f);
    glEnable(GL_LIGHT2);
}

void render_wood_trunks(const Scene* scene)
{
    struct WoodTrunkInstance {
        float x;
        float y;
        float z;
        float rotation;
        float scale;
    } trunks[] = {

        { -18.0f, -18.0f, -1.0f,   0.0f, 0.92f },
        { -14.0f, -22.0f, -1.0f,  35.0f, 0.88f },
        { -20.0f, -10.0f, -1.0f, -20.0f, 0.95f },
        { -12.0f, -16.0f, -1.0f,  15.0f, 0.90f },
        { -16.0f, -26.0f, -1.0f, -30.0f, 0.87f },

        {  14.0f, -18.0f, -1.0f,  50.0f, 0.93f },
        {  18.0f, -12.0f, -1.0f, -15.0f, 0.89f },
        {  22.0f, -20.0f, -1.0f,  25.0f, 0.91f },

        { -22.0f,   6.0f, -1.0f,  40.0f, 0.86f },
        { -16.0f,   2.0f, -1.0f, -35.0f, 0.94f },
        { -12.0f,   8.0f, -1.0f,  10.0f, 0.88f },

        {  12.0f,   6.0f, -1.0f, -20.0f, 0.90f },
        {  18.0f,   2.0f, -1.0f,  30.0f, 0.87f },
        {  22.0f,   8.0f, -1.0f, -10.0f, 0.92f },

        { -10.0f,  18.0f, -1.0f,  15.0f, 0.91f },
        {  10.0f,  20.0f, -1.0f, -25.0f, 0.88f },
        {  16.0f,  14.0f, -1.0f,  35.0f, 0.93f },
    };

    for (size_t i = 0; i < sizeof(trunks) / sizeof(trunks[0]); ++i) {
        glPushMatrix();
        glTranslatef(trunks[i].x, trunks[i].y, trunks[i].z);
        glRotatef(trunks[i].rotation, 0.0f, 0.0f, 1.0f);
        glScalef(trunks[i].scale, trunks[i].scale, trunks[i].scale);
        glBindTexture(GL_TEXTURE_2D, scene->texture_wood_trunk);
        draw_model(&(scene->wood_trunk));
        glPopMatrix();
    }
}

void render_trees(const Scene* scene)
{
    struct TreeInstance {
        float x;
        float y;
        float z;
        float rotation;
        float scale;
    } trees[] = {

        // felső régió
        { -28.0f,  22.0f, -1.0f,  10.0f, 1.05f },
        { -18.0f,  26.0f, -1.0f, -25.0f, 0.95f },
        {  -6.0f,  24.0f, -1.0f,  40.0f, 1.10f },
        {  10.0f,  28.0f, -1.0f, -15.0f, 1.00f },
        {  26.0f,  22.0f, -1.0f,  30.0f, 1.08f },

        // bal oldal
        { -32.0f,   8.0f, -1.0f,  15.0f, 1.02f },
        { -30.0f,  -6.0f, -1.0f, -20.0f, 0.98f },

        // jobb oldal
        {  30.0f,   6.0f, -1.0f,  35.0f, 1.05f },
        {  28.0f,  -8.0f, -1.0f, -30.0f, 0.97f },

        // alsó szélek
        { -24.0f, -26.0f, -1.0f,  20.0f, 1.10f },
        {   6.0f, -28.0f, -1.0f, -10.0f, 1.00f },
    };

    for (size_t i = 0; i < sizeof(trees) / sizeof(trees[0]); ++i)
    {
        glPushMatrix();

        glTranslatef(trees[i].x, trees[i].y, trees[i].z);
        glRotatef(trees[i].rotation, 0.0f, 0.0f, 1.0f);
        glScalef(trees[i].scale, trees[i].scale, trees[i].scale);

        glBindTexture(GL_TEXTURE_2D, scene->texture_dead_tree);
        draw_model(&(scene->dead_tree));

        glPopMatrix();
    }
}

void render_fireplace(const Scene* scene)
{
    glPushMatrix();

    glTranslatef(22.0f, 22.0f, -1.0f);
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    glScalef(0.9f, 0.9f, 0.9f);

    glBindTexture(GL_TEXTURE_2D, scene->texture_fireplace);
    draw_model(&(scene->fireplace));

    float t = SDL_GetTicks() * 0.006f;

    float flameHeight = 0.9f + 0.15f * sin(t * 3.0f);
    float flameWidth  = 0.4f + 0.05f * cos(t * 4.0f);

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glTranslatef(0.0f, 0.0f, 0.35f);

    glBegin(GL_TRIANGLES);

    glColor4f(1.0f, 0.35f, 0.0f, 0.35f);
    glVertex3f(-flameWidth,0.0f,0.0f);

    glColor4f(1.0f,0.8f,0.0f,0.8f);
    glVertex3f(0.0f,0.0f,flameHeight);

    glColor4f(1.0f,0.35f,0.0f,0.35f);
    glVertex3f(flameWidth,0.0f,0.0f);

    glEnd();

    glBegin(GL_TRIANGLES);

    glColor4f(1.0f,1.0f,0.2f,0.7f);
    glVertex3f(-flameWidth*0.5f,0.0f,0.0f);

    glColor4f(1.0f,1.0f,0.8f,1.0f);
    glVertex3f(0.0f,0.0f,flameHeight*0.7f);

    glColor4f(1.0f,1.0f,0.2f,0.7f);
    glVertex3f(flameWidth*0.5f,0.0f,0.0f);

    glEnd();

    glDisable(GL_BLEND);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

    glPopMatrix();
}

void render_scrolls(const Scene* scene)
{
    for (size_t i = 0; i < sizeof(scroll_instances) / sizeof(scroll_instances[0]); ++i)
    {
        if (scene->scroll_collected[i]) {
            continue;
        }

        float bob = sinf(scene->scroll_animation_phase) * 0.25f;

        glPushMatrix();
        glTranslatef(scroll_instances[i].x, scroll_instances[i].y + bob, scroll_instances[i].z);
        glRotatef(scroll_instances[i].rotation, 0.0f, 0.0f, 1.0f);
        glScalef(scroll_instances[i].scale, scroll_instances[i].scale, scroll_instances[i].scale);

        Material scroll_material = scene->material;
        scroll_material.ambient.red = 0.45f;
        scroll_material.ambient.green = 0.45f;
        scroll_material.ambient.blue = 0.40f;

        scroll_material.diffuse.red = 0.95f;
        scroll_material.diffuse.green = 0.95f;
        scroll_material.diffuse.blue = 0.90f;

        scroll_material.specular.red = 1.0f;
        scroll_material.specular.green = 1.0f;
        scroll_material.specular.blue = 1.0f;
        scroll_material.shininess = 120.0f;

        set_material(&scroll_material);

        float emission_color[] = { 0.18f, 0.18f, 0.18f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission_color);

        glBindTexture(GL_TEXTURE_2D, scene->texture_scroll);
        draw_model(&(scene->scroll));

        float no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_emission);

        glPopMatrix();
    }
}


void compute_shadow_position(
    const vec3* light_pos,
    float obj_x, float obj_y, float obj_z,
    float ground_z,
    float* shadow_x, float* shadow_y) {
    float dz_obj = obj_z - light_pos->z;
    float dz_ground = ground_z - light_pos->z;
    
    if (fabs(dz_obj) < 0.01f) {
        *shadow_x = obj_x;
        *shadow_y = obj_y;
        return;
    }
    
    float t = dz_ground / dz_obj;
    if (t <= 0.0f) {
        *shadow_x = obj_x;
        *shadow_y = obj_y;
        return;
    }
    
    *shadow_x = light_pos->x + (obj_x - light_pos->x) * t;
    *shadow_y = light_pos->y + (obj_y - light_pos->y) * t;
}

void render_shadow_for_model(
    const vec3* light_pos,
    float obj_x, float obj_y, float obj_z,
    float obj_scale, float obj_rotation,
    const Model* model) {
    float ground_z = -1.0f;
    float shadow_x, shadow_y;
    float light_distance = sqrtf(
        (obj_x - light_pos->x) * (obj_x - light_pos->x) +
        (obj_y - light_pos->y) * (obj_y - light_pos->y)
    );
    
    if (light_distance > 40.0f || light_distance < 0.5f) {
        return;
    }
    
    float intensity = 0.40f * (1.0f - (light_distance / 40.0f));
    if (intensity < 0.1f) return;
    
    compute_shadow_position(light_pos, obj_x, obj_y, obj_z, ground_z, &shadow_x, &shadow_y);
    
    glPushMatrix();
    glTranslatef(shadow_x, shadow_y, ground_z + 0.005f);
    glRotatef(obj_rotation, 0.0f, 0.0f, 1.0f);
    glScalef(obj_scale * 0.88f, obj_scale * 0.88f, obj_scale * 0.88f);
    
    glDisable(GL_LIGHTING);
    glColor4f(0.0f, 0.0f, 0.0f, intensity);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    draw_model(model);
    
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void render_shadows(const Scene* scene, const Camera* camera)
{
    if (!scene->flashlight_enabled) return;
    
    vec3 light_pos;
    light_pos.x = (float)camera->position.x;
    light_pos.y = (float)camera->position.y;
    light_pos.z = (float)camera->position.z;
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    struct WoodTrunkInstance {
        float x;
        float y;
        float z;
        float rotation;
        float scale;
    } trunks[] = {
        { -18.0f, -18.0f, -1.0f,   0.0f, 0.92f },
        { -14.0f, -22.0f, -1.0f,  35.0f, 0.88f },
        { -20.0f, -10.0f, -1.0f, -20.0f, 0.95f },
        { -12.0f, -16.0f, -1.0f,  15.0f, 0.90f },
        { -16.0f, -26.0f, -1.0f, -30.0f, 0.87f },
        {  14.0f, -18.0f, -1.0f,  50.0f, 0.93f },
        {  18.0f, -12.0f, -1.0f, -15.0f, 0.89f },
        {  22.0f, -20.0f, -1.0f,  25.0f, 0.91f },
        { -22.0f,   6.0f, -1.0f,  40.0f, 0.86f },
        { -16.0f,   2.0f, -1.0f, -35.0f, 0.94f },
        { -12.0f,   8.0f, -1.0f,  10.0f, 0.88f },
        {  12.0f,   6.0f, -1.0f, -20.0f, 0.90f },
        {  18.0f,   2.0f, -1.0f,  30.0f, 0.87f },
        {  22.0f,   8.0f, -1.0f, -10.0f, 0.92f },
        { -10.0f,  18.0f, -1.0f,  15.0f, 0.91f },
        {  10.0f,  20.0f, -1.0f, -25.0f, 0.88f },
        {  16.0f,  14.0f, -1.0f,  35.0f, 0.93f },
    };
    
    for (size_t i = 0; i < sizeof(trunks) / sizeof(trunks[0]); ++i) {
        render_shadow_for_model(&light_pos, trunks[i].x, trunks[i].y, trunks[i].z,
                               trunks[i].scale, trunks[i].rotation, &(scene->wood_trunk));
    }
    
    struct TreeInstance {
        float x;
        float y;
        float z;
        float rotation;
        float scale;
    } trees[] = {
        { -28.0f,  22.0f, -1.0f,  10.0f, 1.05f },
        { -18.0f,  26.0f, -1.0f, -25.0f, 0.95f },
        {  -6.0f,  24.0f, -1.0f,  40.0f, 1.10f },
        {  10.0f,  28.0f, -1.0f, -15.0f, 1.00f },
        {  26.0f,  22.0f, -1.0f,  30.0f, 1.08f },
        { -32.0f,   8.0f, -1.0f,  15.0f, 1.02f },
        { -30.0f,  -6.0f, -1.0f, -20.0f, 0.98f },
        {  30.0f,   6.0f, -1.0f,  35.0f, 1.05f },
        {  28.0f,  -8.0f, -1.0f, -30.0f, 0.97f },
        { -24.0f, -26.0f, -1.0f,  20.0f, 1.10f },
        {   6.0f, -28.0f, -1.0f, -10.0f, 1.00f },
    };
    
    for (size_t i = 0; i < sizeof(trees) / sizeof(trees[0]); ++i) {
        render_shadow_for_model(&light_pos, trees[i].x, trees[i].y, trees[i].z,
                               trees[i].scale, trees[i].rotation, &(scene->dead_tree));
    }
    
    render_shadow_for_model(&light_pos, 20.0f, 20.0f, -1.0f, 0.9f, 45.0f, &(scene->tent));
    
    float bob = sinf(scene->scroll_animation_phase) * 0.25f;
    for (size_t i = 0; i < sizeof(scroll_instances) / sizeof(scroll_instances[0]); ++i) {
        if (scene->scroll_collected[i]) {
            continue;
        }
        render_shadow_for_model(&light_pos,
                               scroll_instances[i].x,
                               scroll_instances[i].y + bob,
                               scroll_instances[i].z,
                               scroll_instances[i].scale,
                               scroll_instances[i].rotation,
                               &(scene->scroll));
    }
    
    glDisable(GL_BLEND);
}

void render_scene(const Scene* scene, const Camera* camera)
{
    if (scene->fog_enabled) {
        glEnable(GL_FOG);

        //float fog_color[] = { 0.15f, 0.15f, 0.18f, 1.0f };
        float fog_color[] = {1, 1, 1, 1};

        glFogfv(GL_FOG_COLOR, fog_color);

        glFogi(GL_FOG_MODE, GL_EXP2);
        glFogf(GL_FOG_DENSITY, 0.06f);

    } else {
        glDisable(GL_FOG);
    }

    set_material(&(scene->material));
    set_lighting(scene);
    set_flashlight(camera, scene->flashlight_enabled, scene->brightness);
    set_moon_light(scene);
    draw_origin();

    //draw terrain
    glPushMatrix();
    glTranslatef(0, 0, -1);
    glBindTexture(GL_TEXTURE_2D, scene->texture_enviroment);
    draw_model(&(scene->enviroment));
    glPopMatrix();

    //draw tent
    glPushMatrix();
    glTranslatef(20.0f, 20.0f, -1.0f);
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
    glScalef(0.9f, 0.9f, 0.9f);
    glBindTexture(GL_TEXTURE_2D, scene->texture_tent);
    draw_model(&(scene->tent));
    glPopMatrix();

    //draw skeleton
    // glPushMatrix();
    // glTranslatef(15.0f, 15.0f, -1.0f);
    // glRotatef(5.0f, 0.0f, 0.0f, 1.0f);
    // glScalef(0.5f, 0.5f, 0.5f);
    // glBindTexture(GL_TEXTURE_2D, scene->texture_skeleton);
    // draw_model(&(scene->skeleton));
    // glPopMatrix();

    //draw fireplace
    render_fireplace(scene);
    //draw scroll
    render_scrolls(scene);
    // Draw multiple wood trunks with varying positions and rotations
    render_wood_trunks(scene);
    // Draw multiple dead trees with varying positions and rotations
    render_trees(scene);
    // Draw the moon with its own settings
    render_moon(scene);
    // Draw the house with its own settings
    render_house(scene);
    
    // Render projected shadows from the flashlight
    render_shadows(scene, camera);
    
    // Draw the camera-attached flashlight character
    render_flashlight(scene);
}

void render_flashlight(const Scene* scene)
{
    glPushMatrix();
    glLoadIdentity();

    // Camera-space placement: slightly right, slightly down, and forward.
    glTranslatef(0.35f, -0.35f, -0.9f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glScalef(0.06f, 0.06f, 0.06f);

    glDisable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, scene->texture_character);
    draw_model(&(scene->character));
    glEnable(GL_LIGHTING);

    glPopMatrix();
}

void draw_origin()
{
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);

    glEnd();
}
