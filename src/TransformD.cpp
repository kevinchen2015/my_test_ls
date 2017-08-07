
#include "TransformD.h"

TransformD::TransformD()
{
}

TransformD::TransformD(Vector3D _position, Vector3D _rotate)
	:position(_position), faceToD4(_rotate)
{

}

TransformD& TransformD::operator=(const TransformD& v)
{
	this->position = v.position;
	this->faceToD4 = v.faceToD4;
	return *this;
}





