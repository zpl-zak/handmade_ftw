// (c) 2016 ZaKlaus; All Rights Reserved

#define HANDMADE_SLOW

#include "hftw.h"
#include <stdio.h>

#include "tests/monster.h"
#include "tests/ecs_test.h"

/* TEST SWITCHES */
#define MONSTER_DYNAMIC_PROPERTY 0
#define RANDOM_DATA 0
#define PLATFORM_ALLOC 0
#define STRING_TEST 1
#define MEMORY_REALLOC 0
#define MEMORY_HEADERS 0
#define ECS_TEST 1
#define DATA_PACKING 0

int
cmp(const void *a, const void *b)
{
    return( - *(int *)a + *(int *)b);
}

typedef struct
{
    s32 Cislo;
    u8 Height;
} some_data;



int
main(void)
{
    #if STRING_TEST
    {
        string hashTest = StringCreate(literal("hello"));
        s32 hash = hash_lit("hello");
        Assert(hashTest.StringHash == hash);
    }
    #endif
    
    
    #if ECS_TEST
    {
        #define ENTITY_COUNT 5
        
        entity Entities[ENTITY_COUNT] = {0};
        
        for(mi Idx = 0;
            Idx < ENTITY_COUNT;
            ++Idx)
        {
            entity *E = &Entities[Idx];
            
            E->ID = Idx;
            ArenaBuild(&E->Components, 4);
            E->Components.Flags |= ArenaFlag_AllowRealloc;
            
            transform_component T = {0}; vec3 P = {0};
            P.X = 1.f;
            T.P = P;
            AddComponent(E, Comp_Transform, &T, sizeof(T));
            
            // NOTE(zaklaus): Make special case for component ID 42
            if(Idx == 3)
            {
                 render_component R = { .RenderID = 42, .IsTransparent = 0, .MeshID = 1337 };
                AddComponent(E, Comp_Render, &R, sizeof(R));
            }
            
            
            // NOTE(zaklaus): Rendition
            {
                fprintf(stdout, "Entity ID: %zd\n", Idx);
                SystemTransform(E);
                SystemRender(E);
                fprintf(stdout, "END OF ENTITY\n\n");
            }
        }
    }
    #endif
    
    #if DATA_PACKING
    {
        some_data dat = {.Cislo = 42, .Height = 128};
        memory_arena TestArena;
        ArenaBuild(&TestArena, 1024);
        //TestArena.Flags |= ArenaFlag_DisallowHeaders;
        ArenaPushValue(&TestArena, s32, 45, ArenaDefaultParams());
        ArenaPushValue(&TestArena, some_data, dat, ArenaDefaultParams());
        ArenaPushValue(&TestArena, u8, 3, ArenaTag(42, ArenaDefaultParams()));
        ArenaPushValue(&TestArena, s32, 12, ArenaDefaultParams());
        ArenaPushValue(&TestArena, s32, 22, ArenaDefaultParams());
        
        size_t DataSize = 0;
          u8 * Data = ArenaSerialize(&TestArena, &DataSize);
        Assert(Data);
        printf("Packed size: %d, Arena size: %zd", (int)DataSize, *(mi *)Data);
        FILE *File = fopen("memdata.dat", "wb");
        fwrite(Data, 1, DataSize, File);
        fclose(File);
        
        FILE *File2 = fopen("memdata.dat", "rb");
        fseek(File2, 0, SEEK_END);
        int fsize = ftell(File2);
        rewind(File2);
        u8 *NewData = PlatformMemAlloc(fsize);
        fread(NewData, 1, fsize, File2);
        fclose(File2);
        
        memory_arena NextArena = {0};
        ArenaDeserialize(&NextArena, NewData);
        
        // NOTE(zaklaus): We need to free the de-serialized memory once we don't need it!
        PlatformMemFree(Data);
        
        printf("\nOld used size: %zu == New used size: %zu", TestArena.Used, NextArena.Used);
        printf("\n1st element: %d, Old 1st element: %d, New arena size: %zd", *(s32 *)NextArena.Base, *(s32 *)TestArena.Base, (mi)NextArena.Size);
        
        memory_arena ThirdArena;
        ArenaDuplicate(&NextArena, &ThirdArena);
        printf("\nDuplicated 1st element: %d, Old 1st element: %d, New arena size: %zd", *(s32 *)ThirdArena.Base, *(s32 *)NextArena.Base, (mi)NextArena.Size);
    }
    #endif
    
    fprintf(stdout, "\n");
    return(0);
}
