#include <cmath>
#include <SDL2/SDL_opengl.h>
#include <GL/GLU.h>
#include "forms.h"

bool ModelSTL::loadSTL(const std::string& path) {
    // printf("Loading STL file %s\n", path );
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return false;
    }

    // Read the header
    char header[80] = "";
    file.read(header, 80);

    // Read number of triangles
    uint32_t numTriangles;
    file.read(reinterpret_cast<char*>(&numTriangles), sizeof(uint32_t));

    // Read triangles
    triangleSTL.resize(numTriangles);
    uint32_t i = 0;
    printf("Starting triangles read from file\n");
    for (i = 0; i < numTriangles; ++i) {
        file.read(reinterpret_cast<char*>(&triangleSTL[i]), sizeof(Triangle));
        // Skip attribute byte count
        file.ignore(2);
    }
    printf("numTriangles : %d\n", i);

    return true;
}

void ModelSTL::render() {
    // Render the STL model
    // printf("Yes men\n");
    // std::cout << org << std::endl;

    glBegin(GL_TRIANGLES);
    for (const auto& tri : triangleSTL) {
        glNormal3f(tri.normal.x, tri.normal.y, tri.normal.z);
        for (const auto& vert : tri.vertices) {
            glVertex3f(vert.x, vert.y, vert.z);
        }
    }
    glEnd();
}

void Form::update(reel delta_t)
{
    // Nothing to do here, animation update is done in child class method
}

void Form::render()
{
    static Point org, rot;
    // Point of view for rendering
    // Common for all Forms
    org = anim.getPos();
    rot = anim.getRotation();
    glTranslated(org.x, org.y, org.z);

    glRotated(rot.x, 1.0, 0.0, 0.0); // Rotation autour de l'axe X
    glRotated(rot.y, 0.0, 1.0, 0.0); // Rotation autour de l'axe Y
    glRotated(rot.z, 0.0, 0.0, 1.0); // Rotation autour de l'axe Z
    
    glColor3f(anim.getColor().r, anim.getColor().g, anim.getColor().b);
    
    // Render the form
    // Add your rendering code here
}

void Form::physique(reel delta_t){
    static reel masse, alpha = 0.01;
    static Vector sumForce, acc, speed, speedRot;
    static Point position;
    // Calcul du PFD 
    sumForce = getFg() + getFn() ;
    //Determination de l'acceleration à partir du PFD
    //Somme des force = masse * acc
    //acc = Somme des force / masse
    masse = anim.getMasse();
    acc = Vector(sumForce.x/masse, sumForce.y/masse, sumForce.z/masse);
    anim.setAccel(acc);
    //Intergrer pour avoir la vitesse
    speed = anim.getSpeed() + anim.getAccel().integral(delta_t);//+v0;
    anim.setSpeed(speed);

    
    // Mettez à jour la position en fonction de la vitesse et du temps
    // On intergre la vitesse pour obtenir le delta position qu'on vient rajouter à notre position actuelle
    // anim.setPosRelative(anim.getSpeed().integral(delta_t));
    position = speed.integral(delta_t);
    anim.setPosRelative(position);
    // std::cout << "position: " << anim.getPos() << std::endl;

    // Vector square = Vector((anim.getSpeedRotation().x * anim.getSpeedRotation().x), (anim.getSpeedRotation().y * anim.getSpeedRotation().y), (anim.getSpeedRotation().z * anim.getSpeedRotation().z));
    speedRot = anim.getSpeedRotation() - alpha * anim.getSpeedRotation();
    // std::cout << "Speed: " << speedRot << std::endl;
    anim.setSpeedRotation(speedRot);
    anim.setRotRelative(speedRot.integral(delta_t));
    // printf("accel : x:%3.2f y:%3.2f z:%3.2f  ;;  speed : x:%3.2f y:%3.2f z:%3.2f ;; position : x:%3.2f y:%3.2f z:%3.2f \n",
    //                                     anim.getAccel().x, anim.getAccel().y, anim.getAccel().z, 
    //                                     anim.getSpeed().x, anim.getSpeed().y, anim.getSpeed().z,
    //                                     anim.getPos().x, anim.getPos().y, anim.getPos().z
    //                                     );
}

void Brique::update(reel delta_t) {
    if (!getPhysics()){
        anim.setAccel(0);
        anim.setSpeed(0);
        return;
    }


    // Alors concerné par la physique
    physique(delta_t);
    
}

void Brique::render() {
    // Render the STL model
    Form::render();
    if(!modelSTL.isLoaded()){//Le stl n'a pas été chargée donc on doit dessiner à la main la brique
        //Alors on affiche une brique normal de base
        //Enzo doit faire une brique de 500/1000 de longeur, 200/1000 de largeur et 200/1000 de profondeur
        // printf("Triangle vide !! Doit donc dessiner la brique à la main %d\n");
            // Form::render();
            glBegin(GL_QUADS);
            {
                glVertex3d(0, 1, 0);
                glVertex3d(1, 1, 0);
                glVertex3d(1, 1, 1);
                glVertex3d(0, 1, 1);
            }
            glEnd();
    }else{
        modelSTL.setAnim(anim);//Anim pas utilisé pour le moment dans modelSTL, mais servirais 
        modelSTL.render();
    }
}

void PlanForm::update(reel delta_t) {

}
void PlanForm::render() {
    // Render the STL model
    Form::render();

    if(!modelSTL.isLoaded()){//Le stl n'a pas été chargée donc on doit dessiner à la main la brique
        //Alors on affiche une brique normal de base
        //Enzo doit faire une brique de 500/1000 de longeur, 200/1000 de largeur et 200/1000 de profondeur
        // printf("Triangle vide !! Doit donc dessiner la brique à la main %d\n");
            Form::render();

            glBegin(GL_QUADS);
            {
                glVertex3d(0, 1, 0);
                glVertex3d(1, 1, 0);
                glVertex3d(1, 1, 1);
                glVertex3d(0, 1, 1);
            }
            glEnd();
    }else{
        // printf("Something\n");
        modelSTL.setAnim(anim);
        modelSTL.render();
    }
}

void staticForm::update(reel delta_t) {

}
void staticForm::render() {
    // Render the STL model
    Form::render();

    if(!modelSTL.isLoaded()){//Le stl n'a pas été chargée donc on doit dessiner à la main la brique
        //Alors on affiche une brique normal de base
        //Enzo doit faire une brique de 500/1000 de longeur, 200/1000 de largeur et 200/1000 de profondeur
        // printf("Triangle vide !! Doit donc dessiner la brique à la main %d\n");
            Form::render();

            glBegin(GL_QUADS);
            {
                glVertex3d(0, 1, 0);
                glVertex3d(1, 1, 0);
                glVertex3d(1, 1, 1);
                glVertex3d(0, 1, 1);
            }
            glEnd();
    }else{
        // printf("Something\n");
        modelSTL.setAnim(anim);
        modelSTL.render();
    }
}