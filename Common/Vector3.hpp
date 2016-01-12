// VECTOR3.HPP

#ifndef _VECTOR3_HPP_
#define _VECTOR3_HPP_

#include "Math.hpp"

/** @brief A 3D Vector class.
  * @author Sean Butterworth
  * @version 1.0
  * @date Oct 2003
**/

/**
	* @todo 
	* Document the public methods
**/

class Vector3 
{
public:
    // Construction
    inline Vector3();
    Vector3( float fX, float fY, float fZ );
    Vector3( const Vector3& rV );
    Vector3( const float* aXYZ );

    // Member access
    float X;
    float Y;
    float Z;

    // Assignment
    Vector3& operator = ( const Vector3& rV );

    // Coordinate access
    operator const float* () const;
    operator float* ();

    // comparison
    bool operator == ( const Vector3& rV ) const;
    bool operator != ( const Vector3& rV ) const;

    // Arithmetic operations
    Vector3 operator + ( const Vector3& rV ) const;
    Vector3 operator - ( const Vector3& rV ) const;
    Vector3 operator * ( float fScalar ) const;
    Vector3 operator / ( float fScalar ) const;
    Vector3 operator - () const;

    // Arithmetic updates
    Vector3& operator += ( const Vector3& rV );
    Vector3& operator -= ( const Vector3& rV );
    Vector3& operator *= ( float fScalar );
    Vector3& operator /= ( float fScalar );

    // Vector operations
    float Length() const;
    float SquaredLength() const;
    float Dot( const Vector3& rV ) const;
    float Normalize();

    // The cross products are computed using the right-handed rule.  Be aware
    // that some graphics APIs use a left-handed rule.  If you have to compute
    // a cross product with these functions and send the result to the API
    // that expects left-handed, you will need to change sign on the vector
    // (replace each component value c by -c).
    Vector3 Cross( const Vector3& rV ) const;
    Vector3 UnitCross( const Vector3& rV ) const;

	Vector3 Project( const Vector3& rV ) const;
	Vector3 BackProject( const Vector3& rV ) const;
	float Component( const Vector3& rV ) const;

    // Special vectors
    static const Vector3 ZERO;
    static const Vector3 UNIT_X;
    static const Vector3 UNIT_Y;
    static const Vector3 UNIT_Z;
};

#endif // _VECTOR3_HPP_
