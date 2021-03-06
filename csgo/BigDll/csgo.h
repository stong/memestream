#pragma once
#include <stdint.h>

// base types

struct Vector
{
    float x, y, z;

    Vector() : x(0), y(0), z(0) {}
    Vector(float x, float y, float z) : x(x), y(y), z(z) {}

    inline Vector& operator=(const Vector& other)
    {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        return *this;
    }

    inline Vector operator+(const Vector& other)
    {
        return Vector(this->x + other.x, this->y + other.y, this->z + other.z);
    }

    inline Vector operator-(const Vector& other)
    {
        return Vector(this->x - other.x, this->y - other.y, this->z - other.z);
    }

    inline Vector operator*(float scalar)
    {
        return Vector(this->x * scalar, this->y * scalar, this->z * scalar);
    }
};

struct VectorAligned
{
    float x, y, z, w;
};

typedef Vector QAngle;

// forward declarations

class ClientClass;
class IClientNetworkable;

// entity inheritance tree bullshit

class IHandleEntity;
class CBaseEntity
{
public:
    virtual void sub_103ABC20() = 0;
    virtual void sub_101DC6A0() = 0;
    virtual void sub_101DC6C0() = 0;
    virtual void sub_101A3A90() = 0;
    virtual IClientNetworkable* GetClientNetworkable() = 0;
};

class CBaseHandle;
class IClientUnknown;
class IClientEntity;

class IClientNetworkable
{
public:
    // Gets at the containing class...
    virtual IClientUnknown* GetIClientUnknown() = 0;

    // Called by the engine when the server deletes the entity.
    virtual void			Release() = 0;

    // Supplied automatically by the IMPLEMENT_CLIENTCLASS macros.
    virtual ClientClass* GetClientClass() = 0;

    virtual void			pad0() = 0;
    virtual void			pad1() = 0;
    virtual void			pad2() = 0;
    virtual void			pad3() = 0;
    virtual void			pad4() = 0;
    virtual void			pad5() = 0;

    // Objects become dormant on the client if they leave the PVS on the server.
    virtual bool			IsDormant(void) const = 0;

    // Ent Index is the server handle used to reference this entity.
    // If the index is < 0, that indicates the entity is not known to the server
    virtual int				entindex(void) const = 0;

    virtual void			pad6() = 0;

    // Get the base pointer to the networked data that GetClientClass->m_pRecvTable starts at.
    // (This is usually just the "this" pointer).
    virtual void* GetDataTableBasePtr() = 0;

    // Tells the entity that it's about to be destroyed due to the client receiving
    // an uncompressed update that's caused it to destroy all entities & recreate them.
    virtual void			SetDestroyedOnRecreateEntities(void) = 0;
};

class C_CSPlayer
{
public:
    // vtable at 0x0
    char pad[0xf0]; // 0x04
    int m_iTeamNum; // 0xf4
    int m_iPendingTeamNum; // 0xf8
    int m_nNextThinkTick; // 0xfc
    int m_iHealth; // 0x100
    int m_fFlags; // 0x104
    Vector m_vecViewOffset; // 0x108

    virtual void sub_103ABC20() = 0;
    virtual void sub_101DC6A0() = 0;
    virtual void sub_101DC6C0() = 0;
    virtual void sub_101A3A90() = 0;
    virtual IClientNetworkable* GetClientNetworkable() = 0;
    virtual void sub_101A3A70() = 0;
    virtual void sub_100BCBF0() = 0;
    virtual void sub_100BCBF0_1() = 0;
    virtual void sub_101A3A60() = 0;
    virtual void sub_101DC2C0() = 0;
    virtual void sub_101DD680() = 0;
    virtual void sub_101DD690() = 0;
    virtual void sub_101C9500() = 0;
    virtual void sub_101E7EF0() = 0;
    virtual void sub_101E3A80() = 0;
    virtual void sub_101E2E50() = 0;
    virtual void sub_1019B680() = 0;
    virtual void sub_103A4F90() = 0;
    virtual void sub_101C4000() = 0;
    virtual void sub_101A6010() = 0;
    virtual void sub_10193F10() = 0;
    virtual void sub_100B6650() = 0;
    virtual void sub_101A69D0() = 0;
    virtual void sub_1019B680_2() = 0;
    virtual void sub_101A6B90() = 0;
    virtual void sub_101A6BA0() = 0;
    virtual void sub_101A6C60() = 0;
    virtual void sub_101AC740() = 0;
    virtual void sub_101A6EE0() = 0;
    virtual void sub_1019D080() = 0;
    virtual void sub_101ACA70() = 0;
    virtual void nullsub_28() = 0;
    virtual void sub_103B1000() = 0;
    virtual void nullsub_1() = 0;
    virtual void nullsub_2() = 0;
    virtual void nullsub_3() = 0;
    virtual void nullsub_4() = 0;
    virtual void sub_101A4D80() = 0;
    virtual void sub_101A4D30() = 0;
    virtual void sub_101A4CD0() = 0;
    virtual void sub_101A47B0() = 0;
    virtual void sub_101A4DF0() = 0;
    virtual void nullsub_5() = 0;
    virtual void sub_101DC2D0() = 0;
    virtual void sub_100BCBF0_3() = 0;
    virtual void sub_100BCBF0_2() = 0;
    virtual void sub_100B7530() = 0;
    virtual void sub_101A3720() = 0;
    virtual void nullsub_6() = 0;
    virtual void sub_101A3A30() = 0;
    virtual void sub_101DD730() = 0;
    virtual void sub_101CE480() = 0;
    virtual void sub_101CE510() = 0;
    virtual void sub_101A36B0() = 0;
    virtual void sub_101DE6A0() = 0;
    virtual void sub_101E0960() = 0;
    virtual void sub_103B1200() = 0;
    virtual void sub_101DEDD0() = 0;
    virtual void sub_101DEDE0() = 0;
    virtual void sub_10193F10_2() = 0;
    virtual void sub_101E2ED0() = 0;
    virtual void sub_101FE6C0() = 0;
    virtual void sub_1019B680_3() = 0;
    virtual void sub_101E2F20() = 0;
    virtual void sub_101E2F40() = 0;
    virtual void sub_10193F10_3() = 0;
    virtual void sub_101A5A10() = 0;
    virtual void sub_101CD1B0() = 0;
    virtual void sub_101ACF10() = 0;
    virtual void sub_101C5960() = 0;
    virtual void sub_101DC940() = 0;
    virtual void sub_101DC9C0() = 0;
    virtual void sub_101E2DD0() = 0;
    virtual void sub_101E2E10() = 0;
    virtual void sub_101E1150() = 0;
    virtual void sub_101DD6B0() = 0;
    virtual void sub_101A3A20() = 0;
    virtual void sub_101A3A10() = 0;
    virtual void sub_101A4530() = 0;
    virtual void nullsub_7() = 0;
    virtual void sub_101DCB40() = 0;
    virtual void sub_101A3A00() = 0;
    virtual void sub_101A39F0() = 0;
    virtual void sub_103BA340() = 0;
    virtual void sub_101C8BC0() = 0;
    virtual void sub_101C8470() = 0;
    virtual void sub_101E0590() = 0;
    virtual void sub_101E05A0() = 0;
    virtual void sub_101E05B0() = 0;
    virtual void sub_101E1E00() = 0;
    virtual void sub_101E05C0() = 0;
    virtual void sub_101E05D0() = 0;
    virtual void sub_101E0610() = 0;
    virtual void sub_10193F10_4() = 0;
    virtual void sub_101A39E0() = 0;
    virtual void sub_101DDE10() = 0;
    virtual void sub_101DC3B0() = 0;
    virtual void sub_103BC730() = 0;
    virtual void sub_1019B680_5() = 0;
    virtual void sub_101CF890() = 0;
    virtual void sub_101DE020() = 0;
    virtual void sub_101DD760() = 0;
    virtual void sub_10200F70() = 0;
    virtual void sub_101D0AF0() = 0;
    virtual void sub_101C7B60() = 0;
    virtual void sub_101FF560() = 0;
    virtual void sub_101CCB20() = 0;
    virtual void sub_101DF030() = 0;
    virtual void sub_101E7D50() = 0;
    virtual void nullsub_8() = 0;
    virtual void nullsub_9() = 0;
    virtual void sub_101CCB50() = 0;
    virtual void sub_103B8750() = 0;
    virtual void sub_101DF610() = 0;
    virtual void sub_10193F10_6() = 0;
    virtual void sub_101CF700() = 0;
    virtual void sub_101DDC00() = 0;
    virtual void sub_101DFBC0() = 0;
    virtual void nullsub_10() = 0;
    virtual void sub_101E0660() = 0;
    virtual void nullsub_11() = 0;
    virtual void sub_102047A0() = 0;
    virtual void sub_101FE7A0() = 0;
    virtual void sub_100B6650_7() = 0;
    virtual void sub_101E03D0() = 0;
    virtual void sub_10193F10_9() = 0;
    virtual void sub_103B11A0() = 0;
    virtual void sub_101E2390() = 0;
    virtual void sub_100BCBF0_20() = 0;
    virtual void sub_101E1920() = 0;
    virtual void sub_101C5AE0() = 0;
    virtual void nullsub_12() = 0;
    virtual void sub_100B6650_8() = 0;
    virtual void sub_10193F10_10() = 0;
    virtual void sub_101A5220() = 0;
    virtual void sub_101A5090() = 0;
    virtual void sub_103FCDE0() = 0;
    virtual void sub_102014C0() = 0;
    virtual void sub_101A39C0() = 0;
    virtual void sub_10202DF0() = 0;
    virtual void sub_1019B680_21() = 0;
    virtual void nullsub_13() = 0;
    virtual void sub_101E1BF0() = 0;
    virtual void sub_1019B680_22() = 0;
    virtual void sub_101E2AB0() = 0;
    virtual void sub_10193F10_11() = 0;
    virtual void sub_101A3870() = 0;
    virtual void sub_103105F0() = 0;
    virtual void nullsub_14() = 0;
    virtual void sub_1030FFC0() = 0;
    virtual void nullsub_15() = 0;
    virtual void sub_103FD630() = 0;
    virtual void sub_10311F70() = 0;
    virtual void sub_10311E00() = 0;
    virtual void sub_102014F0() = 0;
    virtual void sub_101A3CB0() = 0;
    virtual void sub_10193F10_12() = 0;
    virtual void sub_100B6650_23() = 0;
    virtual void sub_10193F10_13() = 0;
    virtual void sub_100B6650_24() = 0;
    virtual void sub_100BCBF0_25() = 0;
    virtual void sub_10193F10_14() = 0;
    virtual void sub_10193F10_15() = 0;
    virtual void sub_10193F10_16() = 0;
    virtual void sub_10193F10_17() = 0;
    virtual void sub_10193F10_18() = 0;
    virtual void sub_1019B680_26() = 0;
    virtual void sub_10193F10_19() = 0;
    virtual void sub_101A9500() = 0;
    virtual void sub_103BA120() = 0;
    virtual void sub_101A94F0() = 0;
    virtual void sub_101A44D0() = 0;
    virtual void sub_101A5060() = 0;
    virtual void sub_103B8380() = 0;
    virtual void sub_101A4520() = 0;
    virtual void sub_101A44F0() = 0;
    virtual void sub_101A3990() = 0;
    virtual void sub_101DFB80() = 0;
    virtual void sub_103B87E0() = 0;
    virtual void sub_101E1280() = 0;
    virtual void sub_101E1DF0() = 0;
    virtual void sub_101DD270() = 0;
    virtual void sub_101DD2A0() = 0;
    virtual void sub_100B6650_27() = 0;
    virtual void sub_101A3980() = 0;
    virtual void nullsub_81() = 0;
    virtual void sub_101C6A90() = 0;
    virtual void sub_101CF810() = 0;
    virtual void sub_101C6AC0() = 0;
    virtual void sub_103BB9A0() = 0;
    virtual void sub_101C7580() = 0;
    virtual void sub_101C8F60() = 0;
    virtual void sub_101C9010() = 0;
    virtual void sub_101CB070() = 0;
    virtual void sub_101CAEC0() = 0;
    virtual void sub_101AA6A0() = 0;
    virtual void sub_101C9420() = 0;
    virtual void sub_103BB1C0() = 0;
    virtual void sub_101D65A0() = 0;
    virtual void sub_103BC2F0() = 0;
    virtual void sub_101CC510() = 0;
    virtual void idk() = 0;
    virtual void sub_101CB840() = 0;
    virtual void sub_101CBAB0() = 0;
    virtual void sub_101A3E20() = 0;
    virtual void sub_101E7EC0() = 0;
    virtual void sub_103BA7A0() = 0;
    virtual void sub_101D0CB0() = 0;
    virtual void sub_101D0DA0() = 0;
    virtual void nullsub_16() = 0;
    virtual void sub_101D0B80() = 0;
    virtual void sub_103AE050() = 0;
    virtual void sub_10193F10_28() = 0;
    virtual void sub_1019B680_33() = 0;
    virtual void sub_101CD8C0() = 0;
    virtual void sub_101C7930() = 0;
    virtual void sub_101C7AA0() = 0;
    virtual void sub_10201EE0() = 0;
    virtual void sub_103A6480() = 0;
    virtual void sub_101CE8C0() = 0;
    virtual void sub_101CED70() = 0;
    virtual void sub_101CE690() = 0;
    virtual void sub_103FE9A0() = 0;
    virtual void sub_103B9D70() = 0;
    virtual void sub_101A3720_34() = 0;
    virtual void nullsub_17() = 0;
    virtual void sub_101A3910() = 0;
    virtual void nullsub_18() = 0;
    virtual void sub_101AC6F0() = 0;
    virtual void sub_103B9F60() = 0;
    virtual void sub_103A4610() = 0;
    virtual void sub_103A4630() = 0;
    virtual void sub_101D0B70() = 0;
    virtual void sub_10193F10_29() = 0;
    virtual void sub_10193F10_30() = 0;
    virtual void sub_10193F10_31() = 0;
    virtual void sub_101A38A0() = 0;
    virtual void sub_101A3FF0() = 0;
    virtual void nullsub_19() = 0;
    virtual void sub_10193F10_32() = 0;
    virtual void sub_101C86C0() = 0;
    virtual void sub_101939C0() = 0;
    virtual void sub_100B6650_35() = 0;
    virtual void sub_101A3890() = 0;
    virtual void sub_101A3880() = 0;
    virtual void sub_101CA860() = 0;
    virtual void sub_103FE830() = 0;
    virtual void sub_103BA8D0() = 0;
    virtual void sub_103BA900() = 0;
    virtual void sub_103BA950() = 0;
    virtual void sub_101E7C70() = 0;
    virtual void sub_101E9F00() = 0;
    virtual void sub_101EA8F0() = 0;
    virtual void sub_101E9DF0() = 0;
    virtual void sub_101DDE00() = 0;
    virtual void sub_103BBD20() = 0;
    virtual void sub_1019D370() = 0;
    virtual void sub_1019D2D0() = 0;
    virtual void sub_1019D790() = 0;
    virtual void sub_1019D4B0() = 0;
    virtual void sub_1019D860() = 0;
    virtual void sub_1019D800() = 0;
    virtual void sub_1019CDE0() = 0;
    virtual void sub_1019CEF0() = 0;
    virtual void sub_1019CF90() = 0;
    virtual void sub_101AA760() = 0;
    virtual void sub_103B9B50() = 0;
    virtual void sub_10201380() = 0;
    virtual void sub_1019CD30() = 0;
    virtual void sub_1019D910() = 0;
    virtual void sub_1019DA60() = 0;
    virtual void sub_101AC780() = 0;
    virtual void sub_10202F30() = 0;
    virtual void sub_103B7FA0() = 0;
    virtual void sub_101FE7A0_36() = 0;
    virtual void sub_103F9A80() = 0;
    virtual void sub_103B3A50() = 0;
    virtual void sub_101ABE80() = 0;
    virtual void nullsub_20() = 0;
    virtual void sub_101AC290() = 0;
    virtual void sub_101AC550() = 0;
    virtual void sub_101AC340() = 0;
    virtual void sub_101ACBE0() = 0;
    virtual void sub_100BCBF0_37() = 0;
    virtual void sub_103F9460() = 0;
    virtual void sub_101AA6A0_38() = 0;
    virtual void nullsub_21() = 0;
    virtual void sub_10201410() = 0;
    virtual void CreateMove(float flInputSampleTime, void* cmd) = 0;
    virtual void sub_101ACA20() = 0;
    virtual void nullsub_22() = 0;
    virtual void sub_103FEA20() = 0;
    virtual void sub_101939D0() = 0;
    virtual void sub_103BCE60() = 0;
    virtual void sub_101FCF80() = 0;
    virtual void sub_103BDB70() = 0;
    virtual void sub_103A63E0() = 0;
    virtual void sub_103BBAC0() = 0;
    virtual void sub_103BBB20() = 0;
    virtual void sub_1019B680_39() = 0;
    virtual void sub_101A1CF0() = 0;
    virtual void sub_101A1CF0_40() = 0;
    virtual void sub_101A1CF0_41() = 0;
    virtual void sub_100B6650_42() = 0;
    virtual void sub_10298890() = 0;
    virtual void sub_10204780() = 0;
    virtual void sub_10200C60() = 0;
    virtual void sub_10200C70() = 0;
    virtual void sub_10193F10_49() = 0;
    virtual void sub_101A3650() = 0;
    virtual void sub_101A36B0_50() = 0;
    virtual void sub_103B38E0() = 0;
    virtual void sub_103BBAB0() = 0;
    virtual void sub_101FE670() = 0;
    virtual void sub_100BCBF0_47() = 0;
    virtual void sub_103A4710() = 0;
    virtual void sub_10193F10_46() = 0;
    virtual void sub_10200E10() = 0;
    virtual void sub_10200EB0() = 0;
    virtual void sub_101A9140() = 0;
    virtual void sub_101A9220() = 0;
    virtual void sub_101AA980() = 0;
    virtual void sub_101AA8A0() = 0;
    virtual void sub_101AA6B0() = 0;
    virtual void sub_101E1DF0_52() = 0;
    virtual void sub_101AAB30() = 0;
    virtual void sub_101AA6E0() = 0;
    virtual void sub_10204750() = 0;
    virtual void sub_101AABE0() = 0;
    virtual void sub_101AAB50() = 0;
    virtual void sub_10200DD0() = 0;
    virtual void sub_103BCB90() = 0;
    virtual void sub_101AC9B0() = 0;
    virtual void sub_10193F10_45() = 0;
    virtual void sub_101AB950() = 0;
    virtual void sub_101ADFA0() = 0;
    virtual void nullsub_23() = 0;
    virtual void sub_101A9750() = 0;
    virtual void sub_101A97D0() = 0;
    virtual void sub_101A9850() = 0;
    virtual void sub_102016B0() = 0;
    virtual void sub_102016E0() = 0;
    virtual void sub_101FD4A0() = 0;
    virtual void nullsub_24() = 0;
    virtual void sub_10201610() = 0;
    virtual void sub_103FD480() = 0;
    virtual void sub_103B1390() = 0;
    virtual void sub_103FD190() = 0;
    virtual void sub_101A9F70() = 0;
    virtual void sub_101AA400() = 0;
    virtual void sub_103FF410() = 0;
    virtual void sub_101AA540() = 0;
    virtual void sub_103B1410() = 0;
    virtual void nullsub_25() = 0;
    virtual void sub_1019B680_53() = 0;
    virtual void sub_102020F0() = 0;
    virtual void sub_10202140() = 0;
    virtual void sub_10202270() = 0;
    virtual void sub_103A46A0() = 0;
    virtual void sub_103B26F0() = 0;
    virtual void sub_10204740() = 0;
    virtual void sub_10204730() = 0;
    virtual void sub_103BBAA0() = 0;
    virtual void nullsub_26() = 0;
    virtual void nullsub_27() = 0;
    virtual void sub_101ABEC0() = 0;
    virtual void sub_103BCF00() = 0;
    virtual void sub_101FF660() = 0;
    virtual void sub_103BEE60() = 0;
    virtual void sub_103BF5D0() = 0;
    virtual void sub_101FFCF0() = 0;
    virtual void sub_103BDE30() = 0;
    virtual void sub_101FD2C0() = 0;
    virtual void sub_101FD2F0() = 0;
    virtual void sub_10204720() = 0;
    virtual void sub_10204710() = 0;
    virtual void sub_10204700() = 0;
    virtual void sub_103BDB50() = 0;
    virtual void sub_103A4720() = 0;
    virtual void sub_103A4660() = 0;
    virtual void sub_103A4680() = 0;
    virtual void nullsub_29() = 0;
    virtual void sub_103A4640() = 0;
    virtual void sub_103A4650() = 0;
    virtual void sub_103A4670() = 0;
    virtual void sub_103A4690() = 0;
    virtual void sub_103A46B0() = 0;
    virtual void sub_103B2120() = 0;
    virtual void sub_103B2510() = 0;
    virtual void sub_103B2660() = 0;
    virtual void sub_103B2530() = 0;
    virtual void sub_103A46C0() = 0;
    virtual void sub_103B2230() = 0;
    virtual void sub_103B1F40() = 0;
    virtual void sub_103A7CD0() = 0;
    virtual void sub_103A7D20() = 0;
};

// createinterface shit

typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);

// raytrace shit

struct matrix3x4_t;

struct Ray_t
{
    VectorAligned  m_Start;	// starting point, centered within the extents
    VectorAligned  m_Delta;	// direction + length of the ray
    VectorAligned  m_StartOffset;	// Add this to m_Start to get the actual ray start
    VectorAligned  m_Extents;	// Describes an axis aligned box extruded along a ray
    const matrix3x4_t* m_pWorldAxisTransform;
    bool	m_IsRay;	// are the extents zero?
    bool	m_IsSwept;	// is delta != 0?
};

inline void Ray_Init(Ray_t* ray, Vector const& start, Vector const& end)
{
    //Assert(&end);

    // VectorSubtract(end, start, ray->m_Delta);
    ray->m_Delta.x = end.x - start.x;
    ray->m_Delta.y = end.y - start.y;
    ray->m_Delta.z = end.z - start.z;
    ray->m_Delta.w = 0;

    // ray->m_IsSwept = (ray->m_Delta.LengthSqr() != 0);
    ray->m_IsSwept = true;

    // VectorClear(ray->m_Extents);
    memset(&ray->m_Extents, 0, sizeof(VectorAligned));

    ray->m_pWorldAxisTransform = NULL;
    ray->m_IsRay = true;

    // Offset m_Start to be in the center of the box...
    // VectorClear(ray->m_StartOffset);
    memset(&ray->m_StartOffset, 0, sizeof(VectorAligned));

    // VectorCopy(start, ray->m_Start);
    memcpy(&ray->m_Start, &start, sizeof(Vector));
    ray->m_Start.w = 0;
}

enum TraceType_t
{
    TRACE_EVERYTHING = 0,
    TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
    TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
    TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

class ITraceFilter
{
public:
    virtual bool ShouldHitEntity(IHandleEntity* pEntity, int contentsMask) = 0;
    virtual TraceType_t	GetTraceType() const = 0;
};

struct cplane_t
{
    Vector	normal;
    float	dist;
    byte	type;			// for fast side tests
    byte	signbits;		// signx + (signy<<1) + (signz<<1)
    byte	pad[2];
};

struct csurface_t
{
    const char* name;
    short		surfaceProps;
    unsigned short	flags;		// BUGBUG: These are declared per surface, not per material, but this database is per-material now
};

struct trace_t
{
    Vector			startpos;				// start position
    Vector			endpos;					// final position
    cplane_t		plane;					// surface normal at impact

    float			fraction;				// time completed, 1.0 = didn't hit anything

    int				contents;				// contents on other side of surface hit
    unsigned short	dispFlags;				// displacement flags for marking surfaces with data

    bool			allsolid;				// if true, plane is not valid
    bool			startsolid;				// if true, the initial point was in a solid area


    // cgametrace

    float			fractionleftsolid;	// time we left a solid, only valid if we started in solid
    csurface_t		surface;			// surface hit (impact surface)

    int				hitgroup;			// 0 == generic, non-zero is specific body part

    short			physicsbone;		// physics bone hit by trace in studio
    unsigned short	worldSurfaceIndex;	// Index of the msurface2_t, if applicable

    CBaseEntity* m_pEnt;
    int			hitbox;					// box hit by trace in studio


    int pad[100]; // LMAO
};

class CEngineTrace
{
public:
    virtual int		pad0() = 0;
    virtual int		pad1() = 0;
    virtual int		pad2() = 0;
    virtual int		pad3() = 0;
    virtual int		pad4() = 0;

    // A version that simply accepts a ray (can work as a traceline or tracehull)
    virtual void	TraceRay(const Ray_t* ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace) = 0;
};

#define DEG2RAD(x) (x*3.14159265358979f/180.f)

void AngleVectors(const Vector* angles, Vector* forward)
{
    float	sp, sy, cp, cy;

    sy = sin(DEG2RAD(angles->y));
    cy = cos(DEG2RAD(angles->y));

    sp = sin(DEG2RAD(angles->x));
    cp = cos(DEG2RAD(angles->x));

    forward->x = cp * cy;
    forward->y = cp * sy;
    forward->z = -sp;
}

#define	CONTENTS_EMPTY			0		// No contents

#define	CONTENTS_SOLID			0x1		// an eye is never valid in a solid
#define	CONTENTS_WINDOW			0x2		// translucent, but not watery (glass)
#define	CONTENTS_AUX			0x4
#define	CONTENTS_GRATE			0x8		// alpha-tested "grate" textures.  Bullets/sight pass through, but solids don't
#define	CONTENTS_SLIME			0x10
#define	CONTENTS_WATER			0x20
#define	CONTENTS_BLOCKLOS		0x40	// block AI line of sight
#define CONTENTS_OPAQUE			0x80	// things that cannot be seen through (may be non-solid though)
#define	LAST_VISIBLE_CONTENTS	CONTENTS_OPAQUE

#define ALL_VISIBLE_CONTENTS (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))

#define CONTENTS_TESTFOGVOLUME	0x100
#define CONTENTS_UNUSED			0x200	

// unused 
// NOTE: If it's visible, grab from the top + update LAST_VISIBLE_CONTENTS
// if not visible, then grab from the bottom.
// CONTENTS_OPAQUE + SURF_NODRAW count as CONTENTS_OPAQUE (shadow-casting toolsblocklight textures)
#define CONTENTS_BLOCKLIGHT		0x400

#define CONTENTS_TEAM1			0x800	// per team contents used to differentiate collisions 
#define CONTENTS_TEAM2			0x1000	// between players and objects on different teams

// ignore CONTENTS_OPAQUE on surfaces that have SURF_NODRAW
#define CONTENTS_IGNORE_NODRAW_OPAQUE	0x2000

// hits entities which are MOVETYPE_PUSH (doors, plats, etc.)
#define CONTENTS_MOVEABLE		0x4000

// remaining contents are non-visible, and don't eat brushes
#define	CONTENTS_AREAPORTAL		0x8000

#define	CONTENTS_PLAYERCLIP		0x10000
#define	CONTENTS_MONSTERCLIP	0x20000

#define	CONTENTS_BRUSH_PAINT	0x40000
#define	CONTENTS_GRENADECLIP	0x80000
#define	CONTENTS_UNUSED2		0x100000
#define	CONTENTS_UNUSED3		0x200000
#define	CONTENTS_UNUSED4		0x400000
#define	CONTENTS_UNUSED5		0x800000

#define	CONTENTS_ORIGIN			0x1000000	// removed before bsping an entity

#define	CONTENTS_MONSTER		0x2000000	// should never be on a brush, only in game
#define	CONTENTS_DEBRIS			0x4000000
#define	CONTENTS_DETAIL			0x8000000	// brushes to be added after vis leafs
#define	CONTENTS_TRANSLUCENT	0x10000000	// auto set if any surface has trans
#define	CONTENTS_LADDER			0x20000000
#define CONTENTS_HITBOX			0x40000000	// use accurate hitboxes on trace

// NOTE: These are stored in a short in the engine now.  Don't use more than 16 bits
#define	SURF_LIGHT		0x0001		// value will hold the light strength
#define	SURF_SKY2D		0x0002		// don't draw, indicates we should skylight + draw 2d sky but not draw the 3D skybox
#define	SURF_SKY		0x0004		// don't draw, but add to skybox
#define	SURF_WARP		0x0008		// turbulent water warp
#define	SURF_TRANS		0x0010
#define SURF_NOPORTAL	0x0020	// the surface can not have a portal placed on it
#define	SURF_TRIGGER	0x0040	// FIXME: This is an xbox hack to work around elimination of trigger surfaces, which breaks occluders
#define	SURF_NODRAW		0x0080	// don't bother referencing the texture

#define	SURF_HINT		0x0100	// make a primary bsp splitter

#define	SURF_SKIP		0x0200	// completely ignore, allowing non-closed brushes
#define SURF_NOLIGHT	0x0400	// Don't calculate light
#define SURF_BUMPLIGHT	0x0800	// calculate three lightmaps for the surface for bumpmapping
#define SURF_NOSHADOWS	0x1000	// Don't receive shadows
#define SURF_NODECALS	0x2000	// Don't receive decals
#define SURF_NOPAINT	SURF_NODECALS	// the surface can not have paint placed on it
#define SURF_NOCHOP		0x4000	// Don't subdivide patches on this surface 
#define SURF_HITBOX		0x8000	// surface is part of a hitbox

#define	MASK_ALL					(0xFFFFFFFF)
// everything that is normally solid
#define	MASK_SOLID					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// everything that blocks player movement
#define	MASK_PLAYERSOLID			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// blocks npc movement
#define	MASK_NPCSOLID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// blocks fluid movement
#define	MASK_NPCFLUID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER)
// water physics in these contents
#define	MASK_WATER					(CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)
// everything that blocks lighting
#define	MASK_OPAQUE					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)
// everything that blocks lighting, but with monsters added.
#define MASK_OPAQUE_AND_NPCS		(MASK_OPAQUE|CONTENTS_MONSTER)
// everything that blocks line of sight for AI
#define MASK_BLOCKLOS				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS)
// everything that blocks line of sight for AI plus NPCs
#define MASK_BLOCKLOS_AND_NPCS		(MASK_BLOCKLOS|CONTENTS_MONSTER)
// everything that blocks line of sight for players
#define	MASK_VISIBLE					(MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)
// everything that blocks line of sight for players, but with monsters added.
#define MASK_VISIBLE_AND_NPCS		(MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)
// bullets see these as solid
#define	MASK_SHOT					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
// for finding floor height
#define MASK_FLOORTRACE				(CONTENTS_SOLID|CONTENTS_MOVEABLE|                 CONTENTS_WINDOW|CONTENTS_DEBRIS)
// for csgo weapon clipping
#define MASK_WEAPONCLIPPING			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS)
// bullets see these as solid, except monsters (world+brush only)
#define MASK_SHOT_BRUSHONLY			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)
// non-raycasted weapons see this as solid (includes grates)
#define MASK_SHOT_HULL				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)
// hits solids (not grates) and passes through everything else
#define MASK_SHOT_PORTAL			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER)
// everything normally solid, except monsters (world+brush only)
#define MASK_SOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE)
// everything normally solid for player movement, except monsters (world+brush only)
#define MASK_PLAYERSOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE)
// everything normally solid for npc movement, except monsters (world+brush only)
#define MASK_NPCSOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
// just the world, used for route rebuilding
#define MASK_NPCWORLDSTATIC			(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
// just the world, used for route rebuilding
#define MASK_NPCWORLDSTATIC_FLUID	(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP)
// These are things that can split areaportals
#define MASK_SPLITAREAPORTAL		(CONTENTS_WATER|CONTENTS_SLIME)

// everything that blocks corpse movement
// UNDONE: Not used yet / may be deleted
#define	MASK_DEADSOLID				(CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_GRATE)

// createmove shit

struct CUserCmd
{
    void* vtable;

    // For matching server and client commands for debugging
    int		command_number;

    // the tick the client created this command
    int		tick_count;

    // Player instantaneous view angles.
    QAngle	viewangles;
    Vector	aimdirection;	// For pointing devices. 
    // Intended velocities
    //	forward velocity.
    float	forwardmove;
    //  sideways velocity.
    float	sidemove;
    //  upward velocity.
    float	upmove;
    // Attack button states
    int		buttons;
    // Impulse command issued.
    byte    impulse;
    // Current weapon id
    int		weaponselect;
    int		weaponsubtype;

    int		random_seed;	// For shared random functions

#ifndef CLIENT_DLL
    //int		server_random_seed; // Only the server populates this seed
#endif

    short	mousedx;		// mouse accum in x from create move
    short	mousedy;		// mouse accum in y from create move

    // Client only, tracks whether we've predicted this command at least once
    bool	hasbeenpredicted;
};

// entity list shit

struct EntityCacheInfo_t
{
    // Cached off because GetClientNetworkable is called a *lot*
    IClientNetworkable* m_pNetworkable;
    unsigned short m_BaseEntitiesIndex;	// Index into m_BaseEntities (or m_BaseEntities.InvalidIndex() if none).
    unsigned short m_bDormant;	// cached dormant state - this is only a bit
};

class IClientEntityList
{
public:
    // Get IClientNetworkable interface for specified entity
    virtual IClientNetworkable* GetClientNetworkable(int entnum) = 0;
    virtual IClientNetworkable* GetClientNetworkableFromHandle(CBaseHandle hEnt) = 0;
    virtual IClientUnknown* GetClientUnknownFromHandle(CBaseHandle hEnt) = 0;

    // NOTE: This function is only a convenience wrapper.
    // It returns GetClientNetworkable( entnum )->GetIClientEntity().
    virtual IClientEntity* GetClientEntity(int entnum) = 0;
    virtual IClientEntity* GetClientEntityFromHandle(CBaseHandle hEnt) = 0;

    // Returns number of entities currently in use
    virtual int					NumberOfEntities(bool bIncludeNonNetworkable) = 0;

    // Returns highest index actually used
    virtual int					GetHighestEntityIndex(void) = 0;

    // Sizes entity list to specified size
    virtual void				SetMaxEntities(int maxents) = 0;
    virtual int					GetMaxEntities() = 0;
    virtual EntityCacheInfo_t* GetClientNetworkableArray() = 0;
};

IClientEntityList* entitylist;

// netvar shit

// The serial number that gets passed in is used for ehandles.
typedef IClientNetworkable* (*CreateClientClassFn)(int entnum, int serialNum);
typedef IClientNetworkable* (*CreateEventFn)();

typedef enum
{
    DPT_Int = 0,
    DPT_Float,
    DPT_Vector,
    DPT_VectorXY, // Only encodes the XY of a vector, ignores Z
    DPT_String,
    DPT_Array,	// An array of the base types (can't be of datatables).
    DPT_DataTable,
#if 0 // We can't ship this since it changes the size of DTVariant to be 20 bytes instead of 16 and that breaks MODs!!!
    DPT_Quaternion,
#endif
    DPT_Int64,
    DPT_NUMSendPropTypes
} SendPropType;

const char* SendPropType_to_string(SendPropType spt)
{
    switch (spt)
    {
    case DPT_Int:
        return "int";
    case DPT_Float:
        return "float";
    case DPT_Vector:
        return "vec3";
    case DPT_VectorXY:
        return "vec2";
    case DPT_String:
        return "string";
    case DPT_Array:
        return "array";
    case DPT_DataTable:
        return "table";
    case DPT_Int64:
        return "i64";
    default:
        return "???";
    }
}

class RecvTable;

class RecvProp
{
public:
    char* m_pVarName;
    SendPropType			m_RecvType;
    int						m_Flags;
    int						m_StringBufferSize;

    bool					m_bInsideArray;		// Set to true by the engine if this property sits inside an array.

    // Extra data that certain special property types bind to the property here.
    const void* m_pExtraData;

    // If this is an array (DPT_Array).
    RecvProp* m_pArrayProp;
    void*	m_ArrayLengthProxy;

    void*			m_ProxyFn;
    void* m_DataTableProxyFn;	// For RDT_DataTable.

    RecvTable* m_pDataTable;		// For RDT_DataTable.
    int						m_Offset;

    int						m_ElementStride;
    int						m_nElements;

    // If it's one of the numbered "000", "001", etc properties in an array, then
    // these can be used to get its array property name for debugging.
    const char* m_pParentArrayPropName;
};

class RecvTable
{
public:

    // Properties described in a table.
    RecvProp* m_pProps;
    int		  m_nProps;

    // The decoder. NOTE: this covers each RecvTable AND all its children (ie: its children
    // will have their own decoders that include props for all their children).
    void* m_pDecoder;

    char* m_pNetTableName;	// The name matched between client and server.


private:

    bool			m_bInitialized;
    bool			m_bInMainList;
};

class ClientClass
{

public:
    CreateClientClassFn     m_pCreateFn;
    CreateEventFn           m_pCreateEventFn;   // Only called for event objects.
    char* m_pNetworkName;
    RecvTable* m_pRecvTable;
    ClientClass* m_pNext;
    int                     m_ClassID;  // Managed by the engine.
    const char* m_pMapClassname;
};

// Linked list of all known client classes
extern ClientClass* g_pClientClassHead;


// cinput shit

class C_BaseCombatWeapon;

typedef void kbutton_t;

class CInput
{
    virtual     void        Init_All(void) = 0;
    virtual     void        Shutdown_All(void) = 0;
    virtual     int         GetButtonBits(bool bResetState) = 0;
    virtual     void        CreateMove(int sequence_number, float input_sample_frametime, bool active) = 0;
    virtual     void        ExtraMouseSample(float frametime, bool active) = 0;
    virtual     bool        WriteUsercmdDeltaToBuffer(int nSlot, void* buf, int from, int to, bool isnewcommand) = 0;
    virtual     void        EncodeUserCmdToBuffer(int nSlot, void* buf, int slot) = 0;
    virtual     void        DecodeUserCmdFromBuffer(int nSlot, void* buf, int slot) = 0;
    virtual     CUserCmd* GetUserCmd(int nSlot, int sequence_number) = 0;
    virtual     void        MakeWeaponSelection(C_BaseCombatWeapon* weapon) = 0;
    virtual     float       KeyState(kbutton_t* key) = 0;
    virtual     int         KeyEvent(int down, uint32_t keynum, const char* pszCurrentBinding) = 0;
    virtual     kbutton_t* FindKey(const char* name) = 0;
    virtual     void        ControllerCommands(void) = 0;
    virtual     void        Joystick_Advanced(bool bSilent) = 0;
    virtual     void        Joystick_SetSampleTime(float frametime) = 0;
    virtual     float       Joystick_GetPitch(void) = 0;
    virtual     float       Joystick_GetYaw(void) = 0;
    virtual     void        Joystick_Querry(float& forward, float& side, float& pitch, float& yaw) = 0;
    virtual     void        Joystick_ForceRecentering(int nStick, bool bSet = true) = 0;
    virtual     void        IN_SetSampleTime(float frametime) = 0;
    virtual     void        AccumulateMouse(int nSlot) = 0;
    virtual     void        ActivateMouse(void) = 0;
    virtual     void        DeactivateMouse(void) = 0;
    virtual     void        ClearStates(void) = 0;
    virtual     float       GetLookSpring(void) = 0;
    virtual     void        GetFullscreenMousePos(int* mx, int* my, int* unclampedx = NULL, int* unclampedy = NULL) = 0;
    virtual     void        SetFullscreenMousePos(int mx, int my) = 0;
    virtual     void        ResetMouse(void) = 0;
    //  virtual     bool        IsNoClipping( void ) = 0;
    virtual     float       GetLastForwardMove(void) = 0;
    virtual     void        ClearInputButton(int bits) = 0;
    virtual     void        CAM_Think(void) = 0;
    virtual     int         CAM_IsThirdPerson(int nSlot = -1) = 0;
    virtual     bool        CAM_IsThirdPersonOverview(int nSlot = -1) = 0;
    virtual     void        CAM_GetCameraOffset(Vector& ofs) = 0;
    virtual     void        CAM_ToThirdPerson(void) = 0;
    virtual     void        CAM_ToFirstPerson(void) = 0;
    virtual     void        CAM_ToThirdPersonShoulder(void) = 0;
    virtual     void        CAM_ToThirdPersonOverview(void) = 0;
    virtual     void        CAM_StartMouseMove(void) = 0;
    virtual     void        CAM_EndMouseMove(void) = 0;
    virtual     void        CAM_StartDistance(void) = 0;
    virtual     void        CAM_EndDistance(void) = 0;
    virtual     int         CAM_InterceptingMouse(void) = 0;
    virtual     void        CAM_Command(int command) = 0;
    virtual     void        CAM_ToOrthographic() = 0;
    virtual     bool        CAM_IsOrthographic() const = 0;
    virtual     void        CAM_OrthographicSize(float& w, float& h) const = 0;
    virtual     void        AddIKGroundContactInfo(int entindex, float minheight, float maxheight) = 0;
    virtual     void        LevelInit(void) = 0;
    virtual     void        CAM_SetCameraThirdData(void* pCameraData, const QAngle& vecCameraOffset) = 0;
    virtual     void        CAM_CameraThirdThink(void) = 0;
    virtual     void        CheckPaused(CUserCmd* cmd) = 0;
    virtual     void        CheckSplitScreenMimic(int nSlot, CUserCmd* cmd, CUserCmd* pPlayer0Command) = 0;
    virtual void        Init_Camera(void) = 0;
    virtual void        ApplyMouse(int nSlot, QAngle& viewangles, CUserCmd* cmd, float mouse_x, float mouse_y) = 0;
};

#define	FL_ONGROUND				(1<<0)	// At rest / on the ground
#define FL_DUCKING				(1<<1)	// Player flag -- Player is fully crouched
#define FL_ANIMDUCKING			(1<<2)	// Player flag -- Player is in the process of crouching or uncrouching but could be in transition
// examples:                                   Fully ducked:  FL_DUCKING &  FL_ANIMDUCKING
//           Previously fully ducked, unducking in progress:  FL_DUCKING & !FL_ANIMDUCKING
//                                           Fully unducked: !FL_DUCKING & !FL_ANIMDUCKING
//           Previously fully unducked, ducking in progress: !FL_DUCKING &  FL_ANIMDUCKING
#define	FL_WATERJUMP			(1<<3)	// player jumping out of water
#define FL_ONTRAIN				(1<<4) // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN				(1<<5)	// Indicates the entity is standing in rain
#define FL_FROZEN				(1<<6) // Player is frozen for 3rd person camera
#define FL_ATCONTROLS			(1<<7) // Player can't move, but keeps key inputs for controlling another entity
#define	FL_CLIENT				(1<<8)	// Is a player
#define FL_FAKECLIENT			(1<<9)	// Fake client, simulated server side; don't send network messages to them
// NON-PLAYER SPECIFIC (i.e., not used by GameMovement or the client .dll ) -- Can still be applied to players, though
#define	FL_INWATER				(1<<10)	// In water

// NOTE if you move things up, make sure to change this value
#define PLAYER_FLAG_BITS		11

#define	FL_FLY					(1<<11)	// Changes the SV_Movestep() behavior to not need to be on ground
#define	FL_SWIM					(1<<12)	// Changes the SV_Movestep() behavior to not need to be on ground (but stay in water)
#define	FL_CONVEYOR				(1<<13)
#define	FL_NPC					(1<<14)
#define	FL_GODMODE				(1<<15)
#define	FL_NOTARGET				(1<<16)
#define	FL_AIMTARGET			(1<<17)	// set if the crosshair needs to aim onto the entity
#define	FL_PARTIALGROUND		(1<<18)	// not all corners are valid
#define FL_STATICPROP			(1<<19)	// Eetsa static prop!		
#define FL_GRAPHED				(1<<20) // worldgraph has this ent listed as something that blocks a connection
#define FL_GRENADE				(1<<21)
#define FL_STEPMOVEMENT			(1<<22)	// Changes the SV_Movestep() behavior to not do any processing
#define FL_DONTTOUCH			(1<<23)	// Doesn't generate touch functions, generates Untouch() for anything it was touching when this flag was set
#define FL_BASEVELOCITY			(1<<24)	// Base velocity has been applied this frame (used to convert base velocity into momentum)
#define FL_WORLDBRUSH			(1<<25)	// Not moveable/removeable brush entity (really part of the world, but represented as an entity for transparency or something)
#define FL_OBJECT				(1<<26) // Terrible name. This is an object that NPCs should see. Missiles, for example.
#define FL_KILLME				(1<<27)	// This entity is marked for death -- will be freed by game DLL
#define FL_ONFIRE				(1<<28)	// You know...
#define FL_DISSOLVING			(1<<29) // We're dissolving!
#define FL_TRANSRAGDOLL			(1<<30) // In the process of turning into a client side ragdoll.
#define FL_UNBLOCKABLE_BY_PLAYER (1<<31) // pusher that can't be blocked by the player

#define IN_ATTACK       (1 << 0)
#define IN_JUMP			(1 << 1)
#define IN_DUCK			(1 << 2)
#define IN_FORWARD		(1 << 3)
#define IN_BACK			(1 << 4)
#define IN_USE			(1 << 5)
#define IN_CANCEL		(1 << 6)
#define IN_LEFT			(1 << 7)
#define IN_RIGHT		(1 << 8)
#define IN_MOVELEFT		(1 << 9)
#define IN_MOVERIGHT	(1 << 10)
#define IN_ATTACK2		(1 << 11)
#define IN_RUN			(1 << 12)
#define IN_RELOAD		(1 << 13)
#define IN_ALT1			(1 << 14)
#define IN_ALT2			(1 << 15)
#define IN_SCORE		(1 << 16)   // Used by client.dll for when scoreboard is held down
#define IN_SPEED		(1 << 17)	// Player is holding the speed key
#define IN_WALK			(1 << 18)	// Player holding walk key
#define IN_ZOOM			(1 << 19)	// Zoom key for HUD zoom
#define IN_WEAPON1		(1 << 20)	// weapon defines these bits
#define IN_WEAPON2		(1 << 21)	// weapon defines these bits
#define IN_BULLRUSH		(1 << 22)
#define IN_GRENADE1		(1 << 23)	// grenade 1
#define IN_GRENADE2		(1 << 24)	// grenade 2
#define	IN_LOOKSPIN		(1 << 25)

class IVEngineClient
{
public:
    virtual int					pad0() = 0;

    // Get the lighting intensivty for a specified point
    // If bClamp is specified, the resulting Vector is restricted to the 0.0 to 1.0 for each element
    virtual Vector				GetLightForPoint(const Vector& pos, bool bClamp) = 0;

    // Traces the line and reports the material impacted as well as the lighting information for the impact point
    virtual void* TraceLineMaterialAndLighting(const Vector& start, const Vector& end,
        Vector& diffuseLightColor, Vector& baseColor) = 0;

    // Given an input text buffer data pointer, parses a single token into the variable token and returns the new
    //  reading position
    virtual const char* ParseFile(const char* data, char* token, int maxlen) = 0;
    virtual bool				CopyLocalFile(const char* source, const char* destination) = 0;

    // Gets the dimensions of the game window
    virtual void				GetScreenSize(int& width, int& height) = 0;

    // Forwards szCmdString to the server, sent reliably if bReliable is set
    virtual void				ServerCmd(const char* szCmdString, bool bReliable = true) = 0;
    // Inserts szCmdString into the command buffer as if it was typed by the client to his/her console.
    // Note: Calls to this are checked against FCVAR_CLIENTCMD_CAN_EXECUTE (if that bit is not set, then this function can't change it).
    //       Call ClientCmd_Unrestricted to have access to FCVAR_CLIENTCMD_CAN_EXECUTE vars.
    virtual void				ClientCmd(const char* szCmdString) = 0;

    // Fill in the player info structure for the specified player index (name, model, etc.)
    virtual bool				GetPlayerInfo(int ent_num, void* pinfo) = 0;

    // Retrieve the player entity number for a specified userID
    virtual int					GetPlayerForUserID(int userID) = 0;

    // Retrieves text message system information for the specified message by name
    virtual void* TextMessageGet(const char* pName) = 0;

    // Returns true if the console is visible
    virtual bool				Con_IsVisible(void) = 0;

    // Get the entity index of the local player
    virtual int					GetLocalPlayer(void) = 0;
};

IVEngineClient* engineclient;

// CBaseHandle.h
class CBaseHandle
{
public:
    uint32_t	m_Index;
};

// const.h

#define NUM_SERIAL_NUM_BITS		16 // (32 - NUM_ENT_ENTRY_BITS)
#define NUM_SERIAL_NUM_SHIFT_BITS (32 - NUM_SERIAL_NUM_BITS)
#define ENT_ENTRY_MASK			(( 1 << NUM_SERIAL_NUM_BITS) - 1)


// cs_weapon_parse.h
/*
    NOTE!!!! Please try not to change the order. If you need to add something, please add it to the end.
    Anytime the order of the weaponID's change, we need to updated a bunch of tables in a couple DB's. Also,
    changing the order can invalidate saved queries and in general makes using the OGS stats difficult.
*/
//--------------------------------------------------------------------------------------------------------
enum CSWeaponID
{
    WEAPON_NONE = 0,

    WEAPON_FIRST,

    WEAPON_DEAGLE = WEAPON_FIRST,
    WEAPON_ELITE,
    WEAPON_FIVESEVEN,
    WEAPON_GLOCK,
    WEAPON_P228,
    WEAPON_USP,

    WEAPON_AK47,
    WEAPON_AUG,
    WEAPON_AWP,
    WEAPON_FAMAS,
    WEAPON_G3SG1,
    WEAPON_GALIL,
    WEAPON_GALILAR,
    WEAPON_M249,
    WEAPON_M3,
    WEAPON_M4A1,
    WEAPON_MAC10,
    WEAPON_MP5NAVY,
    WEAPON_P90,
    WEAPON_SCOUT,
    WEAPON_SG550,
    WEAPON_SG552,
    WEAPON_TMP,
    WEAPON_UMP45,
    WEAPON_XM1014,

    WEAPON_BIZON,
    WEAPON_MAG7,
    WEAPON_NEGEV,
    WEAPON_SAWEDOFF,
    WEAPON_TEC9,
    WEAPON_TASER,

    WEAPON_HKP2000,
    WEAPON_MP7,
    WEAPON_MP9,
    WEAPON_NOVA,
    WEAPON_P250,
    WEAPON_SCAR17,
    WEAPON_SCAR20,
    WEAPON_SG556,
    WEAPON_SSG08,
    WEAPON_LAST = WEAPON_SSG08,

    ITEM_FIRST,
    WEAPON_KNIFE_GG = ITEM_FIRST,
    WEAPON_KNIFE,

    WEAPON_FLASHBANG,
    WEAPON_HEGRENADE,
    WEAPON_SMOKEGRENADE,
    WEAPON_MOLOTOV,
    WEAPON_DECOY,
    WEAPON_INCGRENADE,
    WEAPON_TAGRENADE,
    WEAPON_C4,
    ITEM_MAX = WEAPON_C4,

    EQUIPMENT_FIRST,
    ITEM_KEVLAR = EQUIPMENT_FIRST,
    ITEM_ASSAULTSUIT,
    ITEM_HEAVYASSAULTSUIT,
    ITEM_NVG,
    ITEM_DEFUSER,
    ITEM_CUTTERS,
    EQUIPMENT_MAX,

    WEAPON_HEALTHSHOT,

    WEAPON_MAX = EQUIPMENT_MAX,		// number of weapons weapon index
};

class CCSWeaponInfo
{
    virtual void pad() = 0; // generate a vtable ptr slot
public:
    const char* name;
    uint16_t weaponId;
    uint16_t idk;
};

class C_WeaponCSBase
{
public:
    virtual void vfn_1_sub_1064F1A0() = 0;
    virtual void vfn_2_sub_101DC5D0() = 0;
    virtual void vfn_3_sub_101DC5F0() = 0;
    virtual void vfn_4_sub_101A3A90() = 0;
    virtual void vfn_5_sub_101A3A80() = 0;
    virtual void vfn_6_sub_101A3A70() = 0;
    virtual void vfn_7_sub_100BCBF0() = 0;
    virtual void vfn_8_sub_100BCBF0() = 0;
    virtual void vfn_9_sub_101A3A60() = 0;
    virtual void vfn_10_sub_101DC1F0() = 0;
    virtual void vfn_11_sub_101DD5C0() = 0;
    virtual void vfn_12_sub_101DD5D0() = 0;
    virtual void vfn_13_sub_101C9490() = 0;
    virtual void vfn_14_sub_101E7E50() = 0;
    virtual void vfn_15_sub_101E39E0() = 0;
    virtual void vfn_16_sub_10685020() = 0;
    virtual void vfn_17_sub_ret0() = 0;
    virtual void vfn_18_sub_10650E10() = 0;
    virtual void vfn_19_sub_101C3F70() = 0;
    virtual void vfn_20_sub_101A6010() = 0;
    virtual void vfn_21_sub_10193F10() = 0;
    virtual void vfn_22_sub_100B6650() = 0;
    virtual void vfn_23_sub_101A69D0() = 0;
    virtual void vfn_24_sub_ret0() = 0;
    virtual void vfn_25_sub_101A6B90() = 0;
    virtual void vfn_26_sub_101A6BA0() = 0;
    virtual void vfn_27_sub_101A6C60() = 0;
    virtual void vfn_28_sub_101A0530() = 0;
    virtual void vfn_29_sub_101A6EE0() = 0;
    virtual void vfn_30_sub_101A6F10() = 0;
    virtual void vfn_31_sub_1064B9F0() = 0;
    virtual void vfn_32_nullsub_1() = 0;
    virtual void vfn_33_sub_1019F760() = 0;
    virtual void vfn_34_sub_asdfasdf() = 0;
    virtual void vfn_35_sub_10652FF0() = 0;
    virtual void vfn_36_sub_asdfasdf() = 0;
    virtual void vfn_37_sub_asdfasdf() = 0;
    virtual void vfn_38_sub_101A4D80() = 0;
    virtual void vfn_39_sub_101A4D30() = 0;
    virtual void vfn_40_sub_101A4CD0() = 0;
    virtual void vfn_41_sub_101A47B0() = 0;
    virtual void vfn_42_sub_101A4DF0() = 0;
    virtual void vfn_43_sub_asdfasdf() = 0;
    virtual void vfn_44_sub_101DC200() = 0;
    virtual void vfn_45_sub_100BCBF0() = 0;
    virtual void vfn_46_sub_100BCBF0() = 0;
    virtual void vfn_47_sub_100B7530() = 0;
    virtual void vfn_48_sub_ret0() = 0;
    virtual void vfn_49_sub_asdfasdf() = 0;
    virtual void vfn_50_sub_101A3A30() = 0;
    virtual void vfn_51_sub_101DD670() = 0;
    virtual void vfn_52_sub_101CE410() = 0;
    virtual void vfn_53_sub_101CE4A0() = 0;
    virtual void vfn_54_sub_101A36B0() = 0;
    virtual void vfn_55_sub_101DE5E0() = 0;
    virtual void vfn_56_sub_101D8BF0() = 0;
    virtual void vfn_57_nullsub_1() = 0;
    virtual void vfn_58_sub_101DED10() = 0;
    virtual void vfn_59_sub_101DED20() = 0;
    virtual void vfn_60_sub_10193F10() = 0;
    virtual void vfn_61_sub_101E2E10() = 0;
    virtual void vfn_62_sub_101D8D40() = 0;
    virtual void vfn_63_sub_ret0() = 0;
    virtual void vfn_64_sub_101E2E60() = 0;
    virtual void vfn_65_sub_101E2E80() = 0;
    virtual void vfn_66_sub_10193F10() = 0;
    virtual void vfn_67_sub_101A5A10() = 0;
    virtual void vfn_68_sub_101CD140() = 0;
    virtual void vfn_69_nullsub_1() = 0;
    virtual void vfn_70_sub_101C58D0() = 0;
    virtual void vfn_71_sub_101DC870() = 0;
    virtual void vfn_72_sub_101DC8F0() = 0;
    virtual void vfn_73_sub_101E2D10() = 0;
    virtual void vfn_74_sub_101E2D50() = 0;
    virtual void vfn_75_sub_101E1090() = 0;
    virtual void vfn_76_sub_101DD5F0() = 0;
    virtual void vfn_77_sub_101A3A20() = 0;
    virtual void vfn_78_sub_101A3A10() = 0;
    virtual void vfn_79_sub_101A4530() = 0;
    virtual void vfn_80_nullsub_2() = 0;
    virtual void vfn_81_sub_101DCA70() = 0;
    virtual void vfn_82_sub_101A3A00() = 0;
    virtual void vfn_83_sub_101A39F0() = 0;
    virtual void vfn_84_sub_10686D00() = 0;
    virtual void vfn_85_sub_10686E50() = 0;
    virtual void vfn_86_sub_101C83F0() = 0;
    virtual void vfn_87_sub_101E04D0() = 0;
    virtual void vfn_88_sub_101E04E0() = 0;
    virtual void vfn_89_sub_101E04F0() = 0;
    virtual void vfn_90_sub_101E1D40() = 0;
    virtual void vfn_91_sub_101E0500() = 0;
    virtual void vfn_92_sub_101E0510() = 0;
    virtual void vfn_93_sub_101E0550() = 0;
    virtual void vfn_94_sub_10193F10() = 0;
    virtual void vfn_95_sub_101A39E0() = 0;
    virtual void vfn_96_sub_101DDD50() = 0;
    virtual void vfn_97_sub_10686A40() = 0;
    virtual void vfn_98_sub_101CE340() = 0;
    virtual void vfn_99_sub_ret0() = 0;
    virtual void vfn_100_sub_101CF820() = 0;
    virtual void vfn_101_sub_101DDF60() = 0;
    virtual void vfn_102_sub_101DD6A0() = 0;
    virtual void vfn_103_sub_101D9A20() = 0;
    virtual void vfn_104_sub_101D0A80() = 0;
    virtual void vfn_105_sub_101C7AE0() = 0;
    virtual void vfn_106_sub_101D90A0() = 0;
    virtual void vfn_107_sub_101CCAB0() = 0;
    virtual void vfn_108_sub_101DEF70() = 0;
    virtual void vfn_109_sub_101A0980() = 0;
    virtual void vfn_110_nullsub_2() = 0;
    virtual void vfn_111_nullsub_1() = 0;
    virtual void vfn_112_sub_101CCAE0() = 0;
    virtual void vfn_113_sub_101D5740() = 0;
    virtual void vfn_114_sub_101DF550() = 0;
    virtual void vfn_115_sub_101DF590() = 0;
    virtual void vfn_116_sub_101CF690() = 0;
    virtual void vfn_117_sub_101DDB40() = 0;
    virtual void vfn_118_sub_101DFB00() = 0;
    virtual void vfn_119_sub_asdfasdf() = 0;
    virtual void vfn_120_sub_101E05A0() = 0;
    virtual void vfn_121_nullsub_1() = 0;
    virtual void vfn_122_sub_ret0() = 0;
    virtual void vfn_123_sub_101A3720() = 0;
    virtual void vfn_124_sub_100B6650() = 0;
    virtual void vfn_125_sub_101E0310() = 0;
    virtual void vfn_126_sub_10193F10() = 0;
    virtual void vfn_127_sub_101D8DC0() = 0;
    virtual void vfn_128_sub_101E22D0() = 0;
    virtual void vfn_129_sub_101DEC60() = 0;
    virtual void vfn_130_sub_101E1860() = 0;
    virtual void vfn_131_sub_101C5A50() = 0;
    virtual void vfn_132_nullsub_1() = 0;
    virtual void vfn_133_sub_100B6650() = 0;
    virtual void vfn_134_sub_10193F10() = 0;
    virtual void vfn_135_sub_101A5220() = 0;
    virtual void vfn_136_sub_101A5090() = 0;
    virtual void vfn_137_sub_101A5110() = 0;
    virtual void vfn_138_sub_10654570() = 0;
    virtual void vfn_139_sub_101A39C0() = 0;
    virtual void vfn_140_sub_101E39C0() = 0;
    virtual void vfn_141_sub_ret0() = 0;
    virtual void vfn_142_nullsub_9() = 0;
    virtual void vfn_143_sub_101E1B30() = 0;
    virtual void vfn_144_sub_ret0() = 0;
    virtual void vfn_145_sub_101E29F0() = 0;
    virtual void vfn_146_sub_10193F10() = 0;
    virtual void vfn_147_sub_101A3870() = 0;
    virtual void vfn_148_sub_10310360() = 0;
    virtual void vfn_149_nullsub_1() = 0;
    virtual void vfn_150_sub_1030FD50() = 0;
    virtual void vfn_151_nullsub_1() = 0;
    virtual void vfn_152_sub_10310D30() = 0;
    virtual void vfn_153_sub_10311CE0() = 0;
    virtual void vfn_154_sub_10311B70() = 0;
    virtual void vfn_155_sub_10312450() = 0;
    virtual void vfn_156_sub_101A3CB0() = 0;
    virtual void vfn_157_sub_100B6650() = 0;
    virtual void vfn_158_sub_10193F10() = 0;
    virtual void vfn_159_sub_10193F10() = 0;
    virtual void vfn_160_sub_10193F10() = 0;
    virtual void vfn_161_sub_ret0() = 0;
    virtual void vfn_162_sub_10193F10() = 0;
    virtual void vfn_163_sub_10193F10() = 0;
    virtual void vfn_164_sub_10193F10() = 0;
    virtual void vfn_165_sub_10193F10() = 0;
    virtual void vfn_166_sub_100B6650() = 0;
    virtual void vfn_167_sub_100BCBF0() = 0;
    virtual void vfn_168_sub_10193F10() = 0;
    virtual void vfn_169_sub_101A4460() = 0;
    virtual void vfn_170_sub_101A44B0() = 0;
    virtual void vfn_171_sub_101A44C0() = 0;
    virtual void vfn_172_sub_101A44D0() = 0;
    virtual void vfn_173_sub_101A5060() = 0;
    virtual void vfn_174_sub_101DD620() = 0;
    virtual void vfn_175_sub_101A4520() = 0;
    virtual void vfn_176_sub_101A44F0() = 0;
    virtual void vfn_177_sub_101A3990() = 0;
    virtual void vfn_178_sub_101DFAC0() = 0;
    virtual void vfn_179_sub_101DF800() = 0;
    virtual void vfn_180_sub_101A1760() = 0;
    virtual void vfn_181_sub_101E1D30() = 0;
    virtual void vfn_182_sub_101DD1B0() = 0;
    virtual void vfn_183_sub_101DD1E0() = 0;
    virtual void vfn_184_sub_100B6650() = 0;
    virtual void vfn_185_sub_101A3980() = 0;
    virtual void vfn_186_nullsub_80() = 0;
    virtual void vfn_187_sub_101C6A00() = 0;
    virtual void vfn_188_sub_101CF7A0() = 0;
    virtual void vfn_189_sub_101C6A30() = 0;
    virtual void vfn_190_sub_101E94F0() = 0;
    virtual void vfn_191_sub_101C7500() = 0;
    virtual void vfn_192_sub_101C8EE0() = 0;
    virtual void vfn_193_sub_101C8FA0() = 0;
    virtual void vfn_194_sub_10686A90() = 0;
    virtual void vfn_195_sub_10686BC0() = 0;
    virtual void vfn_196_sub_101AA680() = 0;
    virtual void vfn_197_sub_101C93B0() = 0;
    virtual void vfn_198_nullsub_9() = 0;
    virtual void vfn_199_sub_101D6530() = 0;
    virtual void vfn_200_sub_106867A0() = 0;
    virtual void vfn_201_sub_101CC4A0() = 0;
    virtual void vfn_202_asdgasdgsdg() = 0;
    virtual void vfn_203_sub_101CB7D0() = 0;
    virtual void vfn_204_sub_101CBA40() = 0;
    virtual void vfn_205_sub_101A3E20() = 0;
    virtual void vfn_206_sub_101E7E20() = 0;
    virtual void vfn_207_sub_101D5C10() = 0;
    virtual void vfn_208_sub_101D0C40() = 0;
    virtual void vfn_209_sub_101D0D30() = 0;
    virtual void vfn_210_nullsub_10() = 0;
    virtual void vfn_211_sub_101D0B10() = 0;
    virtual void vfn_212_sub_101CCC00() = 0;
    virtual void vfn_213_sub_10193F10() = 0;
    virtual void vfn_214_sub_101CDC40() = 0;
    virtual void vfn_215_sub_101CD850() = 0;
    virtual void vfn_216_sub_101C78B0() = 0;
    virtual void vfn_217_sub_101C7A20() = 0;
    virtual void vfn_218_sub_101CD7D0() = 0;
    virtual void vfn_219_sub_101CE7B0() = 0;
    virtual void vfn_220_sub_101CE850() = 0;
    virtual void vfn_221_sub_101CED00() = 0;
    virtual void vfn_222_sub_101CE620() = 0;
    virtual void vfn_223_sub_101A3920() = 0;
    virtual void vfn_224_sub_101CE300() = 0;
    virtual void vfn_225_sub_101A3720() = 0;
    virtual void vfn_226_sub_asdfasdf() = 0;
    virtual void vfn_227_sub_101A3910() = 0;
    virtual void vfn_228_nullsub_1() = 0;
    virtual void vfn_229_sub_101D04C0() = 0;
    virtual void vfn_230_sub_asdfasdf() = 0;
    virtual void vfn_231_nullsub_1() = 0;
    virtual void vfn_232_sub_101A3900() = 0;
    virtual void vfn_233_sub_101D0B00() = 0;
    virtual void vfn_234_sub_10193F10() = 0;
    virtual void vfn_235_sub_10193F10() = 0;
    virtual void vfn_236_sub_10193F10() = 0;
    virtual void vfn_237_sub_101A38A0() = 0;
    virtual void vfn_238_sub_101A3FF0() = 0;
    virtual void vfn_239_nullsub_2() = 0;
    virtual void vfn_240_sub_10193F10() = 0;
    virtual void vfn_241_sub_101C8640() = 0;
    virtual void vfn_242_sub_101939C0() = 0;
    virtual void vfn_243_sub_100B6650() = 0;
    virtual void vfn_244_sub_101A3890() = 0;
    virtual void vfn_245_sub_101A3880() = 0;
    virtual void vfn_246_sub_101CA7F0() = 0;
    virtual void vfn_247_sub_101D5F30() = 0;
    virtual void vfn_248_nullsub_2() = 0;
    virtual void vfn_249_nullsub_2() = 0;
    virtual void vfn_250_nullsub_2() = 0;
    virtual void vfn_251_sub_101E7BD0() = 0;
    virtual void vfn_252_sub_101E9E60() = 0;
    virtual void vfn_253_sub_101EA850() = 0;
    virtual void vfn_254_sub_101E9D50() = 0;
    virtual void vfn_255_sub_101DDD40() = 0;
    virtual void vfn_256_sub_100B6650() = 0;
    virtual void vfn_257_sub_106545F0() = 0;
    virtual void vfn_258_sub_10686710() = 0;
    virtual void vfn_259_sub_106861D0() = 0;
    virtual void vfn_260_sub_100B6650() = 0;
    virtual void vfn_261_sub_10686AF0() = 0;
    virtual void vfn_262_sub_10193F10() = 0;
    virtual void vfn_263_sub_10193F10() = 0;
    virtual void vfn_264_sub_101A83D0() = 0;
    virtual void vfn_265_sub_101A3840() = 0;
    virtual void vfn_266_sub_101A3810() = 0;
    virtual void vfn_267_nullsub_351() = 0;
    virtual void vfn_268_sub_10685A40() = 0;
    virtual void vfn_269_sub_10685AC0() = 0;
    virtual void vfn_270_sub_10685A50() = 0;
    virtual void vfn_271_sub_101A37D0() = 0;
    virtual void vfn_272_sub_10685770() = 0;
    virtual void vfn_273_sub_101A37C0() = 0;
    virtual void vfn_274_sub_101A37B0() = 0;
    virtual void vfn_275_sub_101A37A0() = 0;
    virtual void vfn_276_sub_101A3790() = 0;
    virtual void vfn_277_sub_10686EB0() = 0;
    virtual void vfn_278_sub_sadfasdfasdf() = 0;
    virtual void vfn_279_sub_10687210() = 0;
    virtual void vfn_280_sub_10687370() = 0;
    virtual void vfn_281_sub_100B6650() = 0;
    virtual void vfn_282_sub_101A3780() = 0;
    virtual void vfn_283_sub_101A3770() = 0;
    virtual void vfn_284_sub_101A0770() = 0;
    virtual void vfn_285_sub_106538B0() = 0;
    virtual void vfn_286_sub_101A0C70() = 0;
    virtual void vfn_287_sub_100B6650() = 0;
    virtual void vfn_288_sub_10653440() = 0;
    virtual void vfn_289_sub_100B6650() = 0;
    virtual void vfn_290_sub_101A0410() = 0;
    virtual void vfn_291_sub_asdfasdf() = 0;
    virtual void vfn_292_sub_10193F10() = 0;
    virtual void vfn_293_sub_10193F10() = 0;
    virtual void vfn_294_sub_101A0690() = 0;
    virtual void vfn_295_sub_asdfasdf() = 0;
    virtual void vfn_296_sub_asdfasdf() = 0;
    virtual void vfn_297_sub_101A06E0() = 0;
    virtual void vfn_298_sub_asdfasdf() = 0;
    virtual void vfn_299_sub_asdfasdf() = 0;
    virtual void vfn_300_sub_101A0CC0() = 0;
    virtual void vfn_301_sub_10651AF0() = 0;
    virtual void vfn_302_sub_10651B10() = 0;
    virtual void vfn_303_sub_101A0DF0() = 0;
    virtual void vfn_304_sub_101A04C0() = 0;
    virtual void vfn_305_sub_101A04E0() = 0;
    virtual void vfn_306_sub_101A0520() = 0;
    virtual void vfn_307_sub_101A0EE0() = 0;
    virtual void vfn_308_sub_101A0F30() = 0;
    virtual void vfn_309_sub_101A0F60() = 0;
    virtual void vfn_310_sub_100B6650() = 0;
    virtual void vfn_311_sub_10653230() = 0;
    virtual void vfn_312_sub_10653450() = 0;
    virtual void vfn_313_sub_106535D0() = 0;
    virtual void vfn_314_sub_10653530() = 0;
    virtual void vfn_315_sub_100BCBF0() = 0;
    virtual void vfn_316_sub_101A0F90() = 0;
    virtual void vfn_317_sub_101A1120() = 0;
    virtual void vfn_318_sub_101A11B0() = 0;
    virtual void vfn_319_sub_10193F10() = 0;
    virtual void vfn_320_sub_10193F10() = 0;
    virtual void vfn_321_sub_101A1840() = 0;
    virtual void vfn_322_sub_10651E30() = 0;
    virtual void vfn_323_sub_10652530() = 0;
    virtual void vfn_324_sub_asdfasdf() = 0;
    virtual void vfn_325_sub_101A2110() = 0;
    virtual void vfn_326_sub_101A1C30() = 0;
    virtual void vfn_327_sub_100B6650() = 0;
    virtual void vfn_328_nullsub_10() = 0;
    virtual void vfn_329_sub_asdfasdf() = 0;
    virtual void vfn_330_sub_10193F10() = 0;
    virtual void vfn_331_sub_101A2170() = 0;
    virtual void vfn_332_sub_101A2370() = 0;
    virtual void vfn_333_sub_101A24C0() = 0;
    virtual void vfn_334_sub_101A20E0() = 0;
    virtual void vfn_335_sub_101A24E0() = 0;
    virtual void vfn_336_sub_asdfasdf() = 0;
    virtual void vfn_337_sub_101A2890() = 0;
    virtual void vfn_338_sub_101A2140() = 0;
    virtual void vfn_339_sub_101A2150() = 0;
    virtual void vfn_340_sub_101A1540() = 0;
    virtual void vfn_341_sub_101A36B0() = 0;
    virtual void vfn_342_sub_ret0() = 0;
    virtual void vfn_343_sub_101A3740() = 0;
    virtual void vfn_344_sub_101A1CA0() = 0;
    virtual void vfn_345_sub_101A3730() = 0;
    virtual void vfn_346_sub_101A1CF0() = 0;
    virtual void vfn_347_sub_101A1CF0() = 0;
    virtual void vfn_348_sub_101A3720() = 0;
    virtual void vfn_349_sub_101A3720() = 0;
    virtual void vfn_350_sub_101A3710() = 0;
    virtual void vfn_351_sub_101A3700() = 0;
    virtual void vfn_352_sub_101A36D0() = 0;
    virtual void vfn_353_sub_101A1D00() = 0;
    virtual void vfn_354_sub_101A1E70() = 0;
    virtual void vfn_355_sub_101A1CE0() = 0;
    virtual void vfn_356_sub_101A36C0() = 0;
    virtual void vfn_357_sub_101A36B0() = 0;
    virtual void vfn_358_sub_10193F10() = 0;
    virtual void vfn_359_sub_10193F10() = 0;
    virtual void vfn_360_sub_101A36A0() = 0;
    virtual void vfn_361_sub_101A0B30() = 0;
    virtual void vfn_362_sub_asdfasdf() = 0;
    virtual void vfn_363_sub_101A2160() = 0;
    virtual void vfn_364_sub_1064D230() = 0;
    virtual void vfn_365_sub_106562B0() = 0;
    virtual void vfn_366_sub_10655B20() = 0;
    virtual void vfn_367_sub_101A2D00() = 0;
    virtual void vfn_368_sub_101A2D10() = 0;
    virtual void vfn_369_sub_100B6650() = 0;
    virtual void vfn_370_sub_100B6650() = 0;
    virtual void vfn_371_sub_1019FC40() = 0;
    virtual void vfn_372_sub_1019FCE0() = 0;
    virtual void vfn_373_sub_1019FD30() = 0;
    virtual void vfn_374_sub_1019FD80() = 0;
    virtual void vfn_375_sub_1019FE50() = 0;
    virtual void vfn_376_sub_1019FEA0() = 0;
    virtual void vfn_377_sub_1019FEF0() = 0;
    virtual void vfn_378_sub_1019FF40() = 0;
    virtual void vfn_379_sub_101A32A0() = 0;
    virtual void vfn_380_sub_1019FFE0() = 0;
    virtual void vfn_381_sub_101A0030() = 0;
    virtual void vfn_382_sub_101A0080() = 0;
    virtual void vfn_383_sub_101A00D0() = 0;
    virtual void vfn_384_sub_101A0150() = 0;
    virtual void vfn_385_sub_101A01A0() = 0;
    virtual void vfn_386_sub_101A01F0() = 0;
    virtual void vfn_387_sub_1019FE00() = 0;
    virtual void vfn_388_sub_101A0290() = 0;
    virtual void vfn_389_sub_101A02E0() = 0;
    virtual void vfn_390_sub_1019FC00() = 0;
    virtual void vfn_391_sub_1019FC20() = 0;
    virtual void vfn_392_nullsub_1() = 0;
    virtual void vfn_393_sub_1019F900() = 0;
    virtual void vfn_394_sub_101A3AB0() = 0;
    virtual void vfn_395_sub_101A3AA0() = 0;
    virtual void vfn_396_sub_101A2D20() = 0;
    virtual void vfn_397_sub_ret0() = 0;
    virtual void vfn_398_sub_ret0() = 0;
    virtual void vfn_399_sub_100B6650() = 0;
    virtual void vfn_400_sub_asdfasdf() = 0;
    virtual void vfn_401_sub_1019F380() = 0;
    virtual void vfn_402_sub_101D8FB0() = 0;
    virtual void vfn_403_nullsub_2() = 0;
    virtual void vfn_404_sub_10653CF0() = 0;
    virtual void vfn_405_sub_100B6650() = 0;
    virtual void vfn_406_sub_101D8FE0() = 0;
    virtual void vfn_407_sub_101D9070() = 0;
    virtual void vfn_408_sub_101D91F0() = 0;
    virtual void vfn_409_sub_asdfasdf() = 0;
    virtual void vfn_410_nullsub_2() = 0;
    virtual void vfn_411_sub_101A3680() = 0;
    virtual void vfn_412_sub_100B6650() = 0;
    virtual void vfn_413_nullsub_2() = 0;
    virtual void vfn_414_sub_101A3660() = 0;
    virtual void vfn_415_nullsub_1() = 0;
    virtual void vfn_416_sub_101A3650() = 0;
    virtual void vfn_417_sub_10193F10() = 0;
    virtual void vfn_418_sub_10193F10() = 0;
    virtual void vfn_419_sub_10193F10() = 0;
    virtual void vfn_420_sub_10193F10() = 0;
    virtual void vfn_421_sub_101A17B0() = 0;
    virtual void vfn_422_sub_100B6650() = 0;
    virtual void vfn_423_sub_101939C0() = 0;
    virtual void vfn_424_nullsub_1() = 0;
    virtual void vfn_425_sub_asdfasdf() = 0;
    virtual void vfn_426_sub_1064B720() = 0;
    virtual void vfn_427_sub_1064B730() = 0;
    virtual void vfn_428_sub_ret0() = 0;
    virtual void vfn_429_sub_1064B750() = 0;
    virtual void vfn_430_sub_1064B760() = 0;
    virtual void vfn_431_sub_1064B770() = 0;
    virtual void vfn_432_sub_1064B780() = 0;
    virtual void vfn_433_sub_1064B790() = 0;
    virtual void vfn_434_sub_1064B7C0() = 0;
    virtual void vfn_435_sub_1064B7D0() = 0;
    virtual void vfn_436_sub_1064B800() = 0;
    virtual void vfn_437_sub_1064B830() = 0;
    virtual void vfn_438_sub_1064B840() = 0;
    virtual void vfn_439_sub_1064B870() = 0;
    virtual void vfn_440_sub_1064B8A0() = 0;
    virtual void vfn_441_sub_1064B8D0() = 0;
    virtual void vfn_442_sub_1064B900() = 0;
    virtual void vfn_443_sub_1064B930() = 0;
    virtual void vfn_444_sub_1064B940() = 0;
    virtual void vfn_445_sub_1064B950() = 0;
    virtual void vfn_446_sub_1064B960() = 0;
    virtual void vfn_447_sub_1064B970() = 0;
    virtual void vfn_448_sub_1064B980() = 0;
    virtual void vfn_449_sub_1064B990() = 0;
    virtual void vfn_450_sub_1064B9A0() = 0;
    virtual void vfn_451_sub_1064B9B0() = 0;
    virtual void vfn_452_sub_1064B9C0() = 0;
    virtual void vfn_453_sub_10652A50() = 0;
    virtual void vfn_454_sub_1064B9D0() = 0;
    virtual void vfn_455_sub_1064B9E0() = 0;
    virtual void vfn_456_sub_101A36B0() = 0;
    virtual void vfn_457_sub_10657290() = 0;
    virtual void vfn_458_sub_106572D0() = 0;
    virtual void vfn_459_sub_1064BA20() = 0;
    virtual void vfn_460_sub_10652EA0() = 0;
    virtual CCSWeaponInfo* GetCSWpnData() = 0;
    virtual void vfn_462_sub_ret0() = 0;
    virtual void vfn_463_idk() = 0;
    virtual void vfn_464_sub_10653410() = 0;
    virtual void vfn_465_nullsub_1() = 0;
    virtual void OnLand() = 0;
    virtual void vfn_467_sub_10193F10() = 0;
    virtual void vfn_468_sub_10654E20() = 0;
    virtual void vfn_469_sub_10654E90() = 0;
    virtual void vfn_470_sub_10654F10() = 0;
    virtual void vfn_471_sub_10654F40() = 0;
    virtual void vfn_472_sub_1064BA50() = 0;
    virtual void vfn_473_sub_10654CA0() = 0;
    virtual void vfn_474_sub_10654D40() = 0;
    virtual void vfn_475_sub_10654DB0() = 0;
    virtual void vfn_476_sub_10654D10() = 0;
    virtual void vfn_477_sub_101A1540() = 0;
    virtual void vfn_478_sub_10651540() = 0;
    virtual void vfn_479_sub_asdfasdf() = 0;
    virtual void vfn_480_sub_10193F10() = 0;
    virtual void vfn_481_sub_1064BA80() = 0;
    virtual void vfn_482_sub_10193F10() = 0;
    virtual void vfn_483_sub_10652A90() = 0;
    virtual void vfn_484_sub_10656510() = 0;
    virtual void vfn_485_sub_1064BA90() = 0;
};


// not necessarily stable offsets
#define VTABLE_INDEX_ApplyMouse 55
#define VTABLE_INDEX_CreateMove 288

#define HITGROUP_PLAYER_HEAD 1