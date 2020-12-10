#include <stdio.h>
#include "window.h"
#include "renderer.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "game.h"

// colors
vec3_t color_red = {1,0,0};
vec3_t color_green = {0,1,0};
vec3_t color_blue = {0,0,1};




const char* vertex_src = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
      
    uniform mat4 u_model;
    uniform mat4 u_projection;
    uniform mat4 u_view;

      
    out vec4 vertexColor; 

    void main()
    {

        gl_Position = u_projection*u_view*u_model*vec4(aPos, 1.0);
        vertexColor = vec4(0.5, 0.0, 0.0, 1.0);
} 

)";

const char* fragment_src = R"(
    #version 330 core
    out vec4 FragColor;
      
    uniform vec3 u_color; 
    in vec4 vertexColor;

    void main()
    {
        FragColor = vec4(u_color, 1.f);
    }  

)";

vec3_t dir;
mat4_t view;
float ts;
renderer_t* renderer_ins;
window_t* window;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        dir = vec3_construct(0, 2, 0);
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        dir = vec3_construct(-2, 0, 0);
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        dir = vec3_construct(0, -2, 0);
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        dir = vec3_construct(2, 0, 0);

}


int main()
{
    window = window_construct("Snake", 800, 600);
    window->init(window);

    renderer_ins = renderer_construct();
    renderer_ins->init(2048*2048, 2048*2048);
    shader_t shader = renderer_ins->shader_construct(vertex_src, fragment_src);
    renderer_ins->shader_set(shader);

    mat4_t proj = ortho_projection(-1.f, 1.f, -1.f, 1.f, 1.f, 200.f);
    
    view = mat4_identity();
    //__debug_print_mat4(&view);
    view = mat4_scale(view, vec3_construct(1.f/40.f, 1.f/30.f, 1.f));

    //__debug_print_mat4(&view);


    renderer_ins->upload_projection(proj);
    renderer_ins->set_view(&view);

    snake_t snake = snake_init();

    vec3_t applePos = {4, 2, 0};

    g_object_t* apple = g_object_ctor(applePos, vec3_construct(1,1,1), vec3_construct(0.f, 0.8f, 0.f));

    glfwSetKeyCallback(window->window, key_callback);

    float do_t = 0.2f;
    int score = 0;

    glClearColor(0.25f, 0.25f, 0.25f, 1.f);
    float lasttime = 0;
    while(1)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // for timestep
        float time = glfwGetTime();
        ts = time - lasttime;
        lasttime = time;

        //g_object_update(quad);
        system("clear");
        printf("SCORE %d\n", score);
        __debug_print_mat4(&snake.body[1].transform);
        puts("");
        __debug_print_mat4(&snake.body[2].transform);
        do_t -= ts;

        if(do_t <= 0.f)
        {
            snake_move(&snake, dir);
            do_t = 0.2f;
        }

        vec3_t head = mat4_get_position(snake.body[0].transform);
        if(head.x == applePos.x && head.y == applePos.y)
        {
            score++;
            while(1)
            {
                applePos.x = rand() % 40;
                applePos.y = rand() % 30;
                if((int)(applePos.x) % 2 == 0 && (int)(applePos.y) % 2 ==0 ) break;
            }
            apple->transform.rows[3].x = applePos.x;
            apple->transform.rows[3].y = applePos.y;

            snake_addtail(&snake);
        }
            
        g_object_render(apple);

        snake_render(&snake);

        window->update(window);
        
    }






    window->shutdown(window);


    return 0;
}