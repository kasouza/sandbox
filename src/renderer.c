#include "renderer.h"
#include "utils.h"

#include <math.h>
#include <stb_image.h>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG_LOG(...)                                                         \
    if (box_render_log)                                                        \
    {                                                                          \
        box_render_log(__VA_ARGS__);                                           \
    }

#define SETUP(thing)                                                           \
    enum BoxRenderError thing##_error = setup_##thing();                       \
    if (thing##_error != BOX_RENDER_SUCCESS)                                   \
    {                                                                          \
        return thing##_error;                                                  \
    }

// DO NOT CHANGE THE ORDER
static int box_keys_to_glfw_keys[] = {
    [BOX_KEY_SPACE] = GLFW_KEY_SPACE,
    [BOX_KEY_APOSTROPHE] = GLFW_KEY_APOSTROPHE,
    [BOX_KEY_COMMA] = GLFW_KEY_COMMA,
    [BOX_KEY_MINUS] = GLFW_KEY_MINUS,
    [BOX_KEY_PERIOD] = GLFW_KEY_PERIOD,
    [BOX_KEY_SLASH] = GLFW_KEY_SLASH,
    [BOX_KEY_0] = GLFW_KEY_0,
    [BOX_KEY_1] = GLFW_KEY_1,
    [BOX_KEY_2] = GLFW_KEY_2,
    [BOX_KEY_3] = GLFW_KEY_3,
    [BOX_KEY_4] = GLFW_KEY_4,
    [BOX_KEY_5] = GLFW_KEY_5,
    [BOX_KEY_6] = GLFW_KEY_6,
    [BOX_KEY_7] = GLFW_KEY_7,
    [BOX_KEY_8] = GLFW_KEY_8,
    [BOX_KEY_9] = GLFW_KEY_9,
    [BOX_KEY_SEMICOLON] = GLFW_KEY_SEMICOLON,
    [BOX_KEY_EQUAL] = GLFW_KEY_EQUAL,
    [BOX_KEY_A] = GLFW_KEY_A,
    [BOX_KEY_B] = GLFW_KEY_B,
    [BOX_KEY_C] = GLFW_KEY_C,
    [BOX_KEY_D] = GLFW_KEY_D,
    [BOX_KEY_E] = GLFW_KEY_E,
    [BOX_KEY_F] = GLFW_KEY_F,
    [BOX_KEY_G] = GLFW_KEY_G,
    [BOX_KEY_H] = GLFW_KEY_H,
    [BOX_KEY_I] = GLFW_KEY_I,
    [BOX_KEY_J] = GLFW_KEY_J,
    [BOX_KEY_K] = GLFW_KEY_K,
    [BOX_KEY_L] = GLFW_KEY_L,
    [BOX_KEY_M] = GLFW_KEY_M,
    [BOX_KEY_N] = GLFW_KEY_N,
    [BOX_KEY_O] = GLFW_KEY_O,
    [BOX_KEY_P] = GLFW_KEY_P,
    [BOX_KEY_Q] = GLFW_KEY_Q,
    [BOX_KEY_R] = GLFW_KEY_R,
    [BOX_KEY_S] = GLFW_KEY_S,
    [BOX_KEY_T] = GLFW_KEY_T,
    [BOX_KEY_U] = GLFW_KEY_U,
    [BOX_KEY_V] = GLFW_KEY_V,
    [BOX_KEY_W] = GLFW_KEY_W,
    [BOX_KEY_X] = GLFW_KEY_X,
    [BOX_KEY_Y] = GLFW_KEY_Y,
    [BOX_KEY_Z] = GLFW_KEY_Z,
    [BOX_KEY_LEFT_BRACKET] = GLFW_KEY_LEFT_BRACKET,
    [BOX_KEY_BACKSLASH] = GLFW_KEY_BACKSLASH,
    [BOX_KEY_RIGHT_BRACKET] = GLFW_KEY_RIGHT_BRACKET,
    [BOX_KEY_GRAVE_ACCENT] = GLFW_KEY_GRAVE_ACCENT,
    [BOX_KEY_WORLD_1] = GLFW_KEY_WORLD_1,
    [BOX_KEY_WORLD_2] = GLFW_KEY_WORLD_2,
    [BOX_KEY_ESCAPE] = GLFW_KEY_ESCAPE,
    [BOX_KEY_ENTER] = GLFW_KEY_ENTER,
    [BOX_KEY_TAB] = GLFW_KEY_TAB,
    [BOX_KEY_BACKSPACE] = GLFW_KEY_BACKSPACE,
    [BOX_KEY_INSERT] = GLFW_KEY_INSERT,
    [BOX_KEY_DELETE] = GLFW_KEY_DELETE,
    [BOX_KEY_RIGHT] = GLFW_KEY_RIGHT,
    [BOX_KEY_LEFT] = GLFW_KEY_LEFT,
    [BOX_KEY_DOWN] = GLFW_KEY_DOWN,
    [BOX_KEY_UP] = GLFW_KEY_UP,
    [BOX_KEY_PAGE_UP] = GLFW_KEY_PAGE_UP,
    [BOX_KEY_PAGE_DOWN] = GLFW_KEY_PAGE_DOWN,
    [BOX_KEY_HOME] = GLFW_KEY_HOME,
    [BOX_KEY_END] = GLFW_KEY_END,
    [BOX_KEY_CAPS_LOCK] = GLFW_KEY_CAPS_LOCK,
    [BOX_KEY_SCROLL_LOCK] = GLFW_KEY_SCROLL_LOCK,
    [BOX_KEY_NUM_LOCK] = GLFW_KEY_NUM_LOCK,
    [BOX_KEY_PRINT_SCREEN] = GLFW_KEY_PRINT_SCREEN,
    [BOX_KEY_PAUSE] = GLFW_KEY_PAUSE,
    [BOX_KEY_F1] = GLFW_KEY_F1,
    [BOX_KEY_F2] = GLFW_KEY_F2,
    [BOX_KEY_F3] = GLFW_KEY_F3,
    [BOX_KEY_F4] = GLFW_KEY_F4,
    [BOX_KEY_F5] = GLFW_KEY_F5,
    [BOX_KEY_F6] = GLFW_KEY_F6,
    [BOX_KEY_F7] = GLFW_KEY_F7,
    [BOX_KEY_F8] = GLFW_KEY_F8,
    [BOX_KEY_F9] = GLFW_KEY_F9,
    [BOX_KEY_F10] = GLFW_KEY_F10,
    [BOX_KEY_F11] = GLFW_KEY_F11,
    [BOX_KEY_F12] = GLFW_KEY_F12,
    [BOX_KEY_F13] = GLFW_KEY_F13,
    [BOX_KEY_F14] = GLFW_KEY_F14,
    [BOX_KEY_F15] = GLFW_KEY_F15,
    [BOX_KEY_F16] = GLFW_KEY_F16,
    [BOX_KEY_F17] = GLFW_KEY_F17,
    [BOX_KEY_F18] = GLFW_KEY_F18,
    [BOX_KEY_F19] = GLFW_KEY_F19,
    [BOX_KEY_F20] = GLFW_KEY_F20,
    [BOX_KEY_F21] = GLFW_KEY_F21,
    [BOX_KEY_F22] = GLFW_KEY_F22,
    [BOX_KEY_F23] = GLFW_KEY_F23,
    [BOX_KEY_F24] = GLFW_KEY_F24,
    [BOX_KEY_F25] = GLFW_KEY_F25,
    [BOX_KEY_KP_0] = GLFW_KEY_KP_0,
    [BOX_KEY_KP_1] = GLFW_KEY_KP_1,
    [BOX_KEY_KP_2] = GLFW_KEY_KP_2,
    [BOX_KEY_KP_3] = GLFW_KEY_KP_3,
    [BOX_KEY_KP_4] = GLFW_KEY_KP_4,
    [BOX_KEY_KP_5] = GLFW_KEY_KP_5,
    [BOX_KEY_KP_6] = GLFW_KEY_KP_6,
    [BOX_KEY_KP_7] = GLFW_KEY_KP_7,
    [BOX_KEY_KP_8] = GLFW_KEY_KP_8,
    [BOX_KEY_KP_9] = GLFW_KEY_KP_9,
    [BOX_KEY_KP_DECIMAL] = GLFW_KEY_KP_DECIMAL,
    [BOX_KEY_KP_DIVIDE] = GLFW_KEY_KP_DIVIDE,
    [BOX_KEY_KP_MULTIPLY] = GLFW_KEY_KP_MULTIPLY,
    [BOX_KEY_KP_SUBTRACT] = GLFW_KEY_KP_SUBTRACT,
    [BOX_KEY_KP_ADD] = GLFW_KEY_KP_ADD,
    [BOX_KEY_KP_ENTER] = GLFW_KEY_KP_ENTER,
    [BOX_KEY_KP_EQUAL] = GLFW_KEY_KP_EQUAL,
    [BOX_KEY_LEFT_SHIFT] = GLFW_KEY_LEFT_SHIFT,
    [BOX_KEY_LEFT_CONTROL] = GLFW_KEY_LEFT_CONTROL,
    [BOX_KEY_LEFT_ALT] = GLFW_KEY_LEFT_ALT,
    [BOX_KEY_LEFT_SUPER] = GLFW_KEY_LEFT_SUPER,
    [BOX_KEY_RIGHT_SHIFT] = GLFW_KEY_RIGHT_SHIFT,
    [BOX_KEY_RIGHT_CONTROL] = GLFW_KEY_RIGHT_CONTROL,
    [BOX_KEY_RIGHT_ALT] = GLFW_KEY_RIGHT_ALT,
    [BOX_KEY_RIGHT_SUPER] = GLFW_KEY_RIGHT_SUPER,
    [BOX_KEY_MENU] = GLFW_KEY_MENU,
};

// DO NOT CHANGE THE FUCKING ORDER PLEASE OK?
static int glfw_keys_to_box_keys[] = {
    [GLFW_KEY_SPACE] = BOX_KEY_SPACE,
    [GLFW_KEY_APOSTROPHE] = BOX_KEY_APOSTROPHE,
    [GLFW_KEY_COMMA] = BOX_KEY_COMMA,
    [GLFW_KEY_MINUS] = BOX_KEY_MINUS,
    [GLFW_KEY_PERIOD] = BOX_KEY_PERIOD,
    [GLFW_KEY_SLASH] = BOX_KEY_SLASH,
    [GLFW_KEY_0] = BOX_KEY_0,
    [GLFW_KEY_1] = BOX_KEY_1,
    [GLFW_KEY_2] = BOX_KEY_2,
    [GLFW_KEY_3] = BOX_KEY_3,
    [GLFW_KEY_4] = BOX_KEY_4,
    [GLFW_KEY_5] = BOX_KEY_5,
    [GLFW_KEY_6] = BOX_KEY_6,
    [GLFW_KEY_7] = BOX_KEY_7,
    [GLFW_KEY_8] = BOX_KEY_8,
    [GLFW_KEY_9] = BOX_KEY_9,
    [GLFW_KEY_SEMICOLON] = BOX_KEY_SEMICOLON,
    [GLFW_KEY_EQUAL] = BOX_KEY_EQUAL,
    [GLFW_KEY_A] = BOX_KEY_A,
    [GLFW_KEY_B] = BOX_KEY_B,
    [GLFW_KEY_C] = BOX_KEY_C,
    [GLFW_KEY_D] = BOX_KEY_D,
    [GLFW_KEY_E] = BOX_KEY_E,
    [GLFW_KEY_F] = BOX_KEY_F,
    [GLFW_KEY_G] = BOX_KEY_G,
    [GLFW_KEY_H] = BOX_KEY_H,
    [GLFW_KEY_I] = BOX_KEY_I,
    [GLFW_KEY_J] = BOX_KEY_J,
    [GLFW_KEY_K] = BOX_KEY_K,
    [GLFW_KEY_L] = BOX_KEY_L,
    [GLFW_KEY_M] = BOX_KEY_M,
    [GLFW_KEY_N] = BOX_KEY_N,
    [GLFW_KEY_O] = BOX_KEY_O,
    [GLFW_KEY_P] = BOX_KEY_P,
    [GLFW_KEY_Q] = BOX_KEY_Q,
    [GLFW_KEY_R] = BOX_KEY_R,
    [GLFW_KEY_S] = BOX_KEY_S,
    [GLFW_KEY_T] = BOX_KEY_T,
    [GLFW_KEY_U] = BOX_KEY_U,
    [GLFW_KEY_V] = BOX_KEY_V,
    [GLFW_KEY_W] = BOX_KEY_W,
    [GLFW_KEY_X] = BOX_KEY_X,
    [GLFW_KEY_Y] = BOX_KEY_Y,
    [GLFW_KEY_Z] = BOX_KEY_Z,
    [GLFW_KEY_LEFT_BRACKET] = BOX_KEY_LEFT_BRACKET,
    [GLFW_KEY_BACKSLASH] = BOX_KEY_BACKSLASH,
    [GLFW_KEY_RIGHT_BRACKET] = BOX_KEY_RIGHT_BRACKET,
    [GLFW_KEY_GRAVE_ACCENT] = BOX_KEY_GRAVE_ACCENT,
    [GLFW_KEY_WORLD_1] = BOX_KEY_WORLD_1,
    [GLFW_KEY_WORLD_2] = BOX_KEY_WORLD_2,
    [GLFW_KEY_ESCAPE] = BOX_KEY_ESCAPE,
    [GLFW_KEY_ENTER] = BOX_KEY_ENTER,
    [GLFW_KEY_TAB] = BOX_KEY_TAB,
    [GLFW_KEY_BACKSPACE] = BOX_KEY_BACKSPACE,
    [GLFW_KEY_INSERT] = BOX_KEY_INSERT,
    [GLFW_KEY_DELETE] = BOX_KEY_DELETE,
    [GLFW_KEY_RIGHT] = BOX_KEY_RIGHT,
    [GLFW_KEY_LEFT] = BOX_KEY_LEFT,
    [GLFW_KEY_DOWN] = BOX_KEY_DOWN,
    [GLFW_KEY_UP] = BOX_KEY_UP,
    [GLFW_KEY_PAGE_UP] = BOX_KEY_PAGE_UP,
    [GLFW_KEY_PAGE_DOWN] = BOX_KEY_PAGE_DOWN,
    [GLFW_KEY_HOME] = BOX_KEY_HOME,
    [GLFW_KEY_END] = BOX_KEY_END,
    [GLFW_KEY_CAPS_LOCK] = BOX_KEY_CAPS_LOCK,
    [GLFW_KEY_SCROLL_LOCK] = BOX_KEY_SCROLL_LOCK,
    [GLFW_KEY_NUM_LOCK] = BOX_KEY_NUM_LOCK,
    [GLFW_KEY_PRINT_SCREEN] = BOX_KEY_PRINT_SCREEN,
    [GLFW_KEY_PAUSE] = BOX_KEY_PAUSE,
    [GLFW_KEY_F1] = BOX_KEY_F1,
    [GLFW_KEY_F2] = BOX_KEY_F2,
    [GLFW_KEY_F3] = BOX_KEY_F3,
    [GLFW_KEY_F4] = BOX_KEY_F4,
    [GLFW_KEY_F5] = BOX_KEY_F5,
    [GLFW_KEY_F6] = BOX_KEY_F6,
    [GLFW_KEY_F7] = BOX_KEY_F7,
    [GLFW_KEY_F8] = BOX_KEY_F8,
    [GLFW_KEY_F9] = BOX_KEY_F9,
    [GLFW_KEY_F10] = BOX_KEY_F10,
    [GLFW_KEY_F11] = BOX_KEY_F11,
    [GLFW_KEY_F12] = BOX_KEY_F12,
    [GLFW_KEY_F13] = BOX_KEY_F13,
    [GLFW_KEY_F14] = BOX_KEY_F14,
    [GLFW_KEY_F15] = BOX_KEY_F15,
    [GLFW_KEY_F16] = BOX_KEY_F16,
    [GLFW_KEY_F17] = BOX_KEY_F17,
    [GLFW_KEY_F18] = BOX_KEY_F18,
    [GLFW_KEY_F19] = BOX_KEY_F19,
    [GLFW_KEY_F20] = BOX_KEY_F20,
    [GLFW_KEY_F21] = BOX_KEY_F21,
    [GLFW_KEY_F22] = BOX_KEY_F22,
    [GLFW_KEY_F23] = BOX_KEY_F23,
    [GLFW_KEY_F24] = BOX_KEY_F24,
    [GLFW_KEY_F25] = BOX_KEY_F25,
    [GLFW_KEY_KP_0] = BOX_KEY_KP_0,
    [GLFW_KEY_KP_1] = BOX_KEY_KP_1,
    [GLFW_KEY_KP_2] = BOX_KEY_KP_2,
    [GLFW_KEY_KP_3] = BOX_KEY_KP_3,
    [GLFW_KEY_KP_4] = BOX_KEY_KP_4,
    [GLFW_KEY_KP_5] = BOX_KEY_KP_5,
    [GLFW_KEY_KP_6] = BOX_KEY_KP_6,
    [GLFW_KEY_KP_7] = BOX_KEY_KP_7,
    [GLFW_KEY_KP_8] = BOX_KEY_KP_8,
    [GLFW_KEY_KP_9] = BOX_KEY_KP_9,
    [GLFW_KEY_KP_DECIMAL] = BOX_KEY_KP_DECIMAL,
    [GLFW_KEY_KP_DIVIDE] = BOX_KEY_KP_DIVIDE,
    [GLFW_KEY_KP_MULTIPLY] = BOX_KEY_KP_MULTIPLY,
    [GLFW_KEY_KP_SUBTRACT] = BOX_KEY_KP_SUBTRACT,
    [GLFW_KEY_KP_ADD] = BOX_KEY_KP_ADD,
    [GLFW_KEY_KP_ENTER] = BOX_KEY_KP_ENTER,
    [GLFW_KEY_KP_EQUAL] = BOX_KEY_KP_EQUAL,
    [GLFW_KEY_LEFT_SHIFT] = BOX_KEY_LEFT_SHIFT,
    [GLFW_KEY_LEFT_CONTROL] = BOX_KEY_LEFT_CONTROL,
    [GLFW_KEY_LEFT_ALT] = BOX_KEY_LEFT_ALT,
    [GLFW_KEY_LEFT_SUPER] = BOX_KEY_LEFT_SUPER,
    [GLFW_KEY_RIGHT_SHIFT] = BOX_KEY_RIGHT_SHIFT,
    [GLFW_KEY_RIGHT_CONTROL] = BOX_KEY_RIGHT_CONTROL,
    [GLFW_KEY_RIGHT_ALT] = BOX_KEY_RIGHT_ALT,
    [GLFW_KEY_RIGHT_SUPER] = BOX_KEY_RIGHT_SUPER,
    [GLFW_KEY_MENU] = BOX_KEY_MENU,
};

static int box_mouse_button_to_glfw_mouse_button[] = {
    [BOX_MOUSE_BUTTON_1] = GLFW_MOUSE_BUTTON_1,
    [BOX_MOUSE_BUTTON_2] = GLFW_MOUSE_BUTTON_2,
    [BOX_MOUSE_BUTTON_3] = GLFW_MOUSE_BUTTON_3,
    [BOX_MOUSE_BUTTON_4] = GLFW_MOUSE_BUTTON_4,
    [BOX_MOUSE_BUTTON_5] = GLFW_MOUSE_BUTTON_5,
    [BOX_MOUSE_BUTTON_6] = GLFW_MOUSE_BUTTON_6,
    [BOX_MOUSE_BUTTON_7] = GLFW_MOUSE_BUTTON_7,
    [BOX_MOUSE_BUTTON_8] = GLFW_MOUSE_BUTTON_8,
};

static int glfw_mouse_button_to_box_mouse_button[] = {
    [GLFW_MOUSE_BUTTON_1] = BOX_MOUSE_BUTTON_1,
    [GLFW_MOUSE_BUTTON_2] = BOX_MOUSE_BUTTON_2,
    [GLFW_MOUSE_BUTTON_3] = BOX_MOUSE_BUTTON_3,
    [GLFW_MOUSE_BUTTON_4] = BOX_MOUSE_BUTTON_4,
    [GLFW_MOUSE_BUTTON_5] = BOX_MOUSE_BUTTON_5,
    [GLFW_MOUSE_BUTTON_6] = BOX_MOUSE_BUTTON_6,
    [GLFW_MOUSE_BUTTON_7] = BOX_MOUSE_BUTTON_7,
    [GLFW_MOUSE_BUTTON_8] = BOX_MOUSE_BUTTON_8,
};

static int box_input_action_to_glfw_action[] = {
    [BOX_PRESS] = GLFW_PRESS,
    [BOX_RELEASE] = GLFW_RELEASE,
};

static int glfw_action_to_box_input_action[] = {
    [GLFW_PRESS] = BOX_PRESS,
    [GLFW_RELEASE] = BOX_RELEASE,
};

struct Vertex
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

static bool init = false;

static box_render_log_t box_render_log = NULL;
static GLFWwindow *s_window = NULL;
static GLuint s_canvas_program = 0;

static GLuint s_rect_vao = 0;
static GLuint s_rect_vbo = 0;

static int s_window_width = 1024;
static int s_window_height = 600;

#define TEXTURE_COLOR_CHANNELS 4
static int s_canvas_width = 256;
static int s_canvas_height = 256;
static GLuint s_texture = 0;
static unsigned char *s_canvas = NULL;

static float s_zoom = 1;
static float s_cam_x_pos = 0;
static float s_cam_y_pos = 0;

static box_render_mouse_button_cb_t s_mouse_button_cb = NULL;
static box_render_canvas_click_cb_t s_canvas_click_cb = NULL;
static box_render_keyboard_cb_t s_keyboard_cb = NULL;

static void framebuffer_size_callback(GLFWwindow *_window, int width,
                                      int height)
{
    glViewport(0, 0, width, height);
    s_window_width = width;
    s_window_height = height;
}

static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    s_zoom += yoffset / 10;
    s_zoom = BOX_CLAMP(s_zoom, 0.1, 16);
}

static void keyboard_callback(GLFWwindow *window, int key, int scancode,
                              int action, int mods)
{
    if (key == GLFW_KEY_A)
    {
        s_cam_x_pos += 0.1;
    }
    else if (key == GLFW_KEY_D)
    {
        s_cam_x_pos -= 0.1;
    }

    if (key == GLFW_KEY_W)
    {
        s_cam_y_pos += 0.1;
    }
    else if (key == GLFW_KEY_S)
    {
        s_cam_y_pos -= 0.1;
    }

    if (s_keyboard_cb)
    {
        s_keyboard_cb(glfw_keys_to_box_keys[key],
                      glfw_action_to_box_input_action[action]);
    }
}

static void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos) {}

// This function uses the calculations applied to vertices on the vertex shader
// to get a "screen space" coordinate of the pixels in the screen and uses that
// to transform `xpos` and `ypos` into indices for the s_canvas array
static void get_pos_in_indices(double xpos, double ypos, int *x_idx, int *y_idx)
{
    double aspect_ratio = (double)s_window_width / s_window_height;

    if (x_idx != NULL)
    {
        double half_window_width = (double)s_window_width / 2.0;

        double screen_space_canvas_left =
            (-1.0 + s_cam_x_pos) / aspect_ratio * s_zoom * half_window_width +
            half_window_width;

        double screen_space_canvas_right =
            (1.0 + s_cam_x_pos) / aspect_ratio * s_zoom * half_window_width +
            half_window_width;

        // (cursor_pos in canvas) / (screen space canvas width) * (canvas_width)
        *x_idx = floor((xpos - screen_space_canvas_left) /
                       (screen_space_canvas_right - screen_space_canvas_left) *
                       s_canvas_width);
    }

    if (y_idx != NULL)
    {
        double half_window_height = (double)s_window_height / 2.0;

        double screen_space_canvas_top =
            (1.0 + s_cam_y_pos) * s_zoom * half_window_height +
            half_window_height;

        double screen_space_canvas_bottom =
            (-1.0 + s_cam_y_pos) * s_zoom * half_window_height +
            half_window_height;

        // (max Y idx(to invert the y coords)) - floor((cursor_pos in canvas) /
        // (screen space canvas height) * (canvas height))
        *y_idx = floor((ypos - screen_space_canvas_top) /
                       (screen_space_canvas_bottom - screen_space_canvas_top) *
                       s_canvas_height);
    }
}

static void handle_click_on_canvas(int x_pos, int y_pos,
                                   enum BoxMouseButton mouse_button,
                                   enum BoxInputAction action)
{
    if (!s_canvas_click_cb)
    {
        return;
    }

    int clicked_pixel_x = -1;
    int clicked_pixel_y = -1;
    get_pos_in_indices(x_pos, y_pos, &clicked_pixel_x, &clicked_pixel_y);

    if (clicked_pixel_x >= 0 && clicked_pixel_x < s_canvas_width &&
        clicked_pixel_y >= 0 && clicked_pixel_y < s_canvas_height)
    {
        s_canvas_click_cb(clicked_pixel_x, clicked_pixel_y, mouse_button,
                          action);
    }
}

static void mouse_button_callback(GLFWwindow *window, int button, int action,
                                  int mods)
{
    enum BoxMouseButton mouse_button =
        glfw_mouse_button_to_box_mouse_button[button];
    enum BoxInputAction input_action = glfw_action_to_box_input_action[action];

    double x_pos, y_pos;
    glfwGetCursorPos(window, &x_pos, &y_pos);

    handle_click_on_canvas(x_pos, y_pos, mouse_button, input_action);

    if (s_mouse_button_cb)
    {
        s_mouse_button_cb(x_pos, y_pos, mouse_button, input_action);
    }
}

static GLuint load_shader(const char *path, GLenum shader_type)
{
    char *shader_src = box_read_file(path);

    GLuint shader = glCreateShader(shader_type);
    if (!shader)
    {
        DEBUG_LOG("Could not load shader\"%s\"", path);
        return 0;
    }

    // This casting is just to make the warning go away
    glShaderSource(shader, 1, (const char *const *)&shader_src, NULL);
    free(shader_src);

    glCompileShader(shader);

    int res = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
    if (!res)
    {
        const GLsizei INFO_LOG_MAX_LENGTH = 256;
        GLchar info_log[INFO_LOG_MAX_LENGTH];
        GLsizei info_log_length = 0;

        glGetShaderInfoLog(shader, INFO_LOG_MAX_LENGTH, &info_log_length,
                           info_log);

        DEBUG_LOG("Could not compile shader %s\n", info_log);

        glDeleteShader(shader);

        return 0;
    }

    return shader;
}

static GLuint create_program(const char *vertex_shader_path,
                             const char *fragment_shader_path)
{
    GLuint vertex_shader = load_shader(vertex_shader_path, GL_VERTEX_SHADER);
    GLuint fragment_shader =
        load_shader(fragment_shader_path, GL_FRAGMENT_SHADER);
    if (!vertex_shader || !fragment_shader)
    {
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    int res = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &res);
    if (!res)
    {
        const GLsizei INFO_LOG_MAX_LENGTH = 256;
        GLchar info_log[INFO_LOG_MAX_LENGTH];
        GLsizei info_log_length = 0;

        glGetProgramInfoLog(program, INFO_LOG_MAX_LENGTH, &info_log_length,
                            info_log);

        DEBUG_LOG(info_log);

        glDeleteProgram(program);
        return 0;
    }

    return program;
}

static int setup_glfw()
{
    // GLFW Initialization
    if (!glfwInit())
    {
        return BOX_RENDER_GLFW_INIT_ERROR;
    }

    // Window Creation
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    s_window = glfwCreateWindow(s_window_width, s_window_height, "Sandbox",
                                NULL, NULL);
    if (!s_window)
    {
        return BOX_RENDER_WINDOW_CREATION_ERROR;
    }

    glfwMakeContextCurrent(s_window);
    glfwSetFramebufferSizeCallback(s_window, framebuffer_size_callback);
    glfwSetScrollCallback(s_window, scroll_callback);
    glfwSetKeyCallback(s_window, keyboard_callback);
    glfwSetCursorPosCallback(s_window, cursor_pos_callback);
    glfwSetMouseButtonCallback(s_window, mouse_button_callback);

    return BOX_RENDER_SUCCESS;
}

static int setup_gl()
{
    // OpenGL setup
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return BOX_RENDER_GL_LOAD_ERROR;
    }

    glEnable(GL_PROGRAM_POINT_SIZE);
    glViewport(0, 0, s_window_width, s_window_height);

    return BOX_RENDER_SUCCESS;
}

static enum BoxRenderError setup_buffers()
{
    // Rectangle
    struct Vertex rect_vertices[] = {
        {-1.0, -1.0, 0.0}, {1.0, -1.0, 0.0}, {1.0, 1.0, 0.0},
        {-1.0, -1.0, 0.0}, {1.0, 1.0, 0.0},  {-1.0, 1.0, 0.0},
    };

    glGenVertexArrays(1, &s_rect_vao);
    glGenBuffers(1, &s_rect_vbo);

    glBindVertexArray(s_rect_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_rect_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    return BOX_RENDER_SUCCESS;
}

static enum BoxRenderError setup_framebuffer()
{
    s_canvas =
        malloc(s_canvas_width * s_canvas_height * TEXTURE_COLOR_CHANNELS);
    for (int x = 0; x < s_canvas_width; ++x)
    {
        for (int y = 0; y < s_canvas_width; ++y)
        {
            int idx = (x + y * s_canvas_width) * TEXTURE_COLOR_CHANNELS;
            s_canvas[idx] = 255;
            s_canvas[idx + 1] = 255;
            s_canvas[idx + 2] = 255;
            s_canvas[idx + 3] = 255;
        }
    }

    glGenTextures(1, &s_texture);
    glBindTexture(GL_TEXTURE_2D, s_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s_canvas_width, s_canvas_height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, s_canvas);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    return BOX_RENDER_SUCCESS;
}

static enum BoxRenderError setup_programs()
{
    s_canvas_program =
        create_program("shaders/passthrough.vert", "shaders/passthrough.frag");
    if (!s_canvas_program)
    {
        return BOX_RENDER_PROGRAM_CREATION_ERROR;
    }

    return BOX_RENDER_SUCCESS;
}

int box_render_init(box_render_log_t _log)
{
    box_render_log = _log;
    SETUP(glfw);
    SETUP(gl);
    SETUP(programs);
    SETUP(framebuffer);
    SETUP(buffers);

    init = true;

    return BOX_RENDER_SUCCESS;
}

void box_render_draw_pixel(int x, int y, int r, int g, int b)
{
    assert(x >= 0 && x < s_canvas_width);
    assert(y >= 0 && y < s_canvas_height);

    int idx = (x + y * s_canvas_width) * TEXTURE_COLOR_CHANNELS;
    s_canvas[idx] = r;
    s_canvas[idx + 1] = g;
    s_canvas[idx + 2] = b;
}

#define SET_UNIFORM(uniform_func, program, name, ...)                          \
    uniform_func(glGetUniformLocation(program, name), __VA_ARGS__)

void box_render_clear() {}
void box_render_present()
{
    glBindTexture(GL_TEXTURE_2D, s_texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, s_canvas_width, s_canvas_height,
                    GL_RGBA, GL_UNSIGNED_BYTE, s_canvas);

    glClearColor(0.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(s_rect_vao);
    glUseProgram(s_canvas_program);
    SET_UNIFORM(glUniform1f, s_canvas_program, "scale", s_zoom);
    SET_UNIFORM(glUniform1f, s_canvas_program, "x_pos", s_cam_x_pos);
    SET_UNIFORM(glUniform1f, s_canvas_program, "y_pos", s_cam_y_pos);
    SET_UNIFORM(glUniform1f, s_canvas_program, "aspect_ratio",
                (float)s_window_width / s_window_height);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glfwPollEvents();
    glfwSwapBuffers(s_window);
}

void box_render_terminate()
{
    if (!init)
    {
        return;
    }

    // Programs
    glDeleteProgram(s_canvas_program);
    s_canvas_program = 0;

    // VAOs and VBOs
    glDeleteVertexArrays(1, &s_rect_vao);
    glDeleteBuffers(1, &s_rect_vbo);

    // Texture and
    glDeleteTextures(1, &s_texture);
    free(s_canvas);
    s_canvas = NULL;

    glfwDestroyWindow(s_window);
    glfwTerminate();
}

void box_render_set_mouse_button_callback(
    box_render_mouse_button_cb_t mouse_button_cb)
{
    s_mouse_button_cb = mouse_button_cb;
}

void box_render_set_canvas_click_callback(
    box_render_canvas_click_cb_t canvas_click_cb)
{
    s_canvas_click_cb = canvas_click_cb;
}

void box_render_set_keyboard_callback(box_render_keyboard_cb_t keyboard_cb)
{
    s_keyboard_cb = keyboard_cb;
}
