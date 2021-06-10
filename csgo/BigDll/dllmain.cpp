#include "pch.h"

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include "csgo.h"

HMODULE hMod;
HMODULE hClient, hEngine;

CreateInterfaceFn Client_CreateInterface;
CreateInterfaceFn Engine_CreateInterface;

C_CSPlayer* pLocalPlayer = 0;

class MyTraceFilter : ITraceFilter
{
    bool ShouldHitEntity(IHandleEntity* pEntity, int contentsMask) override
    {
        //printf("Call to ShouldHitEntity with ent %p mask %x\n", pEntity, contentsMask);
        if (pEntity == (IHandleEntity*) pLocalPlayer)
            return false;
        return true;
    }

    TraceType_t GetTraceType() const override
    {
        //printf("Call to GetTraceType\n");
        return TRACE_EVERYTHING;
    }
};


MyTraceFilter tracefilter;

void pause()
{
    while (1)
    {
        if (GetAsyncKeyState(VK_F3))
        {
            break;
        }
        Sleep(100);
    }
}

typedef bool(__fastcall* CreateMove_t)(void* ecx, DWORD edx, float flInputSampleTime, void* cmd);

CreateMove_t real_createmove;

struct fake_vtable_t {
    void* rtti;
    void* funcptrs[1500];
} fake_vtable;

CEngineTrace* g_CEngineTrace;

bool __fastcall Hk_CreateMove(void* thisptr, DWORD edx, float flInputSampleTime, CUserCmd* cmd)
{
    //printf("call to hk_createmove %p %f %p\n", thisptr, flInputSampleTime, cmd);

    //printf("buttons %d\n", cmd->buttons);

    void* pLocalPlayer = thisptr;
    Vector* pPos = (Vector*)((uintptr_t)pLocalPlayer + 0xa0);
    Vector* pViewangles = &cmd->viewangles;
    Vector* pCameraOffset = (Vector*)((uintptr_t)pLocalPlayer + 0x108);

    //printf("localplayer = %p\n", pLocalPlayer);
    if (!pLocalPlayer)
    {
        printf("bruhh no localplayer\n");
        return true;
    }

    Vector pSrc;
    pSrc.x = pPos->x + pCameraOffset->x;
    pSrc.y = pPos->y + pCameraOffset->y;
    pSrc.z = pPos->z + pCameraOffset->z;

    Vector pEnd;
    memset(&pEnd, 0, sizeof(Vector));
    AngleVectors(pViewangles, &pEnd);
    //printf("anglevectors %f %f %f\n", pEnd.x, pEnd.y, pEnd.z);
    pEnd.x *= 8192.f;
    pEnd.y *= 8192.f;
    pEnd.z *= 8192.f;
    pEnd.x += pSrc.x;
    pEnd.y += pSrc.y;
    pEnd.z += pSrc.z;

    Ray_t ray;
    Ray_Init(&ray, pSrc, pEnd);

    //printf("tracefilter = %p\n", &tracefilter);

    trace_t trace;

    //printf("TIME FOR THE BIG FUCKY WUCky!!1\n");

    //pause();

    //printf("ddddd\n");

    g_CEngineTrace->TraceRay(&ray, MASK_SHOT, (ITraceFilter*)&tracefilter, &trace);
    //g_CEngineTrace->TraceRay(&ray, MASK_SHOT, NULL, &trace);

    //printf("endpos %f %f %f, fraction %f, ent %p\n", trace.endpos.x, trace.endpos.y, trace.endpos.z, trace.fraction, trace.m_pEnt);


    cmd->buttons &= ~1;
    void* g_worldEnt = entitylist->GetClientEntity(0); // world should always be id 0
    if (trace.m_pEnt && trace.m_pEnt != g_worldEnt)
    {
        //printf("wow we hit something!!\n");
        //printf("hitbox: %d %d\n", trace.hitbox, trace.hitgroup);
        if (trace.hitgroup == 1)
            cmd->buttons |= 1;
    }

    return real_createmove(thisptr, 0, flInputSampleTime, cmd);
}

void DoMyShitttt()
{

    hClient = GetModuleHandleA("client.dll");
    hEngine = GetModuleHandleA("engine.dll");
    if (!hClient || !hEngine)
    {
        printf("GetModuleHandle fails?\n");
        return;
    }

    Client_CreateInterface = (CreateInterfaceFn)GetProcAddress(hClient, "CreateInterface");
    Engine_CreateInterface = (CreateInterfaceFn)GetProcAddress(hEngine, "CreateInterface");

    int returnCode;
    g_CEngineTrace = (CEngineTrace*)Engine_CreateInterface("EngineTraceClient004", &returnCode);
    printf("g_CEngineTrace = %p\n", g_CEngineTrace);

    if (!g_CEngineTrace)
    {
        printf("bruhh no enginetrace\n");
        return;
    }

    entitylist = (IClientEntityList*)Client_CreateInterface("VClientEntityList003", &returnCode);
    printf("entitylist = %p\n", entitylist);
    if (!entitylist)
    {
        printf("bruhh no entitylist\n");
        return;
    }

    printf("hClient = %p\n", hClient);
    pLocalPlayer = (C_CSPlayer*) entitylist->GetClientEntity(1); // localplayer should always be id 1

    printf("localplayer = %p\n", pLocalPlayer);
    if (!pLocalPlayer)
    {
        printf("bruhh no localplayer\n");
        return;
    }

    void** player_vtbl = *(void***)pLocalPlayer;
    printf("original vtable at %p\n", player_vtbl);

    memcpy(&fake_vtable, (void*)((uintptr_t)player_vtbl-4), sizeof(fake_vtable_t));

    real_createmove = (CreateMove_t)fake_vtable.funcptrs[288];

    printf("createmove at %p\n", real_createmove);
    fake_vtable.funcptrs[288] = Hk_CreateMove;

    // install BIG vtable hook !!!!!!
     *(void***)pLocalPlayer = fake_vtable.funcptrs;
    printf("hook installed. now pointing to %p\n", fake_vtable.funcptrs);

    // netvar shit
    ClientClass* pClientClass = pLocalPlayer->GetClientNetworkable()->GetClientClass();
    printf("client class = %p\n", pClientClass);
    
    printf("class name = %s\n", pClientClass->m_pNetworkName);

    RecvTable* netvar_table = pClientClass->m_pRecvTable;
    printf("recv table at %p\n", netvar_table);

    //printf("%f %f %f\n", pPos->x, pPos->y, pPos->z);
    //printf("pitch %f yaw %f\n", pViewangles->x, pViewangles->y);

    pause();
    
    // uninstall vtable hook
    *(void***)pLocalPlayer = player_vtbl;
    
    printf("done\n");
    return;
}

// NOT a hack. it's a game >>> MOD <<< for educational purpose :)
DWORD CALLBACK MyNotAHack_Main(LPVOID arg)
{
    //Beep(440, 1000);

    AllocConsole();
    (void)freopen("CONIN$", "r", stdin);
    (void)freopen("CONOUT$", "w+", stdout);
    SetStdHandle(STD_INPUT_HANDLE, stdin);
    SetStdHandle(STD_OUTPUT_HANDLE, stdout);

    printf("Hello world!\n");

    DoMyShitttt();

    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) FreeLibrary, hMod, 0, NULL);

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    hMod = hModule;

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MyNotAHack_Main, NULL, 0, NULL);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

