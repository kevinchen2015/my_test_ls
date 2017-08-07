#ifndef _MathUtils_H__
#define _MathUtils_H__


#include "QuaternionD.h"
#include "Vector3D.h"


struct SinTableElement
{
    int iAngleD1;
    int iValueD4;
    SinTableElement(int _iAngleD1, int _iValueD4)
    {
        iAngleD1 = _iAngleD1;
        iValueD4 = _iValueD4;
    }
};

class MathUtils
{
public:

	static int SCALE_10K;
	static int SCALE_1000K;

	static  int iPointUnit;
    static  MATH_INT64 lPointUnit;
    static  float floatPointUnit ;

    static int iSqrtInit;

   static SinTableElement sinTable[901];

     // 
    // PI
    static const float PI;
    //

    // 浮点数精度
    static const float FLOAT_EPSION;
    static const float FLOAT_EPSION_SQUARED;

public:
	// Summary:
	//    开平方
	static float Sqrt(float s);

	// Summary:
	// 取绝对值
	template <typename T>
	static T Abs(const T& val)
	{
		return val >= 0 ? val : -val;
	}

	static  int IntAbs(int a)
    {
        return a >= 0 ? a : -a;
    }

   static MATH_INT64 Int64Abs(MATH_INT64 a)
   {
	   return a >= 0 ? a : -a;
   }

	// Summary:
    //     浮点数是否相等(精度范围内)
    static bool IsFloatEqual(float a, float b);

	 // Summary:
    //     角度转弧度
    static float Degree2Radian(float degree);

    // Summary:
    //     弧度转角度
    static float Radian2Degree(float radian);

	// Summary:
    //     clamp
    static int Clamp(int x, int min, int max);

	template <typename T>
	static T Min( T a,  T b)
	{
		return a < b ? a : b;
	}

	template <typename T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}


    /// <summary>
    /// 插值
    /// </summary>
    /// <param name="iNum1"></param>
    /// <param name="iNum2"></param>
    /// <param name="iBlendD4">4位整数插值 10000表示1</param>
    /// <returns></returns>
    static MATH_INT64 LerpD4(MATH_INT64 iNum1, MATH_INT64 iNum2, int iBlendD4)
    {
		MATH_INT64 lRetVal = (MATH_INT64)iNum1 * lPointUnit + (MATH_INT64) (iNum2 - iNum1) * (MATH_INT64)iBlendD4;
        return (MATH_INT64)(lRetVal / lPointUnit);
    }

	  static int LerpD4(int iNum1, int iNum2, int iBlendD4)
    {
		MATH_INT64 lRetVal = (MATH_INT64)iNum1 * lPointUnit + (MATH_INT64) (iNum2 - iNum1) * (MATH_INT64)iBlendD4;
        return (int)(lRetVal / lPointUnit);
    }

    /// <summary>
    /// 求平方
    /// </summary>
    /// <param name="lNum"></param>
    /// <returns></returns>
    static MATH_INT64 SqrtD4(MATH_INT64 lNum)
    {
		if (lNum == 0)
			return 0;

		MATH_INT64 lCurr = (MATH_INT64)iSqrtInit;
		MATH_INT64 lLast = lCurr;

		MATH_INT64 lDelta = 100000;
		int iIter = 0;
		do
		{
			lLast = lCurr;
			lCurr = (lCurr + lNum * lPointUnit / lCurr) / 2;
			lDelta = lCurr - lLast;
			iIter++;
		} while ((lDelta <= -2 || lDelta > 2) && iIter < 10 && lCurr != 0);

		return lCurr;
    }

    static int SinD4(int iAngleD1)
    {
        iAngleD1 = iAngleD1 % 3600;
        int iPositiveAngle = iAngleD1;
        if (iAngleD1 < 0)
        {
            iPositiveAngle = -iAngleD1;
        }

        int iValue = 0;
        int iDomain = iPositiveAngle / 900;
        if (iDomain == 0)
            iValue = sinTable[iAngleD1].iValueD4;
        else if (iDomain == 1)
            iValue = sinTable[1800 - iPositiveAngle].iValueD4;
        else if (iDomain == 2)
            iValue = -sinTable[iPositiveAngle - 1800].iValueD4;
        else if (iDomain == 3)
            iValue = -sinTable[3600 - iPositiveAngle].iValueD4;

        if (iAngleD1 < 0)
            return -iValue;
        return iValue;
    }

    static int CosD4(int iAngleD1)
    {
        return SinD4(iAngleD1 + 900);
    }

	static MATH_INT64 clampAngleD4(MATH_INT64 angle)
    {
        while (angle > 3600000)
            angle -= 3600000;
        while (angle < 0)
            angle += 3600000;

        return angle;
    }

	static Vector3D AngleD4ToDir(int lAngle)
    {
        MATH_INT64 angle = clampAngleD4((MATH_INT64)lAngle);
        QuaternionD quat;
        quat.FromAngleAxis((int)angle, Vector3D::UNIT_Y);
		Vector3D dir = quat * Vector3D::FORWARD;
        return dir;
    }

    static QuaternionD AngleD4ToQuat(int lAngle)
    {
        MATH_INT64 angle = clampAngleD4((MATH_INT64)lAngle);
        QuaternionD quat ;
        quat.FromAngleAxis((int)angle, Vector3D::UNIT_Y);
        return quat;
    }


	static float DirToAngleFloat(Vector3D vDirD4);

	static bool InRect(Vector3D p1, Vector3D p2, Vector3D pos, MATH_INT64 radius)
	{
		p1.y = 0;
		p2.y = 0;
		pos.y = 0;

		int halfWidth = (int)(radius / 2);

		Vector3D d = (p2 - p1);

		// 坐标旋转后的pos的x, y //
		Vector3D c = pos - p1;

		// 因为万单位的偏差，所以2个点的距离小于0.5米，那么则按照圆形来判断 //
		if (d.CompareLen(5000) < 0/*length < 5000*/)
		{
			if (c.CompareLen(halfWidth) <= 0/*c.magnitudeD4 <= halfWidth*/)
			{
				return true;
			}
			return false;
		}

		int length = d.magnitudeD4();
		d.NormalizeD4();

		MATH_INT64 x = (MATH_INT64)(c.x) * (MATH_INT64)(d.x) + (MATH_INT64)(c.z) * (MATH_INT64)(d.z);
		MATH_INT64 z = (MATH_INT64)(-c.x) * (MATH_INT64)(d.z) + (MATH_INT64)(c.z) * (MATH_INT64)(d.x);

		MATH_INT64 ix = x / iPointUnit;
		MATH_INT64 iz = z / iPointUnit;
		int posx = (int)ix;
		int posz = (int)iz;

		if (posx >= 0 && posx <= length &&
			posz >= -halfWidth && posz <= halfWidth
			)
		{
			return true;
		}
		return false;
	}

	static bool InLine(Vector3D from, Vector3D target, Vector3D faceTo, int iLen, int iHeight)
	{
		Vector3D toCenter = target;
		Vector3D fromCenter = from;
		Vector3D dir = toCenter - fromCenter;
		dir.y = 0;

		if (faceTo.CompareLen(100) > 0)
		{
			QuaternionD rot = QuaternionD::GetRotationFromTo(QuaternionD::DefaultDir,faceTo);
			QuaternionD inverse = rot.Inverse();//反方向旋转//
			Vector3D vDir2 = inverse * dir;//把坐标空间转换到轴对齐//
		
			if (vDir2.z > 0 && vDir2.z <= iLen  && IntAbs(vDir2.x) < iHeight / 2)
			{
				return true;
			}
		}
		return false;
	}

	static MATH_INT64 Divid(MATH_INT64 a, MATH_INT64 b)
	{
		return a * MathUtils::lPointUnit / b;
	}
};



#endif
