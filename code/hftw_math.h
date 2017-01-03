// (c) 2016 ZaKlaus; All Bs Reserved
// Original code author: Zakary "zak" Strange

#ifndef HFTW_MATH_H
#define HFTW_MATH_H

// TODO(zaklaus): Implement all OPs for different types.


#include <math.h>

#define MathPI32 3.14159265359f
#define MathPI 3.14159265358979323846f

#define MathMIN(a, b) (a) > (b) ? (b) : (a)
#define MathMAX(a, b) (a) < (b) ? (b) : (a)
#define MathABS(a) (a) < 0 ? -(a) : (a)
#define MathMOD(a, m) ((a) % (m)) >= 0 ? ((a) % (m)) : (((a) % (m)) + (m))
#define MathSQUARE(x) ((x) * (x))

doc(vec2)
doc_string(2-sized vector. Alias: v2)
doc_cat(Math [Unfinished])

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
        real32 A, B;
    };

    real32 Elements[2];
} vec2;

doc(vec3)
doc_string(3-sized vector. Alias: v3)

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

doc(vec4)
doc_string(4-sized vector. Alias: v4)

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

doc(quaternion)
doc_string(Quaternion. Alias: q4.)

typedef struct
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
            
            real32 W;
        };
    };
} quaternion;

doc(mat4)
doc_string(4x4 Matrix. Alias: m4)

typedef union mat4
{
    real32 Elements[4][4];
    real32 Elements16[16];
} mat4;

doc_sep()

doc_ignore()

typedef vec2 v2;
typedef vec3 v3;
typedef vec4 v4;
typedef mat4 m4;
typedef quaternion q4;

doc_hunt()

doc(MathToRadians)
doc_string(Convert Degree value to Radians.)

internal real32
MathToRadians(real32 Degrees)
{
    real32 Result = Degrees * (MathPI32 / 180.0f);

    return (Result);
}

doc(MathSquareRoot)
doc_string(Calculates the 2th square root of value.)

internal real32
MathSquareRoot(real32 Float)
{    
    real32 Result = sqrtf(Float);

    return(Result);
}

doc(MathDot)
doc_string(Calculates the dot product of 2 vectors.)

internal real32
MathDot(vec3 VecOne, vec3 VecTwo)
{
    real32 Result = 0;
    
    Result = (VecOne.X * VecTwo.X) + (VecOne.Y * VecTwo.Y) + (VecOne.Z * VecTwo.Z);
    
    return (Result);
}

doc(MathLengthSquareRoot)
doc_string(Calculates the length of a square root.)

internal real32
MathLengthSquareRoot(vec3 A)
{
    real32 Result = MathDot(A, A);

    return (Result);
}

// Refer to https://en.wikipedia.org/wiki/Fast_inverse_square_root
doc(MathFastInverseSquareRoot)
doc_string(Calculates inverse square root of value.)

internal real32
MathFastInverseSquareRoot(real32 Number)
{
    long i;
    real32 x2, y;
    const real32 threehalfs = 1.5f;

    x2 = Number * 0.5f;
    y  = Number;
    i  = * ( long * ) &y;
    i  = 0x5f3759df - ( i >> 1 );
    y  = * ( real32 * ) &i;
    
    y  = y * ( threehalfs - ( x2 * y * y ) );

    return ( y );
}

doc(MathLength)
doc_string(Calculates the length of vector.)

internal real32
MathLength(vec3 A)
{
    real32 Result = MathSquareRoot(MathLengthSquareRoot(A));
    return (Result);
}

doc(MathPower)
doc_string(Calculates the power of E for value.)

internal real32
MathPower(real32 Base, int Exponent)
{
    real32 Result = 1.f;
    real32 Mul = Exponent < 0 ? 1.f / Base : Base;
    uint32 X = Exponent < 0 ? -Exponent : Exponent;
    
    while(X)
    {
        if(X & 1)
        {
            Result *= Mul;
        }
        
        Mul *= Mul;
        X >>= 1;
    }
    return (Result);
}

doc(MathLerp)
doc_string(Calculates linear interpolation between 2 values by given time.)

internal real32
MathLerp(real32 A, real32 Time, real32 B)
{
    real32 Result = (1.0f - Time) * A + Time * B;

    return (Result);
}

doc(MathClamp)
doc_string(Clamps the value between Minima/Maxima.)

internal real32
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

doc(MathNormalize)
doc_string(Returns normalized vector.)

internal vec3
MathNormalize(vec3 A)
{
    vec3 Result = {0};
    
    real32 Length = MathLength(A);

    Result.X = A.X * (1.0f / Length);
    Result.Y = A.Y * (1.0f / Length);
    Result.Z = A.Z * (1.0f / Length);
    
    return (Result);
}

doc(MathCross)
doc_string(Calculates the cross product of 2 vectors.)

internal vec3
MathCross(vec3 VecOne, vec3 VecTwo)
{
    vec3 Result;

    Result.X = (VecOne.Y * VecTwo.Z) - (VecOne.Z * VecTwo.Y);
    Result.Y = (VecOne.Z * VecTwo.X) - (VecOne.X * VecTwo.Z);
    Result.Z = (VecOne.X * VecTwo.Y) - (VecOne.Y * VecTwo.X);

    return (Result);
}

doc(MathVec2)
doc_string(Returns vec2 from [x;y].)

internal vec2
MathVec2(real32 X, real32 Y)
{
    vec2 Result;

    Result.X = X;
    Result.Y = Y;

    return (Result);
}

doc(MathVec2i)
doc_string(Returns vec2 from [x;y].)

internal vec2
MathVec2i(int X, int Y)
{
    vec2 Result;

    Result.X = (real32)X;
    Result.Y = (real32)Y;

    return (Result);
}


doc(MathVec3)
doc_string(Returns vec3 from [x;y;z].)

internal vec3
MathVec3(real32 X, real32 Y, real32 Z)
{
    vec3 Result;

    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;

    return (Result);
}

doc(MathVec3i)
doc_string(Returns vec3 from [x;y;z].)

internal vec3
MathVec3i(int X, int Y, int Z)
{
    vec3 Result;

    Result.X = (real32)X;
    Result.Y = (real32)Y;
    Result.Z = (real32)Z;

    return (Result);
}

doc(MathVec4)
doc_string(Returns vec4 from [x;y;z;w].)

internal vec4
MathVec4(real32 X, real32 Y, real32 Z, real32 W)
{
    vec4 Result;

    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    Result.W = W;

    return (Result);
}

doc(MathVec4i)
doc_string(Returns vec4 from [x;y;z;w].)

internal vec4
MathVec4i(int X, int Y, int Z, int W)
{
    vec4 Result;

    Result.X = (real32)X;
    Result.Y = (real32)Y;
    Result.Z = (real32)Z;
    Result.W = (real32)W;

    return (Result);
}

doc(MathAddVec2)
doc_string(Adds vector to vec2.)

internal vec2
MathAddVec2(vec2 A, vec2 B)
{
    vec2 Result;

    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;

    return (Result);
}

internal vec3
MathAddVec3(vec3 A, vec3 B)
{
    vec3 Result;

    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    Result.Z = A.Z + B.Z;

    return (Result);
}

internal vec4
MathAddVec4(vec4 A, vec4 B)
{
    vec4 Result;

    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    Result.Z = A.Z + B.Z;
    Result.W = A.W + B.W;

    return (Result);
}

internal vec2
MathSubtractVec2(vec2 A, vec2 B)
{
    vec2 Result;

    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;

    return (Result);
}

internal vec3
MathSubtractVec3(vec3 A, vec3 B)
{
    vec3 Result;

    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    Result.Z = A.Z - B.Z;

    return (Result);
}

internal vec4
MathSubtractVec4(vec4 A, vec4 B)
{
    vec4 Result;

    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    Result.Z = A.Z - B.Z;
    Result.W = A.W - B.W;

    return (Result);
}

internal vec2
MathMultiplyVec2(vec2 A, vec2 B)
{
    vec2 Result;

    Result.X = A.X * B.X;
    Result.Y = A.Y * B.Y;

    return (Result);
}

internal vec3
MathMultiplyVec3(vec3 A, vec3 B)
{
    vec3 Result;

    Result.X = A.Z * B.X;
    Result.Y = A.Y * B.Y;
    Result.Z = A.Z * B.Z;

    return (Result);
}

internal vec3
MathMultiplyVec3f(vec3 A, real32 B)
{
    vec3 Result;
    
    Result.X = A.Z * B;
    Result.Y = A.Y * B;
    Result.Z = A.Z * B;
    
    return (Result);
}

internal vec4
MathMultiplyVec4(vec4 A, vec4 B)
{
    vec4 Result;

    Result.X = A.X * B.X;
    Result.Y = A.Y * B.Y;
    Result.Z = A.Z * B.Z;
    Result.W = A.W * B.W;

    return (Result);
}

internal vec2
MathDivideVec2(vec2 A, vec2 B)
{
    vec2 Result;

    Result.X = A.X / B.X;
    Result.Y = A.Y / B.Y;

    return (Result);
}

internal vec3
MathDivideVec3(vec3 A, vec3 B)
{
    vec3 Result;

    Result.X = A.X / B.X;
    Result.Y = A.Y / B.Y;
    Result.Z = A.Z / B.Z;

    return (Result);
}

internal vec3
MathDivideVec3f(vec3 A, real32 B)
{
    vec3 Result;
    
    Result.X = A.X / B;
    Result.Y = A.Y / B;
    Result.Z = A.Z / B;
    
    return (Result);
}

internal vec4
MathDivideVec4(vec4 A, vec4 B)
{
    vec4 Result;

    Result.X = A.X / B.X;
    Result.Y = A.Y / B.Y;
    Result.Z = A.Z / B.Z;
    Result.W = A.W / B.W;

    return (Result);
}

internal mat4
MathMat4()
{
    mat4 Result = {0};

    return (Result);
}

internal mat4
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

internal mat4
MathMultiplyMat4(mat4 A, mat4 B)
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
                Sum += B.Elements[Rows][CurrentMatrice] * A.Elements[CurrentMatrice][Columns];
            }

            Result.Elements[Rows][Columns] = Sum;
        }
    }

    return (Result);
}

internal vec4
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

internal mat4
MathOrthographic(real32 A, real32 B, real32 Bottom, real32 Top, real32 Near, real32 Far)
{
    mat4 Result = MathMat4d(1.0f);

    Result.Elements[0][0] = 2.0f / (B - A);
    Result.Elements[1][1] = 2.0f / (Top - Bottom);
    Result.Elements[2][2] = 2.0f / (Near - Far);

    Result.Elements[3][0] = (A + B) / (A - B);
    Result.Elements[3][1] = (Bottom + Top) / (Bottom - Top);
    Result.Elements[3][2] = (Far + Near) / (Near - Far);

    return (Result);
}

internal mat4
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

internal mat4
MathTranslate(vec3 Translation)
{
    mat4 Result = MathMat4d(1.0f);

    Result.Elements[3][0] = Translation.X;
    Result.Elements[3][1] = Translation.Y;
    Result.Elements[3][2] = Translation.Z;

    return (Result);
}

internal mat4
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

internal mat4
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

internal mat4
MathScale(vec3 Scale)
{
    mat4 Result = MathMat4d(1.0f);

    Result.Elements[0][0] = Scale.X;
    Result.Elements[1][1] = Scale.Y;
    Result.Elements[2][2] = Scale.Z;

    return (Result);
}

internal quaternion
MathQuaternion(real32 X, real32 Y, real32 Z, real32 W)
{
    quaternion Result = {0};
    
    Result.X = X;
    Result.Y = Y;
    Result.Z = Z;
    Result.W = W;
    
    return(Result);
}

internal quaternion
MathQuaternionFromV4(vec4 Vector)
{
    quaternion Result = {0};
    
    Result.XYZ = Vector.XYZ;
    Result.W = Vector.W;
    
    return(Result);
}

internal quaternion
MathQuaternionFromV3(vec3 Vector, real32 W)
{
    quaternion Result = {0};
    
    Result.XYZ = Vector;
    Result.W = W;
    
    return(Result);
}

internal quaternion
MathAddQuaternion(quaternion A, quaternion B)
{
    quaternion Result = {0};
    
    Result.X = A.X + B.X;
Result.Y = A.Y + B.Y;
Result.Z = A.Z + B.Z;
Result.W = A.W + B.W;
    
    return(Result);
}

internal quaternion
MathSubtractQuaternion(quaternion A, quaternion B)
{
    quaternion Result = {0};
    
    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    Result.Z = A.Z - B.Z;
    Result.W = A.W - B.W;
    
    return(Result);
}

internal quaternion
MathMultiplyQuaternion(quaternion A, quaternion B)
{
    quaternion Result = {0};
    
    Result.X = (A.X * B.W) + (A.Y * B.Z) - (A.Z * B.Y) + (A.W * B.X);
    Result.Y = (-A.X * B.Z) + (A.Y * B.W) + (A.Z * B.X) + (A.W * B.Y);
    Result.Z = (A.X * B.Y) - (A.Y * B.X) + (A.Z * B.W) + (A.W * B.Z);
    Result.W = (-A.X * B.X) - (A.Y * B.Y) - (A.Z * B.Z) + (A.W * B.W);
    
    return(Result);
}

internal quaternion
MathMultiplyQuaternionf(quaternion A, real32 B)
{
    quaternion Result = {0};
    
    Result.X = A.X * B;
    Result.Y = A.Y * B;
    Result.Z = A.Z * B;
    Result.W = A.W * B;
    
    return(Result);
}

internal quaternion
MathDivideQuaternion(quaternion A, quaternion B)
{
    quaternion Result = {0};
    
    Result.X = A.X / B.X;
    Result.Y = A.Y / B.Y;
    Result.Z = A.Z / B.Z;
    Result.W = A.W / B.W;
    
    return(Result);
}


internal quaternion
MathDivideQuaternionf(quaternion A, real32 B)
{
    quaternion Result = {0};
    
    Result.X = A.X / B;
    Result.Y = A.Y / B;
    Result.Z = A.Z / B;
    Result.W = A.W / B;
    
    return(Result);
}

internal real32
MathDotQuaternion(quaternion A, quaternion B)
{
    real32 Result = .0f;
    
    Result = (A.X * B.X) + (A.Y * B.Y) + (A.Z * B.Z) + (A.W * B.W);
    
    return(Result);
}

internal quaternion
MathNormalizeQuaternion(quaternion A)
{
    quaternion Result = {0};
    
    real32 Length = MathSquareRoot(MathDotQuaternion(A, A));
    Result = MathDivideQuaternionf(A, Length);
    
    return(Result);
}

internal quaternion
MathSlerp(quaternion A, quaternion B, real32 Time)
{
    quaternion Result = {0};
    
    real32 CosTheta = MathDotQuaternion(A, B);
    real32 Angle = cosf(CosTheta);
    
    real32 S1 = sinf(1.f - Time * Angle);
    real32 S2 = sinf(Time * Angle);
    real32 Is = 1.f / sinf(Angle);
    
    A = MathMultiplyQuaternionf(A, S1);
    B = MathMultiplyQuaternionf(B, S2);
    
    Result = MathAddQuaternion(A, B);
    Result = MathMultiplyQuaternionf(Result, Is);
    
    return(Result);
}

internal mat4
MathQuaternionToMat4(quaternion A)
{
    mat4 Result = {0};
    Result = MathMat4d(1);
    
    quaternion nQuaternion = MathNormalizeQuaternion(A);
    
    float XX, YY, ZZ,
    XY, XZ, YZ,
    WX, WY, WZ;
    
    XX = nQuaternion.X * nQuaternion.X;
    YY = nQuaternion.Y * nQuaternion.Y;
    ZZ = nQuaternion.Z * nQuaternion.Z;
    XY = nQuaternion.X * nQuaternion.Y;
    XZ = nQuaternion.X * nQuaternion.Z;
    YZ = nQuaternion.Y * nQuaternion.Z;
    WX = nQuaternion.W * nQuaternion.X;
    WY = nQuaternion.W * nQuaternion.Y;
    WZ = nQuaternion.W * nQuaternion.Z;
    
    Result.Elements[0][0] = 1.0f - 2.0f * (YY + ZZ);
    Result.Elements[0][1] = 2.0f * (XY + WZ);
    Result.Elements[0][2] = 2.0f * (XZ - WY);
    
    Result.Elements[1][0] = 2.0f * (XY - WZ);
    Result.Elements[1][1] = 1.0f - 2.0f * (XX + ZZ);
    Result.Elements[1][2] = 2.0f * (YZ + WX);
    
    Result.Elements[2][0] = 2.0f * (XZ + WY);
    Result.Elements[2][1] = 2.0f * (YZ - WX);
    Result.Elements[2][2] = 1.0f - 2.0f * (XX + YY);
    
    return(Result);
}

internal quaternion
MathQuaternionFromAxisAngle(vec3 Axis, real32 Angle)
{
    quaternion Result = {0};
    
    real32 nVec3 = MathSquareRoot(MathDot(Axis, Axis));
    real32 RotationSine = sinf(Angle / 2.f);
    vec3 RotatedVector = MathMultiplyVec3f(Axis, RotationSine);
    
    Result.W = cosf(Angle / 2.f);
    Result.XYZ = MathDivideVec3f(RotatedVector, nVec3);
    
    return(Result);
}

#endif