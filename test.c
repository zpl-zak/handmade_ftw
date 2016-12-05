// (c) 2016 ZaKlaus; All Rights Reserved

#define HANDMADE_SLOW

#include "hftw.h"
#include <stdio.h>
#include <Windows.h>

#include "tests/monster.h"

/* TEST SWITCHES */
#define MONSTER_DYNAMIC_PROPERTY 0
#define RANDOM_DATA 0
#define PLATFORM_ALLOC 0
#define STRING_TEST 1
#define MEMORY_REALLOC 0
#define MEMORY_HEADERS 0
#define ECS_TEST 1

int
cmp(const void *a, const void *b)
{
    return( - *(int *)a + *(int *)b);
}

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
    
#if 0
    {
        int values[] = { 3, 1, 2, 7, 9, 0, 6 };
        qsort(values, 7, sizeof(int), cmp);
        
        
        for(mi Idx = 0;
            Idx < 7;
            ++Idx)
        {
            printf("%d ", values[Idx]);
        }
    }
    #endif
    
    #if ECS_TEST
    {
        // TODO(zaklaus): T.U.
    }
    #endif
    
    fprintf(stdout, "\n");
    return(0);
}