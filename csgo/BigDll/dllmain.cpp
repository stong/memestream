#include "pch.h"

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include <algorithm>
#include <unordered_map>

#include "csgo.h"

// TODO:
// stop using a loop to track localplayer (hook engineclient)
// get rid of getasynckeystate
// use convar to parse m_yaw rather than hardcode value
//
// place all vmt hook pointers in some little cave in client.dll so they're not pointing into "nowhere"

HMODULE hMod;
HMODULE hClient, hEngine;

CreateInterfaceFn Client_CreateInterface;
CreateInterfaceFn Engine_CreateInterface;

C_CSPlayer* pLocalPlayer = 0;

// Am i using a controller today?
const BOOL IsConsolePlayer = FALSE;

ptrdiff_t Get_netvar_offset(RecvTable* netvar_table, const char* var_name, ptrdiff_t offset = 0)
{
    for (int i = 0; i < netvar_table->m_nProps; i++)
    {
        RecvProp* prop = &netvar_table->m_pProps[i];
        if (prop->m_pVarName && !strcmp(prop->m_pVarName, var_name))
        {
            // found. cache the result
            return offset + prop->m_Offset;
        }
        if (prop->m_RecvType == DPT_DataTable && prop->m_pDataTable)
        {
            // recurse
            ptrdiff_t result = Get_netvar_offset(prop->m_pDataTable, var_name, offset + prop->m_Offset);
            if (result != -1)
                return result;
        }
    }
    // not found
    return -1;
}

std::unordered_map<ClientClass*, std::unordered_map<std::string, ptrdiff_t>> netvar_cache;

template <typename T>
T Get_netvar(IClientNetworkable* entity, const char* var_name)
{
    ClientClass* clientClass = entity->GetClientClass();
    // check for cached result
    ptrdiff_t offset = -1;
    if (netvar_cache.find(clientClass) != netvar_cache.end())
    {
        auto& subcache = netvar_cache[clientClass];
        auto it = subcache.find(var_name);
        if (it != subcache.end())
        {
            offset = it->second;
        }
    }
    if (offset == -1) // no cached result
    {
        offset = Get_netvar_offset(clientClass->m_pRecvTable, var_name);
        netvar_cache[clientClass][var_name] = offset; // cache result
    }

    // printf("offset of %s = %x, dt base = %x\n", var_name, offset, (uintptr_t)entity->GetDataTableBasePtr() - (uintptr_t)entity);
    return *(T*)((uintptr_t)entity->GetDataTableBasePtr() + offset);
}

void Dump_netvars(RecvTable* netvar_table, int level = 0)
{
    for (int i = 0; i < netvar_table->m_nProps; i++)
    {
        RecvProp* prop = &netvar_table->m_pProps[i];
        if (prop->m_pVarName && !isdigit(*prop->m_pVarName))
        {
            for (int j = 0; j < level; j++) printf(" ");
            printf("%03d. +%04x %s %s\n", i, prop->m_Offset, prop->m_pVarName, SendPropType_to_string(prop->m_RecvType));
        }
        if (prop->m_RecvType == DPT_DataTable && prop->m_pDataTable)
        {
            Dump_netvars(prop->m_pDataTable, level + 1);
        }
    }
}


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
} fake_player_vtable;

fake_vtable_t fake_cinput_vtable;

CEngineTrace* g_CEngineTrace;

bool vischeck(C_CSPlayer* pLocalPlayer, C_CSPlayer* otherPlayer)
{
    Vector pos = Get_netvar<Vector>(pLocalPlayer->GetClientNetworkable(), "m_vecOrigin");
    auto cameraOffset = Get_netvar<Vector>(pLocalPlayer->GetClientNetworkable(), "m_vecViewOffset[0]");
    Vector src = pos + cameraOffset;

    pos = Get_netvar<Vector>(otherPlayer->GetClientNetworkable(), "m_vecOrigin");
    cameraOffset = Get_netvar<Vector>(otherPlayer->GetClientNetworkable(), "m_vecViewOffset[0]");
    Vector end = pos + cameraOffset;

    Ray_t ray;
    Ray_Init(&ray, src, end);

    trace_t trace;
    g_CEngineTrace->TraceRay(&ray, MASK_SHOT, (ITraceFilter*)&tracefilter, &trace);

    if (trace.m_pEnt && trace.m_pEnt == (CBaseEntity*)otherPlayer)
        return true;

    if (trace.fraction > 0.99f)
        return true;

    return false;
}

void do_Trigger(C_CSPlayer* pLocalPlayer, float flInputSampleTime, CUserCmd* cmd)
{
    static int clickTimeTicks = 0;
    static int clickTimeDelayTicks = 0;

    auto pos = Get_netvar<Vector>(pLocalPlayer->GetClientNetworkable(), "m_vecOrigin");
    Vector viewangles = cmd->viewangles;
    auto cameraOffset = Get_netvar<Vector>(pLocalPlayer->GetClientNetworkable(), "m_vecViewOffset[0]");

    //printf("localplayer = %p\n", pLocalPlayer);
    if (!pLocalPlayer)
    {
        printf("bruhh no localplayer\n");
        return;
    }

    // don't click and unclick so often
    if (clickTimeTicks > 0)
    {
        if (clickTimeTicks < clickTimeDelayTicks / 2) // duty cycle of 50%
            cmd->buttons |= IN_ATTACK;
        clickTimeTicks--;
        return;
    }

    // don't run if the triggerkey not pressed
    if (!GetAsyncKeyState(VK_XBUTTON2) && !IsConsolePlayer)
        return;

    // don't run if user already clicking manually
    if (cmd->buttons & IN_ATTACK)
        return;

    // weapon selection logic
    CBaseHandle weapon_ehandle = Get_netvar<CBaseHandle>(pLocalPlayer->GetClientNetworkable(), "m_hActiveWeapon");
    int weapon_ent_index = weapon_ehandle.m_Index & ENT_ENTRY_MASK;
    auto weapon_ent = (C_WeaponCSBase*)entitylist->GetClientEntity(weapon_ent_index);
    if (!weapon_ent_index || !weapon_ent)
    {
        printf("bad weapon?\n");
        return;
    }
    // printf("weapon %s %d\n", weapon_ent->GetCSWpnData()->name, weapon_ent->GetCSWpnData()->weaponId);
    int weaponId = weapon_ent->GetCSWpnData()->weaponId;
    if (weaponId == WEAPON_KNIFE || weaponId == WEAPON_KNIFE_GG)
        return;
    bool headshotOnly = true;
    if (weaponId == WEAPON_AWP || weaponId == WEAPON_SCAR20 || weaponId == WEAPON_G3SG1)
        headshotOnly = false;

    Vector pSrc = pos + cameraOffset;

    Vector pEnd;
    memset(&pEnd, 0, sizeof(Vector));

    // account for recoil
    viewangles = viewangles + Get_netvar<QAngle>(pLocalPlayer->GetClientNetworkable(), "m_aimPunchAngle") * 2;

    AngleVectors(&viewangles, &pEnd);
    //printf("anglevectors %f %f %f\n", pEnd.x, pEnd.y, pEnd.z);
    pEnd = pSrc + (pEnd * 8192.f);

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

    bool shouldShoot = false;

    if (trace.m_pEnt)
    {
        IClientNetworkable* net_ent = trace.m_pEnt->GetClientNetworkable();
        //printf("You are aiming at %s\n", net_ent->GetClientClass()->m_pNetworkName);

        bool is_player = !strcmp(net_ent->GetClientClass()->m_pNetworkName, "CCSPlayer");
        bool is_chicken = !strcmp(net_ent->GetClientClass()->m_pNetworkName, "CChicken");

        if (is_player || is_chicken)
        {
            //printf("wow we hit something!!\n");
            //printf("hitbox: %d %d\n", trace.hitbox, trace.hitgroup);
            if (is_player)
            {
                C_CSPlayer* aimed_player = (C_CSPlayer*)trace.m_pEnt;
                // don't friendly fire
                if (pLocalPlayer->m_iTeamNum != aimed_player->m_iTeamNum)
                {
                    // don't shoot dead people
                    if (aimed_player->m_iHealth > 0)
                    {
                        if (headshotOnly)
                            shouldShoot = trace.hitgroup == HITGROUP_PLAYER_HEAD;
                        else
                            shouldShoot = true;
                    }
                }
            }
            else if (is_chicken)
            {
                // always shoot chicken. sorry poor chicken you have to go :'(
                shouldShoot = true;
            }
        }
    }

    if (shouldShoot)
    {
        cmd->buttons |= IN_ATTACK;
        clickTimeDelayTicks = 20 + (rand() % 10);
        clickTimeTicks = clickTimeDelayTicks;
    }
}

void do_Bhop(C_CSPlayer* pLocalPlayer, float flInputSampleTime, CUserCmd* cmd)
{
    static int bhopJumpTicks = 0;

    if (pLocalPlayer->m_fFlags & FL_ONGROUND)
    {
        cmd->buttons |= IN_JUMP;
        bhopJumpTicks = 20 + (rand() % 10);
    }

#if 0
    // simulate a mousewheel bhop
    if (bhopJumpTicks)
    {
        bhopJumpTicks--;
    }
    else
    {
        // cast a ray down from feet to ground
        auto src = Get_netvar<Vector>(pLocalPlayer->GetClientNetworkable(), "m_vecOrigin");
        Vector end = src;
        float tracelen = 32;
        end.z -= tracelen;
        Ray_t ray;
        Ray_Init(&ray, src, end);
        trace_t trace;
        g_CEngineTrace->TraceRay(&ray, MASK_SOLID, (ITraceFilter*)&tracefilter, &trace);
        float distance_to_floor = trace.fraction * tracelen;
        if (distance_to_floor < 24.f && distance_to_floor > 3.f)
        {
            cmd->buttons |= IN_JUMP;
            bhopJumpTicks = 10 + (rand() % 10);
            printf("jumping!\n");
        }
    }
#endif

    // don't jump and unjump so quickly
    if (bhopJumpTicks > 0)
    {
        bhopJumpTicks--;
        cmd->buttons |= IN_JUMP;
    }
}

bool __fastcall Hk_CreateMove(void* thisptr, DWORD edx, float flInputSampleTime, CUserCmd* cmd)
{
    // printf("call to hk_createmove %p %f %p\n", thisptr, flInputSampleTime, cmd);

    //printf("buttons %d\n", cmd->buttons);

    C_CSPlayer* pLocalPlayer = (C_CSPlayer*)thisptr;

    do_Trigger(pLocalPlayer, flInputSampleTime, cmd);

    if (GetAsyncKeyState(VK_SPACE))
    {
        do_Bhop(pLocalPlayer, flInputSampleTime, cmd);
    }

    return real_createmove(thisptr, 0, flInputSampleTime, cmd);
}

// O(N^2) search
void* Find_Pattern(BYTE* BaseAddress, uintptr_t SearchLen, BYTE* pattern, BYTE* mask)
{
    BYTE* end = BaseAddress + SearchLen;
    int patt_len = strlen((char*)mask);
    for (BYTE* p = BaseAddress; p < end; p++)
    {
        for (int i = 0; i < patt_len; i++)
        {
            if (mask[i] == '?' || (pattern[i] == p[i]))
                continue;
            goto bad;
        }
        // found it
        return p;
bad:
        continue;
    }
    return NULL;
}

typedef bool(__fastcall* ApplyMouse_t)(void* ecx, DWORD edx, int nSlot, QAngle& viewangles, CUserCmd* cmd, float mouse_x, float mouse_y);

ApplyMouse_t real_ApplyMouse;

static inline float clamp_angle(float f)
{
    if (f < 0) f += 360.f;
    return f;
}

float m_yaw = 0.022f; // klmao
float weapon_recoil_scale = 2.f; // lmao

void antirecoil(float& mouse_x, float& mouse_y)
{
    static int lastUpdateTick = 0;
    static QAngle lastAimPunch;

    auto tickbase = Get_netvar<unsigned>(pLocalPlayer->GetClientNetworkable(), "m_nTickBase");
    auto aimPunch = Get_netvar<QAngle>(pLocalPlayer->GetClientNetworkable(), "m_aimPunchAngle");

    if (lastUpdateTick != tickbase)
    {
        lastUpdateTick = tickbase;

        //printf("%.04f %.04f %.04f\n", aimPunch.x, aimPunch.y, aimPunch.z);

        QAngle aimPunchDelta = aimPunch - lastAimPunch;

        aimPunchDelta = aimPunchDelta * (-1.f * weapon_recoil_scale / m_yaw);

        mouse_x += -aimPunchDelta.y;
        mouse_y += aimPunchDelta.x;

        lastAimPunch = aimPunch;
    }
}

bool __fastcall Hk_ApplyMouse(void* thisptr, DWORD edx, int nSlot, QAngle& viewangles, CUserCmd* cmd, float mouse_x, float mouse_y)
{
    // printf("call to ApplyMouse with mouse delta <%.03f %.03f>\n", mouse_x, mouse_y);

    if (pLocalPlayer)
    {
        // "hard" antirecoil that applies directly to your mouse
        // antirecoil(mouse_x, mouse_y);

        // aimassist
        if (GetAsyncKeyState(VK_MENU) || IsConsolePlayer)
        {
            auto aimPunch = Get_netvar<QAngle>(pLocalPlayer->GetClientNetworkable(), "m_aimPunchAngle");
            auto myPos = Get_netvar<Vector>(pLocalPlayer->GetClientNetworkable(), "m_vecOrigin");

            if (entitylist)
            {
                float my_yaw = clamp_angle(viewangles.y);
                float smallest_d_yaw = 999.f;
                IClientEntity* best_ent = NULL;

                for (int i = 0; i < 64; i++) // todo: replace hardcoded number of 64
                {
                    IClientEntity* ent = entitylist->GetClientEntity(i);
                    IClientNetworkable* net_ent = entitylist->GetClientNetworkable(i);
                    if (!ent || !net_ent)
                        continue;
                    if (net_ent->IsDormant())
                        continue;
                    if (strcmp(net_ent->GetClientClass()->m_pNetworkName, "CCSPlayer"))
                        continue;
                    C_CSPlayer* other_player = (C_CSPlayer*)ent;
                    if (other_player == pLocalPlayer)
                        continue;
                    if (other_player->m_iHealth == 0)
                        continue;
                    if (other_player->m_iTeamNum == pLocalPlayer->m_iTeamNum) // dont shoot friends
                        continue;
                    if (!vischeck(pLocalPlayer, other_player))
                        continue;

                    Vector theirPos = Get_netvar<Vector>(net_ent, "m_vecOrigin");
                    Vector delta = theirPos - myPos;
                    float angle = clamp_angle(atan2f(delta.y, delta.x) * 180.f / 3.14159265358979);

                    // "soft" antirecoil that nudges you in the right direction
                    float aimPunchYaw = -aimPunch.y * weapon_recoil_scale;
                    angle = clamp_angle(angle + aimPunchYaw);

                    float angle_diff = angle - my_yaw;
                    if (abs(angle_diff) < abs(smallest_d_yaw))
                    {
                        smallest_d_yaw = angle_diff;
                        best_ent = ent;
                    }
                }

                if (best_ent)
                {
                    // printf("%p %f\n", best_ent, smallest_d_yaw);
                    float fov = 15.f; // 15 degrees fov aimassist
                    float aimassist_strength = 0.5f; // ratio from 0 to 1 (1 = snap, 0 = no action)
                    if (abs(smallest_d_yaw) < fov)
                    {
                        int correct_way = copysignf(1.f, smallest_d_yaw);
                        int mouse_way = copysignf(1.f, -mouse_x);
                        float scale = aimassist_strength * abs(smallest_d_yaw) / fov;
                        printf("aimassist %.02fx\n", scale);
                        if (correct_way == mouse_way)
                        {
                            mouse_x *= (1.f + aimassist_strength);
                        }
                        else
                        {
                            mouse_x *= (1.f - aimassist_strength);
                        }
                    }
                }
            }
        }
    }

    return real_ApplyMouse(thisptr, 0, nSlot, viewangles, cmd, mouse_x, mouse_y);
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

    engineclient = (IVEngineClient*)Engine_CreateInterface("VEngineClient014", &returnCode);
    printf("engineclient = %p\n", engineclient);
    if (!engineclient)
    {
        printf("bruhh no engineclient\n");
        return;
    }

    printf("hClient = %p\n", hClient);

    // cinput shit

    IMAGE_DOS_HEADER* dos_hdr = (IMAGE_DOS_HEADER*)hClient;
    IMAGE_NT_HEADERS* pe_hdr = (IMAGE_NT_HEADERS*)((uintptr_t)hClient + dos_hdr->e_lfanew);
    DWORD dwSizeOfImage = pe_hdr->OptionalHeader.SizeOfImage;
    printf("Size of client = %x\n", dwSizeOfImage);
    /*
    B9 ?? ?? ?? ??             mov     ecx, offset g_input
    FF 50 ??                   call    dword ptr [eax+7Ch]
    FF D3                      call    ebx ; ThreadInMainThread
    84 C0                      test    al, al
    74 ??                      jz      short loc_1025373F
    8B 15 ?? ?? ?? ??          mov     edx, dword_14D91234
    83 EA 01                   sub     edx, 1
    78 ??                      js      short loc_1025373F
    8B 0D
    */
    void* pattern_location = Find_Pattern((BYTE*)hClient, dwSizeOfImage, (BYTE*)"\xB9\xFF\xFF\xFF\xFF\xFF\x50\xFF\xFF\xD3\x84\xC0\x74\xFF\x8B\x15\xFF\xFF\xFF\xFF\x83\xEA\x01\x78\xFF\x8B\x0D", (BYTE*)"x????xx?xxxxx?xx????xxxx?xx");
    printf("found pattern at %x\n", pattern_location);
    CInput* g_input = *(CInput**)((uintptr_t)pattern_location + 1);
    printf("g_Input at %p\n", g_input);

    void** cinput_vtbl = *(void***)g_input;
    printf("original cinput vtable at %p\n", cinput_vtbl);
    memcpy(&fake_cinput_vtable, (void*)((uintptr_t)cinput_vtbl - 4), sizeof(fake_vtable_t));

    real_ApplyMouse = (ApplyMouse_t)fake_cinput_vtable.funcptrs[55];
    printf("applymouse at %p\n", real_ApplyMouse);
    fake_cinput_vtable.funcptrs[VTABLE_INDEX_ApplyMouse] = Hk_ApplyMouse;

    // install BIG vtable hook !!!!!!
    *(void***)g_input = fake_cinput_vtable.funcptrs;

    void** orig_player_vtbl = 0;
    while (!GetAsyncKeyState(VK_F3))
    {
        printf("localplayer index = %d\n", engineclient->GetLocalPlayer());

        // wait until we have a localplayer. or we changed localplayers.
        C_CSPlayer* currentPlayer;
        // COMMA OPERATOR GOOD.
        while ((currentPlayer = (C_CSPlayer*)entitylist->GetClientEntity(engineclient->GetLocalPlayer())), !currentPlayer || pLocalPlayer == currentPlayer)
        {
            Sleep(100);
            printf(".");
            if (GetAsyncKeyState(VK_F3))
                goto exitMyGameMod;
        }
        pLocalPlayer = currentPlayer;

        // ok now we got a new localplayer. we better hook dat bad b0i.

        printf("localplayer = %p\n", pLocalPlayer);
        if (!pLocalPlayer)
        {
            printf("bruhh no localplayer\n");
            return;
        }

        orig_player_vtbl = *(void***)pLocalPlayer;
        printf("original vtable at %p\n", orig_player_vtbl);

        memcpy(&fake_player_vtable, (void*)((uintptr_t)orig_player_vtbl - 4), sizeof(fake_vtable_t));

        real_createmove = (CreateMove_t)fake_player_vtable.funcptrs[288];
        printf("createmove at %p\n", real_createmove);
        fake_player_vtable.funcptrs[VTABLE_INDEX_CreateMove] = Hk_CreateMove;

        // install BIG vtable hook !!!!!!
        *(void***)pLocalPlayer = fake_player_vtable.funcptrs;
        printf("vmt hooks installed\n");
    }
exitMyGameMod:;
    // uninstall the createmove hook if we is uninjecting. and if that shit still exists
    printf("pointers: %p %p\n", pLocalPlayer, orig_player_vtbl);
    if (pLocalPlayer && orig_player_vtbl)
    {
        // only restore the vtable pointer if that memory holds our hooked pointer value :) safe p100
        PVOID oldValue = InterlockedCompareExchangePointer((volatile PVOID*)pLocalPlayer, (PVOID)orig_player_vtbl, fake_player_vtable.funcptrs);
        if (oldValue != fake_player_vtable.funcptrs)
        {
            printf("wow, something weird happened to our vtable hook. the value was actually %p\n", oldValue);
        }
    }

    pause();
    
    // uninstall vtable hooks
    
    *(void***)g_input = cinput_vtbl;
    
    printf("done\n");
    return;
}

// zero my own image headers. i dont think this actually does anything for security though lol
void ZeroImageHeaders()
{
    auto dos_hdr = (PIMAGE_DOS_HEADER)hMod;
    auto nt_hdr = (PIMAGE_NT_HEADERS)((uintptr_t)hMod + (uintptr_t)dos_hdr->e_lfanew);
    memset(hMod, 0, nt_hdr->OptionalHeader.SizeOfHeaders);
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

    ZeroImageHeaders();

    DoMyShitttt();

    puts("bye");

    // this doesn't work if we're manually mapped, obviously
    // FreeLibraryAndExitThread(hMod, 0);

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

extern "C" __declspec(dllexport) void CALLBACK ManualMapEntrypoint(HMODULE hModule)
{
    // call the REAL entrypoint (NOT DllMain), we need to initialize the CRT and shit properly.
    // else freopen will crash because the stdio FILE doesn't have a valid critical section (it's null)
    // so you'll get a fault in RtlEnterCriticalSection
    auto dos_hdr = (PIMAGE_DOS_HEADER)hModule;
    auto nt_hdr = (PIMAGE_NT_HEADERS) ((uintptr_t)hModule + (uintptr_t)dos_hdr->e_lfanew);
    auto entrypoint = (uintptr_t)hModule + (uintptr_t)nt_hdr->OptionalHeader.AddressOfEntryPoint;
    auto real_dllmain = (BOOL(APIENTRY*)(HMODULE, DWORD, LPVOID)) entrypoint;
    real_dllmain(hModule, DLL_PROCESS_ATTACH, 0);
}
