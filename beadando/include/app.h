#ifndef APP_H
#define APP_H

#include "camera.h"
#include "scene.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#define VIEWPORT_RATIO (4.0 / 3.0)
#define VIEWPORT_ASPECT 50.0

typedef struct TextTexture
{
    GLuint texture_id;
    int width;
    int height;
} TextTexture;

typedef struct App
{
    SDL_Window* window;
    SDL_GLContext gl_context;
    bool is_running;
    double uptime;
    Camera camera;
    Scene scene;
    
    TTF_Font* ui_font;
    TextTexture timer_texture;
    TextTexture scroll_texture;
    Uint32 timer_start_time;
    int scroll_count;
    int scroll_max;
    bool show_help;
    int window_width;
    int window_height;
} App;

/**
 * Initialize the application.
 */
void init_app(App* app, int width, int height);

/**
 * Initialize the OpenGL context.
 */
void init_opengl();

/**
 * Reshape the window.
 */
void reshape(GLsizei width, GLsizei height);

/**
 * Handle the events of the application.
 */
void handle_app_events(App* app);

/**
 * Update the application.
 */
void update_app(App* app);

/**
 * Render the application.
 */
void render_app(App* app);

/**
 * Destroy the application.
 */
void destroy_app(App* app);

/**
 * Create a texture from SDL surface for OpenGL rendering.
 */
TextTexture create_text_texture(SDL_Surface* surface);

/**
 * Delete a text texture.
 */
void delete_text_texture(TextTexture* texture);

/**
 * Render UI text (timer and scroll counter).
 */
void render_ui(App* app);

/**
 * Update UI text textures.
 */
void update_ui_textures(App* app);

/**
 * Move the camera based on its speed and handle collision with the scene.
*/
void move_camera_with_collision(App* app, double time);

#endif /* APP_H */
