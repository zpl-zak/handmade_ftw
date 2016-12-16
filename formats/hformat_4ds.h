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
} hformat_4ds_mesh;

typedef struct
{
    u8 Signature[4];
    u16 FormatVersion; // PC : 0x1D (29)
    u64 Timestamp;
    
    u16 MaterialCount;
     memory_arena Materials;
    
    u16 MeshCount;
    memory_arena Meshes;
} hformat_4ds_header;

internal void
HFormatLoad4DSMaterial(hformat_4ds_header *Model, FILE *File)
{
    ArenaBuild(&Model->Materials, sizeof(hformat_4ds_material)*Model->MaterialCount);
    
    for(mi Idx = 0;
        Idx < Model->MaterialCount;
        ++Idx)
    {
        hformat_4ds_material Mat = {0};
        {
            ms msize = sizeof(r32);
            fread(&Mat.Flags, msize, 1, File);
            fread(&Mat.Ambient, sizeof(v3), 1, File);
            fread(&Mat.Diffuse, sizeof(v3), 1, File);
            fread(&Mat.Emission, sizeof(v3), 1, File);
            fread(&Mat.Transparency, msize, 1, File);
            
            if(Mat.Flags & HFormatMaterialFlag_EnvironmentMap)
            {
                fread(&Mat.EnvRatio, sizeof(r32), 1, File);
                u8 EnvNameLength = 0;
                fread(&EnvNameLength, sizeof(u8), 1, File);
                fread(Mat.EnvMapName, sizeof(s8), EnvNameLength, File);
            }
            
            u8 DiffuseNameLength = 0;
            fread(&DiffuseNameLength, sizeof(u8), 1, File);
            fread(Mat.DiffuseMapName, sizeof(s8), DiffuseNameLength, File);
            
            
            if(Mat.Flags & HFormatMaterialFlag_AlphaTexture)
            {
                u8 AlphaNameLength = 0;
                fread(&AlphaNameLength, sizeof(u8), 1, File);
                fread(Mat.AlphaMapName, sizeof(s8), AlphaNameLength, File);
            }
            
            if(Mat.Flags & HFormatMaterialFlag_AnimatedTextureDiffuse ||
               Mat.Flags & HFormatMaterialFlag_AnimatedTextureAlpha)
            {
                
                fread(&Mat.AnimSequenceLength, sizeof(u32), 1, File);
                fread(&Mat._Ignored0, sizeof(u16), 1, File);
                fread(&Mat.FramePeriod, sizeof(u32), 1, File);
                fread(&Mat._Ignored1, sizeof(u32), 1, File);
                fread(&Mat._Ignored2, sizeof(u32), 1, File);
                
            }
            
        }
        ArenaPushValue(&Model->Materials, hformat_4ds_material, Mat, ArenaDefaultParams());
    }
    
}

internal void
HFormatLoad4DSMesh(hformat_4ds_header *Model, FILE *File)
{
    fread(&Model->MeshCount, sizeof(u16), 1, File);
    
    ArenaBuild(&Model->Meshes, sizeof(hformat_4ds_mesh)*Model->MeshCount);
    
    for(mi Idx = 0;
        Idx < Model->MeshCount;
        ++Idx)
    {
        hformat_4ds_mesh Mesh = {0};
        {
            fread(&Mesh.MeshType, sizeof(u8), 1, File);
            
            if(Mesh.MeshType & HFormatMeshType_Standard)
            {
                fread(&Mesh.VisualMeshType, sizeof(u8), 1, File);
                fread(&Mesh.MeshRenderFlags, sizeof(u16), 1, File);
            }
            
            fread(&Mesh.ParentID, sizeof(u16), 1, File);
            
            fread(&Mesh.Pos, sizeof(v3), 1, File);
            fread(&Mesh.Scale, sizeof(v3), 1, File);
            fread(&Mesh.Rot, sizeof(v4), 1, File);
            

            f32 _Swap0 = Mesh.Rot.W;
            f32 _Swap1 = Mesh.Rot.X;
            f32 _Swap2 = Mesh.Rot.Y;
            f32 _Swap3 = Mesh.Rot.Z;
            
            Mesh.Rot.X = _Swap1;
            Mesh.Rot.Y = _Swap2;
            Mesh.Rot.Z = _Swap3;
            Mesh.Rot.W = _Swap0;
            
            fread(&Mesh.CullingFlags, sizeof(u8), 1, File);
            
            u8 MeshNameLength;
            fread(&MeshNameLength, sizeof(u8), 1, File);
            fread(Mesh.MeshName, sizeof(s8), MeshNameLength, File);
            
            u8 MeshParamsLength;
            fread(&MeshParamsLength, sizeof(u8), 1, File);
            fread(Mesh.MeshParams, sizeof(s8), MeshParamsLength, File);
        }
        ArenaPushValue(&Model->Meshes, hformat_4ds_mesh, Mesh, ArenaDefaultParams());
    }
}

internal hformat_4ds_header *
HFormatLoad4DSModel(s8 *Path)
{
    hformat_4ds_header *Model = PlatformMemAlloc(sizeof(hformat_4ds_header));
    FILE *File = fopen((char *)Path, "rb");
    {
        fread(&Model->Signature, sizeof(u8), 4, File);
        fread(&Model->FormatVersion, sizeof(u16), 1, File);
        fread(&Model->Timestamp, sizeof(u64), 1, File);
        fread(&Model->MaterialCount, sizeof(u16), 1, File);
        
        HFormatLoad4DSMaterial(Model, File);
        HFormatLoad4DSMesh(Model, File);
    }
    fclose(File);
    return(Model);
}

#define HFORMAT_4DS_H
#endif