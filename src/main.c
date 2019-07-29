#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "vert.h"
#include "vr_bullshit.h"


vert verts[256] = {0};
int numverts = 0;
double height = 2.42;

void createfaces()
{
    for(int i = 0; i < numverts; i++)
    {
        printf("[\n");
        vert* v1 = &verts[i];
        vert* v2 = &verts[(i + 1) % numverts];
        printf("[ %lf, %lf, %lf ],\n", v2->x, 0.0, v2->z);
        printf("[ %lf, %lf, %lf ],\n", v2->x, height, v2->z);
        printf("[ %lf, %lf, %lf ],\n", v1->x, height, v1->z);
        printf("[ %lf, %lf, %lf ]\n", v1->x, 0.0, v1->z);

        if(i == numverts - 1)
        {
            printf("]\n");
        }
        else
        {
            printf("],\n");
        }
    }
}

int main(int argc, char* argv[])
{
    int c;

    while((c = getopt(argc, argv, "hw:")) != -1)
    {
        switch(c)
        {
            case 'h':
            {
                printf("-w  wall height(2.42m default)\n");
                break;
            }

            case 'w':
            {
                height = atof(optarg);
                break;
            }

            default:
                break;
        }
    }
    if(CVR_Init() != 0)
    {
        return 1;
    }

    printf("place chaperone corners with the controllers by pressing the trigger\n");
    printf("in a clockwise order and press the menu button to finish up\n");
    printf("max 256 points\n");
    printf("replace the .universes.collision_bounds data in .local/share/Steam/config/chaperone_info.vrchap with the output of this program and restart steamvr to get the new chaperone\n");

    while(1)
    {
        if(CVR_HandleInput())
        {
            break;
        }
    }

    CVR_Shutdown();
    return 0;
}
