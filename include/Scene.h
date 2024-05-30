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
#include "forms.h"




class Scene
{
private:
    Form* formlist[MAX_FORMS_NUMBER];
    int formIndex =0;

    bool _initGL();
    bool _initWindow(SDL_Window** window, SDL_GLContext* context);
    bool setupProps();
    char checkInput();
    bool useSDL=false;
    void close(SDL_Window** window);

public:
    Scene();


    bool init();
    void run();
    bool addForm(Form* form);
    bool useSDL(){return useSDL;}
};



