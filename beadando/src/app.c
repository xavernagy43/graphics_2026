#include "app.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void init_app(App* app, int width, int height)
{
    int error_code;
    int inited_loaders;

    app->is_running = false;

    error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0) {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return;
    }

    app->window = SDL_CreateWindow(
        "Codex Collect",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL);
    if (app->window == NULL) {
        printf("[ERROR] Unable to create the application window!\n");
        return;
    }

    inited_loaders = IMG_Init(IMG_INIT_PNG);
    if (inited_loaders == 0) {
        printf("[ERROR] IMG initialization error: %s\n", IMG_GetError());
        return;
    }

    if (TTF_Init() == -1) {
        printf("[ERROR] TTF initialization error: %s\n", TTF_GetError());
        return;
    }

    app->gl_context = SDL_GL_CreateContext(app->window);
    if (app->gl_context == NULL) {
        printf("[ERROR] Unable to create the OpenGL context!\n");
        return;
    }

    app->window_width = width;
    app->window_height = height;

    init_opengl();
    reshape(width, height);

    init_camera(&(app->camera));
    init_scene(&(app->scene));

    /* Initialize UI */
    app->ui_font = TTF_OpenFont("assets/fonts/Arial.ttf", 24);
    if (app->ui_font == NULL) {
        /* Try Windows system fonts as fallback */
        app->ui_font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 24);
    }
    if (app->ui_font == NULL) {
        printf("[WARNING] Could not load font: %s\n", TTF_GetError());
        app->ui_font = NULL;
    }
    
    app->timer_start_time = SDL_GetTicks();
    app->scroll_count = 0;
    app->scroll_max = 8;
    app->show_help = false;
    app->timer_texture.texture_id = 0;
    app->scroll_texture.texture_id = 0;

    update_ui_textures(app);

    app->uptime = (double)SDL_GetTicks() / 1000.0;
    app->is_running = true;
}

void init_opengl()
{
    glShadeModel(GL_SMOOTH);

    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);

    glClearColor(0.1, 0.1, 0.1, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    glClearDepth(1.0);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void reshape(GLsizei width, GLsizei height)
{
    int x, y, w, h;
    double ratio;

    ratio = (double)width / height;
    if (ratio > VIEWPORT_RATIO) {
        w = (int)((double)height * VIEWPORT_RATIO);
        h = height;
        x = (width - w) / 2;
        y = 0;
    }
    else {
        w = width;
        h = (int)((double)width / VIEWPORT_RATIO);
        x = 0;
        y = (height - h) / 2;
    }

    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(
        -.08, .08,
        -.06, .06,
        .1, 100.0
    );
    /* glFrustum(
        -.08, .08,
        -.06, .06,
        .1, 10
    ); */
}

void move_camera_with_collision(App* app, double time)
{
    Camera* camera = &(app->camera);
    double angle = degree_to_radian(camera->rotation.z);
    double side_angle = degree_to_radian(camera->rotation.z + 90.0);

    double x_delta = cos(angle) * camera->speed.y * time;
    double y_delta = sin(angle) * camera->speed.y * time;
    double x_side = cos(side_angle) * camera->speed.x * time;
    double y_side = sin(side_angle) * camera->speed.x * time;

    double current_z = camera->position.z;

    // Try X movement and Y movement separately so the camera can slide along obstacles.
    double x_test = camera->position.x + x_delta + x_side;
    double y_test = camera->position.y;
    if (!scene_is_position_blocked(&app->scene, x_test, y_test, current_z)) {
        camera->position.x = x_test;
    }

    x_test = camera->position.x;
    y_test = camera->position.y + y_delta + y_side;
    if (!scene_is_position_blocked(&app->scene, x_test, y_test, current_z)) {
        camera->position.y = y_test;
    }

    if (scene_collect_scroll_if_touching(&app->scene, camera->position.x, camera->position.y, current_z)) {
        app->scroll_count += 1;
        if (app->scroll_count > app->scroll_max) {
            app->scroll_count = app->scroll_max;
        }
    }
}

void handle_app_events(App* app)
{
    SDL_Event event;
    static bool is_mouse_down = false;
    static int mouse_x = 0;
    static int mouse_y = 0;
    int x;
    int y;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_z || event.key.keysym.sym == SDLK_y) {
                app->scene.brightness -= 0.1f;
                if (app->scene.brightness < 0.1f) {
                    app->scene.brightness = 0.1f;
                }
                break;
            }
            if (event.key.keysym.sym == SDLK_t) {
                app->scene.brightness += 0.1f;
                if (app->scene.brightness > 2.0f) {
                    app->scene.brightness = 2.0f;
                }
                break;
            }
            if (event.key.keysym.sym == SDLK_k) {
                app->scene.fog_enabled = !app->scene.fog_enabled;
                break;
            }

            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                app->is_running = false;
                break;
            case SDL_SCANCODE_W:
                set_camera_speed(&(app->camera), 1);
                break;
            case SDL_SCANCODE_S:
                set_camera_speed(&(app->camera), -1);
                break;
            case SDL_SCANCODE_A:
                set_camera_side_speed(&(app->camera), 1);
                break;
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), -1);
                break;
            case SDL_SCANCODE_F:
                app->scene.flashlight_enabled = !app->scene.flashlight_enabled;
                break;
            case SDL_SCANCODE_H:
                app->show_help = !app->show_help;
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_S:
                set_camera_speed(&(app->camera), 0);
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), 0);
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            is_mouse_down = true;
            break;
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&x, &y);
            if (is_mouse_down) {
                rotate_camera(&(app->camera), mouse_x - x, mouse_y - y);
            }
            mouse_x = x;
            mouse_y = y;
            break;
        case SDL_MOUSEBUTTONUP:
            is_mouse_down = false;
            break;
        case SDL_QUIT:
            app->is_running = false;
            break;
        default:
            break;
        }
    }
}

void update_app(App* app)
{
    double current_time;
    double elapsed_time;

    current_time = (double)SDL_GetTicks() / 1000;
    elapsed_time = current_time - app->uptime;
    app->uptime = current_time;

    move_camera_with_collision(app, elapsed_time);
    update_scene(&(app->scene));
    update_ui_textures(app);
}

void render_app(App* app)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    set_view(&(app->camera));
    render_scene(&(app->scene), &(app->camera));
    glPopMatrix();

    if (app->camera.is_preview_visible) {
        show_texture_preview();
    }

    render_ui(app);

    SDL_GL_SwapWindow(app->window);
}

void destroy_app(App* app)
{
    if (app->timer_texture.texture_id != 0) {
        glDeleteTextures(1, &app->timer_texture.texture_id);
    }
    
    if (app->scroll_texture.texture_id != 0) {
        glDeleteTextures(1, &app->scroll_texture.texture_id);
    }
    
    if (app->ui_font != NULL) {
        TTF_CloseFont(app->ui_font);
    }
    
    TTF_Quit();

    if (app->gl_context != NULL) {
        SDL_GL_DeleteContext(app->gl_context);
    }

    if (app->window != NULL) {
        SDL_DestroyWindow(app->window);
    }

    SDL_Quit();
}

TextTexture create_text_texture(SDL_Surface* surface)
{
    TextTexture texture;
    GLuint tex_id;

    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

    texture.texture_id = tex_id;
    texture.width = surface->w;
    texture.height = surface->h;

    return texture;
}

void delete_text_texture(TextTexture* texture)
{
    if (texture->texture_id != 0) {
        glDeleteTextures(1, &texture->texture_id);
        texture->texture_id = 0;
        texture->width = 0;
        texture->height = 0;
    }
}

void update_ui_textures(App* app)
{
    char timer_text[16];
    char scroll_text[32];
    Uint32 current_time;
    Uint32 elapsed_ms;
    int minutes, seconds;
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* surface;

    if (app->ui_font == NULL) {
        return;
    }

    /* Calculate remaining time (120 seconds countdown) */
    current_time = SDL_GetTicks();
    elapsed_ms = current_time - app->timer_start_time;
    
    int remaining_ms = 120000 - elapsed_ms;
    if (remaining_ms < 0) {
        remaining_ms = 0;
    }
    
    minutes = remaining_ms / 60000;
    seconds = (remaining_ms % 60000) / 1000;
    
    snprintf(timer_text, sizeof(timer_text), "%02d:%02d", minutes, seconds);

    /* Update timer texture */
    surface = TTF_RenderText_Solid(app->ui_font, timer_text, white);
    if (surface != NULL) {
        /* Convert RGBA for OpenGL */
        SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
        SDL_FreeSurface(surface);
        
        if (converted != NULL) {
            delete_text_texture(&app->timer_texture);
            app->timer_texture = create_text_texture(converted);
            SDL_FreeSurface(converted);
        }
    }

    /* Update scroll counter texture */
    snprintf(scroll_text, sizeof(scroll_text), "Tekercsek: %d / %d", 
             app->scroll_count, app->scroll_max);
    
    surface = TTF_RenderText_Solid(app->ui_font, scroll_text, white);
    if (surface != NULL) {
        SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
        SDL_FreeSurface(surface);
        
        if (converted != NULL) {
            delete_text_texture(&app->scroll_texture);
            app->scroll_texture = create_text_texture(converted);
            SDL_FreeSurface(converted);
        }
    }
}

void render_ui(App* app)
{
    int padding = 10;
    int x, y;

    if (app->timer_texture.texture_id == 0 ||
        app->scroll_texture.texture_id == 0) {
        return;
    }

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, app->window_width,
            app->window_height, 0,
            -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_TRANSFORM_BIT | GL_COLOR_BUFFER_BIT);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    if (app->show_help && app->scene.help != 0) {
        glBindTexture(GL_TEXTURE_2D, app->scene.help);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f((float)app->window_width, 0.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f((float)app->window_width, (float)app->window_height);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, (float)app->window_height);
        glEnd();
    }

    x = app->window_width - app->timer_texture.width - padding;
    y = padding;

    glBindTexture(GL_TEXTURE_2D, app->timer_texture.texture_id);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(x, y);
        glTexCoord2f(1, 0); glVertex2f(x + app->timer_texture.width, y);
        glTexCoord2f(1, 1); glVertex2f(x + app->timer_texture.width, y + app->timer_texture.height);
        glTexCoord2f(0, 1); glVertex2f(x, y + app->timer_texture.height);
    glEnd();

    y = padding + app->timer_texture.height + 5;
    x = app->window_width - app->scroll_texture.width - padding;

    glBindTexture(GL_TEXTURE_2D, app->scroll_texture.texture_id);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(x, y);
        glTexCoord2f(1, 0); glVertex2f(x + app->scroll_texture.width, y);
        glTexCoord2f(1, 1); glVertex2f(x + app->scroll_texture.width, y + app->scroll_texture.height);
        glTexCoord2f(0, 1); glVertex2f(x, y + app->scroll_texture.height);
    glEnd();

    glPopAttrib();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}
