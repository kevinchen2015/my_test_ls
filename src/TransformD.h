#ifndef _TransformD_H__
#define _TransformD_H__


#include "Vector3D.h" 


class TransformD
{
public:
	
	//λ��
	Vector3D position;

	//����
	Vector3D faceToD4;

public:
	TransformD();
	TransformD(Vector3D _position, Vector3D _rotate);

	TransformD& operator=(const TransformD& v);
};



#endif // 