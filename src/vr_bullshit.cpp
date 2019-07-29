#include <openvr/openvr.h>
#include "vr_bullshit.h"
#include "vert.h"

extern vert verts[256];
extern int numverts;

using namespace vr;

IVRSystem* vrsys;
VRControllerState_t cstates[k_unMaxTrackedDeviceCount] = {0};

char CVR_Init()
{
    if(!(VR_IsHmdPresent() && VR_IsRuntimeInstalled()))
    {
        return -1;
    }

    vrsys = VR_Init(0, VRApplication_Scene);
    return 0;
}

char CVR_Shutdown()
{
    VR_Shutdown();
    return 0;
}

char CVR_HandleInput()
{
    VREvent_t event;

    while(vrsys->PollNextEvent(&event, sizeof(event)))
    {
        switch(event.eventType)
        {
            case vr::VREvent_TrackedDeviceActivated:
                printf("Device %u attached.\n", event.trackedDeviceIndex);
                break;

            case vr::VREvent_TrackedDeviceDeactivated:
                printf("Device %u detached.\n", event.trackedDeviceIndex);
                break;

            case vr::VREvent_TrackedDeviceUpdated:
                printf("Device %u updated.\n", event.trackedDeviceIndex);
                break;
        }
    }

    TrackedDevicePose_t poses[k_unMaxTrackedDeviceCount] = {0};
    VRCompositor()->GetLastPoses(poses, k_unMaxTrackedDeviceCount, 0, 0);

    for(TrackedDeviceIndex_t i = 0; i < k_unMaxTrackedDeviceCount; i++)
    {
        VRControllerState_t cstate;

        if(!vrsys->IsTrackedDeviceConnected(i))
        {
            continue;
        }

        if(vrsys->GetTrackedDeviceClass(i) != TrackedDeviceClass_Controller)
        {
            continue;
        }

        if(!poses[i].bPoseIsValid)
        {
            continue;
        }

        double x = poses[i].mDeviceToAbsoluteTracking.m[0][3];
        double y = poses[i].mDeviceToAbsoluteTracking.m[1][3];
        double z = poses[i].mDeviceToAbsoluteTracking.m[2][3];

        if(vrsys->GetControllerState(i, &cstate, sizeof(cstate)))
        {
            if(cstate.ulButtonPressed != cstates[i].ulButtonPressed &&
               cstate.ulButtonPressed & ButtonMaskFromId(EVRButtonId::k_EButton_SteamVR_Trigger))
            {
                verts[numverts].x = x;
                verts[numverts].y = y;
                verts[numverts].z = z;
                numverts++;

                if(numverts == 256)
                {
                    return 1;
                }
            }

            if(cstate.ulButtonPressed != cstates[i].ulButtonPressed &&
               cstate.ulButtonPressed & ButtonMaskFromId(EVRButtonId::k_EButton_ApplicationMenu))
            {
                return 1; // finishup and exit
            }

            cstates[i] = cstate;
            //printf("%i %lu\n", i, cstate.ulButtonPressed);
        }
    }

    return 0;
}
