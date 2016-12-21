// (c) 2016 ZaKlaus; All Rights Reserved
/*
KLZ File Format
Usage: Collision format
SW: Mafia TCOLH
Contributor: ASM, DavoSK
*/
#if !defined(HFORMAT_KLZ_H)

typedef struct
{
    u32 Signature;				// GifC
    u32 Version;				// 5
    u32 CollisionDataOffset;
    u32 NumLinks;
    u32 _Unknown[2];
    
} hformat_klz_header;

typedef struct
{
    u32 Flags;
    u32 NameLength;
    s8 *Name;
} hformat_klz_link;	

#pragma pack(push, 1)
typedef struct
{
    f32 GridMinX;
    f32 GridMinY;
    f32 GridMaxX;
    f32 GridMaxY;
    f32 CellWidth;
    f32 CellHeight;
    u32 GridWidth;
    u32 GridHeight;
    u32 _Unknown0;
    u32 _Reserved0[2];
    u32 _Reserved1; 
    u32 NumFaces;
    u32 _Reserved2;
    u32 NumXTOBBs;
    u32 _Reserved3;
    u32 NumAAABBs;
    u32 _Reserved4;
    u32 NumSpheres;
    u32 _Reserved5;
    u32 NumOBBs;
    u32 _Reserved6;
    u32 NumCylinders;
    u32 _Reserved7;
    u32 NumUnknownType;  // always 0
    u32 _Unknown1;
    
}  hformat_klz_data_header;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    
    u32 Properties;		// NOTE(ASM): Material (8 bit) | Flags (8 bit) | SortInfo (8 bit) | 0 (8 bit)
    u32 Indices[3];		// NOTE(ASM): (Link (index into LinkNameOffsetTable) (16bit) | Index of vertex in mesh's vertex buffer (16 bit))					// plane the triangle lies in
    v3 Normal;			// NOTE(ASM): needs to point in opposite direction compared to the mesh face normal (IIRC!), i.e. if the mesh face normal is (1 0 0), the col face normal needs to be (-1 0 0)
    f32 Distance;
    
} hformat_klz_data_facecol;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    
    uint32 Properties;	// NOTE(ASM): Material(8 bit) | Flags (8 bit) | 0 (8 bit) | 0x81 (8 bit)
    u32 Link;			// NOTE(ASM): index into LinkNameOffsetTable
    v3 Min;
    v3 Max;
    
} hformat_klz_data_abbcol;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    
    uint32 Properties;	// NOTE(ASM): Material(8 bit) | Flags (8 bit) | 0 (8 bit) | 0x80 (8 bit)
    uint32 Link;
    // NOTE(ASM): AABB
    v3 Min;
    v3 Max;
    v3 Extends[2];
    m4 Transform;
    m4 InverseTransform;
    
} hformat_klz_data_xtobbcol; 
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    
    int32 Properties;	// NOTE(ASM): Material(8 bit) | Flags (8 bit) | 0 (8 bit) | 0x84 (8 bit)
    u32 Link;
    v2 Position;		// NOTE(ASM): cylinders only have a 2d position!
    float Radius;
    
} hformat_klz_data_cylindercol;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    
    u32 Properties;		// NOTE(ASM): Material(8 bit) | Flags (8 bit) | 0 (8 bit) | 0x83 (8 bit)
    u32 Link;
    v3 Extends[2];
    m4 Transform;
    m4 InverseTransform;
    
} hformat_klz_data_obbcol;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
    
    u32 Properties;		// NOTE(ASM): Material(8 bit) | Flags (8 bit) | 0 (8 bit) | 0x82 (8 bit)
    u32 Link;
    v3 Position;
    f32 Radius;
    
} hformat_klz_data_spherecol;
#pragma pack(pop)

typedef struct {
    u32 NumObjects;
    u32 Reserved[2];
    f32 Height;
    u32* References;  // NOTE(ASM): (Type (8 bit)) | (Offset into array of Type (24 bit)))
    u8* Flags;
} hformat_klz_cell;

enum
{
    HFormatKLZCollisionGridReference_Face = 0x00,
    HFormatKLZCollisionGridReference_XTOBB = 0x80,
    HFormatKLZCollisionGridReference_AABB = 0x81,
    HFormatKLZCollisionGridReference_Sphere = 0x82,
    HFormatKLZCollisionGridReference_OBB = 0x83,
    HFormatKLZCollisionGridReference_Cylinder = 0x84
};

typedef struct
{
    hformat_klz_header KlzHeader;
    u32* LinkNameOffsetTable;
    hformat_klz_link* LinkTables;
    f32* CellBoundariesX;
    f32* CellBoundariesY;
    hformat_klz_data_header DataHeader;
    u32 CollisionDataMagic;
    hformat_klz_data_facecol* FaceCols;
    hformat_klz_data_abbcol* ABBCols;
    hformat_klz_data_xtobbcol* XTOBBCols;
    hformat_klz_data_cylindercol* CylinderCols;
    hformat_klz_data_obbcol* OBBCols;
    hformat_klz_data_spherecol* SphereCols;
    u32 CollisionGridMagic;
    hformat_klz_cell* GridCellsMemory;
    
} hformat_klz;

hformat_klz
ParseKlzFile(s32 FileIdx)
{
    hformat_klz KLZ = { 0 };
    
    IOFileRead(FileIdx, &KLZ.KlzHeader, sizeof(hformat_klz_header));
    
    KLZ.LinkNameOffsetTable = malloc( sizeof(u32) * KLZ.KlzHeader.NumLinks );
    IOFileRead(FileIdx, KLZ.LinkNameOffsetTable, sizeof(u32)*KLZ.KlzHeader.NumLinks);
    KLZ.LinkTables = malloc(sizeof(hformat_klz_link)* KLZ.KlzHeader.NumLinks);
    
    for (u32 i = 0; i < KLZ.KlzHeader.NumLinks; i++)
    {
        hformat_klz_link newTable = { 0 };
        
        IOFileSeek(FileIdx, KLZ.LinkNameOffsetTable[i], SeekOrigin_Set);
        IOFileRead(FileIdx, &newTable.Flags, sizeof(u32)); 
        
        u32 NameLength = IOGetStringLength(FileIdx);
        newTable.NameLength = NameLength;
        newTable.Name = malloc(NameLength); 
        IOFileRead(FileIdx, newTable.Name, sizeof(u8)*NameLength); 
        
        KLZ.LinkTables[i] = newTable;
    }
    
    IOFileSeek(FileIdx, KLZ.KlzHeader.CollisionDataOffset, SeekOrigin_Set);
    IOFileRead(FileIdx, &KLZ.DataHeader, sizeof(hformat_klz_data_header));
    
    KLZ.CellBoundariesX = malloc(sizeof(f32) * (KLZ.DataHeader.GridWidth + 1));
    IOFileRead(FileIdx, KLZ.CellBoundariesX, sizeof(f32) * (KLZ.DataHeader.GridWidth + 1));
    
    KLZ.CellBoundariesY = malloc(sizeof(f32) * (KLZ.DataHeader.GridHeight + 1)); 
    IOFileRead(FileIdx, KLZ.CellBoundariesY, sizeof(f32) * (KLZ.DataHeader.GridHeight + 1));
    
    IOFileRead(FileIdx, &KLZ.CollisionDataMagic, sizeof(u32));
    
    KLZ.FaceCols = malloc(KLZ.DataHeader.NumFaces * sizeof(hformat_klz_data_facecol));
    IOFileRead(FileIdx, KLZ.FaceCols, sizeof(hformat_klz_data_facecol)*KLZ.DataHeader.NumFaces);
    
    KLZ.ABBCols = malloc(KLZ.DataHeader.NumAAABBs * sizeof(hformat_klz_data_abbcol));
    IOFileRead(FileIdx, KLZ.ABBCols, sizeof(hformat_klz_data_abbcol)*KLZ.DataHeader.NumAAABBs);
    
    KLZ.XTOBBCols = malloc(KLZ.DataHeader.NumXTOBBs * sizeof(hformat_klz_data_xtobbcol));
    IOFileRead(FileIdx, KLZ.XTOBBCols, sizeof(hformat_klz_data_xtobbcol)*KLZ.DataHeader.NumXTOBBs);
    
    KLZ.CylinderCols = malloc(KLZ.DataHeader.NumCylinders * sizeof(hformat_klz_data_cylindercol));
    IOFileRead(FileIdx, KLZ.CylinderCols, sizeof(hformat_klz_data_cylindercol)*KLZ.DataHeader.NumCylinders);
    
    KLZ.OBBCols = malloc(KLZ.DataHeader.NumOBBs * sizeof(hformat_klz_data_obbcol));
    IOFileRead(FileIdx, KLZ.OBBCols, sizeof(hformat_klz_data_obbcol)*KLZ.DataHeader.NumOBBs);
    
    KLZ.SphereCols = malloc(KLZ.DataHeader.NumSpheres * sizeof(hformat_klz_data_spherecol));
    IOFileRead(FileIdx, KLZ.SphereCols, sizeof(hformat_klz_data_spherecol)*KLZ.DataHeader.NumSpheres);
    
    IOFileRead(FileIdx, &KLZ.CollisionGridMagic, sizeof(u32));
    
    u32 girdsize = KLZ.DataHeader.GridWidth * KLZ.DataHeader.GridHeight; 
    KLZ.GridCellsMemory = malloc(sizeof(hformat_klz_cell) * girdsize);
    
    for (u32 i = 0; i < girdsize; i++) 
    {
        IOFileRead(FileIdx, &KLZ.GridCellsMemory[i].NumObjects, sizeof(u32));
        IOFileRead(FileIdx, KLZ.GridCellsMemory[i].Reserved, sizeof(u32)*2); IOFileRead(FileIdx, &KLZ.GridCellsMemory[i].Height, sizeof(f32));
        
        if (KLZ.GridCellsMemory[i].NumObjects)
        {
            KLZ.GridCellsMemory[i].References = malloc(sizeof(u32) * KLZ.GridCellsMemory[i].NumObjects);
            IOFileRead(FileIdx, KLZ.GridCellsMemory[i].References, sizeof(u32)*KLZ.GridCellsMemory[i].NumObjects);
            
            // NOTE(ASM): needs to be aligned to 4 bytes, purpose unknown
            KLZ.GridCellsMemory[i].Flags = malloc((KLZ.GridCellsMemory[i].NumObjects + 3) / 4 * sizeof(u32));
            IOFileRead(FileIdx, KLZ.GridCellsMemory[i].Flags, (KLZ.GridCellsMemory[i].NumObjects + 3) / 4 * sizeof(u32));
        }
    }
    
    return KLZ;
}


#define HFORMAT_KLZ_H
#endif