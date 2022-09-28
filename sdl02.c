#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>

#define bool char
#define true 1
#define false 0

typedef struct {
    SDL_Window  *window;
    SDL_Surface *window_surface;
    SDL_Event    event;

    SDL_Surface *image;
    SDL_Rect     image_position;
    double       image_x;
    double       image_y;
    double       image_v_x;
    double       image_v_y;
} Application;

int init_window(Application* app)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize the SDL2 library\n");
        printf("SDL2 Error: %s\n", SDL_GetError());
        return 0;
    }

    app->window = SDL_CreateWindow("SDL2 Window",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   640, 480,
                                   0);

    if(!app->window)
    {
        printf("Failed to create window\n");
        printf("SDL2 Error: %s\n", SDL_GetError());
        return 0;
    }

    return 1;
}

int get_surface(Application* app)
{
    app->window_surface = SDL_GetWindowSurface(app->window);

    if(!app->window_surface)
    {
        printf("Failed to get the surface from the window\n");
        printf("SDL2 Error: %s\n", SDL_GetError());
        return 0;
    }

    return 1;
}

int load_image(Application* app, char* image_name)
{
    app->image = SDL_LoadBMP(image_name);

    if(!app->image)
    {
        printf("Failed to load image\n");
        printf("SDL2 Error: %s\n", SDL_GetError());
        return 0;
    }

    return 1;
}

void init_coords(Application *app)
{
    app->image_position.x = 0;
    app->image_position.y = 0;
    app->image_position.w = 0;
    app->image_position.h = 0;

    app->image_x = 0.0;
    app->image_y = 0.0;

    app->image_v_x = 5.0;
    app->image_v_y = 3.0;
}

void update(Application *app, double delta_time)
{
    app->image_x = app->image_x + (app->image_v_x * delta_time);
    app->image_position.x = app->image_x;

    app->image_y = app->image_y + (app->image_v_y * delta_time);
    app->image_position.y = app->image_y;

// reflect from the wall
    if (app->image_x > 540 || app->image_x < 0) app->image_v_x *= -1;
    if (app->image_y > 380 || app->image_y < 0) app->image_v_y *= -1;
}

void draw(Application *app)
{
    SDL_FillRect(app->window_surface, NULL, SDL_MapRGB(app->window_surface->format, 0, 0, 0));
    SDL_BlitSurface(app->image, NULL, app->window_surface, &app->image_position);
    SDL_UpdateWindowSurface(app->window);
}

int main(int argc, char *argv[])
{
    Application *app = (Application*) malloc(sizeof(Application));

    init_window(app);
    get_surface(app);
    load_image(app, "image.bmp");
    init_coords(app);

    bool keep_window_open = true;
    while(keep_window_open)
    {
        while(SDL_PollEvent(&(app->event)) > 0) // app->event <==> (*app).event
        {
            switch(app->event.type)
            {
                case SDL_QUIT:
                    keep_window_open = false;
                    break;
            }
        }

//        usleep(10); // just for smoothness on the VM

        update(app, 1.0/60.0);
        draw(app);
    }
}

