#ifndef __RENDER_H
#define __RENDER_H

#include <GL/glut.h>
#include <vector> 
#include "dif.h"

extern std::vector<Context> contexts;  // итк два тела то и контекста два теперь массив 
extern std::vector<RigidBody> bodies;   // аналогшично с вышще 
extern double OldTime, DeltaTime; //  пермеенные времени 

void Run(int argc, char *argv[]);
void UpdateInertiaTensor(Context &context, double time);
void DrawCube(int size, float stretch_x, float stretch_y, float stretch_z); 

#endif //__RENDER_H