#include "Scene.h"
#include "config.h"
Scene::Scene()
{
    gWindow = NULL;
    camera_position=Point(0, 1.0, 10.0);
    for (i=0; i<MAX_FORMS_NUMBER; i++)
    {
        forms_list[i] = NULL;
    }
    init();
    setupObjects();

}

bool Scene::_initWindow(SDL_Window** window, SDL_GLContext* context)
{
 // Initialization flag
    bool success = true;

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        success = false;
    }
    else
    {
        // Use OpenGL 2.1
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

        // Create window
        *window = SDL_CreateWindow( "TP intro OpenGL / SDL 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
        if( *window == NULL )
        {
            std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
            success = false;
        }
        else
        {
            // Create context
            *context = SDL_GL_CreateContext(*window);
            if( *context == NULL )
            {
                std::cerr << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
                success = false;
            }
            else
            {
                // Use Vsync
                if(SDL_GL_SetSwapInterval(1) < 0)
                {
                    std::cerr << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
                }

                // Initialize OpenGL
                if( !initGL() )
                {
                    std::cerr << "Unable to initialize OpenGL!"  << std::endl;
                    success = false;
                }
            }
        }
    }

    return success;
}



bool Scene::_initGL()
{
    bool success = true;
    GLenum error = GL_NO_ERROR;

    // Initialize Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport : use all the window to display the rendered scene
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Fix aspect ratio and depth clipping planes
    gluPerspective(40.0, (GLdouble)SCREEN_WIDTH/SCREEN_HEIGHT, 1.0, 100.0);


    // Initialize Modelview Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Initialize clear color : black with no transparency
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

    // Activate Z-Buffer
    glEnable(GL_DEPTH_TEST);


    // Lighting basic configuration and activation
    const GLfloat light_ambient[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
    const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

    const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
    const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
    const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat high_shininess[] = { 100.0f };

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);


    // Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
    {
        std::cerr << "Error initializing OpenGL!  " << gluErrorString( error ) << std::endl;
        success = false;
    }

    return success;
}

bool Scene::init()
{


    // Start up SDL and create window
    if( !_initWindow(&gWindow, &gContext))
    {
        std::cerr << "Failed to initialize!" << std::endl;
    }
    else
    {
        quit=false;


    }

}

void Scene::checkCollisionAll()
{
    int i=0;
    while(formlist[i]!=NULL)
    {
        Form form = *formlist[i];
        switch(form.getTypeForm()){
            case SHAPE_ID.CUBE:
            break;

            default:
            break;



        }








        formlist[i]
    }
    

}

void Scene::run()
{
    while(!quit)
    {
        checkInput();
    // Update the scene
        current_time = SDL_GetTicks(); // get the elapsed time from SDL initialization (ms)
        elapsed_time = current_time - previous_time;
        if (elapsed_time > ANIM_DELAY)
        {
            previous_time = current_time;
            update(forms_list, 1e-3 * elapsed_time); // International system units : seconds
        }

        // Render the scene
        render(forms_list, camera_position);
        // Update window screen
        SDL_GL_SwapWindow(gWindow);
    }
    close(&gWindow);

}

void Scene::close(SDL_Window** window)
{
    //Destroy window
    SDL_DestroyWindow(*window);
    *window = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

bool Scene::addForm(Form* form)
{
    formIndex++;
    if(formIndex >= MAX_FORMS_NUMBER);
    {
        return false;
    }
    formlist[formIndex] = form;
    return true;
}

char checkInput()
{
// Handle events on queue
    while(SDL_PollEvent(&event) != 0)
    {
        int x = 0, y = 0;
        SDL_Keycode key_pressed = event.key.keysym.sym;

        switch(event.type)
        {
        // User requests quit
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYDOWN:
            // Handle key pressed with current mouse position
            SDL_GetMouseState( &x, &y );
            switch(key_pressed)
            {
            // Quit the program when 'q' or Escape keys are pressed
            case SDLK_q:
            case SDLK_ESCAPE:
                quit = true;
                break;
            case SDLK_LEFT:
                camera_position.x -= 1;
                printf("Left\n");
            break;
            case SDLK_RIGHT:
                camera_position.x += 1;
                printf("Right\n");
            break;
            case SDLK_UP:
                camera_position.y += 1;
                printf("Up\n");
            break;
            case SDLK_DOWN:
                camera_position.y -= 1;
                printf("Down\n");
            break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

bool Scene::setupObjects() // Initialisation des objets
{
    // Create here specific forms and add them to the list...
    // Don't forget to update the actual number_of_forms !
    // Cube *pFace = NULL;
    // pFace = new Cube(Vector(1,0,0), Vector(0,1,0), Point(-0.5, -0.5, -0.5), 1, 1, ORANGE);
    // forms_list[number_of_forms] = pFace;
    // number_of_forms++;

    Sol *sol = new Sol(GREEN); // Créez un nouvel objet de brique en dehors de la boucle
    if (!sol->loadSTL("Solidworks/sol.STL")){
        printf("Failed to load sol.STL model!\n");
        delete sol; // Supprimez l'objet brique si le chargement échoue
    }
    Point sizeSol(50, 0, 50);//sol de taille de 50 m , 0 m, 50 m
    sol->setSize(sizeSol);
    printf("Size Objet : size X = %2.1f, size Y = %2.1f, size Z = %2.1f\n", sizeSol.x, sizeSol.y, sizeSol.z);
    Point pt(-sizeSol.x/2, 0, -sizeSol.z/2);
    sol->moveAbsolue(pt); // Déplacez le nouvel objet brique
    addForm(sol);


}


bool removeForm(int index)
{


}

bool removeForm(Form* form);

bool Scene::popForm()
{
    delete(formlist[formIndex]);
    formlist[formIndex] = NULL;
    if(formIndex>0)
    {
        formIndex--;
    }
}