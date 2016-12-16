// (c) 2016 ZaKlaus; All Rights Reserved

doc(v3_4x)
doc_cat(SIMD)
doc_string(3-dimensional vector component.)
doc_sig(
typedef union v3_4x
{
    struct
    {
        __m128 X;
        __m128 R;
    };
    
    struct
    {
        __m128 Y;
        __m128 G;
    };
    
    struct
    {
        __m128 Z;
        __m128 B;
    };
} v3_4x;
)


doc(v4_4x)
doc_string(4-dimensional vector component.)
doc_sig(
typedef union v4_4x
{
    struct
    {
        __m128 X;
        __m128 R;
    };
    
    struct
    {
        __m128 Y;
        __m128 G;
    };
    
    struct
    {
        __m128 Z;
        __m128 B;
    };
    
    struct
    {
        __m128 W;
        __m128 A;
    };
} v4_4x;
)

#define mmSquare(a) _mm_mul_ps(a, a)    
#define M(a, i) ((float *)&(a))[i]
#define Mi(a, i) ((uint32 *)&(a))[i]

doc_sep()

doc(SIMDMultiplyv3_4x)
doc_string(Multiply v3_4x with scalar.)
doc_sig(
internal v3_4x
SIMDMultiplyv3_4x(f32 As,
                  v3_4x B)
)
{
    v3_4x Result;
    
    __m128 A = _mm_set1_ps(As);
    Result.X = _mm_mul_ps(A, B.X);
    Result.Y = _mm_mul_ps(A, B.Y);
    Result.Z = _mm_mul_ps(A, B.Z);
    
    return(Result);
}


doc(SIMDAddv3_4x)
doc_string(Add scalar to v3_4x.)
doc_sig(
 internal v3_4x
SIMDAddv3_4x(v3_4x A, 
             v3_4x B)
)
{
    v3_4x Result;
    
    Result.X = _mm_add_ps(A.X, B.X);
    Result.Y = _mm_add_ps(A.Y, B.Y);
    Result.Z = _mm_add_ps(A.Z, B.Z);
    
    return(Result);
}


doc(SIMDMultiplyv4_4x)
doc_string(Multiply v4_4x with scalar.)
doc_sig(
internal v4_4x
SIMDMultiplyv4_4x(f32 As, 
                  v4_4x B)
)
{
    v4_4x Result;
    
    __m128 A = _mm_set1_ps(As);
    Result.X = _mm_mul_ps(A, B.X);
    Result.Y = _mm_mul_ps(A, B.Y);
    Result.Z = _mm_mul_ps(A, B.Z);
    Result.W = _mm_mul_ps(A, B.W);
    
    return(Result);
}


doc(SIMDAddv4_4x)
doc_string(Add scalar to v4_4x.)
doc_sig(
internal v4_4x
SIMDAddv4_4x(v4_4x A, 
             v4_4x B)
)
{
    v4_4x Result;
    
    Result.X = _mm_add_ps(A.X, B.X);
    Result.Y = _mm_add_ps(A.Y, B.Y);
    Result.Z = _mm_add_ps(A.Z, B.Z);
    Result.W = _mm_add_ps(A.W, B.W);
    
    return(Result);
}


doc(SIMDTov3_4x)
doc_string(Convert v3 to v3_4x.)
doc_sig(
internal v3_4x 
SIMDTov3_4x(
v3 A)
)
{
    v3_4x Result;
    
    Result.X = _mm_set1_ps(A.X);
    Result.Y = _mm_set1_ps(A.Y);
    Result.Z = _mm_set1_ps(A.Z);
    
    return(Result);
}