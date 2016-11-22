// (c) 2016 ZaKlaus; All Rights Reserved
// Code author: Zakary "zak" Strange

#ifndef HFTW_MATH_H
#define HFTW_MATH_H

#include <math.h> // TODO(zak): Remove this later on

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef HANDMADEMATH_STATIC
#define HFTWDEF static
#else
#define HFTWDEF extern
#endif

#if _MSC_VER && !__INTEL_COMPILER
#define HINLINE __inline
#else
#define HINLINE inline
#endif

#define HFTW_PI32 3.14159265359f
#define HFTW_PI 3.14159265358979323846f

#define HFTW_MIN(a, b) (a) > (b) ? (b) : (a)
#define HFTW_MAX(a, b) (a) < (b) ? (b) : (a)
#define HMN_ABS(a) (a) < 0 ? -(a) : (a)
#define HFTW_MOD(a, m) ((a) % (m)) >= 0 ? ((a) % (m)) : (((a) % (m)) + (m))
#define HFTW_SQUARE(x) ((x) * (x))

typedef union hmm_vec2
{
    struct
    {
        float X, Y;
    };

    struct
    {
        float U, V;
    };

    struct
    {
        float Left, Right;
    };

    float Elements[2];
} hmm_vec2;

typedef union hmm_vec3
{
    struct
    {
        float X, Y, Z;
    };

    struct
    {
        float U, V, W;
    };

    struct
    {
        float R, G, B;
    };

    struct
    {
        hmm_vec2 XY;
        float Ignored0_;
    };

    struct
    {
        float Ignored1_;
        hmm_vec2 YZ;
    };

    struct
    {
        hmm_vec2 UV;
        float Ignored2_;
    };

    struct
    {
        float Ignored3_;
        hmm_vec2 VW;
    };

    float Elements[3];
} hmm_vec3;

typedef union hmm_vec4
{
    struct
    {
        union
        {
            hmm_vec3 XYZ;
            struct
            {
                float X, Y, Z;
            };
        };

        float W;
    };
    struct
    {
        union
        {
            hmm_vec3 RGB;
            struct
            {
                float R, G, B;
            };
        };

        float A;
    };

    struct
    {
        hmm_vec2 XY;
        float Ignored0_;
        float Ignored1_;
    };

    struct
    {
        float Ignored2_;
        hmm_vec2 YZ;
        float Ignored3_;
    };

    struct
    {
        float Ignored4_;
        float Ignored5_;
        hmm_vec2 ZW;
    };

    float Elements[4];
} hmm_vec4;

typedef union hmm_mat4
{
    float Elements[4][4];
} hmm_mat4;

typedef hmm_vec2 hmm_v2;
typedef hmm_vec3 hmm_v3;
typedef hmm_vec4 hmm_v4;
typedef hmm_mat4 hmm_m4;    

HFTWDEF float HFTW_ToRadians(float Degrees);
HFTWDEF float HFTW_Inner(hmm_vec3 A, hmm_vec3 B);
HFTWDEF float HFTW_SquareRoot(float Float);
HFTWDEF float HFTW_LengthSquareRoot(hmm_vec3 A);
HFTWDEF float HFTW_FastInverseSquareRoot(float Number);
HFTWDEF float HFTW_Length(hmm_vec3 A);    
HFTWDEF float HFTW_Power(float Base, int Exponent);
HFTWDEF float HFTW_Clamp(float Min, float Value, float Max);

HFTWDEF hmm_vec3 HFTW_Normalize(hmm_vec3 A);
HFTWDEF hmm_vec3 HFTW_Cross(hmm_vec3 VecOne, hmm_vec3 VecTwo);
HFTWDEF float HFTW_Dot(hmm_vec3 VecOne, hmm_vec3 VecTwo);

HFTWDEF hmm_vec2 HFTW_Vec2i(int X, int Y);
HFTWDEF hmm_vec2 HFTW_Vec2(float X, float Y);
HFTWDEF hmm_vec3 HFTW_Vec3(float X, float Y, float Z);
HFTWDEF hmm_vec3 HFTW_Vec3i(int X, int Y, int Z);
HFTWDEF hmm_vec4 HFTW_Vec4(float X, float Y, float Z, float W);
HFTWDEF hmm_vec4 HFTW_Vec4i(int X, int Y, int Z, int W);

HFTWDEF hmm_vec2 HFTW_AddVec2(hmm_vec2 Left, hmm_vec2 Right);
HFTWDEF hmm_vec3 HFTW_AddVec3(hmm_vec3 Left, hmm_vec3 Right);
HFTWDEF hmm_vec4 HFTW_AddVec4(hmm_vec4 Left, hmm_vec4 Right);

HFTWDEF hmm_vec2 HFTW_SubtractVec2(hmm_vec2 Left, hmm_vec2 Right);
HFTWDEF hmm_vec3 HFTW_SubtractVec3(hmm_vec3 Left, hmm_vec3 Right);
HFTWDEF hmm_vec4 HFTW_SubtractVec4(hmm_vec4 Left, hmm_vec4 Right);

HFTWDEF hmm_vec2 HFTW_MultiplyVec2(hmm_vec2 Left, hmm_vec2 Right);
HFTWDEF hmm_vec3 HFTW_MultiplyVec3(hmm_vec3 Left, hmm_vec3 Right);
HFTWDEF hmm_vec4 HFTW_MultiplyVec4(hmm_vec4 Left, hmm_vec4 Right);

HFTWDEF hmm_vec2 HFTW_DivideVec2(hmm_vec2 Left, hmm_vec2 Right);
HFTWDEF hmm_vec3 HFTW_DivideVec3(hmm_vec3 Left, hmm_vec3 Right);
HFTWDEF hmm_vec4 HFTW_DivideVec4(hmm_vec4 Left, hmm_vec4 Right);

HFTWDEF hmm_mat4 HFTW_Mat4(void);
HFTWDEF hmm_mat4 HFTW_Mat4d(float Diagonal);
HFTWDEF hmm_mat4 HFTW_MultiplyMat4(hmm_mat4 Left, hmm_mat4 Right);
HFTWDEF hmm_vec4 HFTW_MultiplyMat4ByVec4(hmm_mat4 Matrix, hmm_vec4 Vector);

HFTWDEF hmm_mat4 HFTW_Orthographic(float Left, float Right, float Bottom, float Top, float Near, float Far);
HFTWDEF hmm_mat4 HFTW_Perspective(float FOV, float AspectRatio, float Near, float Far);
HFTWDEF hmm_mat4 HFTW_Translate(hmm_vec3 Translation);
HFTWDEF hmm_mat4 HFTW_Rotate(float Angle, hmm_vec3 Axis);
HFTWDEF hmm_mat4 HFTW_Scale(hmm_vec3 Scale);

HFTWDEF hmm_mat4 HFTW_LookAt(hmm_vec3 Eye, hmm_vec3 Center, hmm_vec3 Up);

#ifdef __cplusplus
}
#endif

#ifdef HANDMADE_MATH_CPP_MODE

HFTWDEF hmm_vec2 HFTW_Add(int X, int Y);
HFTWDEF hmm_vec3 HFTW_Add(int X, int Y, int Z);
HFTWDEF hmm_vec4 HFTW_Add(int X, int Y, int Z, int W);

HFTWDEF hmm_vec2 HFTW_Subtract(int X, int Y);
HFTWDEF hmm_vec3 HFTW_Subtract(int X, int Y, int Z);
HFTWDEF hmm_vec4 HFTW_Subtract(int X, int Y, int Z, int W);

HFTWDEF hmm_vec2 HFTW_Multiply(int X, int Y);
HFTWDEF hmm_vec3 HFTW_Multiply(int X, int Y, int Z);
HFTWDEF hmm_vec4 HFTW_Multiply(int X, int Y, int Z, int W);
HFTWDEF hmm_mat4 HFTW_Multiply(hmm_mat4 Left, hmm_mat4 Right);
HFTWDEF hmm_vec4 HFTW_Multiply(hmm_mat4 Matrix, hmm_vec4 Vector);

HFTWDEF hmm_vec2 HFTW_Divide(int X, int Y);
HFTWDEF hmm_vec3 HFTW_Divide(int X, int Y, int Z);
HFTWDEF hmm_vec4 HFTW_Divide(int X, int Y, int Z, int W);

HFTWDEF hmm_vec2 operator+(hmm_vec2 Left, hmm_vec2 Right);
HFTWDEF hmm_vec3 operator+(hmm_vec3 Left, hmm_vec3 Right);
HFTWDEF hmm_vec4 operator+(hmm_vec4 Left, hmm_vec4 Right);

HFTWDEF hmm_vec2 operator-(hmm_vec2 Left, hmm_vec2 Right);
HFTWDEF hmm_vec3 operator-(hmm_vec3 Left, hmm_vec3 Right);
HFTWDEF hmm_vec4 operator-(hmm_vec4 Left, hmm_vec4 Right);

HFTWDEF hmm_vec2 operator*(hmm_vec2 Left, hmm_vec2 Right);
HFTWDEF hmm_vec3 operator*(hmm_vec3 Left, hmm_vec3 Right);
HFTWDEF hmm_vec4 operator*(hmm_vec4 Left, hmm_vec4 Right);
HFTWDEF hmm_mat4 operator*(hmm_mat4 Left, hmm_mat4 Right);

HFTWDEF hmm_vec3 operator*(hmm_vec3 Left, float Right);
HFTWDEF hmm_vec2 operator*(hmm_vec2 Left, float Right);

HFTWDEF hmm_vec4 operator*(hmm_mat4 Matrix, hmm_vec4 Vector);

HFTWDEF hmm_vec2 operator/(hmm_vec2 Left, hmm_vec2 Right);
HFTWDEF hmm_vec3 operator/(hmm_vec3 Left, hmm_vec3 Right);
HFTWDEF hmm_vec4 operator/(hmm_vec4 Left, hmm_vec4 Right);

#endif /* HANDMADE_MATH_CPP */

#endif /* HANDMADE_MATH_H */

#ifdef HANDMADE_MATH_IMPLEMENTATION

HINLINE float
HFTW_ToRadians(float Degrees)
{
    float Result = Degrees * (HFTW_PI32 / 180.0f);

    return (Result);
}

HINLINE float
HFTW_Inner(hmm_vec3 A, hmm_vec3 B)
{
    float Result = A.X * B.X + A.Y * B.Y + A.Z * B.Z;

    return (Result);
}

HINLINE float
HFTW_SquareRoot(float Float)
{    
    float Result = sqrtf(Float);

    return(Result);
}

HINLINE float
HFTW_LengthSquareRoot(hmm_vec3 A)
{
    float Result = HFTW_Inner(A, A);

    return (Result);
}

// Refer to https://en.wikipedia.org/wiki/Fast_inverse_square_root
HINLINE float
HFTW_FastInverseSquareRoot(float Number)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5f;

    x2 = Number * 0.5f;
    y  = Number;
    i  = * ( long * ) &y;          // evil floating point bit level hacking
    i  = 0x5f3759df - ( i >> 1 );  // what the fuck? 
    y  = * ( float * ) &i;
    
    y  = y * ( threehalfs - ( x2 * y * y ) );

    return ( y );
}

HINLINE float
HFTW_Length(hmm_vec3 A)
{
    float Result = HFTW_SquareRoot(HFTW_LengthSquareRoot(A));
    return (Result);
}

HINLINE float
HFTW_Power(float Base, int Exponent)
{
    float Result = 1;
    if(Exponent > 0)
    {
        int i;
        for(i = 0; i < Exponent; ++i)
        {
            Result *= Base;
        }
    }
    else
    {
        int i;
        for(i = 0; i > Exponent; --i)
        {
            Result /= Base;
        }
    }
    return (Result);
}

HINLINE float
HFTW_Lerp(float A, float Time, float B)
{
    float Result = (1.0f - Time) * A + Time * B;

    return (Result);
}

HINLINE float
HFTW_Clamp(float Min, float Value, float Max)
{
    float Result = Value;

    if(Result < Min)
    {
        Result = Min;
    }
    else if(Result > Max)
    {
        Result = Max;
    }

    return (Result);
}

HINLINE hmm_vec3
HFTW_Normalize(hmm_vec3 A)
{
    hmm_vec3 Result = {0};

    Result.X = A.X * (1.0f / HFTW_Length(A));
    Result.Y = A.Y * (1.0f / HFTW_Length(A));
    Result.Z = A.Z * (1.0f / HFTW_Length(A));
    
    return (Result);
}

HINLINE hmm_vec3
HFTW_Cross(hmm_vec3 VecOne, hmm_vec3 VecTwo)
{
    hmm_vec3 Result;

    Result.X = (VecOne.Y * VecTwo.Z) - (VecOne.Z * VecTwo.Y);
    Result.Y = (VecOne.Z * VecTwo.X) - (VecOne.X * VecTwo.Z);
    Result.Z = (VecOne.X * VecTwo.Y) - (VecOne.Y * VecTwo.X);

    return (Result);
}

HINLINE float
HFTW_Dot(hmm_vec3 VecOne, hmm_vec3 VecTwo)
{
    float Result = 0;

    Result = (VecOne.X * VecTwo.X) + (VecOne.Y * VecTwo.Y) + (VecOne.Z * VecTwo.Z);

    return (Result);
}

HINLINE hmm_vec2
HFTW_Vec2(float X, float Y)
{
    hmm_vec2 Result;

    Result.X = X;
    Result.Y = Y;

    return (Result);
}

HINLINE hmm_vec2
HFTW_Vec2i(int X, int Y)
{
    hmm_vec2 Result;

    Result.X = (float)X;
    Result.Y = (float)Y;

    return (Result);
}

HINLINE hmm_vec3
HFTW_Vec3(float X, float Y, float Z)
{
    hmm_vec3 Result;

    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;

    return (Result);
}

HINLINE hmm_vec3
HFTW_Vec3i(int X, int Y, int Z)
{
    hmm_vec3 Result;

    Result.X = (float)X;
    Result.Y = (float)Y;
    Result.Z = (float)Z;

    return (Result);
}

HINLINE hmm_vec4
HFTW_Vec4(float X, float Y, float Z, float W)
{
    hmm_vec4 Result;

    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    Result.W = W;

    return (Result);
}

HINLINE hmm_vec4
HFTW_Vec4i(int X, int Y, int Z, int W)
{
    hmm_vec4 Result;

    Result.X = (float)X;
    Result.Y = (float)Y;
    Result.Z = (float)Z;
    Result.W = (float)W;

    return (Result);
}

HINLINE hmm_vec2
HFTW_AddVec2(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result;

    Result.X = Left.X + Right.X;
    Result.Y = Left.Y + Right.Y;

    return (Result);
}

HINLINE hmm_vec3
HFTW_AddVec3(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result;

    Result.X = Left.X + Right.X;
    Result.Y = Left.Y + Right.Y;
    Result.Z = Left.Z + Right.Z;

    return (Result);
}

HINLINE hmm_vec4
HFTW_AddVec4(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result;

    Result.X = Left.X + Right.X;
    Result.Y = Left.Y + Right.Y;
    Result.Z = Left.Z + Right.Z;
    Result.W = Left.W + Right.W;

    return (Result);
}

HINLINE hmm_vec2
HFTW_SubtractVec2(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result;

    Result.X = Left.X - Right.X;
    Result.Y = Left.Y - Right.Y;

    return (Result);
}

HINLINE hmm_vec3
HFTW_SubtractVec3(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result;

    Result.X = Left.X - Right.X;
    Result.Y = Left.Y - Right.Y;
    Result.Z = Left.Z - Right.Z;

    return (Result);
}

HINLINE hmm_vec4
HFTW_SubtractVec4(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result;

    Result.X = Left.X - Right.X;
    Result.Y = Left.Y - Right.Y;
    Result.Z = Left.Z - Right.Z;
    Result.W = Left.W - Right.W;

    return (Result);
}

HINLINE hmm_vec2
HFTW_MultiplyVec2(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result;

    Result.X = Left.X * Right.X;
    Result.Y = Left.Y * Right.Y;

    return (Result);
}

HINLINE hmm_vec3
HFTW_MultiplyVec3(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result;

    Result.X = Left.Z * Right.X;
    Result.Y = Left.Y * Right.Y;
    Result.Z = Left.Z * Right.Z;

    return (Result);
}

HINLINE hmm_vec4
HFTW_MultiplyVec4(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result;

    Result.X = Left.X * Right.X;
    Result.Y = Left.Y * Right.Y;
    Result.Z = Left.Z * Right.Z;
    Result.W = Left.W * Right.W;

    return (Result);
}

HINLINE hmm_vec2
HFTW_DivideVec2(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result;

    Result.X = Left.X / Right.X;
    Result.Y = Left.Y / Right.Y;

    return (Result);
}

HINLINE hmm_vec3
HFTW_DivideVec3(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result;

    Result.X = Left.X / Right.X;
    Result.Y = Left.Y / Right.Y;
    Result.Z = Left.Z / Right.Z;

    return (Result);
}

HINLINE hmm_vec4
HFTW_DivideVec4(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result;

    Result.X = Left.X / Right.X;
    Result.Y = Left.Y / Right.Y;
    Result.Z = Left.Z / Right.Z;
    Result.W = Left.W / Right.W;

    return (Result);
}

HINLINE hmm_mat4
HFTW_Mat4()
{
    hmm_mat4 Result = {0};

    return (Result);
}

hmm_mat4
HFTW_Mat4d(float Diagonal)
{
    hmm_mat4 Result;

    int Rows;
    for(Rows = 0; Rows < 4; ++Rows)
    {
        int Columns;
        for(Columns = 0; Columns < 4; ++Columns)
        {
            Result.Elements[Rows][Columns] = 0.0f;
        }
    }

    Result.Elements[0][0] = Diagonal;
    Result.Elements[1][1] = Diagonal;
    Result.Elements[2][2] = Diagonal;
    Result.Elements[3][3] = Diagonal;

    return (Result);
}

hmm_mat4
HFTW_MultiplyMat4(hmm_mat4 Left, hmm_mat4 Right)
{
    hmm_mat4 Result = HFTW_Mat4();

    int Rows;
    for(Rows = 0; Rows < 4; ++Rows)
    {
        int Columns;
        for(Columns = 0; Columns < 4; ++Columns)
        {
            float Sum = 0;
            int CurrentMatrice;
            for(CurrentMatrice = 0; CurrentMatrice < 4; ++CurrentMatrice)
            {
                Sum += Right.Elements[Rows][CurrentMatrice] * Left.Elements[CurrentMatrice][Columns];
            }

            Result.Elements[Rows][Columns] = Sum;
        }
    }

    return (Result);
}

hmm_vec4
HFTW_MultiplyMat4ByVec4(hmm_mat4 Matrix, hmm_vec4 Vector)
{
    hmm_vec4 Result = HFTW_Vec4(0.0f, 0.0f, 0.0f, 0.0f);
    
    int Rows, Columns;
    for(Rows = 0; Rows < 4; ++Rows)
    {
        float Sum = 0;
        for(Columns = 0; Columns < 4; ++Columns)
        {
            Sum += Matrix.Elements[Rows][Columns] * Vector.Elements[Columns];
        }
        
        Result.Elements[Rows] = Sum;
    }
    
    return (Result);
}

hmm_mat4
HFTW_Orthographic(float Left, float Right, float Bottom, float Top, float Near, float Far)
{
    hmm_mat4 Result = HFTW_Mat4d(1.0f);

    Result.Elements[0][0] = 2.0f / (Right - Left);
    Result.Elements[1][1] = 2.0f / (Top - Bottom);
    Result.Elements[2][2] = 2.0f / (Near - Far);

    Result.Elements[3][0] = (Left + Right) / (Left - Right);
    Result.Elements[3][1] = (Bottom + Top) / (Bottom - Top);
    Result.Elements[3][2] = (Far + Near) / (Near - Far);

    return (Result);
}

hmm_mat4
HFTW_Perspective(float FOV, float AspectRatio, float Near, float Far)
{
    hmm_mat4 Result = HFTW_Mat4d(1.0f);

    float TanThetaOver2 = tanf(FOV * (HFTW_PI32 / 360.0f));
    
    Result.Elements[0][0] = 1.0f / TanThetaOver2;
    Result.Elements[1][1] = AspectRatio / TanThetaOver2;
    Result.Elements[2][3] = -1.0f;
    Result.Elements[2][2] = (Near + Far) / (Near - Far);
    Result.Elements[3][2] = (2.0f * Near * Far) / (Near - Far);
    Result.Elements[3][3] = 0.0f;

    return (Result);
}

hmm_mat4
HFTW_Translate(hmm_vec3 Translation)
{
    hmm_mat4 Result = HFTW_Mat4d(1.0f);

    Result.Elements[0][3] = Translation.X;
    Result.Elements[1][3] = Translation.Y;
    Result.Elements[2][3] = Translation.Z;

    return (Result);
}

hmm_mat4
HFTW_Rotate(float Angle, hmm_vec3 Axis)
{
    hmm_mat4 Result = HFTW_Mat4d(1.0f);
    
    Axis = HFTW_Normalize(Axis);
    
    float SinTheta = sinf(HFTW_ToRadians(Angle));
    float CosTheta = cosf(HFTW_ToRadians(Angle));
    float CosValue = 1.0f - CosTheta;
    
    Result.Elements[0][0] = (Axis.X * Axis.X * CosValue) + CosTheta;
    Result.Elements[0][1] = (Axis.X * Axis.Y * CosValue) + (Axis.Z * SinTheta);
    Result.Elements[0][2] = (Axis.X * Axis.Z * CosValue) - (Axis.Y * SinTheta);
    
    Result.Elements[1][0] = (Axis.Y * Axis.X * CosValue) - (Axis.Z * SinTheta);
    Result.Elements[1][1] = (Axis.Y * Axis.Y * CosValue) + CosTheta;
    Result.Elements[1][2] = (Axis.Y * Axis.Z * CosValue) + (Axis.X * SinTheta);
    
    Result.Elements[2][0] = (Axis.Z * Axis.X * CosValue) + (Axis.Y * SinTheta);
    Result.Elements[2][1] = (Axis.Z * Axis.Y * CosValue) - (Axis.X * SinTheta);
    Result.Elements[2][2] = (Axis.Z * Axis.Z * CosValue) + CosTheta;
    
    return (Result);
}

hmm_mat4
HFTW_LookAt(hmm_vec3 Eye, hmm_vec3 Center, hmm_vec3 Up)
{
    hmm_mat4 Result = {0};

    hmm_vec3 F = HFTW_Normalize(HFTW_SubtractVec3(Center, Eye));
    hmm_vec3 S = HFTW_Normalize(HFTW_Cross(F, Up));
    hmm_vec3 U = HFTW_Cross(S, F);

    Result.Elements[0][0] = S.X;
    Result.Elements[0][1] = U.X;
    Result.Elements[0][2] = -F.X;

    Result.Elements[1][0] = S.Y;
    Result.Elements[1][1] = U.Y;
    Result.Elements[1][2] = -F.Y;

    Result.Elements[2][0] = S.Z;
    Result.Elements[2][1] = U.Z;
    Result.Elements[2][2] = -F.Z;

    Result.Elements[3][0] = -HFTW_Dot(S, Eye);
    Result.Elements[3][1] = -HFTW_Dot(U, Eye);
    Result.Elements[3][2] = HFTW_Dot(F, Eye);
    Result.Elements[3][3] = 1.0f;

    return (Result);
}

hmm_mat4
HFTW_Scale(hmm_vec3 Scale)
{
    hmm_mat4 Result = HFTW_Mat4d(1.0f);

    Result.Elements[0][0] = Scale.X;
    Result.Elements[1][1] = Scale.Y;
    Result.Elements[2][2] = Scale.Z;

    return (Result);
}

#ifdef HANDMADE_MATH_CPP_MODE

HINLINE hmm_vec2
Add(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result = HFTW_AddVec2(Left, Right);

    return (Result);
}

HINLINE hmm_vec3
Add(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result = HFTW_AddVec3(Left, Right);

    return (Result);
}

HFTWDEF HINLINE hmm_vec4
Add(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result = HFTW_AddVec4(Left, Right);

    return (Result);
}

HINLINE hmm_vec2
Subtract(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result = HFTW_SubtractVec2(Left, Right);

    return (Result);
}

HINLINE hmm_vec3
Subtract(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result = HFTW_SubtractVec3(Left, Right);

    return (Result);
}

HINLINE hmm_vec4
Subtract(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result = HFTW_SubtractVec4(Left, Right);

    return (Result);
}

HINLINE hmm_vec2
Multiply(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result = HFTW_MultiplyVec2(Left, Right);

    return (Result);
}

HINLINE hmm_vec3
Multiply(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result = HFTW_MultiplyVec3(Left, Right);

    return (Result);
}

HINLINE hmm_vec4
Multiply(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result = HFTW_MultiplyVec4(Left, Right);

    return (Result);
}

HINLINE hmm_mat4
Multiply(hmm_mat4 Left, hmm_mat4 Right)
{
    hmm_mat4 Result = HFTW_MultiplyMat4(Left, Right);

    return (Result);
}

HINLINE hmm_vec4
Multiply(hmm_mat4 Matrix, hmm_vec4 Vector)
{
    hmm_vec4 Result = HFTW_MultiplyMat4ByVec4(Matrix, Vector);
    
    return (Result);
}

HINLINE hmm_vec2
Divide(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result = HFTW_DivideVec2(Left, Right);

    return (Result);
}

HINLINE hmm_vec3
Divide(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result = HFTW_DivideVec3(Left, Right);

    return (Result);
}

HINLINE hmm_vec4
Divide(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result = HFTW_DivideVec4(Left, Right);

    return (Result);
}

HINLINE hmm_vec2
operator+(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result = Add(Left, Right);

    return (Result);
}

HINLINE hmm_vec3
operator+(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result = Add(Left, Right);

    return (Result);
}

HINLINE hmm_vec4
operator+(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result = Add(Left, Right);

    return (Result);
}

HINLINE hmm_vec2
operator-(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result = Subtract(Left, Right);

    return (Result);
}

HINLINE hmm_vec3
operator-(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result = Subtract(Left, Right);

    return (Result);
}

HINLINE hmm_vec4
operator-(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result = Subtract(Left, Right);

    return (Result);
}

HINLINE hmm_vec2
operator*(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result = Multiply(Left, Right);

    return (Result);
}

HINLINE hmm_vec3
operator*(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result = Multiply(Left, Right);

    return (Result);
}

HINLINE hmm_vec3
operator*(hmm_vec3 Left, float Right)
{
    hmm_vec3 Result;

    Result.X = Right * Left.X;
    Result.Y = Right * Left.Y;
    Result.Z = Right * Left.Z;

    return (Result);
}

HINLINE hmm_vec2
operator*(hmm_vec2 Left, float Right)
{
    hmm_vec2 Result;

    Result.X = Right * Left.X;
    Result.Y = Right * Left.Y;

    return (Result);
}

HINLINE hmm_vec4
operator*(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result = Multiply(Left, Right);

    return (Result);
}

HINLINE hmm_mat4
operator*(hmm_mat4 Left, hmm_mat4 Right)
{
    hmm_mat4 Result = Multiply(Left, Right);

    return (Result);
}

HINLINE hmm_vec4
operator*(hmm_mat4 Matrix, hmm_vec4 Vector)
{
    hmm_vec4 Result = Multiply(Matrix, Vector);
    
    return (Result);
}

HINLINE hmm_vec2
operator/(hmm_vec2 Left, hmm_vec2 Right)
{
    hmm_vec2 Result = Divide(Left, Right);

    return (Result);
}

HINLINE hmm_vec3
operator/(hmm_vec3 Left, hmm_vec3 Right)
{
    hmm_vec3 Result = Divide(Left, Right);

    return (Result);
}

HINLINE hmm_vec4
operator/(hmm_vec4 Left, hmm_vec4 Right)
{
    hmm_vec4 Result = Divide(Left, Right);

    return (Result);
}

#endif /* HANDMADE_MATH_CPP_MODE */


#endif /* HANDMADE_MATH_IMPLEMENTATION */