#ifndef ALL_H_
#define ALL_H_

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>

#define RUNLENGTH 100000 //how many frames to run the simulation
#define TSIZE 257
#define WIDTH TSIZE //129
#define HEIGHT TSIZE //65
#define ROUGHNESS 0.6 //0.5 //the amount of noise on the first step
#define DELAY 3000 //milliseconds

double randomDouble();
int max(int a, int b);
double get(double* map, int y, int x);
void set(double* map, int y, int x, double set);
void initMap(double* map);
void drawMap(double* map, int maxY, int maxX);
void checkMakeColors(void);
char getRepresenter(double cellValue);
void printThatThingInTheCenter(int maxY, int maxX, int timer);
void diamondSquare(double* map);
void diamondSquareRecurse(double* map, int x, int y, int w, int h, double noiseScale);
void setColorOn(double here);
void setColorOff(double here);

#endif

// 1	0.5
// 2	0.5 * Math.pow(2.0, -0.75)
// 3	0.5 * Math.pow(2.0, -0.75) * Math.pow(2.0, -0.75)
// 4	0.5 * Math.pow(2.0, -0.75) * Math.pow(2.0, -0.75) * Math.pow(2.0, -0.75)

// f(x) = 0.5 * Math.pow(Math.pow(2.0, -0.75), step - 1)
