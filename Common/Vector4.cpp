// VECTOR4.CPP

#include <memory.h>
#include <assert.h>
#include "Vector4.hpp"

const Vector4 Vector4::ZERO( 0.0f, 0.0f, 0.0f, 0.0f );
const Vector4 Vector4::UNIT_X( 1.0f, 0.0f, 0.0f, 0.0f );
const Vector4 Vector4::UNIT_Y( 0.0f, 1.0f, 0.0f, 0.0f );
const Vector4 Vector4::UNIT_Z( 0.0f, 0.0f, 1.0f, 0.0f );
const Vector4 Vector4::UNIT_W( 0.0f, 0.0f, 0.0f, 1.0f );

Vector4::Vector4()
{
    // the vector is uninitialized
}

Vector4::Vector4( float fX, float fY, float fZ, float fW )
{
    m_afTuple[ 0 ] = fX;
    m_afTuple[ 1 ] = fY;
    m_afTuple[ 2 ] = fZ;
    m_afTuple[ 3 ] = fW;
}

Vector4::Vector4( const Vector4& rV )
{
    memcpy( m_afTuple, rV.m_afTuple, 4 * sizeof( float ) );
}

Vector4& Vector4::operator = ( const Vector4& rV )
{
    memcpy( m_afTuple, rV.m_afTuple, 4 * sizeof( float ) );
    return *this;
}

float Vector4::X() const
{
    return m_afTuple[ 0 ];
}

float& Vector4::X()
{
    return m_afTuple[ 0 ];
}

float Vector4::Y() const
{
    return m_afTuple[ 1 ];
}

float& Vector4::Y()
{
    return m_afTuple[ 1 ];
}

float Vector4::Z() const
{
    return m_afTuple[ 2 ];
}

float& Vector4::Z()
{
    return m_afTuple[ 2 ];
}

float Vector4::W() const
{
    return m_afTuple[ 3 ];
}

float& Vector4::W()
{
    return m_afTuple[ 3 ];
}

Vector4::operator const float* () const
{
    return m_afTuple;
}

Vector4::operator float* ()
{
    return m_afTuple;
}

float Vector4::operator[] ( int i ) const
{
    assert( 0 <= i && i < 4 );
    return m_afTuple[ i ];
}

float& Vector4::operator[] ( int i )
{
    assert( 0 <= i && i < 4 );
    return m_afTuple[ i ];
}

bool Vector4::operator == ( const Vector4& rV ) const
{
    return memcmp( m_afTuple, rV.m_afTuple, 4 * sizeof( float ) ) == 0;
}

bool Vector4::operator != ( const Vector4& rV ) const
{
    return memcmp( m_afTuple, rV.m_afTuple, 4 * sizeof( float ) ) != 0;
}

bool Vector4::operator < ( const Vector4& rV ) const
{
    return Math::CompareArrays( 4, *this, rV ) < 0;
}

bool Vector4::operator <= ( const Vector4& rV ) const
{
    return Math::CompareArrays( 4, *this, rV ) <= 0;
}

bool Vector4::operator > ( const Vector4& rV ) const
{
    return Math::CompareArrays( 4, *this, rV ) > 0;
}

bool Vector4::operator >= ( const Vector4& rV ) const
{
    return Math::CompareArrays( 4, *this, rV ) >= 0;
}

Vector4 Vector4::operator + ( const Vector4& rV ) const
{
    Vector4 kSum;

    for( int i = 0; i < 4; i++ )
	{
        kSum.m_afTuple[ i ] = m_afTuple[ i ] + rV.m_afTuple[ i ];
	}

    return kSum;
}

Vector4 Vector4::operator - ( const Vector4& rV ) const
{
    Vector4 kDiff;

    for( int i = 0; i < 4; i++ )
	{
        kDiff.m_afTuple[ i ] = m_afTuple[ i ] - rV.m_afTuple[ i ];
	}

    return kDiff;
}

Vector4 Vector4::operator * ( float fScalar ) const
{
    Vector4 kProd;

    for( int i = 0; i < 4; i++ )
	{
        kProd.m_afTuple[ i ] = fScalar * m_afTuple[ i ];
	}

    return kProd;
}

Vector4 Vector4::operator / ( float fScalar ) const
{
    Vector4 kQuot;
    int i;

    if( fScalar != 0.0f )
    {
        float fInvScalar = 1.0f / fScalar;

        for( i = 0; i < 4; i++ )
		{
            kQuot.m_afTuple[ i ] = fInvScalar * m_afTuple[ i ];
		}
    }
    else
    {
        for( i = 0; i < 4; i++ )
		{
            kQuot.m_afTuple[ i ] = Math::MAX_FLOAT;
		}
    }

    return kQuot;
}

Vector4 Vector4::operator - () const
{
    Vector4 kNeg;

    for( int i = 0; i < 4; i++ )
	{
        kNeg.m_afTuple[ i ] = -m_afTuple[ i ];
	}

    return kNeg;
}

Vector4& Vector4::operator += ( const Vector4& rV )
{
    for( int i = 0; i < 4; i++ )
	{
        m_afTuple[ i ] += rV.m_afTuple[ i ];
	}

    return *this;
}

Vector4& Vector4::operator -= ( const Vector4& rV )
{
    for( int i = 0; i < 4; i++ )
	{
        m_afTuple[ i ] -= rV.m_afTuple[ i ];
	}

    return *this;
}

Vector4& Vector4::operator *= ( float fScalar )
{
    for( int i = 0; i < 4; i++ )
	{
        m_afTuple[ i ] *= fScalar;
	}

    return *this;
}

Vector4& Vector4::operator /= ( float fScalar )
{
    int i;

    if( fScalar != 0.0f )
    {
        float fInvScalar = 1.0f / fScalar;

        for( i = 0; i < 4; i++ )
		{
            m_afTuple[ i ] *= fInvScalar;
		}
    }
    else
    {
        for( i = 0; i < 4; i++ )
		{
            m_afTuple[ i ] = Math::MAX_FLOAT;
		}
    }

    return *this;
}

float Vector4::Length() const
{
    float fSqrLen = 0.0f;

    for( int i = 0; i < 4; i++ )
	{
        fSqrLen += m_afTuple[ i ] * m_afTuple[ i ];
	}

    return Math::Sqrt( fSqrLen );
}

float Vector4::SquaredLength() const
{
    float fSqrLen = 0.0f;

    for( int i = 0; i < 4; i++ )
	{
        fSqrLen += m_afTuple[ i ] * m_afTuple[ i ];
	}

    return fSqrLen;
}

float Vector4::Dot( const Vector4& rV ) const
{
    float fDot = 0.0f;

    for( int i = 0; i < 4; i++ )
	{
        fDot += m_afTuple[ i ] * rV.m_afTuple[ i ];
	}

    return fDot;
}

float Vector4::Normalize()
{
    float fLength = Length();
    int i;

    if( fLength > Math::EPSILON )
    {
        float fInvLength = 1.0f / fLength;

        for( i = 0; i < 4; i++ )
		{
            m_afTuple[ i ] *= fInvLength;
		}
    }
    else
    {
        fLength = 0.0f;

        for( i = 0; i < 4; i++ )
		{
            m_afTuple[ i ] = 0.0f;
		}
    }

    return fLength;
}
