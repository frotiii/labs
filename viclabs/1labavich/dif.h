#ifndef __DIF_H
#define __DIF_H

#define SIZE 50 //размер куба для отрисовки 

#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace glm; // чтобы кратче писать 

// просто константные вещи 
struct Context
{
    double M_inv; //обратная масса ( те 1/масса )
    dmat3 I_inv; // обратный тензор инерции в теле
    double deformation; // параметр деформации от 1/2 до 3/2
    dvec3 F1, F2; // веткоры сил
    dvec3 p1, p2;  // точки приложенияч сил 
};

// изменяемые состояния
struct RigidBody
{
    dvec3 r, l,  L; // положение центра масс, импульс ( масса на скорость ), момент импульса
    dquat q; // кватернион ( описывает ориентацию тела )
};

double SolveRungeKutta(RigidBody &rb, const Context &context, double h, double cur_time);
RigidBody f_rigidbody(const RigidBody &rb, const Context &context, double time);
RigidBody MulRB(const RigidBody &rb, double num);
RigidBody SumRB(const RigidBody &r1, const RigidBody &r2);



#endif 