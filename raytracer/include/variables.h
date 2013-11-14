/*
 * external variables
 */
#include <string>

#include "main.h"

#ifdef MAINPROG
#define EXTERN
#else
#define EXTERN extern
#endif

#ifdef MAINPROG
int wsizew = 0; wsizeh = 0;
int maxdepth = 5;
std::string ofilename("raytrace.png");

vec3 eyeinit(0.0, 0.0, 5.0);
vec3 upinit(0.0, 1.0, 0.0);
vec3 center(0.0, 0.0, 0.0);
float fovy = 90.0;
#else
EXTERN int wsizew, wsizeh; // window size width & height
EXTERN int maxdepth;       // maximum depth for a ray. default to 5;
EXTERN std::string ofilename;

EXTERN vec3 eyeinit;
EXTERN vec3 upinit;
EXTERN vec3 center;
EXTERN float fovy;
#endif

static enum { view, translate, scale } transop;
enum shape { sphere, tri, trinormal };

// Objects
const int maxobjects = 10;
EXTERN int numobjects;
EXTERN struct object {
    shape type;
} objects[maxobjects];

