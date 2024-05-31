// Using SDL, SDL OpenGL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/GLU.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <GL/gl.h>
// Module for space geometry
#include "geometry.h"
// Module for generating and rendering forms
#include "config.h"
#include "forms.h"

class Scene
{
private:


    Form* formlist[MAX_FORMS_NUMBER];
    int formIndex =0;
    Uint32 current_time, previous_time, elapsed_time;
    SDL_Event event;
    Point camera_position;
    SDL_Window* gWindow ;
    SDL_GLContext gContext;



    bool _quit=true;
    bool _initGL();
    bool _initWindow(SDL_Window** window, SDL_GLContext* context);
    bool setupObjects();
    char checkInput();
    bool _useSDL=false;
    void close(SDL_Window** window);
    void checkColisionAll();
    void checkColisionAll(Form* form);


public:
    Scene();

    bool init();
    void render();
    void update(double delta_t);
    void run();
    void checkCollisionAll();
    bool popForm();
    bool gameOver(){return _quit;}
    bool addForm(Form* form);
    bool removeForm(int index);
    bool removeForm(Form* form);
    bool useSDL(){return _useSDL;}
};



