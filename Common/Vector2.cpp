// VECTOR2.CPP

#include <memory.h>
#include <assert.h>
#include <strstrea.h>
#include "Vector2.hpp"

const Vector2 Vector2::ZERO( 0.0f, 0.0f );
const Vector2 Vector2::UNIT_X( 1.0f, 0.0f );
const Vector2 Vector2::UNIT_Y( 0.0f, 1.0f );

Vector2::Vector2()
{
    // the vector is uninitialized
}

Vector2::Vector2( float fX, float fY )
{
    m_afTuple[ 0 ] = fX;
    m_afTuple[ 1 ] = fY;
}

Vector2::Vector2( const float* aXY )
{
	memcpy( m_afTuple, aXY, 2 * sizeof( float ) );
}

Vector2::Vector2( const double* aXY )
{
    m_afTuple[ 0 ] = (float)( aXY[ 0 ] );
    m_afTuple[ 1 ] = (float)( aXY[ 1 ] );
}

Vector2::Vector2( const Vector2& rV )
{
    memcpy( m_afTuple, rV.m_afTuple, 2 * sizeof( float ) );
}

Vector2& Vector2::operator = ( const Vector2& rV )
{
    memcpy( m_afTuple, rV.m_afTuple, 2 * sizeof( float ) );
    return *this;
}

float Vector2::X() const
{
    return m_afTuple[ 0 ];
}

float& Vector2::X()
{
    return m_afTuple[ 0 ];
}

float Vector2::Y() const
{
    return m_afTuple[ 1 ];
}

float& Vector2::Y()
{
    return m_afTuple[ 1 ];
}

Vector2::operator const float* () const
{
    return m_afTuple;
}

Vector2::operator float* ()
{
    return m_afTuple;
}

float Vector2::operator[] ( int i ) const
{
    assert( 0 <= i && i < 2 );
    return m_afTuple[ i ];
}

float& Vector2::operator[] ( int i )
{
    assert( 0 <= i && i < 2 );
    return m_afTuple[ i ];
}

bool Vector2::operator == ( const Vector2& rV ) const
{
    return memcmp( m_afTuple, rV.m_afTuple, 2 * sizeof( float ) ) == 0;
}

bool Vector2::operator != ( const Vector2& rV ) const
{
    return memcmp( m_afTuple, rV.m_afTuple, 2 * sizeof( float ) ) != 0;
}

bool Vector2::operator < ( const Vector2& rV ) const
{
    return Math::CompareArrays( 2, *this, rV ) < 0;
}

bool Vector2::operator <= ( const Vector2& rV ) const
{
    return Math::CompareArrays( 2, *this, rV ) <= 0;
}

bool Vector2::operator > ( const Vector2& rV ) const
{
    return Math::CompareArrays( 2, *this, rV ) > 0;
}

bool Vector2::operator >= ( const Vector2& rV ) const
{
    return Math::CompareArrays( 2, *this, rV ) >= 0;
}

Vector2 Vector2::operator + ( const Vector2& rV ) const
{
    Vector2 kSum;

    for( int i = 0; i < 2; i++ )
	{
        kSum.m_afTuple[ i ] = m_afTuple[ i ] + rV.m_afTuple[ i ];
	}

    return kSum;
}

Vector2 Vector2::operator - ( const Vector2& rV ) const
{
    Vector2 kDiff;

    for( int i = 0; i < 2; i++ )
	{
        kDiff.m_afTuple[ i ] = m_afTuple[ i ] - rV.m_afTuple[ i ];
	}

    return kDiff;
}

Vector2 Vector2::operator * ( float fScalar ) const
{
    Vector2 kProd;

    for( int i = 0; i < 2; i++ )
	{
        kProd.m_afTuple[ i ] = fScalar * m_afTuple[ i ];
	}

    return kProd;
}

Vector2 Vector2::operator / ( float fScalar ) const
{
    Vector2 kQuot;
    int i;

    if( fScalar != 0.0f )
    {
        float fInvScalar = 1.0f / fScalar;

        for( i = 0; i < 2; i++ )
		{
            kQuot.m_afTuple[ i ] = fInvScalar * m_afTuple[ i ];
		}
    }
    else
    {
        for( i = 0; i < 2; i++ )
		{
            kQuot.m_afTuple[ i ] = Math::MAX_FLOAT;
		}
    }

    return kQuot;
}

Vector2 Vector2::operator - () const
{
    Vector2 kNeg;

    for( int i = 0; i < 2; i++ )
	{
        kNeg.m_afTuple[ i ] = -m_afTuple[ i ];
	}

    return kNeg;
}

Vector2& Vector2::operator += ( const Vector2& rV )
{
    for( int i = 0; i < 2; i++ )
	{
        m_afTuple[ i ] += rV.m_afTuple[ i ];
	}

    return *this;
}

Vector2& Vector2::operator -= ( const Vector2& rV )
{
    for( int i = 0; i < 2; i++ )
	{
        m_afTuple[ i ] -= rV.m_afTuple[ i ];
	}

    return *this;
}

Vector2& Vector2::operator *= ( float fScalar )
{
    for( int i = 0; i < 2; i++ )
	{
        m_afTuple[ i ] *= fScalar;
	}

    return *this;
}

Vector2& Vector2::operator /= ( float fScalar )
{
    int i;

    if( fScalar != 0.0f )
    {
        float fInvScalar = 1.0f / fScalar;

        for( i = 0; i < 2; i++ )
		{
            m_afTuple[ i ] *= fInvScalar;
		}
    }
    else
    {
        for( i = 0; i < 2; i++ )
		{
            m_afTuple[ i ] = Math::MAX_FLOAT;
		}
    }

    return *this;
}

float Vector2::Length() const
{
    float fSqrLen = 0.0f;

    for( int i = 0; i < 2; i++ )
	{
        fSqrLen += m_afTuple[ i ] * m_afTuple[ i ];
	}

    return Math::Sqrt( fSqrLen );
}

float Vector2::SquaredLength() const
{
    float fSqrLen = 0.0f;

    for( int i = 0; i < 2; i++ )
	{
        fSqrLen += m_afTuple[ i ] * m_afTuple[ i ];
	}

    return fSqrLen;
}

ostream& operator << ( ostream& os, const Vector2& rV )
{
	return os << "[" << rV[0] << ", " << rV[1] << "]";
}
