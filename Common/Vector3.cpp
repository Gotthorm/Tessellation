// VECTOR3.CPP

#include <memory.h>
#include <assert.h>
#include "Vector3.hpp"

const Vector3 Vector3::ZERO( 0.0f, 0.0f, 0.0f );
const Vector3 Vector3::UNIT_X( 1.0f, 0.0f, 0.0f );
const Vector3 Vector3::UNIT_Y( 0.0f, 1.0f, 0.0f );
const Vector3 Vector3::UNIT_Z( 0.0f, 0.0f, 1.0f );

Vector3::Vector3()
{
    // the vector is uninitialized
}

Vector3::Vector3( float fX, float fY, float fZ )
{
    X = fX;
    Y = fY;
    Z = fZ;
}

Vector3::Vector3( const Vector3& rV )
{
    X = rV.X;
    Y = rV.Y;
    Z = rV.Z;
}

Vector3::Vector3( const float* aXYZ )
{
    X = aXYZ[ 0 ];
    Y = aXYZ[ 1 ];
    Z = aXYZ[ 2 ];
}

Vector3& Vector3::operator = ( const Vector3& rV )
{
    X = rV.X;
    Y = rV.Y;
    Z = rV.Z;
    return *this;
}

Vector3 Vector3::Cross( const Vector3& rV ) const
{
    return Vector3(	Y * rV.Z - Z * rV.Y, Z * rV.X - X * rV.Z, X * rV.Y - Y * rV.X );
}

Vector3 Vector3::UnitCross( const Vector3& rV ) const
{
    Vector3 kCross(	Y * rV.Z - Z * rV.Y, Z * rV.X - X * rV.Z, X * rV.Y - Y * rV.X );

    kCross.Normalize();
    return kCross;
}

Vector3::operator const float* () const
{
    return &X;
}

Vector3::operator float* ()
{
    return &X;
}

bool Vector3::operator == ( const Vector3& rV ) const
{
    return ( X == rV.X && Y == rV.Y && Y == rV.Y );
}

bool Vector3::operator != ( const Vector3& rV ) const
{
    return ( X != rV.X || Y != rV.Y || Y != rV.Y );
}

Vector3 Vector3::operator + ( const Vector3& rV ) const
{
    Vector3 kSum( X + rV.X, Y + rV.Y, Z + rV.Z );

    return kSum;
}

Vector3 Vector3::operator - ( const Vector3& rV ) const
{
    Vector3 kDiff( X - rV.X, Y - rV.Y, Z - rV.Z );

    return kDiff;
}

Vector3 Vector3::operator * ( float fScalar ) const
{
    Vector3 kProd( X * fScalar, Y * fScalar, Z * fScalar );

    return kProd;
}

Vector3 Vector3::operator / ( float fScalar ) const
{
    Vector3 kQuot;

    if( fScalar != 0.0f )
    {
        float fInvScalar = 1.0f / fScalar;

		kQuot.X = fInvScalar * X;
		kQuot.Y = fInvScalar * Y;
		kQuot.Z = fInvScalar * Z;
    }
    else
    {
		kQuot.X = Math::MAX_FLOAT;
		kQuot.Y = Math::MAX_FLOAT;
		kQuot.Z = Math::MAX_FLOAT;
    }

    return kQuot;
}

Vector3 Vector3::operator - () const
{
    Vector3 kNeg( -X, -Y, -Z );

    return kNeg;
}

Vector3& Vector3::operator += ( const Vector3& rV )
{
	X += rV.X;
	Y += rV.Y;
	Z += rV.Z;

    return *this;
}

Vector3& Vector3::operator -= ( const Vector3& rV )
{
	X -= rV.X;
	Y -= rV.Y;
	Z -= rV.Z;

    return *this;
}

Vector3& Vector3::operator *= ( float fScalar )
{
	X *= fScalar;
	Y *= fScalar;
	Z *= fScalar;

    return *this;
}

Vector3& Vector3::operator /= ( float fScalar )
{
    if( fScalar != 0.0f )
    {
        float fInvScalar = 1.0f / fScalar;

		X *= fInvScalar;
		Y *= fInvScalar;
		Z *= fInvScalar;
    }
    else
    {
		X = Math::MAX_FLOAT;
		Y = Math::MAX_FLOAT;
		Z = Math::MAX_FLOAT;
    }

    return *this;
}

float Vector3::Length() const
{
    return Math::Sqrt( X * X + Y * Y + Z * Z );
}

float Vector3::SquaredLength() const
{
    return X * X + Y * Y + Z * Z;
}

float Vector3::Dot( const Vector3& rV ) const
{
    return X * rV.X + Y * rV.Y + Z * rV.Z;
}

float Vector3::Normalize()
{
    float fLength = Length();

    if( fLength > Math::EPSILON )
    {
        float fInvLength = 1.0f / fLength;

        X *= fInvLength;
        Y *= fInvLength;
        Z *= fInvLength;
    }
    else
    {
        fLength = 0.0f;

        X = 0.0f;
        Y = 0.0f;
        Z = 0.0f;
    }

    return fLength;
}