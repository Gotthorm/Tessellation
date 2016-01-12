// VECTOR4.HPP

#ifndef _VECTOR4_HPP_
#define _VECTOR4_HPP_

#include "Math.hpp"

/** @brief A 4D Vector class.
  * @author Sean Butterworth
  * @version 1.0
  * @date Oct 2003
**/

/**
	* @todo 
	* Document the public methods
**/

class Vector4
{
public:
    // Construction
    inline Vector4();
    Vector4( float fX, float fY, float fZ, float fW );
    Vector4( const Vector4& rV );

    // Member access
    float X() const;
    float& X();
    float Y() const;
    float& Y();
    float Z() const;
    float& Z();
    float W() const;
    float& W();

    // Assignment
    Vector4& operator = ( const Vector4& rV );

    // Coordinate access
    operator const float* () const;
    operator float* ();
    float operator[] ( int i ) const;
    float& operator[] ( int i );

    // comparison
    bool operator == ( const Vector4& rV ) const;
    bool operator != ( const Vector4& rV ) const;
    bool operator <  ( const Vector4& rV ) const;
    bool operator <= ( const Vector4& rV ) const;
    bool operator >  ( const Vector4& rV ) const;
    bool operator >= ( const Vector4& rV ) const;

    // Arithmetic operations
    Vector4 operator + ( const Vector4& rV ) const;
    Vector4 operator - ( const Vector4& rV ) const;
    Vector4 operator * ( float fScalar ) const;
    Vector4 operator / ( float fScalar ) const;
    Vector4 operator - () const;

    // Arithmetic updates
    Vector4& operator += ( const Vector4& rV );
    Vector4& operator -= ( const Vector4& rV );
    Vector4& operator *= ( float fScalar );
    Vector4& operator /= ( float fScalar );

    // Vector operations
    float Length() const;
    float SquaredLength() const;
    float Dot( const Vector4& rV ) const;
    float Normalize();

    // Special vectors
    static const Vector4 ZERO;
    static const Vector4 UNIT_X;
    static const Vector4 UNIT_Y;
    static const Vector4 UNIT_Z;
    static const Vector4 UNIT_W;

private:
    float m_afTuple[ 4 ];
};

#endif // _VECTOR4_HPP_
