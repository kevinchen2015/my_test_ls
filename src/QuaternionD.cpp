
#include "QuaternionD.h"
#include "MathUtils.h"



// Summary:
//     几个常量
const QuaternionD QuaternionD::IDENTITY(0, 0, 0, MathUtils::iPointUnit);
const Vector3D QuaternionD::DefaultDir(0, 0, MathUtils::iPointUnit);//默认z正向为正前方//

QuaternionD::QuaternionD()
{
	x = 0;
    y = 0;
    z = 0;
    w = MathUtils::iPointUnit;
}

QuaternionD::QuaternionD(int _w, int _x, int _y, int _z)
{
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

Vector3D QuaternionD::Rotate(Vector3D v)
{
    Vector3D uv;
    Vector3D uuv;

    Vector3D qvec =  Vector3D(x, y, z);

    uv = qvec.CrossD4(v);
    uuv = qvec.CrossD4(uv);
    uv *= (20000 * w) / MathUtils::iPointUnit;
    uuv *= 20000;

    uv /= MathUtils::iPointUnit;
    uuv /= MathUtils::iPointUnit;

    return v + uv + uuv;
}

void QuaternionD::FromAngleAxis(int AngleD4, Vector3D rkAxis)
{
    // assert:  axis[] is unit length
    //
    // The quaternion representing the rotation is
    //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

    if (AngleD4 < 0)
    {
        //Negative_FromAngleAxis(-AngleD4, rkAxis);
        //return;
    }

    MATH_INT64 HalfAngle = ((MATH_INT64)AngleD4 * (MATH_INT64)5000 / MathUtils::lPointUnit);//Radian fHalfAngle ( 0.5*rfAngle );
    int fSin = MathUtils::SinD4((int)HalfAngle / 1000);

    w = MathUtils::CosD4((int)HalfAngle / 1000);//Math::Cos(fHalfAngle);
    x = fSin * rkAxis.x / MathUtils::iPointUnit;
    y = fSin * rkAxis.y / MathUtils::iPointUnit;
    z = fSin * rkAxis.z / MathUtils::iPointUnit;
}

void QuaternionD::Negative_FromAngleAxis(int AngleD4, Vector3D rkAxis)
{
    MATH_INT64 HalfAngle = ((MATH_INT64)AngleD4 * (MATH_INT64)5000 / MathUtils::lPointUnit);//Radian fHalfAngle ( 0.5*rfAngle );
    int fSin = -MathUtils::SinD4((int)HalfAngle / 1000);

    w = MathUtils::CosD4((int)HalfAngle / 1000);//Math::Cos(fHalfAngle);
    x = fSin * rkAxis.x / MathUtils::iPointUnit;
    y = fSin * rkAxis.y / MathUtils::iPointUnit;
    z = fSin * rkAxis.z / MathUtils::iPointUnit;
}

//-----------------------------------------------------------------------
MATH_INT64 QuaternionD::Norm()
{
    return ((MATH_INT64)w * (MATH_INT64)w + (MATH_INT64)x * (MATH_INT64)x + (MATH_INT64)y * (MATH_INT64)y + (MATH_INT64)z * (MATH_INT64)z) / MathUtils::lPointUnit;
}

int QuaternionD::Normalise()
{
    MATH_INT64 len = Norm();
    MATH_INT64 factor = 10000 * MathUtils::lPointUnit / MathUtils::SqrtD4(len);

    Product(factor);

    return (int)len;
}

void QuaternionD::Product(MATH_INT64 flagD4)
{
    x = (int)((MATH_INT64)x * flagD4 / MathUtils::lPointUnit);
    y = (int)((MATH_INT64)y * flagD4 / MathUtils::lPointUnit);
    z = (int)((MATH_INT64)z * flagD4 / MathUtils::lPointUnit);
    w = (int)((MATH_INT64)w * flagD4 / MathUtils::lPointUnit);
}

QuaternionD QuaternionD::Inverse()
{
    MATH_INT64 ww = w;
    MATH_INT64 xx = x;
    MATH_INT64 yy = y;
    MATH_INT64 zz = z;

    MATH_INT64 fNorm = (ww * ww + xx * xx + yy * yy + zz * zz) / MathUtils::lPointUnit;

    if (fNorm > 0.0)
    {
        MATH_INT64 fInvNorm = MathUtils::lPointUnit * MathUtils::lPointUnit / fNorm;
        ww = ww * fInvNorm / MathUtils::lPointUnit;
        xx = -xx * fInvNorm / MathUtils::lPointUnit;
        yy = -yy * fInvNorm / MathUtils::lPointUnit;
        zz = -zz * fInvNorm / MathUtils::lPointUnit;
        return QuaternionD((int)ww, (int)xx, (int)yy, (int)zz);
    }
    else
    {
        return QuaternionD(0, 0, 0, 0);
    }
}

QuaternionD QuaternionD::UnitInverse()
{
    return QuaternionD(w, -x, -y, -z);
}

QuaternionD& QuaternionD::operator *(const QuaternionD& b)
{
    QuaternionD temp = QuaternionD
    (
        (int)(((MATH_INT64)w * (MATH_INT64)b.w - (MATH_INT64)x * (MATH_INT64)b.x - (MATH_INT64)y * (MATH_INT64)b.y - (MATH_INT64)z * (MATH_INT64)b.z) / MathUtils::lPointUnit),
        (int)(((MATH_INT64)w * (MATH_INT64)b.x + (MATH_INT64)x * (MATH_INT64)b.w + (MATH_INT64)y * (MATH_INT64)b.z - (MATH_INT64)z * (MATH_INT64)b.y) / MathUtils::lPointUnit),
        (int)(((MATH_INT64)w * (MATH_INT64)b.y + (MATH_INT64)y * (MATH_INT64)b.w + (MATH_INT64)z * (MATH_INT64)b.x - (MATH_INT64)x * (MATH_INT64)b.z) / MathUtils::lPointUnit),
        (int)(((MATH_INT64)w * (MATH_INT64)b.z + (MATH_INT64)z * (MATH_INT64)b.w + (MATH_INT64)x * (MATH_INT64)b.y - (MATH_INT64)y * (MATH_INT64)b.x) / MathUtils::lPointUnit)
    );

	 *this = temp;

	 return *this;
}

Vector3D QuaternionD::operator *(const Vector3D& b)
{
    return Rotate(b);
}

QuaternionD QuaternionD::GetRotationFromTo(Vector3D from, Vector3D dest)
{
	// Based on Stan Melax's article in Game Programming Gems
	QuaternionD q;
	// Copy, since cannot modify local
	Vector3D v0 = from;
	Vector3D v1 = dest;

	v0.NormalizeD4();
	v1.NormalizeD4();

	MATH_INT64 d = Vector3D::DotD4(v0, v1);//v0.dotProduct(v1);
	// If dot == 1, vectors are the same
	if (d >= MathUtils::iPointUnit)
	{
		return QuaternionD(MathUtils::iPointUnit, 0, 0, 0);
	}
	if (d <= (0 - MathUtils::iPointUnit))
	{
		Vector3D xzhou = Vector3D(MathUtils::iPointUnit, 0, 0);
		Vector3D axis = xzhou.CrossD4(from);
		if (axis.magnitudeD4() < 100)
		{
			Vector3D yzhou(0, MathUtils::iPointUnit, 0);
			axis = yzhou.CrossD4(from);
		}
		axis.NormalizeD4();
		q.FromAngleAxis(1800000, axis);
	}
	else
	{
		MATH_INT64 s = MathUtils::SqrtD4((MathUtils::iPointUnit + d) * 2);//Real s = Math::Sqrt( (1+d)*2 );
		MATH_INT64 invs = MathUtils::iPointUnit * MathUtils::lPointUnit / s;

		Vector3D c = v0.CrossD4(v1);

		q.x = (int)(c.x * invs / MathUtils::lPointUnit);
		q.y = (int)(c.y * invs / MathUtils::lPointUnit);
		q.z = (int)(c.z * invs / MathUtils::lPointUnit);
		q.w = (int)(s * 5000 / MathUtils::lPointUnit);

		q.Normalise();
	}
	return q;
}

