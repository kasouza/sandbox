#ifndef KASOUZA_SANDBOX_INCLUDE_RENDERER_H
#define KASOUZA_SANDBOX_INCLUDE_RENDERER_H

#include "input.h"

// These two must be imported in this order
// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

typedef void (*box_render_log_t)(const char *, ...);

typedef void (*box_render_mouse_button_cb_t)(float x_pos, float y_pos,
                                             enum BoxMouseButton mouse_button,
                                             enum BoxInputAction action);

typedef void (*box_render_keyboard_cb_t)(enum BoxKey key,
                                         enum BoxInputAction action);

typedef void (*box_render_canvas_click_cb_t)(int x_pos, int y_pos,
                                             enum BoxMouseButton mouse_button,
                                             enum BoxInputAction action);
enum BoxRenderError
{
    BOX_RENDER_SUCCESS,
    BOX_RENDER_GLFW_INIT_ERROR,
    BOX_RENDER_WINDOW_CREATION_ERROR,
    BOX_RENDER_GL_LOAD_ERROR,
    BOX_RENDER_PROGRAM_CREATION_ERROR,
    BOX_RENDER_FRAMEBUFFER_CREATION_ERROR,
};

int box_render_init(box_render_log_t log);
void box_render_draw_pixel(int x, int y, int r, int g, int b);
void box_render_clear();
void box_render_present();
void box_render_terminate();

void box_render_set_mouse_button_callback(
    box_render_mouse_button_cb_t mouse_button_cb);

void box_render_set_canvas_click_callback(
    box_render_canvas_click_cb_t canvas_click_cb);

void box_render_set_keyboard_callback(box_render_keyboard_cb_t keyboard_cb);

#endif
