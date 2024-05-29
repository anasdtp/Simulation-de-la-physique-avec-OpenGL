#ifndef FORMS_H_INCLUDED
#define FORMS_H_INCLUDED

#include "geometry.h"
#include "animation.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <GL/gl.h>



//l'echelle est en metre. Soit 1 egale 1 metre


enum SHAPE_ID{
    CUBE,
    SPHERE,
    CYLINDER,
    CONE,
    STL
};

class Color
{
public:
    float r, g, b;
    Color(float rr = 1.0f, float gg = 1.0f, float bb = 1.0f) {r=rr; g=gg; b=bb;}
};

// Constant Colors
const Color RED(1.0f, 0.0f, 0.0f);
const Color BLUE(0.0f, 0.0f, 1.0f);
const Color GREEN(0.0f, 1.0f, 0.0f);
const Color YELLOW(1.0f, 1.0f, 0.0f);
const Color WHITE(1.0f, 1.0f, 1.0f);
const Color ORANGE(1.0f, 0.65f, 0.0f);

struct Vertex {
    float x, y, z;
};

struct Triangle {
    Vertex normal;
    Vertex vertices[3];
};

// Generic class to render and animate an object
class Form
{
protected:
    Color col;
    Animation anim;
    SHAPE_ID _id;

public:
    Animation& getAnim() {return anim;}
    void setAnim(Animation ani) {anim = ani;}
    void setID(SHAPE_ID id) {_id = id;}
    SHAPE_ID getID() {return _id;}

    std::vector<Triangle> triangleSTL;//Pour le STL
    bool loadSTL(const std::string& path);
    // This method should update the anim object with the corresponding physical model
    // It has to be done in each inherited class, otherwise all forms will have the same movements !
    // Virtual method for dynamic function call
    // Pure virtual to ensure all objects have their physics implemented
    virtual void update(double delta_t) = 0;
    // Virtual method : Form is a generic type, only setting color and reference position
    virtual void render();
};

// A particular Form
class Sphere : public Form
{
private:
    // The sphere center is aligned with the coordinate system origin
    // => no center required here, information is stored in the anim object
    double radius;
public:
    Sphere(double r = 1.0, Color cl = Color());
    double getRadius() const {return radius;}
    void setRadius(double r) {radius = r;}
    void update(double delta_t);
    void render();
};

// A face of a cube
class Cube : public Form
{
private:
    Vector vdir1, vdir2;
    double length, width;
    float _masse;
    Point size;
public:
    Cube(Vector v1 = Vector(1,0,0), Vector v2 = Vector(0,0,1),
          Point org = Point(), double l = 1.0, double w = 1.0,
          Color cl = Color());


    void update(double delta_t);
    void render();
};

class Brique : public Form
{
private:
    float _posX, _posY, _posZ;
    Point _sizeObjet;//La place que prend l'objet dans les trois axes
    float _masse;
public:
    Brique(Color cl = Color()) {
        setID(STL);
        col = cl;
        _masse = 1;
    }
    void setTriangles(const std::vector<Triangle>& tris) {
        triangleSTL = tris;
    }
    void getTriangles(std::vector<Triangle>& tr){
        tr = triangleSTL;
    }
    void setColor(Color cl) {col = cl;}
    void setSize(const Point size) {//La place que prend l'objet dans les trois axes
        _sizeObjet = size;
    }
    
    void render();
    void update(double delta_t);

    void moveRelative(Point position) {
        anim.setPos(anim.getPos() + position);
    }
    void moveAbsolue(Point position) {
        anim.setPos(position);
    }

};

#endif // FORMS_H_INCLUDED