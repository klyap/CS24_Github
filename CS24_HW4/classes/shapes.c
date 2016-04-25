#include "shapes.h"
#include <assert.h>
#include <stdlib.h>

/*============================================================================
 * Global State
 *
 * These are the instances of class-information used by objects of the various
 * types.  Pass a pointer to these to the various object-init functions.
 */

static Shape_Class  Shape;
static Box_Class    Box;
static Sphere_Class Sphere;
static Cone_Class   Cone;


/*============================================================================
 * General Functions
 */


/*!
 * This function performs static initialization of all classes.  It must be
 * called before any of the classes can be used.
 */  
void static_init() {
    Shape_class_init(&Shape);
    Box_class_init(&Box);
    Sphere_class_init(&Sphere);
    Cone_class_init(&Cone);
}


/*============================================================================
 * Shape base class
 */


/*! Static initialization for the Shape class. */
void Shape_class_init(Shape_Class *class) {
    /* TODO */
    class->getVolume = NULL;
}


/*!
 * Object initialization (i.e. the constructor) for the Shape class.  This
 * function initializes the density of the shape, as well as the class info.
 */
void Shape_init(Shape_Data *this, Shape_Class *class, float D) {
    /* TODO */
    this->class = class;
    Shape_setDensity(this, D);
    this->mass = Shape_getMass(this);
}


/*! Sets the density of this shape.  The argument must be nonnegative! */
void Shape_setDensity(Shape_Data *this, float D) {
    /* TODO */
    assert(D >= 0);
    this->density = D;
}


/*! Returns the mass of this shape, computed from the density and volume. */
float Shape_getMass(Shape_Data *this) {
    /* TODO */
    return this->class->getVolume(this) * this->density;
}


/*
 * THERE IS NO Shape_getVolume() FUNCTION, because Shape doesn't provide an
 * implementation!  In the class initialization, set the function-pointer to
 * NULL to signify this.
 */
 
/*
 * There is also no new_Shape() function, since Shape is abstract (missing
 * some of its implementation), and therefore is not instantiable.
 */


/*============================================================================
 * Box subclass
 */


/*! Static initialization for the Box class. */
void Box_class_init(Box_Class *class) {
    /* TODO */
    class->getVolume = Box_getVolume;
}


/*!
 * Object initialization (i.e. the constructor) for the Box class.  This
 * function first calls the Shape constructor to initialize the class info and
 * density, and then it initializes its data members with the specified values.
 */
void Box_init(Box_Data *this, Box_Class *class,
    float L, float W, float H, float D) {
    /* TODO */
    Shape_init((Shape_Data *) this, (Shape_Class *) class, D);
    Box_setSize(this, L, W, H);
}


/*!
 * This function implements the operation corresponding to the C++ code
 * "new Box(L, W, H, D)", performing both heap-allocation and initialization. 
 */
Box_Data * new_Box(float L, float W, float H, float D) {
    /* TODO */
    Box_Data *new_box = (Box_Data *) malloc(sizeof(Box_Data));
    Box_init(new_box, &Box, L, W, H, D);
    return new_box;
}


/*!
 * Sets the dimensions of the box.  The arguments are asserted to be positive.
 */
void Box_setSize(Box_Data *this, float L, float W, float H) {
    /* TODO */
    assert(L >= 0);
    assert(W >= 0);
    assert(H >= 0);
    this->length = L;
    this->width = W;
    this->height = H;
}


/*!
 * Computes the volume of the box.  This function provides the implementation
 * of Shape::getVolume(), which is abstract (i.e. pure-virtual).
 */
float Box_getVolume(Box_Data *this) {
    /* TODO */
    return this->length * this->width * this->height;
}


/*============================================================================
 * Sphere subclass
 */


/*! Static initialization for the Sphere class. */
void Sphere_class_init(Sphere_Class *class) {
    /* TODO */
    class->getVolume = Sphere_getVolume;
}


/*!
 * Object initialization (i.e. the constructor) for the Sphere class.  This
 * function first calls the Shape constructor to initialize the class info and
 * density, and then it initializes its data members with the specified values.
 */
void Sphere_init(Sphere_Data *this, Sphere_Class *class, float R, float D) {
    /* TODO */
    Shape_init((Shape_Data *)this,(Shape_Class *) class, D);
    Sphere_setRadius(this, R);
    
}


/*!
 * This function implements the operation corresponding to the C++ code
 * "new Sphere(R, D)", performing both heap-allocation and initialization. 
 */
Sphere_Data * new_Sphere(float R, float D) {
    /* TODO */
    Sphere_Data *new_sphere = (Sphere_Data *) malloc(sizeof(Sphere_Data));
    Sphere_init(new_sphere, &Sphere, R, D);
    return new_sphere;
}


/*! Sets the radius of the sphere.  The argument is asserted to be positive. */
void Sphere_setRadius(Sphere_Data *this, float R) {
    /* TODO */
    assert(R >= 0);
    this->radius = R;
}


/*!
 * Computes the volume of the sphere.  This function provides the implementation
 * of Shape::getVolume(), which is abstract (i.e. pure-virtual).
 */
float Sphere_getVolume(Sphere_Data *this) {
    /* TODO */
    return (4/3) * 3.14159 * this->radius * this->radius * this->radius;
}


/*============================================================================
 * Cone subclass
 */


/*! Static initialization for the Cone class. */
void Cone_class_init(Cone_Class *class) {
    /* TODO */
    class->getVolume = Cone_getVolume;
}


/*!
 * Object initialization (i.e. the constructor) for the Cone class.  This
 * function first calls the Shape constructor to initialize the class info and
 * density, and then it initializes its data members with the specified values.
 */
void Cone_init(Cone_Data *this, Cone_Class *class, float BR, float H, float D) {
    /* TODO */
    // Call Shape constructor
    
    Shape_init((Shape_Data *)this,(Shape_Class *) class, D);
    Cone_setBaseHeight(this, BR, H);
}


/*!
 * This function implements the operation corresponding to the C++ code
 * "new Cone(BR, H, D)", performing both heap-allocation and initialization. 
 */
Cone_Data * new_Cone(float BR, float H, float D) {
    /* TODO */
    Cone_Data *new_cone = (Cone_Data *) malloc(sizeof(Cone_Class));
    Cone_init(new_cone, &Cone, BR, H, D);
    return new_cone;
}


/*!
 * Sets the dimensions of the cone.  The arguments are asserted to be positive.
 */
void Cone_setBaseHeight(Cone_Data *this, float BR, float H) {
    /* TODO */
    assert(BR > 0);
    assert(H > 0);
    this->base_radius = BR;
    this->height = H;
}


/*!
 * Computes the volume of the cone.  This function provides the implementation
 * of Shape::getVolume(), which is abstract (i.e. pure-virtual).
 */
float Cone_getVolume(Cone_Data *this) {
    /* TODO */
    return (1 / 3) * 3.14159 * this->base_radius * this->base_radius
     * this->height;
}

