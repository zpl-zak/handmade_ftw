typedef struct
{
    enum
    {
        Uninitialized = 0x1,
        Brute,
        Fast
    } MONSTER_TYPE;
    
    enum 
    {
        Use_Grenades = 0x1
    } MONSTER_FLAGS;
    
    u8 HP;
    r32 MP;
    u8 EnemyType;
    u8 Damage;
    u8 Flags;
    Memory_Arena Properties;
} Monster;

#define M_BRUTE_SIZE sizeof(Monster_Brute)
typedef struct 
{
    u8 TimeLeftForKicking;
    u8 MadnessDamage;
} Monster_Brute;

static void
InitializeMonster(Monster * Enemy, u8 HP, u8 MP, u8 Damage, u8 Flags)
{
    Enemy->HP = HP;
    Enemy->MP = MP;
    Enemy->Damage = Damage;
    Enemy->Flags |= Flags;
    Enemy->EnemyType = Uninitialized;
}

static Monster
MakeBrute(void)
{
    Monster Result;
    InitializeMonster(&Result, 100, 25, 80, Use_Grenades);
    
    Result.EnemyType = Brute;
    
    BuildArena(&Result.Properties, M_BRUTE_SIZE);
    Monster_Brute * Properties = PushStruct(&Result.Properties, Monster_Brute, DefaultArenaParams());
    Properties->TimeLeftForKicking = 20;
    Properties->MadnessDamage = 64;
    
    return(Result);
}

static Monster
MakeHeavy(void)
{
    Monster Result = MakeBrute();
    Result.HP += 20;
    Result.MP *= 0.5f;
    Result.Damage += 5;
    
    Monster_Brute * Properties = (Monster_Brute *)((Result.Properties).Base);
    Properties->MadnessDamage *= 2;
    
    return Result;
}

static void
DisposeMonster(Monster * Enemy)
{
    FreeArena(&Enemy->Properties);
    InitializeMonster(Enemy, 0, 0, 0, 0);
}
