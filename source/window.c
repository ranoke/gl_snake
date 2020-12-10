#include "window.h"

#include <stdbool.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <assert.h>

void __window_init(window_t* w);
void __window_update(window_t* w);
void __window_shutdown(window_t* w);


window_t* window_construct(const char* title, u_int32_t width, u_int32_t height)
{
    window_t* w = malloc(sizeof(window_t));
    w->title = title; 
    w->width = width;
    w->height = height;

    w->init = &__window_init;
    w->update = &__window_update;
    w->shutdown = &__window_shutdown;
    return w;
}

void __window_init(struct window_t* w)
{
    if(!glfwInit())
    {
        assert(false &&
            "failed to init glfw");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    w->window = (void*)glfwCreateWindow(w->width, w->height, w->title, 0, 0);
    if(!w->window)
    {
        assert(false && "export MESA_GLSL_VERSION_OVERRIDE=410; export MESA_GL_VERSION_OVERRIDE=4.1COMPAT");
        return;
    }
    glfwMakeContextCurrent(w->window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSwapInterval(1);
}

void __window_update(window_t* w)
{

    glfwPollEvents();
    glfwSwapBuffers(w->window);
}

void __window_shutdown(window_t* w)
{
    glfwDestroyWindow(w->window);
    glfwTerminate();
}