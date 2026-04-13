#include "render.h"

int main(int argc, char *argv[])
{
    Run(argc, argv);
    return 0;
}

//  g++ -std=c++17 main.cpp dif.cpp render.cpp -o Lab1.exe -I. -I./freeglut/include -I./glm -L./freeglut/lib -lopengl32 -lglu32 -lfreeglut