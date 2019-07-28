#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <linux/limits.h>

char inputfilename[PATH_MAX] = {0};
typedef struct vert
{
    double x, y, z;
} vert;

vert* verts = 0;
int numverts = 0;
double height = 2.42;


void readobj()
{
    FILE* f = fopen(inputfilename, "rb");

    if(!f)
    {
        return;
    }

    ssize_t bytesread = 0;
    char* line = 0;
    size_t len = 0;

    while((bytesread = getline(&line, &len, f)) != -1)
    {
        if(line[0] == 'v')
        {
            numverts++;
        }
    }

    verts = malloc(sizeof(vert) * numverts);
    fseek(f, 0, SEEK_SET);
    int i = 0;

    while((bytesread = getline(&line, &len, f)) != -1)
    {
        /*printf("%s", line);*/
        if(line[0] == 'v')
        {
            double x, y, z;
            sscanf(line, "v %lf %lf %lf", &x, &y, &z);
            verts[i].x = x;
            verts[i].y = y;
            verts[i].z = z;
            i++;
        }
    }

    if(line)
    {
        free(line);
    }

    fclose(f);
}

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
/* no openvr
 *
int main(int argc, char* argv[])
{
    int c;

    while((c = getopt(argc, argv, "hf:w:")) != -1)
    {
        switch(c)
        {
            case 'h':
            {
                printf("-f  input obj filename with just positions\n");
                printf("-w  wall height(2.42m default)\n");
                break;
            }

            case 'f':
            {
                sprintf(inputfilename, "%s", optarg);
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

    if(inputfilename[0] == 0)
    {
        return 1;
    }

    readobj();
    createfaces();

    if(verts)
    {
        free(verts);
    }

    return 0;
}
*/
/*
inline IVRSystem* VR_Init(EVRInitError* peError, EVRApplicationType eApplicationType, const char* pStartupInfo)
{
    IVRSystem* pVRSystem = 0;
    EVRInitError eError;
    VRToken() = VR_InitInternal2(&eError, eApplicationType, pStartupInfo);
    COpenVRContext& ctx = OpenVRInternal_ModuleContext();
    ctx.Clear();

    if(eError == VRInitError_None)
    {
        if(VR_IsInterfaceVersionValid(IVRSystem_Version))
        {
            pVRSystem = VRSystem();
        }
        else
        {
            VR_ShutdownInternal();
            eError = VRInitError_Init_InterfaceNotFound;
        }
    }

    if(peError)
    {
        *peError = eError;
    }

    return pVRSystem;
}


*/
#include "vr_bullshit.h"

int main(int argc, char* argv[])
{
    if(CVR_Init() != 0)
    {
        return 1;
    }

    while(1)
    {
        CVR_HandleInput();
    }

    CVR_Shutdown();
    return 0;
}
