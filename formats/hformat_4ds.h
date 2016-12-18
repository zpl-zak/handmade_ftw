// (c) 2016 ZaKlaus; All Rights Reserved
/*
4DS File Format
Usage: Model format
SW: Mafia TCOLH
*/
#if !defined(HFORMAT_4DS_H)

#include "hftw.h"
#include "stdio.h"

enum
{
    HFormatMaterialFlag_TextureDiffuse = 0x0004,
    HFormatMaterialFlag_Colored = 0x08,
    HFormatMaterialFlag_MipMapping = 0x008,
    HFormatMaterialFlag_AnimatedTextureDiffuse = 0x04,
    HFormatMaterialFlag_AnimatedTextureAlpha = 0x02,
    HFormatMaterialFlag_DoubleSidedMaterial = 0x1,
    HFormatMaterialFlag_EnvironmentMap = 0x0008,
    HFormatMaterialFlag_NormalTextureBlend = 0x000001,
    HFormatMaterialFlag_MultiplyTextureBlend = 0x000002,
    HFormatMaterialFlag_AdditiveTextureBlend = 0x000004,
    HFormatMaterialFlag_CalcReflectTextureY = 0x00001,
    HFormatMaterialFlag_ProjectReflectTextureY = 0x00002,
    HFormatMaterialFlag_ProjectReflectTextureZ = 0x00004,
    HFormatMaterialFlag_AdditionalEffect = 0x00008,
    HFormatMaterialFlag_AlphaTexture = 0x4,
    HFormatMaterialFlag_ColorKey = 0x2,
    HFormatMaterialFlag_AdditiveMixing = 0x8
};

enum
{
    HFormatMeshType_Standard = 0x01,
    HFormatMeshType_Sector = 0x05,
    HFormatMeshType_Dummy = 0x06,
    HFormatMeshType_Target = 0x07,
    HFormatMeshType_Bone = 0x0a
};

enum
{
    HFormatVisualMeshType_Standard = 0x0,
    HFormatVisualMeshType_SingleMesh = 0x02,
    HFormatVisualMeshType_SingleMorph = 0x03,
    HFormatVisualMeshType_Billboard = 0x04,
    HFormatVisualMeshType_Morph = 0x05,
    HFormatVisualMeshType_Glow = 0x06,
    HFormatVisualMeshType_Mirror = 0x08
};

enum
{
    HFormatMeshRenderFlag_UseDepthBias = 0x0001,
    HFormatMeshRenderFlag_UseShadows = 0x0002,
    HFormatMeshRenderFlag_Unknown = 0x0008,
    HFormatMeshRenderFlag_UseProjection = 0x002,
    HFormatMeshRenderFlag_ForbidFog = 0x008
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
    s8 EnvMapName[255];
    
    s8 DiffuseMapName[255];
    
    // NOTE(zaklaus): Alpha map
    s8 AlphaMapName[255];
    
    // NOTE(zaklaus): Anim map
    u32 AnimSequenceLength;
    u16 _Ignored0;
    u32 FramePeriod;
    u32 _Ignored1;
    u32 _Ignored2;
    
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
    r32 _Ignored[4];
    mat4 ReflectionMatrix;
    v3 BackgroundColor;
    r32 ViewDistance;
    u32 VertexCount;
    u32 FaceCount;
    v3 *Vertices;
    hformat_4ds_face *Faces;
} hformat_4ds_mirror;

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
    
    s8 MeshName[255];
    s8 MeshParams[255];
    
    hformat_4ds_standard Standard;
    hformat_4ds_dummy Dummy;
    hformat_4ds_mirror Mirror;
    hformat_4ds_glow Glow;
    hformat_4ds_billboard Billboard;
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
            
            if(Mat.Flags & HFormatMaterialFlag_EnvironmentMap)
            {
                IOFileRead(FileIdx, &Mat.EnvRatio, sizeof(r32));
                u8 EnvNameLength = 0;
                IOFileRead(FileIdx, &EnvNameLength, sizeof(u8));
                IOFileRead(FileIdx, Mat.EnvMapName, EnvNameLength);
            }
            
            u8 DiffuseNameLength = 0;
            IOFileRead(FileIdx, &DiffuseNameLength, sizeof(u8));
            IOFileRead(FileIdx, Mat.DiffuseMapName, DiffuseNameLength);
            
            
            if(Mat.Flags & HFormatMaterialFlag_AlphaTexture)
            {
                u8 AlphaNameLength = 0;
                IOFileRead(FileIdx, &AlphaNameLength, sizeof(u8));
                IOFileRead(FileIdx, Mat.AlphaMapName, AlphaNameLength);
            }
            
            if(Mat.Flags & HFormatMaterialFlag_AnimatedTextureDiffuse ||
               Mat.Flags & HFormatMaterialFlag_AnimatedTextureAlpha)
            {
                
                IOFileRead(FileIdx, &Mat.AnimSequenceLength, sizeof(u32));
                IOFileRead(FileIdx, &Mat._Ignored0, sizeof(u16));
                IOFileRead(FileIdx, &Mat.FramePeriod, sizeof(u32));
                IOFileRead(FileIdx, &Mat._Ignored1, sizeof(u32));
                IOFileRead(FileIdx, &Mat._Ignored2, sizeof(u32));
                
            }
            
        }
        Model->Materials[Idx] = Mat;
    }
    
}

internal hformat_4ds_lod
HFormat4DSLoadLOD(s32 FileIdx)
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
            Lod = HFormat4DSLoadLOD(FileIdx);
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
    IOFileRead(FileIdx, Mirror._Ignored, sizeof(r32)*4);
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
            
            if(Mesh.MeshType & HFormatMeshType_Standard)
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
            
            u8 MeshNameLength;
            IOFileRead(FileIdx, &MeshNameLength, sizeof(u8));
            IOFileRead(FileIdx, Mesh.MeshName, MeshNameLength);
            
            u8 MeshParamsLength;
            IOFileRead(FileIdx, &MeshParamsLength, sizeof(u8));
            IOFileRead(FileIdx, Mesh.MeshParams, MeshParamsLength);
            
            switch(Mesh.MeshType)
            {
                case HFormatMeshType_Standard:
                {
                    switch(Mesh.VisualMeshType)
                    {
                        case HFormatVisualMeshType_Standard:
                        {
                            hformat_4ds_standard Standard = {0};
                            Standard = HFormatLoad4DSStandard(FileIdx);
                            Mesh.Standard = Standard;
                        }break;
                        
                        case HFormatVisualMeshType_Mirror:
                        {
                            hformat_4ds_mirror Mirror = {0};
                            Mirror = HFormatLoad4DSMirror(FileIdx);
                            Mesh.Mirror = Mirror;
                        }break;
                        
                        case HFormatVisualMeshType_Glow:
                        {
                            hformat_4ds_glow Glow = {0};
                            Glow = HFormatLoad4DSGlow(FileIdx);
                            Mesh.Glow = Glow;
                        }break;
                        
                        case HFormatVisualMeshType_Billboard:
                        {
                            hformat_4ds_billboard Billboard = {0};
                            Billboard.Standard = HFormatLoad4DSStandard(FileIdx);
                            IOFileRead(FileIdx, &Billboard.RotationAxis, sizeof(u32));
                            IOFileRead(FileIdx, &Billboard.IgnoreCamera, sizeof(u8));
                            Mesh.Billboard = Billboard;
                        }break;
                        
                        default:
                        {
                            fprintf(stderr, "Not Implemented [VisualMeshType]: %d\n", Mesh.VisualMeshType);
                        }break;
                    }
                }break;
                
                case HFormatMeshType_Dummy:
                {
                    hformat_4ds_dummy Dummy = {0};
                    IOFileRead(FileIdx, &Dummy, sizeof(hformat_4ds_dummy));
                    Mesh.Dummy = Dummy;
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
