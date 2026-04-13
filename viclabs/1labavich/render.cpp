#include <cmath>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include "render.h"

#define WIDTH 1000
#define HEIGHT 600

#define TIME_SCALE 1000


// Global wariables (it's bad but ok for glut)
double OldTime = -1, DeltaTime;
std::vector<RigidBody> bodies(2);  // два тела ( 0 для гара 1 длчя куба )
std::vector<Context> contexts(2);   // два контекста
void SaveAngularMomentumData();

void DrawAxes() //оси просто так
{
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    
    // красная
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0, 0, 0);
    glVertex3f(100, 0, 0);
    
    // зелёная
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 100, 0);
    
    // синяя
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 100);
    
    glEnd();
}

void DrawSphere(int size, float stretch_x, float stretch_y, float stretch_z)
{
    int segments = 20;  
    
    // основные оси обвалакивающие шар 
    glLineWidth(1.5f);
    glColor3f(1.0f, 0.8f, 0.2f);
    
    // верт
    // для каждой верт рисуем линию вдоль
    for (int i = 0; i < segments; i++) {
        float lon1 = (float)i / segments * 2.0f * 3.14159f; 
        
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j <= segments; j++) {
            float lat = (float)j / segments * 3.14159f - 3.14159f/2.0f; 
            
            //  формула сферы
            float x = size * cos(lat) * cos(lon1);
            float y = size * sin(lat);
            float z = size * cos(lat) * sin(lon1);
            
            glVertex3f(x * stretch_x, y * stretch_y, z * stretch_z); //  деформацичя при таком умножении 
        }
        glEnd();
    }
    
    // горизонт
    //  для каждого горизонта рисуем круг 
    for (int j = 0; j < segments; j++) {
        float lat1 = (float)j / segments * 3.14159f - 3.14159f/2.0f;
        
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i <= segments; i++) {
            float lon = (float)i / segments * 2.0f * 3.14159f;
            
            float x = size * cos(lat1) * cos(lon);
            float y = size * sin(lat1);
            float z = size * cos(lat1) * sin(lon);
            
            glVertex3f(x * stretch_x, y * stretch_y, z * stretch_z);
        }
        glEnd();
    }
    
    glLineWidth(3.0f);  

    // контрастная полоска чтобы видно вращение 
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for (int j = 0; j <= segments; j++) {
        float lat = (float)j / segments * 3.14159f - 3.14159f/2.0f;
        float x = size * cos(lat) * cos(3.14159f/2);
        float y = size * sin(lat);
        float z = size * cos(lat) * sin(3.14159f/2);
        glVertex3f(x * stretch_x, y * stretch_y, z * stretch_z);
    }
    glEnd();
}

void DrawCube(int size, float stretch_x, float stretch_y, float stretch_z)
{

    glBegin(GL_QUADS);
    
    // верхняя грань (y = size) зелёная полупрозрачная
    glColor4f(0.0f, 1.0f, 0.0f, 0.3f);
    glVertex3f(size * stretch_x, size * stretch_y, -size * stretch_z);
    glVertex3f(-size * stretch_x, size * stretch_y, -size * stretch_z);
    glVertex3f(-size * stretch_x, size * stretch_y, size * stretch_z);
    glVertex3f(size * stretch_x, size * stretch_y, size * stretch_z);

    // нижняя грань (y = -size) оранжевая полупрозрачная
    glColor4f(1.0f, 0.5f, 0.0f, 0.3f);
    glVertex3f(size * stretch_x, -size * stretch_y, size * stretch_z);
    glVertex3f(-size * stretch_x, -size * stretch_y, size * stretch_z);
    glVertex3f(-size * stretch_x, -size * stretch_y, -size * stretch_z);
    glVertex3f(size * stretch_x, -size * stretch_y, -size * stretch_z);

    // передняя грань (z = size) красная полупрозрачная
    glColor4f(1.0f, 0.0f, 0.0f, 0.3f);
    glVertex3f(size * stretch_x, size * stretch_y, size * stretch_z);
    glVertex3f(-size * stretch_x, size * stretch_y, size * stretch_z);
    glVertex3f(-size * stretch_x, -size * stretch_y, size * stretch_z);
    glVertex3f(size * stretch_x, -size * stretch_y, size * stretch_z);

    // задняя грань (z = -size) жёлтая полупрозрачная
    glColor4f(1.0f, 1.0f, 0.0f, 0.3f);
    glVertex3f(size * stretch_x, -size * stretch_y, -size * stretch_z);
    glVertex3f(-size * stretch_x, -size * stretch_y, -size * stretch_z);
    glVertex3f(-size * stretch_x, size * stretch_y, -size * stretch_z);
    glVertex3f(size * stretch_x, size * stretch_y, -size * stretch_z);

    // левая грань (x = -size) синяя полупрозрачная
    glColor4f(0.0f, 0.0f, 1.0f, 0.3f);
    glVertex3f(-size * stretch_x, size * stretch_y, size * stretch_z);
    glVertex3f(-size * stretch_x, size * stretch_y, -size * stretch_z);
    glVertex3f(-size * stretch_x, -size * stretch_y, -size * stretch_z);
    glVertex3f(-size * stretch_x, -size * stretch_y, size * stretch_z);

    // правая грань (x = size) фиолетовая полупрозрачная
    glColor4f(1.0f, 0.0f, 1.0f, 0.3f);
    glVertex3f(size * stretch_x, size * stretch_y, -size * stretch_z);
    glVertex3f(size * stretch_x, size * stretch_y, size * stretch_z);
    glVertex3f(size * stretch_x, -size * stretch_y, size * stretch_z);
    glVertex3f(size * stretch_x, -size * stretch_y, -size * stretch_z);
    
    glEnd();
}

// Change size of window
void Reshape(int W, int H)
{
    // set camera model
    glViewport(0, 0, W, H);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (double)W / H, 1, 500); // угол обзора, отн сторон
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND); //  рпозранчосот
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //  стандратный режим

    // обновление физики всех тех ( онбова тензора, рунге кутт шщаг )
    for (int i = 0; i < 2; i++) {
        UpdateInertiaTensor(contexts[i], OldTime); 
        double invariant = SolveRungeKutta(bodies[i], contexts[i], DeltaTime, OldTime);
    }

    SaveAngularMomentumData(); //  для графиков , сохранение момента импульса в файл 

    // НАСТРОКА КАМЕРЫ ПОЛОЖЕНИЯ ТУТ , первые три позиция
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(200.0, 100.0, 200.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glDisable(GL_LIGHTING);
    DrawAxes();


    
    // рисовка куба ( первое тело ) 
    //  тут вычисление деформации , ниже оценка экстрем значений 
    double base_inertia1 = 1.0 / (3.0 * (1.0/contexts[1].M_inv) * SIZE * SIZE);
    float stretch_x1 = sqrt(base_inertia1 / contexts[1].I_inv[0][0]);
    float stretch_y1 = sqrt(base_inertia1 / contexts[1].I_inv[1][1]);
    float stretch_z1 = sqrt(base_inertia1 / contexts[1].I_inv[2][2]);
    
    stretch_x1 = std::max(0.5f, std::min(2.0f, stretch_x1));
    stretch_y1 = std::max(0.5f, std::min(2.0f, stretch_y1));
    stretch_z1 = std::max(0.5f, std::min(2.0f, stretch_z1));

    
    // отрисовка шара ( нуцлевое телдо )
    //  тут тавк то аналогично 
    double base_inertia0 = 1.0 / (3.0 * (1.0/contexts[0].M_inv) * SIZE * SIZE);
    float stretch_x0 = sqrt(base_inertia0 / contexts[0].I_inv[0][0]);
    float stretch_y0 = sqrt(base_inertia0 / contexts[0].I_inv[1][1]);
    float stretch_z0 = sqrt(base_inertia0 / contexts[0].I_inv[2][2]);
    
    stretch_x0 = std::max(0.5f, std::min(2.0f, stretch_x0));
    stretch_y0 = std::max(0.5f, std::min(2.0f, stretch_y0));
    stretch_z0 = std::max(0.5f, std::min(2.0f, stretch_z0));

    // сохранение текущ матрицы
    //  перемещение + поворот + отрисовки + восстановление матрицы попом 
    glPushMatrix();
    glTranslated(bodies[0].r.x, bodies[0].r.y, bodies[0].r.z);
    bodies[0].q = glm::normalize(bodies[0].q);
    glRotated(glm::degrees(acos(bodies[0].q.w)) * 2, bodies[0].q.x, bodies[0].q.y, bodies[0].q.z);
    DrawSphere(SIZE, stretch_x0, stretch_y0, stretch_z0);
    glPopMatrix();

    //  аналогично с выше но для шара 
    glPushMatrix();
    glTranslated(bodies[1].r.x, bodies[1].r.y, bodies[1].r.z);
    bodies[1].q = glm::normalize(bodies[1].q);
    glRotated(glm::degrees(acos(bodies[1].q.w)) * 2, 
              bodies[1].q.x, bodies[1].q.y, bodies[1].q.z);
    DrawCube(SIZE, stretch_x1, stretch_y1, stretch_z1);
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}

// All events processed -> put new Display event in queue
void Idle()
{
    // update time
    long Time;
    if (OldTime == -1)
        OldTime = clock();
    Time = clock();
    DeltaTime = (double)(Time - OldTime) / CLOCKS_PER_SEC / 1000; //  в секундах
    OldTime = Time;

    // redraw frame
    glutPostRedisplay();
}

// Button click event
void Keyboard(unsigned char Key, int MouseX, int MouseY)
{
    if (Key == 27)
        exit(0);
}

// обновлдение тензора 
void UpdateInertiaTensor(Context &context, double time)
{  
    double base_inertia = 1.0 / (3.0 * (1.0/context.M_inv) * SIZE * SIZE); //  базовый тензор для недеформ 

    dmat3 new_I_inv(0.0);

    double speed = 0.001;  //  медленно шоб 
    
    double kx = 1.0 / (1.0 + 0.3 * sin(time * speed));   // сжатие разжатие по абсциссе         
    double ky = 1.0 / (1.0 + 0.3 * sin(time * speed + 2.0)); // аналогично по ординате со сдвигом
    double kz = 1.0 / (1.0 + 0.3 * sin(time * speed + 4.0));  // аналогично по з со свдигом 
    // разные чтоб волку дать по осям , дефыормацичя с остатсаванием . иначе была бы туп пульсация , как масштабирование 
    
    //  обновыление обновление 
    // тензор всгеда диагональный 
    new_I_inv[0][0] = base_inertia * kx;
    new_I_inv[1][1] = base_inertia * ky;
    new_I_inv[2][2] = base_inertia * kz;
    
    context.I_inv = new_I_inv;

    //  прост ввод 
    static double last_time = 0;
    if (time - last_time > 0.5) { 
        std::cout << "Деформация: [" << 1.0/kx << ", " << 1.0/ky << ", " << 1.0/kz << "]" << std::endl;
        last_time = time;
    }
}

//  сохр для графика, сохр кжаддые 0,1 сек
void SaveAngularMomentumData()
{
    static std::ofstream file("angular_momentum.txt");
    static double last_save_time = 0;
    
    if (OldTime - last_save_time > 0.01) {  
        for (int i = 0; i < bodies.size(); i++) {
            file << OldTime << " " << i << " " << length(bodies[i].L) << std::endl;
        }
        last_save_time = OldTime;
    }
}

// Start all calculations and drawing
void Run(int argc, char *argv[])
{
    //иниц шщара. масса 10
    contexts[0].M_inv = 1.0/10.0;  
    contexts[0].deformation = 1.0; 
    
    contexts[0].I_inv = dmat3(0);
    double base_inertia = 1.0 / (3.0 * (1.0/contexts[0].M_inv) * SIZE * SIZE);
    for (int i = 0; i < 3; i++)
        contexts[0].I_inv[i][i] = base_inertia;

    contexts[0].p1 = dvec3(SIZE/2.0, 0, 0);  
    contexts[0].p2 = dvec3(-SIZE/2.0, 0, 0);  
    contexts[0].F1 = dvec3(0, 0, 0);    //  силы ( равные ! )
    contexts[0].F2 = dvec3(0, 0, 0);   
    
    bodies[0].r = dvec3(0, 0, 0);  // цуентр
    bodies[0].q = dquat(1.0, 0, 0, 0);  
    bodies[0].l = dvec3(0, 0, 0);     
    bodies[0].L = dvec3(0, 10000000, 0); //  момент импульса 

    // кубик миасса 8
    contexts[1].M_inv = 1.0/8.0;  
    contexts[1].deformation = 1.0; 
    
    contexts[1].I_inv = dmat3(0);
    double base_inertia2 = 1.0 / (3.0 * (1.0/contexts[1].M_inv) * SIZE * SIZE);
    for (int i = 0; i < 3; i++)
        contexts[1].I_inv[i][i] = base_inertia2;

    contexts[1].p1 = dvec3(SIZE/2.0, 0, 0);  
    contexts[1].p2 = dvec3(-SIZE/2.0, 0, 0);  
    contexts[1].F1 = dvec3(0, 500, 0);    // силы ( неравныек ! )
    contexts[1].F2 = dvec3(0, -200, 0);   
    
    bodies[1].r = dvec3(150, 0, -150);   // центр
    bodies[1].q = dquat(1.0, 0, 0, 0);  
    bodies[1].l = dvec3(0, 0, 0);     
    bodies[1].L = dvec3(0, 5000000, 0);   // поменьше момент

    // initialization
    glutInit(&argc, argv);

    // Request double buffered true color window with Z-buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    // Set size of window
    glutInitWindowSize(WIDTH, HEIGHT);
    // Windows position on screen
    glutInitWindowPosition(0, 0);
    // Creating window with name
    glutCreateWindow("кубик шарик");

    // Fill background color
    glClearColor(0.3, 0.5, 0.7, 0);

    // Set functions for GLUT loop
    glutReshapeFunc(Reshape);
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    glutKeyboardFunc(Keyboard);

    // Enable 3D mode
    glEnable(GL_DEPTH_TEST);

    // Start infinite loop
    glutMainLoop();
}