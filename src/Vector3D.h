#ifndef __Vector3D_H__
#define __Vector3D_H__

typedef long long MATH_INT64 ;

struct Vector3D
{
	static const Vector3D UNIT_X;
	static const Vector3D UNIT_Y;
	static const Vector3D UNIT_Z;
	static const Vector3D ZERO;
	static const Vector3D ONE;
	static const Vector3D FORWARD;

    int x;
    int y;
    int z;

	Vector3D()
	{
		x = 0;
		y = 0;
		z = 0;
	}

    Vector3D(int _x, int _y, int _z)
    {
        x = _x;
		y = _y;
        z = _z;
    }

    Vector3D(const Vector3D& src)
    {
        x = src.x;
        y = src.y;
        z = src.z;
    }

    void Set(int _x, int _y, int _z)
    {
        x = _x;
		y = _y;
        z = _z;
    }

    void Set(const Vector3D& src)
    {
        x = src.x;
        y = src.y;
        z = src.z;
    }

    Vector3D ProductD4(int val);
    
	// Summary:
	//     负号
	Vector3D operator -() const;

	// Summary:
	//     加减
	Vector3D operator +(const Vector3D& v) const;
	Vector3D operator -(const Vector3D& v) const;
	Vector3D& operator +=(const Vector3D& v);
	Vector3D& operator -=(const Vector3D& v);

	// Summary:
	//     向量与标量的乘法和除法
	Vector3D operator *(int s) const;
	Vector3D operator /(int s) const;
	Vector3D& operator *=(int s);
	Vector3D& operator /=(int s);
	Vector3D operator +(int s);
	Vector3D operator -(int s);

    // Summary:
    //     向量左乘
    friend Vector3D operator *(int s, const Vector3D& v);

	// Summary:
	//      operator ==
	bool operator ==(const Vector3D& v) const;
    bool operator != (const Vector3D& v) const;


	Vector3D& operator=(const Vector3D& v);

    static int DotD4(Vector3D a, Vector3D b);

    Vector3D CrossD4(Vector3D b);
   
    static Vector3D CrossD4(Vector3D a, Vector3D b);

    static Vector3D LerpD4(Vector3D a, Vector3D b, int iBlendD4);

    MATH_INT64 magnitudeSq();

    int magnitudeD4();

    void NormalizeD4();

    Vector3D ForwardOnDir(int iLen);
  
    int CompareLen(MATH_INT64 len);

	//const std::string ToString();

	static Vector3D Zero()
    {
        return ZERO;
    }

    static Vector3D One()
    {
        return ONE;
    }

	
};




#endif // 
