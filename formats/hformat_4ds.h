// (c) 2016 ZaKlaus; All Rights Reserved
/*
4DS File Format
Usage: Model format
SW: Mafia TCOLH
*/
#if !defined(HFORMAT_4DS_H)

#include "hftw.h"

enum
{
    HFormat4DSMaterialFlag_TextureDiffuse = 0x0004,
    HFormat4DSMaterialFlag_Colored = 0x08,
    HFormat4DSMaterialFlag_MipMapping = 0x008,
    HFormat4DSMaterialFlag_AnimatedTextureDiffuse = 0x04,
    HFormat4DSMaterialFlag_AnimatedTextureAlpha = 0x02,
    HFormat4DSMaterialFlag_DoubleSidedMaterial = 0x1,
    HFormat4DSMaterialFlag_EnvironmentMap = 0x0008,
    HFormat4DSMaterialFlag_NormalTextureBlend = 0x000001,
    HFormat4DSMaterialFlag_MultiplyTextureBlend = 0x000002,
    HFormat4DSMaterialFlag_AdditiveTextureBlend = 0x000004,
    HFormat4DSMaterialFlag_CalcReflectTextureY = 0x00001,
    HFormat4DSMaterialFlag_ProjectReflectTextureY = 0x00002,
    HFormat4DSMaterialFlag_ProjectReflectTextureZ = 0x00004,
    HFormat4DSMaterialFlag_AdditionalEffect = 0x00008,
    HFormat4DSMaterialFlag_AlphaTexture = 0x4,
    HFormat4DSMaterialFlag_ColorKey = 0x2,
    HFormat4DSMaterialFlag_AdditiveMixing = 0x8
};

enum
{
    HFormat4DSMeshType_Standard = 0x01,
    HFormat4DSMeshType_Sector = 0x05,
    HFormat4DSMeshType_Dummy = 0x06,
    HFormat4DSMeshType_Target = 0x07,
    HFormat4DSMeshType_Bone = 0x0a
};

enum
{
    HFormat4DSVisualMeshType_Standard = 0x0,
    HFormat4DSVisualMeshType_SingleMesh = 0x02,
    HFormat4DSVisualMeshType_SingleMorph = 0x03,
    HFormat4DSVisualMeshType_Billboard = 0x04,
    HFormat4DSVisualMeshType_Morph = 0x05,
    HFormat4DSVisualMeshType_Glow = 0x06,
    HFormat4DSVisualMeshType_Mirror = 0x08
};

enum
{
    HFormat4DSMeshRenderFlag_UseDepthBias = 0x0001,
    HFormat4DSMeshRenderFlag_UseShadows = 0x0002,
    HFormat4DSMeshRenderFlag_Unknown = 0x0008,
    HFormat4DSMeshRenderFlag_UseProjection = 0x002,
    HFormat4DSMeshRenderFlag_ForbidFog = 0x008
};

enum
{
    HFormat4DSMeshOccludingFlag_Normal = 0x09,
    HFormat4DSMeshOccludingFlag_Sector = 0x7D,
    HFormat4DSMeshOccludingFlag_Wall = 0x3D, // NOTE(zaklaus): Mesh in sector (walls).
    HFormat4DSMeshOccludingFlag_Portal = 0x1D,   // NOTE(zaklaus): Mesh in portal.
    HFormat4DSMeshOccludingFlag_Inactive = 0x11
};

typedef struct
{
    u32 Flags;
    
    v3 Ambient;
    v3 Diffuse;
    v3 Emission;
    
    r32 Transparency; // 0.0 - invisible; 1.0 - solid
    
    // NOTE(zaklaus): Environment map
    r32 EnvRatio;
    u8 EnvMapNameLength;
    s8 EnvMapName[255];
    
    u8 DiffuseMapNameLength;
    s8 DiffuseMapName[255];
    
    // NOTE(zaklaus): Alpha map
    u8 AlphaMapNameLength;
    s8 AlphaMapName[255];
    
    // NOTE(zaklaus): Anim map
    u32 AnimSequenceLength;
    u16 _Unk0;
    u32 FramePeriod;
    u32 _Unk1;
    u32 _Unk2;
    
} hformat_4ds_material;

typedef struct
{
    v3 Pos;
    v3 Normal;
    v2 UV;
} hformat_4ds_vertex;

typedef struct
{
    u16 A,B,C;
} hformat_4ds_face;

typedef struct
{
    u16 FaceCount;
    hformat_4ds_face *Faces;
    u16 MaterialID;
} hformat_4ds_facegroup;

typedef struct
{
    r32 RelativeDistance;
    u16 VertexCount;
    hformat_4ds_vertex *Vertices;
    
    u8 FaceGroupCount;
    hformat_4ds_facegroup *FaceGroups;
} hformat_4ds_lod;

typedef struct
{
    u16 Instanced;
    
    // NOTE(zaklaus): For non-instanced mesh
    u8 LODLevel;
    hformat_4ds_lod *LODs;
} hformat_4ds_standard;

typedef struct
{
    u16 _Unk0;
    u8 TargetCount;
    u16 *Targets;
} hformat_4ds_target;

#pragma pack(push, 1)
typedef struct
{
    m4 Transform;
    u32 BoneID;
} hformat_4ds_bone;
#pragma pack(pop)

typedef struct
{
    u8 VertexCount;
    u32 _Unk0; // NOTE(zaklaus): Always 4.
    r32 _Unk1[6];
    v3 *Vertices;
    
} hformat_4ds_portal;

typedef struct
{
    u32 _Unk0; // NOTE(zaklaus): Always 2049.
    u32 _Unk1; // NOTE(zaklaus): Always 0.
    u32 VertexCount;
    u32 FaceCount;
    v3 *Vertices;
    hformat_4ds_face *Faces;
    v3 MinBox, MaxBox;
    u8 PortalCount;
    hformat_4ds_portal *Portals;
    
} hformat_4ds_sector;

typedef struct
{
    hformat_4ds_standard Standard;
    u32 RotationAxis;
    u8 IgnoreCamera;
} hformat_4ds_billboard;

typedef struct
{
    // NOTE(zaklaus): Bounding box
    v3 MinBox, MaxBox;
} hformat_4ds_dummy;

typedef struct
{
    r32 Position;
    u16 MaterialID;
} hformat_4ds_glowdata;

#pragma pack(push, 1)
typedef struct
{
    u8 GlowCount;
    hformat_4ds_glowdata *GlowData;
} hformat_4ds_glow;
#pragma pack(pop)

typedef struct
{
    v3 MinBox, MaxBox;
    r32 _Unk[4];
    mat4 ReflectionMatrix;
    v3 BackgroundColor;
    r32 ViewDistance;
    u32 VertexCount;
    u32 FaceCount;
    v3 *Vertices;
    hformat_4ds_face *Faces;
} hformat_4ds_mirror;

#pragma pack(push, 1)
typedef struct
{
    v3 Position;
    v3 Normals;
} hformat_4ds_morph_lod_vertex;
#pragma pack(pop)

typedef struct
{
    u16 VertexCount;
    hformat_4ds_morph_lod_vertex *Vertices;
    
    u8 _Unk0;
    
    u16 *VertexLinks; // NOTE(zaklaus): Addresses vertices from Standard's LOD mesh.
} hformat_4ds_morph_lod;

typedef struct
{
    hformat_4ds_standard Standard;
    u8 FrameCount;
    
    u8 LODLevel; // NOTE(zaklaus): Should equal to Standard.LODLevel.
    u8 _Unk0;
    hformat_4ds_morph_lod *LODs;
    v3 MinBox, MaxBox;
    r32 _Unk1[4];
    
} hformat_4ds_morph;

typedef struct
{
    u8 MeshType;
    
    // NOTE(zaklaus): Standard mesh type
    u8 VisualMeshType;
    u16 MeshRenderFlags;
    
    u16 ParentID; // 0 - not connected
    
    v3 Pos;
    v3 Scale;
    v4 Rot;
    
    u8 CullingFlags;
    
    u8 MeshNameLength;
    s8 MeshName[255];
    u8 MeshParamsLength;
    s8 MeshParams[255];
    
    hformat_4ds_standard Standard;
    hformat_4ds_dummy Dummy;
    hformat_4ds_mirror Mirror;
    hformat_4ds_glow Glow;
    hformat_4ds_billboard Billboard;
    hformat_4ds_sector Sector;
    hformat_4ds_target Target;
    hformat_4ds_bone Bone;
    hformat_4ds_morph Morph;
} hformat_4ds_mesh;

typedef struct
{
    u8 Signature[4];
    u16 FormatVersion; // PC : 0x1D (29)
    u64 Timestamp;
    
    u16 MaterialCount;
    hformat_4ds_material *Materials;
    
    u16 MeshCount;
    hformat_4ds_mesh *Meshes;
} hformat_4ds_header;

internal void
HFormatLoad4DSMaterial(hformat_4ds_header *Model, s32 FileIdx)
{
    IOFileRead(FileIdx, &Model->MaterialCount, sizeof(u16));
    
    Model->Materials = (hformat_4ds_material *) PlatformMemAlloc(sizeof(hformat_4ds_material)*Model->MaterialCount);
    
    for(mi Idx = 0;
        Idx < Model->MaterialCount;
        ++Idx)
    {
        hformat_4ds_material Mat = {0};
        {
            ms msize = sizeof(r32);
            IOFileRead(FileIdx, &Mat.Flags, msize);
            IOFileRead(FileIdx, &Mat.Ambient, sizeof(v3));
            IOFileRead(FileIdx, &Mat.Diffuse, sizeof(v3));
            IOFileRead(FileIdx, &Mat.Emission, sizeof(v3));
            IOFileRead(FileIdx, &Mat.Transparency, msize);
            
            if(Mat.Flags & HFormat4DSMaterialFlag_EnvironmentMap)
            {
                IOFileRead(FileIdx, &Mat.EnvRatio, sizeof(r32));
                IOFileRead(FileIdx, &Mat.EnvMapNameLength, sizeof(u8));
                IOFileRead(FileIdx, Mat.EnvMapName, Mat.EnvMapNameLength);
            }
            
            IOFileRead(FileIdx, &Mat.DiffuseMapNameLength, sizeof(u8));
            IOFileRead(FileIdx, Mat.DiffuseMapName, Mat.DiffuseMapNameLength);
            
            
            if(Mat.Flags & HFormat4DSMaterialFlag_AlphaTexture)
            {
                IOFileRead(FileIdx, &Mat.AlphaMapNameLength, sizeof(u8));
                IOFileRead(FileIdx, Mat.AlphaMapName, Mat.AlphaMapNameLength);
            }
            
            if(Mat.Flags & HFormat4DSMaterialFlag_AnimatedTextureDiffuse ||
               Mat.Flags & HFormat4DSMaterialFlag_AnimatedTextureAlpha)
            {
                
                IOFileRead(FileIdx, &Mat.AnimSequenceLength, sizeof(u32));
                IOFileRead(FileIdx, &Mat._Unk0, sizeof(u16));
                IOFileRead(FileIdx, &Mat.FramePeriod, sizeof(u32));
                IOFileRead(FileIdx, &Mat._Unk1, sizeof(u32));
                IOFileRead(FileIdx, &Mat._Unk2, sizeof(u32));
                
            }
            
        }
        Model->Materials[Idx] = Mat;
    }
    
}

internal hformat_4ds_lod
HFormat4DS4DSLoadLOD(s32 FileIdx)
{
    hformat_4ds_lod Lod = {0};
    IOFileRead(FileIdx, &Lod.RelativeDistance, sizeof(r32));
    IOFileRead(FileIdx, &Lod.VertexCount, sizeof(u16));
    
    Lod.Vertices = (hformat_4ds_vertex *) PlatformMemAlloc(sizeof(hformat_4ds_vertex)*Lod.VertexCount);
    
    IOFileRead(FileIdx, Lod.Vertices, sizeof(hformat_4ds_vertex)*Lod.VertexCount);
    
    IOFileRead(FileIdx, &Lod.FaceGroupCount, sizeof(u8));
    
    Lod.FaceGroups = (hformat_4ds_facegroup *)PlatformMemAlloc(sizeof(hformat_4ds_facegroup)*Lod.FaceGroupCount);
    
    for(mi Idx = 0;
        Idx < Lod.FaceGroupCount;
        ++Idx)
    {
        hformat_4ds_facegroup FaceGroup = {0};
        {
            IOFileRead(FileIdx, &FaceGroup.FaceCount, sizeof(u16));
            
            FaceGroup.Faces = (hformat_4ds_face *)PlatformMemAlloc(sizeof(hformat_4ds_face)*FaceGroup.FaceCount);
            
            IOFileRead(FileIdx, FaceGroup.Faces, sizeof(hformat_4ds_face)*FaceGroup.FaceCount);
            
            IOFileRead(FileIdx, &FaceGroup.MaterialID, sizeof(u16));
        }
        Lod.FaceGroups[Idx] = FaceGroup;
    }
    
    return(Lod);
}

internal hformat_4ds_standard
HFormatLoad4DSStandard(s32 FileIdx)
{
    hformat_4ds_standard Geo = {0};
    IOFileRead(FileIdx, &Geo.Instanced, sizeof(u16));
    
    if(!Geo.Instanced)
    {
        IOFileRead(FileIdx, &Geo.LODLevel, sizeof(u8));
        
        Geo.LODs = (hformat_4ds_lod *)PlatformMemAlloc(sizeof(hformat_4ds_lod)*Geo.LODLevel);
        
        for(mi Idx = 0;
            Idx < Geo.LODLevel;
            ++Idx)
        {
            hformat_4ds_lod Lod = {0};
            Lod = HFormat4DS4DSLoadLOD(FileIdx);
            Geo.LODs[Idx] = Lod;
        }
    }
    return(Geo);
}


internal hformat_4ds_mirror
HFormatLoad4DSMirror(s32 FileIdx)
{
    hformat_4ds_mirror Mirror = {0};
    IOFileRead(FileIdx, &Mirror.MinBox, sizeof(v3));
    IOFileRead(FileIdx, &Mirror.MaxBox, sizeof(v3));
    IOFileRead(FileIdx, Mirror._Unk, sizeof(r32)*4);
    IOFileRead(FileIdx, &Mirror.ReflectionMatrix, sizeof(mat4));
    IOFileRead(FileIdx, &Mirror.BackgroundColor, sizeof(v3));
    IOFileRead(FileIdx, &Mirror.ViewDistance, sizeof(r32));
    
    IOFileRead(FileIdx, &Mirror.VertexCount, sizeof(u32));
    IOFileRead(FileIdx, &Mirror.FaceCount, sizeof(u32));
    
    Mirror.Vertices = (v3 *)PlatformMemAlloc(sizeof(v3)*Mirror.VertexCount);
    Mirror.Faces = (hformat_4ds_face *)PlatformMemAlloc(sizeof(hformat_4ds_face)*Mirror.FaceCount);
    
    IOFileRead(FileIdx, Mirror.Vertices, sizeof(v3)*Mirror.VertexCount);
    IOFileRead(FileIdx, Mirror.Faces, sizeof(hformat_4ds_face)*Mirror.FaceCount);
    
    return(Mirror);
}

internal hformat_4ds_glow
HFormatLoad4DSGlow(s32 FileIdx)
{
    hformat_4ds_glow Glow = {0};
    IOFileRead(FileIdx, &Glow.GlowCount, sizeof(u8));
    
    Glow.GlowData = (hformat_4ds_glowdata *)PlatformMemAlloc(sizeof(hformat_4ds_glowdata)*Glow.GlowCount);
    
    for(mi Idx = 0;
        Idx < Glow.GlowCount;
        Idx++)
    {
        hformat_4ds_glowdata GlowData = {0};
        IOFileRead(FileIdx, &GlowData.Position, sizeof(r32));
        IOFileRead(FileIdx, &GlowData.MaterialID, sizeof(u16));
        Glow.GlowData[Idx] = GlowData;
    }
    
    return(Glow);
}

internal hformat_4ds_portal
HFormatLoad4DSPortal(s32 FileIdx)
{
    hformat_4ds_portal Portal = {0};
    
    IOFileRead(FileIdx, &Portal.VertexCount, sizeof(u8));
    IOFileRead(FileIdx, &Portal._Unk0, sizeof(u32));
    IOFileRead(FileIdx, &Portal._Unk1, sizeof(r32)*6);
    
    Portal.Vertices = (v3 *)PlatformMemAlloc(sizeof(v3)*Portal.VertexCount);
    IOFileRead(FileIdx, Portal.Vertices, sizeof(v3)*Portal.VertexCount);
    
    return(Portal);
}

internal hformat_4ds_sector
HFormatLoad4DSSector(s32 FileIdx)
{
    hformat_4ds_sector Sector = {0};
    IOFileRead(FileIdx, &Sector._Unk0, sizeof(u32));
    IOFileRead(FileIdx, &Sector._Unk1, sizeof(u32));
    IOFileRead(FileIdx, &Sector.VertexCount, sizeof(u32));
    IOFileRead(FileIdx, &Sector.FaceCount, sizeof(u32));
    
    Sector.Vertices = (v3 *)PlatformMemAlloc(Sector.VertexCount);
    IOFileRead(FileIdx, Sector.Vertices, sizeof(v3));
    
    Sector.Faces = (hformat_4ds_face *)PlatformMemAlloc(Sector.FaceCount);
    IOFileRead(FileIdx, Sector.Faces, sizeof(hformat_4ds_face));
    
    IOFileRead(FileIdx, &Sector.MinBox, sizeof(v3));
    IOFileRead(FileIdx, &Sector.MaxBox, sizeof(v3));
    
    IOFileRead(FileIdx, &Sector.PortalCount, sizeof(u8));
    
    Sector.Portals = (hformat_4ds_portal *)PlatformMemAlloc(sizeof(hformat_4ds_portal)*Sector.PortalCount);
    
    for(mi Idx = 0;
        Idx < Sector.PortalCount;
        Idx++)
    {
        hformat_4ds_portal Portal = {0};
        Portal = HFormatLoad4DSPortal(FileIdx);
        Sector.Portals[Idx] = Portal;
    }
    
    return(Sector);
}

internal hformat_4ds_target
HFormatLoad4DSTarget(s32 FileIdx)
{
    hformat_4ds_target Target = {0};
    IOFileRead(FileIdx, &Target._Unk0, sizeof(u16));
    IOFileRead(FileIdx, &Target.TargetCount, sizeof(u8));
    
    Target.Targets = (u16 *)PlatformMemAlloc(sizeof(u16)*Target.TargetCount);
    IOFileRead(FileIdx, Target.Targets, sizeof(u16)*Target.TargetCount);
    
    return(Target);
}

internal hformat_4ds_morph
HFormatLoad4DSMorph(s32 FileIdx, b32 IgnoreStandard)
{
    hformat_4ds_morph Morph = {0};
    // NOTE(zaklaus): Single Morph contains Standard Mesh in Single Mesh already.
    if(!IgnoreStandard) 
    {
        Morph.Standard = HFormatLoad4DSStandard(FileIdx);
    } 
    // NOTE(zaklaus): ELSE ignore Standard Mesh, since Single Mesh has it.
    
    IOFileRead(FileIdx, &Morph.FrameCount, sizeof(u8));
    
    if(Morph.FrameCount)
    {
        IOFileRead(FileIdx, &Morph.LODLevel, sizeof(u8));
        IOFileRead(FileIdx, &Morph._Unk0, sizeof(u8));
        
        Morph.LODs = (hformat_4ds_morph_lod *)PlatformMemAlloc(sizeof(hformat_4ds_morph_lod)*Morph.LODLevel);
        
        for(mi Idx = 0;
            Idx < Morph.LODLevel;
            Idx++)
        {
            hformat_4ds_morph_lod LOD = {0};
            
            IOFileRead(FileIdx, &LOD.VertexCount, sizeof(u16));
            
            LOD.Vertices = (hformat_4ds_morph_lod_vertex *)PlatformMemAlloc(sizeof(hformat_4ds_morph_lod_vertex)*LOD.VertexCount);
            
            IOFileRead(FileIdx, LOD.Vertices, sizeof(hformat_4ds_morph_lod_vertex)*LOD.VertexCount);
            
            if(LOD.VertexCount * Morph.FrameCount)
            {
                IOFileRead(FileIdx, &LOD._Unk0, sizeof(u8));
            }
            
            LOD.VertexLinks = (u16 *)PlatformMemAlloc(sizeof(u16)*LOD.VertexCount);
            
            IOFileRead(FileIdx, LOD.VertexLinks, sizeof(u16)*LOD.VertexCount);
            
            Morph.LODs[Idx] = LOD;
        }
        
        IOFileRead(FileIdx, &Morph.MinBox, sizeof(v3));
        IOFileRead(FileIdx, &Morph.MaxBox, sizeof(v3));
        IOFileRead(FileIdx, &Morph._Unk1, sizeof(r32)*4);
    }
    return(Morph);
}

internal void
HFormatLoad4DSMesh(hformat_4ds_header *Model, s32 FileIdx)
{
    IOFileRead(FileIdx, &Model->MeshCount, sizeof(u16));
    
    Model->Meshes = (hformat_4ds_mesh *)PlatformMemAlloc(sizeof(hformat_4ds_mesh)*Model->MeshCount);
    
    for(mi Idx = 0;
        Idx < Model->MeshCount;
        ++Idx)
    {
        hformat_4ds_mesh Mesh = {0};
        {
            IOFileRead(FileIdx, &Mesh.MeshType, sizeof(u8));
            
            if(Mesh.MeshType & HFormat4DSMeshType_Standard)
            {
                IOFileRead(FileIdx, &Mesh.VisualMeshType, sizeof(u8));
                IOFileRead(FileIdx, &Mesh.MeshRenderFlags, sizeof(u16));
            }
            
            IOFileRead(FileIdx, &Mesh.ParentID, sizeof(u16));
            
            IOFileRead(FileIdx, &Mesh.Pos, sizeof(v3));
            IOFileRead(FileIdx, &Mesh.Scale, sizeof(v3));
            IOFileRead(FileIdx, &Mesh.Rot, sizeof(v4));
            
            
            f32 _Swap0 = Mesh.Rot.W;
            f32 _Swap1 = Mesh.Rot.X;
            f32 _Swap2 = Mesh.Rot.Y;
            f32 _Swap3 = Mesh.Rot.Z;
            
            Mesh.Rot.X = _Swap1;
            Mesh.Rot.Y = _Swap2;
            Mesh.Rot.Z = _Swap3;
            Mesh.Rot.W = _Swap0;
            
            IOFileRead(FileIdx, &Mesh.CullingFlags, sizeof(u8));
            
            IOFileRead(FileIdx, &Mesh.MeshNameLength, sizeof(u8));
            IOFileRead(FileIdx, Mesh.MeshName, Mesh.MeshNameLength);
            
            IOFileRead(FileIdx, &Mesh.MeshParamsLength, sizeof(u8));
            IOFileRead(FileIdx, Mesh.MeshParams, Mesh.MeshParamsLength);
            
            switch(Mesh.MeshType)
            {
                case HFormat4DSMeshType_Standard:
                {
                    switch(Mesh.VisualMeshType)
                    {
                        case HFormat4DSVisualMeshType_Standard:
                        {
                            hformat_4ds_standard Standard = {0};
                            Standard = HFormatLoad4DSStandard(FileIdx);
                            Mesh.Standard = Standard;
                        }break;
                        
                        case HFormat4DSVisualMeshType_Mirror:
                        {
                            hformat_4ds_mirror Mirror = {0};
                            Mirror = HFormatLoad4DSMirror(FileIdx);
                            Mesh.Mirror = Mirror;
                        }break;
                        
                        case HFormat4DSVisualMeshType_Glow:
                        {
                            hformat_4ds_glow Glow = {0};
                            Glow = HFormatLoad4DSGlow(FileIdx);
                            Mesh.Glow = Glow;
                        }break;
                        
                        case HFormat4DSVisualMeshType_Billboard:
                        {
                            hformat_4ds_billboard Billboard = {0};
                            Billboard.Standard = HFormatLoad4DSStandard(FileIdx);
                            IOFileRead(FileIdx, &Billboard.RotationAxis, sizeof(u32));
                            IOFileRead(FileIdx, &Billboard.IgnoreCamera, sizeof(u8));
                            Mesh.Billboard = Billboard;
                        }break;
                        
                        case HFormat4DSVisualMeshType_Morph:
                        {
                            hformat_4ds_morph Morph = {0};
                            Morph = HFormatLoad4DSMorph(FileIdx, 0);
                            Mesh.Morph = Morph;
                        }break;
                        
                        default:
                        {
                            fprintf(stderr, "Not Implemented [VisualMeshType]: %d\n", Mesh.VisualMeshType);
                        }break;
                    }
                }break;
                
                case HFormat4DSMeshType_Dummy:
                {
                    hformat_4ds_dummy Dummy = {0};
                    IOFileRead(FileIdx, &Dummy, sizeof(hformat_4ds_dummy));
                    Mesh.Dummy = Dummy;
                }break;
                
                case HFormat4DSMeshType_Sector:
                {
                    hformat_4ds_sector Sector = {0};
                    Sector = HFormatLoad4DSSector(FileIdx);
                    Mesh.Sector = Sector;
                }break;
                
                case HFormat4DSMeshType_Target:
                {
                    hformat_4ds_target Target = {0};
                    Target = HFormatLoad4DSTarget(FileIdx);
                    Mesh.Target = Target;
                }break;
                
                case HFormat4DSMeshType_Bone:
                {
                    hformat_4ds_bone Bone = {0};
                    IOFileRead(FileIdx, &Bone, sizeof(hformat_4ds_bone));
                    Mesh.Bone = Bone;
                }break;
                
                default:
                {
                    fprintf(stderr, "Not Implemented [MeshType]: %d\n", Mesh.MeshType);
                }break;
            }
        }
        Model->Meshes[Idx] = Mesh;
    }
}

internal hformat_4ds_header *
HFormatLoad4DSModel(s32 FileIdx)
{
    hformat_4ds_header *Model = (hformat_4ds_header *)PlatformMemAlloc(sizeof(hformat_4ds_header));
    {
        IOFileRead(FileIdx, &Model->Signature, 4);
        
        if(!StringsAreEqualA(3, "4DS", (char *)Model->Signature))
        {
            PlatformMemFree(Model);
            return(0);
        }
        
        IOFileRead(FileIdx, &Model->FormatVersion, sizeof(u16));
        IOFileRead(FileIdx, &Model->Timestamp, sizeof(u64));
        
        HFormatLoad4DSMaterial(Model, FileIdx);
        HFormatLoad4DSMesh(Model, FileIdx);
    }
    return(Model);
}

#define HFORMAT_4DS_H
#endif
