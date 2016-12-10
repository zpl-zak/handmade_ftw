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
#define HFTWDEF internal
#else
#define HFTWDEF extern
#endif

#if _MSC_VER && !__INTEL_COMPILER
#define HINLINE __internal
#else
#define HINLINE internal
#endif

#define MathPI32 3.14159265359f
#define MathPI 3.14159265358979323846f

#define MathMIN(a, b) (a) > (b) ? (b) : (a)
#define MathMAX(a, b) (a) < (b) ? (b) : (a)
#define MathABS(a) (a) < 0 ? -(a) : (a)
#define MathMOD(a, m) ((a) % (m)) >= 0 ? ((a) % (m)) : (((a) % (m)) + (m))
#define MathSQUARE(x) ((x) * (x))

typedef union vec2
{
    struct
    {
        real32 X, Y;
    };

    struct
    {
        real32 U, V;
    };

    struct
    {
        real32 Left, Right;
    };

    real32 Elements[2];
} vec2;

typedef union vec3
{
    struct
    {
        real32 X, Y, Z;
    };

    struct
    {
        real32 U, V, W;
    };

    struct
    {
        real32 R, G, B;
    };

    struct
    {
        vec2 XY;
        real32 Ignored0_;
    };

    struct
    {
        real32 Ignored1_;
        vec2 YZ;
    };

    struct
    {
        vec2 UV;
        real32 Ignored2_;
    };

    struct
    {
        real32 Ignored3_;
        vec2 VW;
    };

    real32 Elements[3];
} vec3;

typedef union vec4
{
    struct
    {
        union
        {
            vec3 XYZ;
            struct
            {
                real32 X, Y, Z;
            };
        };

        real32 W;
    };
    struct
    {
        union
        {
            vec3 RGB;
            struct
            {
                real32 R, G, B;
            };
        };

        real32 A;
    };

    struct
    {
        vec2 XY;
        real32 Ignored0_;
        real32 Ignored1_;
    };

    struct
    {
        real32 Ignored2_;
        vec2 YZ;
        real32 Ignored3_;
    };

    struct
    {
        real32 Ignored4_;
        real32 Ignored5_;
        vec2 ZW;
    };

    real32 Elements[4];
} vec4;

typedef union mat4
{
    real32 Elements[4][4];
} mat4;

typedef vec2 v2;
typedef vec3 v3;
typedef vec4 v4;
typedef mat4 m4;    

HFTWDEF real32 MathToRadians(real32 Degrees);
HFTWDEF real32 MathInner(vec3 A, vec3 B);
HFTWDEF real32 MathSquareRoot(real32 Float);
HFTWDEF real32 MathLengthSquareRoot(vec3 A);
HFTWDEF real32 MathFastInverseSquareRoot(real32 Number);
HFTWDEF real32 MathLength(vec3 A);    
HFTWDEF real32 MathPower(real32 Base, int Exponent);
HFTWDEF real32 MathClamp(real32 Min, real32 Value, real32 Max);

HFTWDEF vec3 MathNormalize(vec3 A);
HFTWDEF vec3 MathCross(vec3 VecOne, vec3 VecTwo);
HFTWDEF real32 MathDot(vec3 VecOne, vec3 VecTwo);

HFTWDEF vec2 MathVec2i(int X, int Y);
HFTWDEF vec2 MathVec2(real32 X, real32 Y);
HFTWDEF vec3 MathVec3(real32 X, real32 Y, real32 Z);
HFTWDEF vec3 MathVec3i(int X, int Y, int Z);
HFTWDEF vec4 MathVec4(real32 X, real32 Y, real32 Z, real32 W);
HFTWDEF vec4 MathVec4i(int X, int Y, int Z, int W);

HFTWDEF vec2 MathAddVec2(vec2 Left, vec2 Right);
HFTWDEF vec3 MathAddVec3(vec3 Left, vec3 Right);
HFTWDEF vec4 MathAddVec4(vec4 Left, vec4 Right);

HFTWDEF vec2 MathSubtractVec2(vec2 Left, vec2 Right);
HFTWDEF vec3 MathSubtractVec3(vec3 Left, vec3 Right);
HFTWDEF vec4 MathSubtractVec4(vec4 Left, vec4 Right);

HFTWDEF vec2 MathMultiplyVec2(vec2 Left, vec2 Right);
HFTWDEF vec3 MathMultiplyVec3(vec3 Left, vec3 Right);
HFTWDEF vec4 MathMultiplyVec4(vec4 Left, vec4 Right);

HFTWDEF vec2 MathDivideVec2(vec2 Left, vec2 Right);
HFTWDEF vec3 MathDivideVec3(vec3 Left, vec3 Right);
HFTWDEF vec4 MathDivideVec4(vec4 Left, vec4 Right);

HFTWDEF mat4 MathMat4(void);
HFTWDEF mat4 MathMat4d(real32 Diagonal);
HFTWDEF mat4 MathMultiplyMat4(mat4 Left, mat4 Right);
HFTWDEF vec4 MathMultiplyMat4ByVec4(mat4 Matrix, vec4 Vector);

HFTWDEF mat4 MathOrthographic(real32 Left, real32 Right, real32 Bottom, real32 Top, real32 Near, real32 Far);
HFTWDEF mat4 MathPerspective(real32 FOV, real32 AspectRatio, real32 Near, real32 Far);
HFTWDEF mat4 MathTranslate(vec3 Translation);
HFTWDEF mat4 MathRotate(real32 Angle, vec3 Axis);
HFTWDEF mat4 MathScale(vec3 Scale);

HFTWDEF mat4 MathLookAt(vec3 Eye, vec3 Center, vec3 Up);

#ifdef __cplusplus
}
#endif

#ifdef HANDMADE_MATH_CPP_MODE

HFTWDEF vec2 MathAdd(int X, int Y);
HFTWDEF vec3 MathAdd(int X, int Y, int Z);
HFTWDEF vec4 MathAdd(int X, int Y, int Z, int W);

HFTWDEF vec2 MathSubtract(int X, int Y);
HFTWDEF vec3 MathSubtract(int X, int Y, int Z);
HFTWDEF vec4 MathSubtract(int X, int Y, int Z, int W);

HFTWDEF vec2 MathMultiply(int X, int Y);
HFTWDEF vec3 MathMultiply(int X, int Y, int Z);
HFTWDEF vec4 MathMultiply(int X, int Y, int Z, int W);
HFTWDEF mat4 MathMultiply(mat4 Left, mat4 Right);
HFTWDEF vec4 MathMultiply(mat4 Matrix, vec4 Vector);

HFTWDEF vec2 MathDivide(int X, int Y);
HFTWDEF vec3 MathDivide(int X, int Y, int Z);
HFTWDEF vec4 MathDivide(int X, int Y, int Z, int W);

HFTWDEF vec2 operator+(vec2 Left, vec2 Right);
HFTWDEF vec3 operator+(vec3 Left, vec3 Right);
HFTWDEF vec4 operator+(vec4 Left, vec4 Right);

HFTWDEF vec2 operator-(vec2 Left, vec2 Right);
HFTWDEF vec3 operator-(vec3 Left, vec3 Right);
HFTWDEF vec4 operator-(vec4 Left, vec4 Right);

HFTWDEF vec2 operator*(vec2 Left, vec2 Right);
HFTWDEF vec3 operator*(vec3 Left, vec3 Right);
HFTWDEF vec4 operator*(vec4 Left, vec4 Right);
HFTWDEF mat4 operator*(mat4 Left, mat4 Right);

HFTWDEF vec3 operator*(vec3 Left, real32 Right);
HFTWDEF vec2 operator*(vec2 Left, real32 Right);

HFTWDEF vec4 operator*(mat4 Matrix, vec4 Vector);

HFTWDEF vec2 operator/(vec2 Left, vec2 Right);
HFTWDEF vec3 operator/(vec3 Left, vec3 Right);
HFTWDEF vec4 operator/(vec4 Left, vec4 Right);

#endif /* HANDMADE_MATH_CPP */

#endif /* HANDMADE_MATH_H */

#ifdef HANDMADE_MATH_IMPLEMENTATION

HINLINE real32
MathToRadians(real32 Degrees)
{
    real32 Result = Degrees * (MathPI32 / 180.0f);

    return (Result);
}

HINLINE real32
MathInner(vec3 A, vec3 B)
{
    real32 Result = A.X * B.X + A.Y * B.Y + A.Z * B.Z;

    return (Result);
}

HINLINE real32
MathSquareRoot(real32 Float)
{    
    real32 Result = sqrtf(Float);

    return(Result);
}

HINLINE real32
MathLengthSquareRoot(vec3 A)
{
    real32 Result = MathInner(A, A);

    return (Result);
}

// Refer to https://en.wikipedia.org/wiki/Fast_inverse_square_root
HINLINE real32
MathFastInverseSquareRoot(real32 Number)
{
    long i;
    real32 x2, y;
    const real32 threehalfs = 1.5f;

    x2 = Number * 0.5f;
    y  = Number;
    i  = * ( long * ) &y;          // evil real32ing point bit level hacking
    i  = 0x5f3759df - ( i >> 1 );  // what the fuck? 
    y  = * ( real32 * ) &i;
    
    y  = y * ( threehalfs - ( x2 * y * y ) );

    return ( y );
}

HINLINE real32
MathLength(vec3 A)
{
    real32 Result = MathSquareRoot(MathLengthSquareRoot(A));
    return (Result);
}

HINLINE real32
MathPower(real32 Base, int Exponent)
{
    real32 Result = 1;
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

HINLINE real32
MathLerp(real32 A, real32 Time, real32 B)
{
    real32 Result = (1.0f - Time) * A + Time * B;

    return (Result);
}

HINLINE real32
MathClamp(real32 Min, real32 Value, real32 Max)
{
    real32 Result = Value;

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

HINLINE vec3
MathNormalize(vec3 A)
{
    vec3 Result = {0};

    Result.X = A.X * (1.0f / MathLength(A));
    Result.Y = A.Y * (1.0f / MathLength(A));
    Result.Z = A.Z * (1.0f / MathLength(A));
    
    return (Result);
}

HINLINE vec3
MathCross(vec3 VecOne, vec3 VecTwo)
{
    vec3 Result;

    Result.X = (VecOne.Y * VecTwo.Z) - (VecOne.Z * VecTwo.Y);
    Result.Y = (VecOne.Z * VecTwo.X) - (VecOne.X * VecTwo.Z);
    Result.Z = (VecOne.X * VecTwo.Y) - (VecOne.Y * VecTwo.X);

    return (Result);
}

HINLINE real32
MathDot(vec3 VecOne, vec3 VecTwo)
{
    real32 Result = 0;

    Result = (VecOne.X * VecTwo.X) + (VecOne.Y * VecTwo.Y) + (VecOne.Z * VecTwo.Z);

    return (Result);
}

HINLINE vec2
MathVec2(real32 X, real32 Y)
{
    vec2 Result;

    Result.X = X;
    Result.Y = Y;

    return (Result);
}

HINLINE vec2
MathVec2i(int X, int Y)
{
    vec2 Result;

    Result.X = (real32)X;
    Result.Y = (real32)Y;

    return (Result);
}

HINLINE vec3
MathVec3(real32 X, real32 Y, real32 Z)
{
    vec3 Result;

    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;

    return (Result);
}

HINLINE vec3
MathVec3i(int X, int Y, int Z)
{
    vec3 Result;

    Result.X = (real32)X;
    Result.Y = (real32)Y;
    Result.Z = (real32)Z;

    return (Result);
}

HINLINE vec4
MathVec4(real32 X, real32 Y, real32 Z, real32 W)
{
    vec4 Result;

    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    Result.W = W;

    return (Result);
}

HINLINE vec4
MathVec4i(int X, int Y, int Z, int W)
{
    vec4 Result;

    Result.X = (real32)X;
    Result.Y = (real32)Y;
    Result.Z = (real32)Z;
    Result.W = (real32)W;

    return (Result);
}

HINLINE vec2
MathAddVec2(vec2 Left, vec2 Right)
{
    vec2 Result;

    Result.X = Left.X + Right.X;
    Result.Y = Left.Y + Right.Y;

    return (Result);
}

HINLINE vec3
MathAddVec3(vec3 Left, vec3 Right)
{
    vec3 Result;

    Result.X = Left.X + Right.X;
    Result.Y = Left.Y + Right.Y;
    Result.Z = Left.Z + Right.Z;

    return (Result);
}

HINLINE vec4
MathAddVec4(vec4 Left, vec4 Right)
{
    vec4 Result;

    Result.X = Left.X + Right.X;
    Result.Y = Left.Y + Right.Y;
    Result.Z = Left.Z + Right.Z;
    Result.W = Left.W + Right.W;

    return (Result);
}

HINLINE vec2
MathSubtractVec2(vec2 Left, vec2 Right)
{
    vec2 Result;

    Result.X = Left.X - Right.X;
    Result.Y = Left.Y - Right.Y;

    return (Result);
}

HINLINE vec3
MathSubtractVec3(vec3 Left, vec3 Right)
{
    vec3 Result;

    Result.X = Left.X - Right.X;
    Result.Y = Left.Y - Right.Y;
    Result.Z = Left.Z - Right.Z;

    return (Result);
}

HINLINE vec4
MathSubtractVec4(vec4 Left, vec4 Right)
{
    vec4 Result;

    Result.X = Left.X - Right.X;
    Result.Y = Left.Y - Right.Y;
    Result.Z = Left.Z - Right.Z;
    Result.W = Left.W - Right.W;

    return (Result);
}

HINLINE vec2
MathMultiplyVec2(vec2 Left, vec2 Right)
{
    vec2 Result;

    Result.X = Left.X * Right.X;
    Result.Y = Left.Y * Right.Y;

    return (Result);
}

HINLINE vec3
MathMultiplyVec3(vec3 Left, vec3 Right)
{
    vec3 Result;

    Result.X = Left.Z * Right.X;
    Result.Y = Left.Y * Right.Y;
    Result.Z = Left.Z * Right.Z;

    return (Result);
}

HINLINE vec4
MathMultiplyVec4(vec4 Left, vec4 Right)
{
    vec4 Result;

    Result.X = Left.X * Right.X;
    Result.Y = Left.Y * Right.Y;
    Result.Z = Left.Z * Right.Z;
    Result.W = Left.W * Right.W;

    return (Result);
}

HINLINE vec2
MathDivideVec2(vec2 Left, vec2 Right)
{
    vec2 Result;

    Result.X = Left.X / Right.X;
    Result.Y = Left.Y / Right.Y;

    return (Result);
}

HINLINE vec3
MathDivideVec3(vec3 Left, vec3 Right)
{
    vec3 Result;

    Result.X = Left.X / Right.X;
    Result.Y = Left.Y / Right.Y;
    Result.Z = Left.Z / Right.Z;

    return (Result);
}

HINLINE vec4
MathDivideVec4(vec4 Left, vec4 Right)
{
    vec4 Result;

    Result.X = Left.X / Right.X;
    Result.Y = Left.Y / Right.Y;
    Result.Z = Left.Z / Right.Z;
    Result.W = Left.W / Right.W;

    return (Result);
}

HINLINE mat4
MathMat4()
{
    mat4 Result = {0};

    return (Result);
}

mat4
MathMat4d(real32 Diagonal)
{
    mat4 Result;

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

mat4
MathMultiplyMat4(mat4 Left, mat4 Right)
{
    mat4 Result = MathMat4();

    int Rows;
    for(Rows = 0; Rows < 4; ++Rows)
    {
        int Columns;
        for(Columns = 0; Columns < 4; ++Columns)
        {
            real32 Sum = 0;
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

vec4
MathMultiplyMat4ByVec4(mat4 Matrix, vec4 Vector)
{
    vec4 Result = MathVec4(0.0f, 0.0f, 0.0f, 0.0f);
    
    int Rows, Columns;
    for(Rows = 0; Rows < 4; ++Rows)
    {
        real32 Sum = 0;
        for(Columns = 0; Columns < 4; ++Columns)
        {
            Sum += Matrix.Elements[Rows][Columns] * Vector.Elements[Columns];
        }
        
        Result.Elements[Rows] = Sum;
    }
    
    return (Result);
}

mat4
MathOrthographic(real32 Left, real32 Right, real32 Bottom, real32 Top, real32 Near, real32 Far)
{
    mat4 Result = MathMat4d(1.0f);

    Result.Elements[0][0] = 2.0f / (Right - Left);
    Result.Elements[1][1] = 2.0f / (Top - Bottom);
    Result.Elements[2][2] = 2.0f / (Near - Far);

    Result.Elements[3][0] = (Left + Right) / (Left - Right);
    Result.Elements[3][1] = (Bottom + Top) / (Bottom - Top);
    Result.Elements[3][2] = (Far + Near) / (Near - Far);

    return (Result);
}

mat4
MathPerspective(real32 FOV, real32 AspectRatio, real32 Near, real32 Far)
{
    mat4 Result = MathMat4d(1.0f);

    real32 TanThetaOver2 = tanf(FOV * (MathPI32 / 360.0f));
    
    Result.Elements[0][0] = 1.0f / TanThetaOver2;
    Result.Elements[1][1] = AspectRatio / TanThetaOver2;
    Result.Elements[2][3] = -1.0f;
    Result.Elements[2][2] = (Near + Far) / (Near - Far);
    Result.Elements[3][2] = (2.0f * Near * Far) / (Near - Far);
    Result.Elements[3][3] = 0.0f;

    return (Result);
}

mat4
MathTranslate(vec3 Translation)
{
    mat4 Result = MathMat4d(1.0f);

    Result.Elements[0][3] = Translation.X;
    Result.Elements[1][3] = Translation.Y;
    Result.Elements[2][3] = Translation.Z;

    return (Result);
}

mat4
MathRotate(real32 Angle, vec3 Axis)
{
    mat4 Result = MathMat4d(1.0f);
    
    Axis = MathNormalize(Axis);
    
    real32 SinTheta = sinf(MathToRadians(Angle));
    real32 CosTheta = cosf(MathToRadians(Angle));
    real32 CosValue = 1.0f - CosTheta;
    
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

mat4
MathLookAt(vec3 Eye, vec3 Center, vec3 Up)
{
    mat4 Result = {0};

    vec3 F = MathNormalize(MathSubtractVec3(Center, Eye));
    vec3 S = MathNormalize(MathCross(F, Up));
    vec3 U = MathCross(S, F);

    Result.Elements[0][0] = S.X;
    Result.Elements[0][1] = U.X;
    Result.Elements[0][2] = -F.X;

    Result.Elements[1][0] = S.Y;
    Result.Elements[1][1] = U.Y;
    Result.Elements[1][2] = -F.Y;

    Result.Elements[2][0] = S.Z;
    Result.Elements[2][1] = U.Z;
    Result.Elements[2][2] = -F.Z;

    Result.Elements[3][0] = -MathDot(S, Eye);
    Result.Elements[3][1] = -MathDot(U, Eye);
    Result.Elements[3][2] = MathDot(F, Eye);
    Result.Elements[3][3] = 1.0f;

    return (Result);
}

mat4
MathScale(vec3 Scale)
{
    mat4 Result = MathMat4d(1.0f);

    Result.Elements[0][0] = Scale.X;
    Result.Elements[1][1] = Scale.Y;
    Result.Elements[2][2] = Scale.Z;

    return (Result);
}

#ifdef HANDMADE_MATH_CPP_MODE

HINLINE vec2
Add(vec2 Left, vec2 Right)
{
    vec2 Result = MathAddVec2(Left, Right);

    return (Result);
}

HINLINE vec3
Add(vec3 Left, vec3 Right)
{
    vec3 Result = MathAddVec3(Left, Right);

    return (Result);
}

HFTWDEF HINLINE vec4
Add(vec4 Left, vec4 Right)
{
    vec4 Result = MathAddVec4(Left, Right);

    return (Result);
}

HINLINE vec2
Subtract(vec2 Left, vec2 Right)
{
    vec2 Result = MathSubtractVec2(Left, Right);

    return (Result);
}

HINLINE vec3
Subtract(vec3 Left, vec3 Right)
{
    vec3 Result = MathSubtractVec3(Left, Right);

    return (Result);
}

HINLINE vec4
Subtract(vec4 Left, vec4 Right)
{
    vec4 Result = MathSubtractVec4(Left, Right);

    return (Result);
}

HINLINE vec2
Multiply(vec2 Left, vec2 Right)
{
    vec2 Result = MathMultiplyVec2(Left, Right);

    return (Result);
}

HINLINE vec3
Multiply(vec3 Left, vec3 Right)
{
    vec3 Result = MathMultiplyVec3(Left, Right);

    return (Result);
}

HINLINE vec4
Multiply(vec4 Left, vec4 Right)
{
    vec4 Result = MathMultiplyVec4(Left, Right);

    return (Result);
}

HINLINE mat4
Multiply(mat4 Left, mat4 Right)
{
    mat4 Result = MathMultiplyMat4(Left, Right);

    return (Result);
}

HINLINE vec4
Multiply(mat4 Matrix, vec4 Vector)
{
    vec4 Result = MathMultiplyMat4ByVec4(Matrix, Vector);
    
    return (Result);
}

HINLINE vec2
Divide(vec2 Left, vec2 Right)
{
    vec2 Result = MathDivideVec2(Left, Right);

    return (Result);
}

HINLINE vec3
Divide(vec3 Left, vec3 Right)
{
    vec3 Result = MathDivideVec3(Left, Right);

    return (Result);
}

HINLINE vec4
Divide(vec4 Left, vec4 Right)
{
    vec4 Result = MathDivideVec4(Left, Right);

    return (Result);
}

HINLINE vec2
operator+(vec2 Left, vec2 Right)
{
    vec2 Result = Add(Left, Right);

    return (Result);
}

HINLINE vec3
operator+(vec3 Left, vec3 Right)
{
    vec3 Result = Add(Left, Right);

    return (Result);
}

HINLINE vec4
operator+(vec4 Left, vec4 Right)
{
    vec4 Result = Add(Left, Right);

    return (Result);
}

HINLINE vec2
operator-(vec2 Left, vec2 Right)
{
    vec2 Result = Subtract(Left, Right);

    return (Result);
}

HINLINE vec3
operator-(vec3 Left, vec3 Right)
{
    vec3 Result = Subtract(Left, Right);

    return (Result);
}

HINLINE vec4
operator-(vec4 Left, vec4 Right)
{
    vec4 Result = Subtract(Left, Right);

    return (Result);
}

HINLINE vec2
operator*(vec2 Left, vec2 Right)
{
    vec2 Result = Multiply(Left, Right);

    return (Result);
}

HINLINE vec3
operator*(vec3 Left, vec3 Right)
{
    vec3 Result = Multiply(Left, Right);

    return (Result);
}

HINLINE vec3
operator*(vec3 Left, real32 Right)
{
    vec3 Result;

    Result.X = Right * Left.X;
    Result.Y = Right * Left.Y;
    Result.Z = Right * Left.Z;

    return (Result);
}

HINLINE vec2
operator*(vec2 Left, real32 Right)
{
    vec2 Result;

    Result.X = Right * Left.X;
    Result.Y = Right * Left.Y;

    return (Result);
}

HINLINE vec4
operator*(vec4 Left, vec4 Right)
{
    vec4 Result = Multiply(Left, Right);

    return (Result);
}

HINLINE mat4
operator*(mat4 Left, mat4 Right)
{
    mat4 Result = Multiply(Left, Right);

    return (Result);
}

HINLINE vec4
operator*(mat4 Matrix, vec4 Vector)
{
    vec4 Result = Multiply(Matrix, Vector);
    
    return (Result);
}

HINLINE vec2
operator/(vec2 Left, vec2 Right)
{
    vec2 Result = Divide(Left, Right);

    return (Result);
}

HINLINE vec3
operator/(vec3 Left, vec3 Right)
{
    vec3 Result = Divide(Left, Right);

    return (Result);
}

HINLINE vec4
operator/(vec4 Left, vec4 Right)
{
    vec4 Result = Divide(Left, Right);

    return (Result);
}

#endif /* HANDMADE_MATH_CPP_MODE */


#endif /* HANDMADE_MATH_IMPLEMENTATION */