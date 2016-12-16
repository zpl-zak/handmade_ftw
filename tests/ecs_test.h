// (c) 2016 ZaKlaus; All Rights Reserved

#define COMPONENT_CHUNK 256

typedef struct
{
    u64 ID;
    memory_arena Components;
} entity;

typedef enum
{
    Comp_Null,
    Comp_Transform,
    Comp_Render,
    Num_Of_Components
} component_id;

typedef struct
{
    vec3 P;
    vec3 dP;
    vec2 R;
} transform_component;

typedef struct
{
    u32 RenderID;
    b32 IsTransparent;
    u32 MeshID;
} render_component;

internal void
AddComponent(entity *E, component_id CompID, void *Component, ms Size)
{
    void *C = ArenaPushSize_(&E->Components, 
                             Size, 
                             ArenaTag(CompID, 
                                      ArenaExpect(
        COMPONENT_CHUNK, 0)));
    
    Copy(Size, Component, C);
}

internal void
SystemTransform(entity *E)
{
    transform_component *T = ArenaGetVaryBlockTagValue(&E->Components, ArenaDefaultTagScan(), transform_component, Comp_Transform);
    
    if(T)
    {
        fprintf(stdout, "Transform Pos [%f, %f, %f]\n", T->P.X, T->P.Y, T->P.Z);
    }
}

internal void
SystemRender(entity *E)
{
    render_component *R = ArenaGetVaryBlockTagValue(&E->Components, ArenaDefaultTagScan(), render_component, Comp_Render);
    
    if(R)
    {
        fprintf(stdout, "Render ID: %d\n", R->RenderID);
    }
}