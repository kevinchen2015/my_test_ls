
#include "Vector3D.h"
#include "MathUtils.h"
//

//

// Summary:
//     几个常量
const Vector3D Vector3D::UNIT_X(MathUtils::iPointUnit, 0, 0);
const Vector3D Vector3D::UNIT_Y(0, MathUtils::iPointUnit, 0);
const Vector3D Vector3D::UNIT_Z(0, 0, MathUtils::iPointUnit);
const Vector3D Vector3D::ZERO(0, 0, 0);
const Vector3D Vector3D::ONE(MathUtils::iPointUnit, MathUtils::iPointUnit, MathUtils::iPointUnit);
const Vector3D Vector3D::FORWARD(0, 0, MathUtils::iPointUnit);


// Summary:
//     负号
 Vector3D Vector3D::operator -() const
{
	return Vector3D(-x, -y, -z);
}

// Summary:
//     加减
 Vector3D Vector3D::operator +(const Vector3D& v) const
{
	return Vector3D(v.x + x, v.y + y, v.z + z);
}

 Vector3D Vector3D::operator -(const Vector3D& v) const
{
	return Vector3D(x - v.x, y - v.y, z - v.z);
}

 Vector3D& Vector3D::operator +=(const Vector3D& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

 Vector3D& Vector3D::operator -=(const Vector3D& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

// Summary:
//     向量与标量的乘法和除法
 Vector3D Vector3D::operator *(int s) const
{
	return Vector3D(x * s, y * s, z * s);
}
 
 Vector3D Vector3D::operator /(int s) const
{
	return Vector3D(x / s, y / s, z / s);
}

 Vector3D& Vector3D::operator *=(int s)
{
	x *= s;
	y *= s;
	z *= s;

	return *this;
}
 
 Vector3D& Vector3D::operator /=(int s)
{
	x /= s;
	y /= s;
	z /= s;

	return *this;
}

 Vector3D Vector3D::operator +(int s)
{
	return Vector3D(x + s, y + s, z + s);
}

 Vector3D Vector3D::operator -(int s)
{
	return Vector3D(x - s, y - s, z - s);
}


// Summary:
//      operator ==
 bool Vector3D::operator ==(const Vector3D& v) const
{
	return (v.x == x) && (v.y == y) && (v.z == z);
}

 bool Vector3D::operator !=(const Vector3D& v) const
{
    return (v.x != x) || (v.y != y) || (v.z != z);
}

Vector3D& Vector3D::operator=(const Vector3D& v)
{
	x = v.x;
	y = v.y;
	z = v.z;

	return *this;
}

// Summary:
//     向量左乘
 Vector3D operator *(int s, const Vector3D& v)
{
    return v * s;
}

Vector3D  Vector3D::ProductD4(int val)
{
    x = (int)((MATH_INT64)x * (MATH_INT64)val / MathUtils::lPointUnit);
    y = (int)((MATH_INT64)y * (MATH_INT64)val / MathUtils::lPointUnit);
    z = (int)((MATH_INT64)z * (MATH_INT64)val / MathUtils::lPointUnit);

    return *this;
}

int  Vector3D::DotD4(Vector3D a, Vector3D b)
{
    int iRet =
        (int)((MATH_INT64)a.x * (MATH_INT64)b.x / MathUtils::lPointUnit)
        + (int)((MATH_INT64)a.y * (MATH_INT64)b.y / MathUtils::lPointUnit)
        + (int)((MATH_INT64)a.z * (MATH_INT64)b.z /MathUtils::lPointUnit);
    return iRet;
}

Vector3D  Vector3D::CrossD4(Vector3D b)
{
    return Vector3D(
            (int)(((MATH_INT64)y * (MATH_INT64)b.z - (MATH_INT64)z * (MATH_INT64)b.y) / MathUtils::lPointUnit),
            (int)(((MATH_INT64)z * (MATH_INT64)b.x - (MATH_INT64)x * (MATH_INT64)b.z) / MathUtils::lPointUnit),
            (int)(((MATH_INT64)x * (MATH_INT64)b.y - (MATH_INT64)y * (MATH_INT64)b.x) / MathUtils::lPointUnit));
}

Vector3D  Vector3D::CrossD4(Vector3D a, Vector3D b)
{
    return Vector3D(
            (int)(((MATH_INT64)a.y * (MATH_INT64)b.z - (MATH_INT64)a.z * (MATH_INT64)b.y) / MathUtils::lPointUnit),
            (int)(((MATH_INT64)a.z * (MATH_INT64)b.x - (MATH_INT64)a.x * (MATH_INT64)b.z) / MathUtils::lPointUnit),
            (int)(((MATH_INT64)a.x * (MATH_INT64)b.y - (MATH_INT64)a.y * (MATH_INT64)b.x) / MathUtils::lPointUnit));
}

Vector3D  Vector3D::LerpD4(Vector3D a, Vector3D b, int iBlendD4)
{
    Vector3D ret;
    ret.x = MathUtils::LerpD4(a.x, b.x, iBlendD4);
    ret.y = MathUtils::LerpD4(a.y, b.y, iBlendD4);
    ret.z = MathUtils::LerpD4(a.z, b.z, iBlendD4);
    return ret;
}

MATH_INT64 Vector3D::magnitudeSq()
{
    return (MATH_INT64)x * (MATH_INT64)x + (MATH_INT64)y * (MATH_INT64)y + (MATH_INT64)z * (MATH_INT64)z; 
}

int Vector3D::magnitudeD4()
{
	//按4位精度定点数计算
	MATH_INT64 ret = MathUtils::SqrtD4(magnitudeSq() / MathUtils::lPointUnit);
	return (int)ret;
}

void  Vector3D::NormalizeD4()
{
    int iMag = magnitudeD4();
    if (iMag == 0)
    {
        return;
    }
    x = (int)(x * MathUtils::lPointUnit / iMag);
    y = (int)(y * MathUtils::lPointUnit / iMag);
    z = (int)(z * MathUtils::lPointUnit / iMag);
}

Vector3D  Vector3D::ForwardOnDir(int iLen)
{
    int mag = magnitudeD4();
	Vector3D ret = ZERO;
    if (mag != 0)
    {
        ret.x = (int)((MATH_INT64)iLen * (MATH_INT64)x / (MATH_INT64)mag);
        ret.y = (int)((MATH_INT64)iLen * (MATH_INT64)y / (MATH_INT64)mag);
        ret.z = (int)((MATH_INT64)iLen * (MATH_INT64)z / (MATH_INT64)mag);
    }
    return ret;
}
  
int Vector3D::CompareLen(MATH_INT64 len)
{
    if (len < 0)
        return 1;

    MATH_INT64 myLen = magnitudeSq();
    len *= len;

    if (myLen > len)
        return 1;
    else if (myLen < len)
        return -1;

    return 0;
}

/*
const std::string Vector3D::ToString()
{
	char szLog[128] = {0};
	sprintf(szLog, "[Vector3D:x=%d, y=%d, z=%d, magnitudeD4=%d]", x, y, z, magnitudeD4());
	return szLog;
}
*/
