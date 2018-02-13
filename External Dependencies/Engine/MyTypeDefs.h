typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned char uchar;
typedef void (*uniformCallback)(const void* data,const uint& programID,const char* variableName);

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define MEGABYTE 1048576
#define MEGABYTE 4090000
#define KILOBYTE 1024
#define TARGETFRAMERATE 0.01666666666666666666666666666667f
#define FLOAT_MAX 3.402823466e+38F  
#define FLOAT_MIN 1.175494351e-38F  
#define PI 3.1415926535897932384626433832795
#define PI_2 1.57079632679489661923
#define PI_4 0.785398163397448309616
#define _180_PI 57.295779513082320876798154814105
#define _2_PI 6.283185307179586476925286766559
#define UNBOUND_TEXTURE -1
#define NUM_UNIFORM_CALLBACKS 16
#define HUGE_K 1.0e10

#ifndef ENUMS
#define ENUMS

enum BufferType{vertexBuffer,indexBuffer};
enum DrawType{drawTriangle,drawLine};
enum Direction {LEFT , RIGHT, UP, DOWN , BACKWARD , FORWARD};
enum VectorDataType{position,color,normal,uv,tangent,bitangent};
enum DataType{INT1=0,INT2=1,INT3=2,INT4=3,UINT1=4,UINT2=5,UINT3=6,UINT4=7,FLOAT1=8,FLOAT2=9,FLOAT3=10,FLOAT4=11,MAT2x2=12,MAT3=13,MAT4=14,TEXTURE=15,SUBROUTINE=16};

#endif

