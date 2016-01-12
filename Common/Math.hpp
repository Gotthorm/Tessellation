// MATH.HPP

#ifndef _MATH_HPP_
#define _MATH_HPP_

/** @brief A core math utility class.
  * @author Sean Butterworth
  * @version 1.0
  * @date Sept 2003
**/

/**
	* @todo 
	* Document the public methods
**/

class Math
{
public:
    // Wrappers to hide implementations of functions.  The ACos and ASin
    // functions clamp the input argument to [-1,1] to avoid NaN issues
    // when the input is slightly larger than 1 or slightly smaller than -1.
    // Other functions have the potential for using a fast and approximate
    // algorithm rather than calling the standard math library functions.
    static float ACos( float fValue );
    static float ASin( float fValue );
    static float ATan( float fValue );
    static float ATan2( float fY, float fX );
    static float Ceil( float fValue );
    static float Cos( float fValue );
    static float Exp( float fValue );
    static float FAbs( float fValue );
    static float Floor( float fValue );
    static float FMod( float fX, float fY );
    static float InvSqrt( float fValue );
    static float Log( float fValue );
    static float Pow( float fBase, float fExponent );
    static float Sin( float fValue );
    static float Sqr( float fValue );
    static float Sqrt( float fValue );
    static float Tan( float fValue );

    // Return -1 if the input is negative, 0 if the input is zero, 
	// and +1 if the input is positive.
    static int Sign( int nValue );
    static float Sign( float fValue );

    // Generate a random number in [0,1).  The random number generator may
    // be seeded by a first call to UnitRandom with a positive seed.
    static float UnitRandom( float fSeed = 0.0f );

    // Generate a random number in [-1,1).  The random number generator may
    // be seeded by a first call to SymmetricRandom with a positive seed.
    static float SymmetricRandom( float fSeed = 0.0f );

    // Generate a random number in [min,max).  The random number generator may
    // be seeded by a first call to IntervalRandom with a positive seed.
    static float IntervalRandom( float fMin, float fMax, float fSeed = 0.0f );

    // Fast evaluation of sin(angle) by polynomial approximations.  The angle
    // must be in [0,pi/2].  The maximum absolute error is about 1.7e-04 for
    // FastSin0 and about 2.3e-09 for FastSin1.  The speed up is about 2 for
    // FastSin0 and about 1.5 for FastSin1.
    static float FastSin0( float fAngle );
    static float FastSin1( float fAngle );

    // Fast evaluation of cos(angle) by polynomial approximations.  The angle
    // must be in [0,pi/2].  The maximum absolute error is about 1.2e-03 for
    // FastCos0 and about 2.3e-09 for FastCos1.  The speed up is about 2 for
    // FastCos0 and about 1.5 for FastCos1.
    static float FastCos0( float fAngle );
    static float FastCos1( float fAngle );

    // Fast evaluation of tan(angle) by polynomial approximations.  The angle
    // must be in [0,pi/4].  The maximum absolute error is about 8.1e-04 for
    // FastTan0 and about 1.9e-08 for FastTan1.  The speed up is about 2.5 for
    // FastTan0 and about 1.75 for FastTan1.
    static float FastTan0( float fAngle );
    static float FastTan1( float fAngle );

    // Fast evaluation of asin(value) by a sqrt times a polynomial.  The value
    // must be in [0,1].  The maximum absolute error is about 6.8e-05 and the
    // speed up is about 2.5.
    static float FastInvSin( float fValue );

    // Fast evaluation of acos(value) by a sqrt times a polynomial.  The value
    // must be in [0,1].  The maximum absolute error is about 6.8e-05 and the
    // speed up is about 2.5.
    static float FastInvCos( float fValue );

    // Fast evaluation of atan(value) by polynomial approximations.  The value
    // must be in [-1,1].  The maximum absolute error is about 1.2e-05 for
    // FastInvTan0 and about 1.43-08 for FastInvTan1.  The speed up is about
    // 2.2 for FastInvTan0 and about 1.5 for FastInvTan1.
    static float FastInvTan0( float fValue );
    static float FastInvTan1( float fValue );

    // A fast approximation to 1/sqrt.
    static float FastInvSqrt( float fValue );

    // Gamma and related functions
    static float LogGamma( float fX );
    static float Gamma( float fX );
    static float IncompleteGamma( float fA, float fX );

    // Error functions
    static float Erf( float fX );   // polynomial approximation
    static float Erfc( float fX );  // erfc(x) = 1-erf(x)

    // Modified Bessel functions of order 0 and 1
    static float ModBessel0( float fX );
    static float ModBessel1( float fX );

	static float Max( float fValue1, float fValue2 );
	static float Min( float fValue1, float fValue2 );

    // common constants
    static const float EPSILON;
    static const float MAX_FLOAT;
    static const float PI;
    static const float TWO_PI;
    static const float HALF_PI;
    static const float INV_PI;
    static const float INV_TWO_PI;
    static const float DEG_TO_RAD;
    static const float RAD_TO_DEG;

	// Compare two arrays of floats of a given length
	static int CompareArrays( int nCount, const float* afObject1, const float* afObject2 );

	// Regardless of sign, the target value will be brought closer zero by the given amount
	// The amount value must be positive
	static void DecreaseTowardsZero( float& rTargetValue, float fAmount );

private:
    // Series form (used when fX < 1+fA)
    static float IncompleteGammaS( float fA, float fX );

    // Continued fraction form (used when fX >= 1+fA)
    static float IncompleteGammaCF( float fA, float fX );
};

#endif // _MATH_HPP_

