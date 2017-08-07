#ifndef _QuaternionD_H__
#define _QuaternionD_H__


#include "Vector3D.h"


struct QuaternionD
{
	static const QuaternionD IDENTITY;
	static const Vector3D DefaultDir;

    int x, y, z, w;

	QuaternionD();

    QuaternionD(int _w, int _x, int _y, int _z);

    Vector3D Rotate(Vector3D v);

    void FromAngleAxis(int AngleD4, Vector3D rkAxis);

    void Negative_FromAngleAxis(int AngleD4, Vector3D rkAxis);

    //-----------------------------------------------------------------------
    MATH_INT64 Norm();

    int Normalise();

    void Product(MATH_INT64 flagD4);

    QuaternionD Inverse();

    QuaternionD UnitInverse();

    QuaternionD& operator *(const QuaternionD& b);

    Vector3D operator *(const Vector3D& b);

	static QuaternionD GetRotationFromTo(Vector3D from, Vector3D dest);

};



#endif // 
