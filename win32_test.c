// (c) 2016 ZaKlaus; All Rights Reserved

#define HANDMADE_SLOW

#include "hftw.h"

#include "formats/hformat_4ds.h"
#include "formats/hformat_klz.h"

global_variable b32 IsRunning;

LRESULT CALLBACK
WndProc(HWND hwnd,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_CLOSE:
        {
            IsRunning = 0;
        }break;
        
        default:
        {
            
        }break;
    }
    return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}


int CALLBACK
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow)
{
    WindowCreateClass(hInstance, &WndProc);
    
    window_dim PosDim = {0};
    window_dim ResDim = {0};
    ResDim.X = 800;
    ResDim.Y = 600;
    
    WindowCreateWindowed("Win32 Test", 0, 0, ResDim, PosDim, CW_USEDEFAULT);
    
    WindowUpdate();
    TimeInit();
    
    IsRunning = 0;
    r64 OldTime = TimeGet();
    
    // NOTE(zaklaus): 4ds test
    {
        s32 FileHandle = IOFileOpenRead("data/test.4ds", 0);
        hformat_4ds_header *Model = HFormatLoad4DSModel(FileHandle);
        IOFileClose(FileHandle);
        
        if(!Model)
        {
            fprintf(stderr, "Not a correct 4ds format!\n");
            return(1);
        }
        
        fprintf(stdout, "Signature: %.*s\n", 4, &Model->Signature);
        fprintf(stdout, "Format: %d\n", Model->FormatVersion);
        fprintf(stdout, "Timestamp: %lld\n", Model->Timestamp);
        fprintf(stdout, "Material Count: %d\n", Model->MaterialCount);
        
        for(mi Idx = 0;
            Idx < Model->MaterialCount;
            ++Idx)
        {
            hformat_4ds_material *Mat = &Model->Materials[Idx];
            
            fprintf(stdout, "Material ID: %zd\nDiffuseR: %f\n", Idx, Mat->Diffuse.R);
            fprintf(stdout, "DiffuseG: %f\n", Mat->Diffuse.G);
            fprintf(stdout, "DiffuseB: %f\n", Mat->Diffuse.B);
            fprintf(stdout, "Transparency: %f\n\n", Mat->Transparency);
            
            
        }
        
        fprintf(stdout, "Mesh Count: %d\n", Model->MeshCount);
        
        for(mi Idx = 0;
            Idx < Model->MeshCount;
            ++Idx)
        {
            hformat_4ds_mesh *Mesh = &Model->Meshes[Idx];
            
            fprintf(stdout, "\nMeshType ID: %d\n", Mesh->MeshType);
            fprintf(stdout, "MeshName: %s\n", Mesh->MeshName);
            fprintf(stdout, "Rotation X: %f\n", Mesh->Rot.X);
            fprintf(stdout, "Rotation W: %f\n", Mesh->Rot.W);
            
        }
        
    }
    
    while(IsRunning)
    {
        r64 NewTime = TimeGet();
        {
            WindowUpdate();
            Sleep(10);
        }
        OldTime = NewTime;
    }
    
    return(0);
}
