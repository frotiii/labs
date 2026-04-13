#include <cmath>
#include <iostream>
#include "dif.h"

// проивзодлная состояния тела в конкретный момент времени 
RigidBody f_rigidbody(const RigidBody &rb, const Context &context, double time)
{
    RigidBody dt; // result

    // r' = l / M ( как раз для этого и инверт хранится ). скорость с которой движется центр масс
    dt.r = rb.l * context.M_inv;

    // transform quaternion to rotation matrix ( матрица поворота по сути )
    dmat3 R(rb.q);

    // omega = I(t) * L = R * I_body * R^(-1) * L ( угловая скорость )
    dvec3 omega = R * context.I_inv * glm::transpose(R) * rb.L; 
    
    // q' = 1/2 * q * ω  (для GLM!) ( кватернион углово  скоростьи )
    dquat omega_q(0.0, omega.x, omega.y, omega.z);
    dt.q = 0.5 * rb.q * omega_q;
    // это все по сути нужно тк тензор инцерции в система тела постояннфыый. а в системе мировой менятеся рпи повороте 
    // выше получили углову. скорость именно в мировой системе 

    // точки приложения в мировых координатазх
    dvec3 world_p1 = rb.r + R * context.p1;
    dvec3 world_p2 = rb.r + R * context.p2;
    
    // векторы от центра масс до точек приложения 
    dvec3 r1 = world_p1 - rb.r;  
    dvec3 r2 = world_p2 - rb.r;
    
    // суммарная сила и суммарный мсомент ( ниже )
    dvec3 Force = context.F1 + context.F2;
    dt.l = Force;

    dvec3 Torque = cross(r1, context.F1) + cross(r2, context.F2);
    dt.L = Torque;

    return dt;
}

// для метода РК4 , умножает все состояните на скаляр 
RigidBody MulRB(const RigidBody &rb, double num)
{
    RigidBody res;

    res.r = rb.r * num;
    res.q = rb.q * num;
    res.l = rb.l * num;
    res.L = rb.L * num;

    return res;
}

// аналогично для суммирования состояний покомпонентно для метода РК4 опять же
RigidBody SumRB(const RigidBody &r1, const RigidBody &r2)
{
    RigidBody res;

    res.r = r1.r + r2.r;
    res.q = r1.q + r2.q;
    res.l = r1.l + r2.l;
    res.L = r1.L + r2.L;

    return res;
}

//    X(t + h) = X(t) + h / 6 * (k1 + 2 * k2 + 2 * k3 + k4)
double SolveRungeKutta(RigidBody &rb, const Context &context, double h, double cur_time)
{
    // k1 = f(X(t), t)
    RigidBody k1 = f_rigidbody(rb, context, cur_time);

    // k2 = f(X(t) + h * k1 / 2, t + h / 2)
    RigidBody tmp1 = SumRB(rb, MulRB(k1, h/2.0));
    RigidBody k2 = f_rigidbody(tmp1, context, cur_time + h/2.0);

    // k3 = f(X(t) + h * k2 / 2, t + h / 2)
    RigidBody tmp2 = SumRB(rb, MulRB(k2, h/2.0));
    RigidBody k3 = f_rigidbody(tmp2, context, cur_time + h/2.0);

    // k4 = f(X(t) + h * k3, t + h)  
    RigidBody tmp3 = SumRB(rb, MulRB(k3, h));
    RigidBody k4 = f_rigidbody(tmp3, context, cur_time + h);

    // sum = k1 + 2*k2 + 2*k3 + k4
    RigidBody sum = SumRB(
        SumRB(k1, MulRB(k2, 2.0)),
        SumRB(MulRB(k3, 2.0), k4)
    );

    // обновление состоний ( чек первый коммент к фкунции ). после нормализация кватерниона 
    rb = SumRB(rb, MulRB(sum, h/6.0));
    rb.q = normalize(rb.q);

    // сначяала матрица поворота, потом угловая скорость, возврат проивдееиня угловой скорости на момент импльса для проверки инварианта 
    // тк при отсуствии внешних сил инвариант ( момент импульса ) сохраняется
    dmat3 R = dmat3(normalize(rb.q));
    dvec3 omega = R * context.I_inv * transpose(R) * rb.L;
    return dot(omega, rb.L);
}
