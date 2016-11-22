// (c) 2016 ZaKlaus; All Rights Reserved

#define HANDMADE_SLOW

#include "hftw.h"
#include <stdio.h>
#include <Windows.h>

#include "tests/monster.h"

/* TEST SWITCHES */
#define MONSTER_DYNAMIC_PROPERTY 1
#define RANDOM_DATA 1
#define PLATFORM_ALLOC 1
#define STRING_TEST 1

int
main(void)
{
#if MONSTER_DYNAMIC_PROPERTY // monster dynamic properties test
    Memory_Arena Enemies;
    BuildArena(&Enemies, sizeof(Monster) * 32);
    
    for(size_t i = 0;
        i < 32;
        i++)
    {
        Monster * Enemy = PushStruct(&Enemies, Monster, DefaultArenaParams());
        *Enemy = MakeBrute();
    }
    
    for(Monster * Instance = (Monster *)(Enemies.Base);
        Instance;
        Instance += sizeof(Monster))
    {
        Monster_Brute *Props = (Monster_Brute *)&(Instance->Properties.Base);
        fprintf(stdout, "Madness Damage: %d\n", Props->MadnessDamage);
    }
    
#endif
#if RANDOM_DATA // random data
    Memory_Arena Test;
    BuildArena(&Test, 1024);
    
    Memory_Arena DataTable;
    SubArena(&DataTable, &Test, 256, DefaultArenaParams());
    
    int * Data[64];
    
    for(s32 i=0;i<64;i++)
    {
        Data[i] = PushType(&DataTable, int, DefaultArenaParams());
        *Data[i] = i;
    }
    
    int * DataFactor = PushType(&Test, int, DefaultArenaParams());
    *DataFactor = 4;
    
    int * DataModified[64];
    
    Temp_Memory DataMemory = BeginTemporaryMemory(&Test);
    {
        for(size_t i=0;i<64;i++)
        {
            DataModified[i] =  PushType(&Test, int, DefaultArenaParams());
            *DataModified[i] = *Data[i] * *DataFactor;
            fprintf(stdout, "%d\n", *DataModified[i]);
        }
    }
    EndTemporaryMemory(DataMemory);
    CheckArena(&Test);
    
    int64 * P = PushType(&Test, int64, DefaultArenaParams());
    *P = 42;
    
    fprintf(stdout, "%lld\n", *((s64 *)DataModified[0]));
    
    CleanUnusedArenaRoom(&Test);
    
    fprintf(stdout, "%lld\n", *((s64 *)DataModified[0]));
#endif
#if PLATFORM_ALLOC
#define RAWMEM_SIZE sizeof(s32)*4
    Memory_Arena FourNumbers;
    BuildArena(&FourNumbers, RAWMEM_SIZE);
    
    for(mi Idx=0;
        Idx < 4;
        ++Idx)
    {
        s32 *Number = PushType(&FourNumbers, s32, DefaultArenaParams());
        *Number = (s32)Idx+1;
        fprintf(stdout, "Number: %d\n", *Number);
    }
    FreeArena(&FourNumbers);
#endif
    
    
#if STRING_TEST
    String Test1 = MakeString(literal("Hello World"));
    CheckString(&Test1);
    
    fprintf(stdout, "String: %s\nString Hash: %d\nString Length: %d",
            GetRawString(&Test1), Test1.StringHash, Test1.Length);
    
    char *EditTestStr = BeginStringEdit(&Test1);
    {
        EditTestStr[0] = 'a';
    }
    EndStringEdit(&Test1);
    
    CheckString(&Test1);
    
    fprintf(stdout, "\nAfter edit:\nString: %s\nString Hash: %d\nString Length: %d",
            GetRawString(&Test1), Test1.StringHash, Test1.Length);
    
    String LoremIpsum = MakeString(literal("Lorem Ipsum dolor sit amet"));
    String LoremIpsum2 = MakeString(literal("Lorem Ipsum dolor sit amet"));
    
    fprintf(stdout, "\nLoremIpsum == LoremIpsum2? %d", CompareStrings(&LoremIpsum, &LoremIpsum2));
    
    
#endif
    fprintf(stdout, "\n");
    return(0);
}