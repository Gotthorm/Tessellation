// MATRIX4.CPP

#include <memory.h>
#include <assert.h>
#include "Matrix4.hpp"

const Matrix4 Matrix4::ZERO( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
const Matrix4 Matrix4::IDENTITY( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );

Matrix4::Matrix4 ()
{
    // the matrix is uninitialized
}

Matrix4::Matrix4( const Matrix4& rM )
{
    memcpy( m_afEntry, rM.m_afEntry, 16 * sizeof( float ) );
}

Matrix4::Matrix4( bool bZero )
{
    memset( m_afEntry, 0, 16 * sizeof( float ) );

    if( !bZero )
    {
        for( int i = 0; i < 4; i++ )
		{
            m_afEntry[ Index( i, i ) ] = 1.0f;
		}
    }
}

Matrix4& Matrix4::operator = ( const Matrix4& rM )
{
    memcpy( m_afEntry, rM.m_afEntry, 16 * sizeof( float ) );
    return *this;
}

Matrix4::Matrix4( float fM00, float fM01, float fM02, float fM03, float fM10, float fM11, float fM12, float fM13, float fM20, float fM21, float fM22, float fM23, float fM30, float fM31, float fM32, float fM33 )
{
    m_afEntry[  0 ] = fM00;
    m_afEntry[  1 ] = fM01;
    m_afEntry[  2 ] = fM02;
    m_afEntry[  3 ] = fM03;
    m_afEntry[  4 ] = fM10;
    m_afEntry[  5 ] = fM11;
    m_afEntry[  6 ] = fM12;
    m_afEntry[  7 ] = fM13;
    m_afEntry[  8 ] = fM20;
    m_afEntry[  9 ] = fM21;
    m_afEntry[ 10 ] = fM22;
    m_afEntry[ 11 ] = fM23;
    m_afEntry[ 12 ] = fM30;
    m_afEntry[ 13 ] = fM31;
    m_afEntry[ 14 ] = fM32;
    m_afEntry[ 15 ] = fM33;
}

Matrix4::Matrix4( const float afEntry[ 16 ], bool bRowMajor )
{
    if ( bRowMajor )
    {
        memcpy( m_afEntry, afEntry, 16 * sizeof( float ) );
    }
    else
    {
        m_afEntry[  0 ] = afEntry[  0 ];
        m_afEntry[  1 ] = afEntry[  4 ];
        m_afEntry[  2 ] = afEntry[  8 ];
        m_afEntry[  3 ] = afEntry[ 12 ];
        m_afEntry[  4 ] = afEntry[  1 ];
        m_afEntry[  5 ] = afEntry[  5 ];
        m_afEntry[  6 ] = afEntry[  9 ];
        m_afEntry[  7 ] = afEntry[ 13 ];
        m_afEntry[  8 ] = afEntry[  2 ];
        m_afEntry[  9 ] = afEntry[  6 ];
        m_afEntry[ 10 ] = afEntry[ 10 ];
        m_afEntry[ 11 ] = afEntry[ 14 ];
        m_afEntry[ 12 ] = afEntry[  3 ];
        m_afEntry[ 13 ] = afEntry[  7 ];
        m_afEntry[ 14 ] = afEntry[ 11 ];
        m_afEntry[ 15 ] = afEntry[ 15 ];
    }
}

Matrix4 Matrix4::Inverse() const
{
    float fA0 = m_afEntry[  0 ] * m_afEntry[  5 ] - m_afEntry[  1 ] * m_afEntry[  4 ];
    float fA1 = m_afEntry[  0 ] * m_afEntry[  6 ] - m_afEntry[  2 ] * m_afEntry[  4 ];
    float fA2 = m_afEntry[  0 ] * m_afEntry[  7 ] - m_afEntry[  3 ] * m_afEntry[  4 ];
    float fA3 = m_afEntry[  1 ] * m_afEntry[  6 ] - m_afEntry[  2 ] * m_afEntry[  5 ];
    float fA4 = m_afEntry[  1 ] * m_afEntry[  7 ] - m_afEntry[  3 ] * m_afEntry[  5 ];
    float fA5 = m_afEntry[  2 ] * m_afEntry[  7 ] - m_afEntry[  3 ] * m_afEntry[  6 ];
    float fB0 = m_afEntry[  8 ] * m_afEntry[ 13 ] - m_afEntry[  9 ] * m_afEntry[ 12 ];
    float fB1 = m_afEntry[  8 ] * m_afEntry[ 14 ] - m_afEntry[ 10 ] * m_afEntry[ 12 ];
    float fB2 = m_afEntry[  8 ] * m_afEntry[ 15 ] - m_afEntry[ 11 ] * m_afEntry[ 12 ];
    float fB3 = m_afEntry[  9 ] * m_afEntry[ 14 ] - m_afEntry[ 10 ] * m_afEntry[ 13 ];
    float fB4 = m_afEntry[  9 ] * m_afEntry[ 15 ] - m_afEntry[ 11 ] * m_afEntry[ 13 ];
    float fB5 = m_afEntry[ 10 ] * m_afEntry[ 15 ] - m_afEntry[ 11 ] * m_afEntry[ 14 ];

    float fDet = fA0 * fB5 - fA1 * fB4 + fA2 * fB3 + fA3 * fB2 - fA4 * fB1 + fA5 * fB0;
    if( Math::FAbs( fDet ) <= Math::EPSILON )
	{
        return Matrix4::ZERO;
	}

    Matrix4 kInv;
    kInv[ 0 ][ 0 ] = + m_afEntry[  5 ] * fB5 - m_afEntry[  6 ] * fB4 + m_afEntry[  7 ] * fB3;
    kInv[ 1 ][ 0 ] = - m_afEntry[  4 ] * fB5 + m_afEntry[  6 ] * fB2 - m_afEntry[  7 ] * fB1;
    kInv[ 2 ][ 0 ] = + m_afEntry[  4 ] * fB4 - m_afEntry[  5 ] * fB2 + m_afEntry[  7 ] * fB0;
    kInv[ 3 ][ 0 ] = - m_afEntry[  4 ] * fB3 + m_afEntry[  5 ] * fB1 - m_afEntry[  6 ] * fB0;
    kInv[ 0 ][ 1 ] = - m_afEntry[  1 ] * fB5 + m_afEntry[  2 ] * fB4 - m_afEntry[  3 ] * fB3;
    kInv[ 1 ][ 1 ] = + m_afEntry[  0 ] * fB5 - m_afEntry[  2 ] * fB2 + m_afEntry[  3 ] * fB1;
    kInv[ 2 ][ 1 ] = - m_afEntry[  0 ] * fB4 + m_afEntry[  1 ] * fB2 - m_afEntry[  3 ] * fB0;
    kInv[ 3 ][ 1 ] = + m_afEntry[  0 ] * fB3 - m_afEntry[  1 ] * fB1 + m_afEntry[  2 ] * fB0;
    kInv[ 0 ][ 2 ] = + m_afEntry[ 13 ] * fA5 - m_afEntry[ 14 ] * fA4 + m_afEntry[ 15 ] * fA3;
    kInv[ 1 ][ 2 ] = - m_afEntry[ 12 ] * fA5 + m_afEntry[ 14 ] * fA2 - m_afEntry[ 15 ] * fA1;
    kInv[ 2 ][ 2 ] = + m_afEntry[ 12 ] * fA4 - m_afEntry[ 13 ] * fA2 + m_afEntry[ 15 ] * fA0;
    kInv[ 3 ][ 2 ] = - m_afEntry[ 12 ] * fA3 + m_afEntry[ 13 ] * fA1 - m_afEntry[ 14 ] * fA0;
    kInv[ 0 ][ 3 ] = - m_afEntry[  9 ] * fA5 + m_afEntry[ 10 ] * fA4 - m_afEntry[ 11 ] * fA3;
    kInv[ 1 ][ 3 ] = + m_afEntry[  8 ] * fA5 - m_afEntry[ 10 ] * fA2 + m_afEntry[ 11 ] * fA1;
    kInv[ 2 ][ 3 ] = - m_afEntry[  8 ] * fA4 + m_afEntry[  9 ] * fA2 - m_afEntry[ 11 ] * fA0;
    kInv[ 3 ][ 3 ] = + m_afEntry[  8 ] * fA3 - m_afEntry[  9 ] * fA1 + m_afEntry[ 10 ] * fA0;

    float fInvDet = 1.0f / fDet;
    for( int iRow = 0; iRow < 4; iRow++ )
    {
        for( int iCol = 0; iCol < 4; iCol++ )
		{
            kInv[ iRow ][ iCol ] *= fInvDet;
		}
    }

    return kInv;
}

Matrix4 Matrix4::Adjoint() const
{
    float fA0 = m_afEntry[  0 ] * m_afEntry[  5 ] - m_afEntry[  1 ] * m_afEntry[  4 ];
    float fA1 = m_afEntry[  0 ] * m_afEntry[  6 ] - m_afEntry[  2 ] * m_afEntry[  4 ];
    float fA2 = m_afEntry[  0 ] * m_afEntry[  7 ] - m_afEntry[  3 ] * m_afEntry[  4 ];
    float fA3 = m_afEntry[  1 ] * m_afEntry[  6 ] - m_afEntry[  2 ] * m_afEntry[  5 ];
    float fA4 = m_afEntry[  1 ] * m_afEntry[  7 ] - m_afEntry[  3 ] * m_afEntry[  5 ];
    float fA5 = m_afEntry[  2 ] * m_afEntry[  7 ] - m_afEntry[  3 ] * m_afEntry[  6 ];
    float fB0 = m_afEntry[  8 ] * m_afEntry[ 13 ] - m_afEntry[  9 ] * m_afEntry[ 12 ];
    float fB1 = m_afEntry[  8 ] * m_afEntry[ 14 ] - m_afEntry[ 10 ] * m_afEntry[ 12 ];
    float fB2 = m_afEntry[  8 ] * m_afEntry[ 15 ] - m_afEntry[ 11 ] * m_afEntry[ 12 ];
    float fB3 = m_afEntry[  9 ] * m_afEntry[ 14 ] - m_afEntry[ 10 ] * m_afEntry[ 13 ];
    float fB4 = m_afEntry[  9 ] * m_afEntry[ 15 ] - m_afEntry[ 11 ] * m_afEntry[ 13 ];
    float fB5 = m_afEntry[ 10 ] * m_afEntry[ 15 ] - m_afEntry[ 11 ] * m_afEntry[ 14 ];

    Matrix4 kAdj;
    kAdj[ 0 ][ 0 ] = + m_afEntry[  5 ] * fB5 - m_afEntry[  6 ] * fB4 + m_afEntry[  7 ] * fB3;
    kAdj[ 1 ][ 0 ] = - m_afEntry[  4 ] * fB5 + m_afEntry[  6 ] * fB2 - m_afEntry[  7 ] * fB1;
    kAdj[ 2 ][ 0 ] = + m_afEntry[  4 ] * fB4 - m_afEntry[  5 ] * fB2 + m_afEntry[  7 ] * fB0;
    kAdj[ 3 ][ 0 ] = - m_afEntry[  4 ] * fB3 + m_afEntry[  5 ] * fB1 - m_afEntry[  6 ] * fB0;
    kAdj[ 0 ][ 1 ] = - m_afEntry[  1 ] * fB5 + m_afEntry[  2 ] * fB4 - m_afEntry[  3 ] * fB3;
    kAdj[ 1 ][ 1 ] = + m_afEntry[  0 ] * fB5 - m_afEntry[  2 ] * fB2 + m_afEntry[  3 ] * fB1;
    kAdj[ 2 ][ 1 ] = - m_afEntry[  0 ] * fB4 + m_afEntry[  1 ] * fB2 - m_afEntry[  3 ] * fB0;
    kAdj[ 3 ][ 1 ] = + m_afEntry[  0 ] * fB3 - m_afEntry[  1 ] * fB1 + m_afEntry[  2 ] * fB0;
    kAdj[ 0 ][ 2 ] = + m_afEntry[ 13 ] * fA5 - m_afEntry[ 14 ] * fA4 + m_afEntry[ 15 ] * fA3;
    kAdj[ 1 ][ 2 ] = - m_afEntry[ 12 ] * fA5 + m_afEntry[ 14 ] * fA2 - m_afEntry[ 15 ] * fA1;
    kAdj[ 2 ][ 2 ] = + m_afEntry[ 12 ] * fA4 - m_afEntry[ 13 ] * fA2 + m_afEntry[ 15 ] * fA0;
    kAdj[ 3 ][ 2 ] = - m_afEntry[ 12 ] * fA3 + m_afEntry[ 13 ] * fA1 - m_afEntry[ 14 ] * fA0;
    kAdj[ 0 ][ 3 ] = - m_afEntry[  9 ] * fA5 + m_afEntry[ 10 ] * fA4 - m_afEntry[ 11 ] * fA3;
    kAdj[ 1 ][ 3 ] = + m_afEntry[  8 ] * fA5 - m_afEntry[ 10 ] * fA2 + m_afEntry[ 11 ] * fA1;
    kAdj[ 2 ][ 3 ] = - m_afEntry[  8 ] * fA4 + m_afEntry[  9 ] * fA2 - m_afEntry[ 11 ] * fA0;
    kAdj[ 3 ][ 3 ] = + m_afEntry[  8 ] * fA3 - m_afEntry[  9 ] * fA1 + m_afEntry[ 10 ] * fA0;

    return kAdj;
}

float Matrix4::Determinant() const
{
    float fA0 = m_afEntry[  0 ] * m_afEntry[  5 ] - m_afEntry[  1 ] * m_afEntry[ 4 ];
    float fA1 = m_afEntry[  0 ] * m_afEntry[  6 ] - m_afEntry[  2 ] * m_afEntry[ 4 ];
    float fA2 = m_afEntry[  0 ] * m_afEntry[  7 ] - m_afEntry[  3 ] * m_afEntry[ 4 ];
    float fA3 = m_afEntry[  1 ] * m_afEntry[  6 ] - m_afEntry[  2 ] * m_afEntry[ 5 ];
    float fA4 = m_afEntry[  1 ] * m_afEntry[  7 ] - m_afEntry[  3 ] * m_afEntry[ 5 ];
    float fA5 = m_afEntry[  2 ] * m_afEntry[  7 ] - m_afEntry[  3 ] * m_afEntry[ 6 ];
    float fB0 = m_afEntry[  8 ] * m_afEntry[ 13 ] - m_afEntry[  9 ] * m_afEntry[ 12 ];
    float fB1 = m_afEntry[  8 ] * m_afEntry[ 14 ] - m_afEntry[ 10 ] * m_afEntry[ 12 ];
    float fB2 = m_afEntry[  8 ] * m_afEntry[ 15 ] - m_afEntry[ 11 ] * m_afEntry[ 12 ];
    float fB3 = m_afEntry[  9 ] * m_afEntry[ 14 ] - m_afEntry[ 10 ] * m_afEntry[ 13 ];
    float fB4 = m_afEntry[  9 ] * m_afEntry[ 15 ] - m_afEntry[ 11 ] * m_afEntry[ 13 ];
    float fB5 = m_afEntry[ 10 ] * m_afEntry[ 15 ] - m_afEntry[ 11 ] * m_afEntry[ 14 ];

    float fDet = fA0 * fB5 - fA1 * fB4 + fA2 * fB3 + fA3 * fB2 - fA4 * fB1 + fA5 * fB0;
    return fDet;
}

void Matrix4::MakeZero ()
{
    memset( m_afEntry, 0, 16 * sizeof( float ) );
}

void Matrix4::MakeIdentity()
{
    memset( m_afEntry, 0, 16 * sizeof( float ) );
    for( int i = 0; i < 4; i++ )
	{
        m_afEntry[ Index( i, i ) ] = 1.0f;
	}
}

void Matrix4::MakeDiagonal( const float* afDiag )
{
    memset( m_afEntry, 0, 16 * sizeof( float ) );
    for( int i = 0; i < 4; i++ )
	{
        m_afEntry[ Index( i, i ) ] = afDiag[ i ];
	}
}

Matrix4::operator const float* () const
{
    return m_afEntry;
}

Matrix4::operator float* ()
{
    return m_afEntry;
}

const float* Matrix4::operator[] ( int iRow ) const
{
    assert( 0 <= iRow && iRow < 4 );
    return &m_afEntry[ 4 * iRow ];
}

float* Matrix4::operator[] ( int iRow )
{
    assert( 0 <= iRow && iRow < 4 );
    return &m_afEntry[ 4 * iRow ];
}

float Matrix4::operator() ( int iRow, int iCol ) const
{
    return m_afEntry[ Index( iRow, iCol ) ];
}

float& Matrix4::operator() ( int iRow, int iCol )
{
    return m_afEntry[ Index( iRow, iCol ) ];
}

void Matrix4::SetRow( int iRow, const Vector4& rV )
{
    assert( 0 <= iRow && iRow < 4 );
    for( int iCol = 0, i = 4 * iRow; iCol < 4; iCol++, i++ )
	{
        m_afEntry[ i ] = rV[ iCol ];
	}
}

Vector4 Matrix4::GetRow( int iRow ) const
{
    assert( 0 <= iRow && iRow < 4 );
    Vector4 kV;
    for( int iCol = 0, i = 4*iRow; iCol < 4; iCol++, i++ )
	{
        kV[ iCol ] = m_afEntry[ i ];
	}

    return kV;
}

void Matrix4::SetColumn( int iCol, const Vector4& rV )
{
    assert( 0 <= iCol && iCol < 4 );
    for( int iRow = 0, i = iCol; iRow < 4; iRow++, i += 4 )
	{
        m_afEntry[ i ] = rV[ iRow ];
	}
}

Vector4 Matrix4::GetColumn( int iCol ) const
{
    assert( 0 <= iCol && iCol < 4 );
    Vector4 kV;
    for( int iRow = 0, i = iCol; iRow < 4; iRow++, i += 4 )
	{
        kV[ iRow ] = m_afEntry[ i ];
	}

    return kV;
}

void Matrix4::GetColumnMajor( float* afCMajor ) const
{
    for( int iRow = 0, i = 0; iRow < 4; iRow++ )
    {
        for( int iCol = 0; iCol < 4; iCol++ )
		{
            afCMajor[ i++ ] = m_afEntry[ Index( iCol, iRow ) ];
		}
    }
}

/*
// Create a skew symmetric matrix
Matrix4::Matrix4( const Vector4& rVector )
{
	MakeSkewSymmetric( rVector );
}

void Matrix3::MakeSkewSymmetric( const Vector4& rVector )
{
    m_afEntry[  0 ] = 0.0f;
    m_afEntry[  1 ] = -rVector.Z;
    m_afEntry[  2 ] = rVector.Y;
	m_afEntry[  3 ] = 0.0f;

    m_afEntry[  4 ] = rVector.Z;
    m_afEntry[  5 ] = 0.0f;
    m_afEntry[  6 ] = -rVector.X;
    m_afEntry[  7 ] = 0.0f;

    m_afEntry[  8 ] = -rVector.Y;
    m_afEntry[  9 ] = rVector.X;
    m_afEntry[ 10 ] = 0.0f;
    m_afEntry[ 11 ] = 0.0f;

    m_afEntry[ 12 ] = 0.0f;
    m_afEntry[ 13 ] = 0.0f;
    m_afEntry[ 14 ] = 0.0f;
    m_afEntry[ 15 ] = 1.0f;
}
*/

bool Matrix4::operator == ( const Matrix4& rM ) const
{
    return memcmp( m_afEntry, rM.m_afEntry, 16 * sizeof( float ) ) == 0;
}

bool Matrix4::operator != ( const Matrix4& rM ) const
{
    return memcmp( m_afEntry, rM.m_afEntry, 16 * sizeof( float ) ) != 0;
}

bool Matrix4::operator < ( const Matrix4& rM ) const
{
    return Math::CompareArrays( 4, *this, rM ) < 0;
}

bool Matrix4::operator <= ( const Matrix4& rM ) const
{
    return Math::CompareArrays( 4, *this, rM ) <= 0;
}

bool Matrix4::operator > ( const Matrix4& rM ) const
{
    return Math::CompareArrays( 4, *this, rM ) > 0;
}

bool Matrix4::operator >= ( const Matrix4& rM ) const
{
    return Math::CompareArrays( 4, *this, rM ) >= 0;
}

Matrix4 Matrix4::operator + ( const Matrix4& rM ) const
{
    Matrix4 kSum;
    for( int i = 0; i < 16; i++ )
	{
        kSum.m_afEntry[ i ] = m_afEntry[ i ] + rM.m_afEntry[ i ];
	}

    return kSum;
}

Matrix4 Matrix4::operator - ( const Matrix4& rM ) const
{
    Matrix4 kDiff;
    for( int i = 0; i < 16; i++ )
	{
        kDiff.m_afEntry[ i ] = m_afEntry[ i ] - rM.m_afEntry[ i ];
	}

    return kDiff;
}

Matrix4 Matrix4::operator * ( const Matrix4& rM ) const
{
    Matrix4 kProd;
    for( int iRow = 0; iRow < 4; iRow++ )
    {
        for( int iCol = 0; iCol < 4; iCol++ )
        {
            int i = Index( iRow, iCol );
            kProd.m_afEntry[ i ] = 0.0f;
            for( int iMid = 0; iMid < 4; iMid++ )
            {
                kProd.m_afEntry[ i ] += m_afEntry[ Index( iRow, iMid ) ] * rM.m_afEntry[ Index( iMid, iCol ) ];
            }
        }
    }
    return kProd;
}

Matrix4 Matrix4::operator * ( float fScalar ) const
{
    Matrix4 kProd;
    for( int i = 0; i < 16; i++ )
	{
        kProd.m_afEntry[ i ] = fScalar * m_afEntry[ i ];
	}
    return kProd;
}

Matrix4 Matrix4::operator / ( float fScalar ) const
{
    Matrix4 kQuot;
    int i;

    if( fScalar != 0.0f )
    {
        float fInvScalar = 1.0f / fScalar;
        for( i = 0; i < 16; i++ )
		{
            kQuot.m_afEntry[ i ] = fInvScalar * m_afEntry[ i ];
		}
    }
    else
    {
        for( i = 0; i < 16; i++ )
		{
            kQuot.m_afEntry[ i ] = Math::MAX_FLOAT;
		}
    }

    return kQuot;
}

Matrix4 Matrix4::operator - () const
{
    Matrix4 kNeg;
    for( int i = 0; i < 16; i++ )
	{
        kNeg.m_afEntry[ i ] = -m_afEntry[ i ];
	}
    return kNeg;
}

Matrix4& Matrix4::operator += ( const Matrix4& rM )
{
    for( int i = 0; i < 16; i++ )
	{
        m_afEntry[ i ] += rM.m_afEntry[ i ];
	}
    return *this;
}

Matrix4& Matrix4::operator -= ( const Matrix4& rM )
{
    for( int i = 0; i < 16; i++ )
	{
        m_afEntry[ i ] -= rM.m_afEntry[ i ];
	}
    return *this;
}

Matrix4& Matrix4::operator *= ( float fScalar )
{
    for( int i = 0; i < 16; i++ )
	{
        m_afEntry[ i ] *= fScalar;
	}
    return *this;
}

Matrix4& Matrix4::operator /= ( float fScalar )
{
    int i;

    if( fScalar != 0.0f )
    {
        float fInvScalar = 1.0f / fScalar;
        for( i = 0; i < 16; i++ )
		{
            m_afEntry[ i ] *= fInvScalar;
		}
    }
    else
    {
        for( i = 0; i < 16; i++ )
		{
            m_afEntry[ i ] = Math::MAX_FLOAT;
		}
    }

    return *this;
}

Matrix4 Matrix4::Transpose() const
{
    Matrix4 kTranspose;
    for( int iRow = 0; iRow < 4; iRow++ )
    {
        for( int iCol = 0; iCol < 4; iCol++ )
		{
            kTranspose.m_afEntry[ Index( iRow, iCol ) ] = m_afEntry[ Index( iCol, iRow ) ];
		}
    }
    return kTranspose;
}

Matrix4 Matrix4::TransposeTimes( const Matrix4& rM ) const
{
    // P = A^T*B, P[r][c] = sum_m A[m][r]*B[m][c]
    Matrix4 kProd;
    for( int iRow = 0; iRow < 4; iRow++ )
    {
        for( int iCol = 0; iCol < 4; iCol++ )
        {
            int i = Index( iRow, iCol );
            kProd.m_afEntry[ i ] = 0.0f;
            for( int iMid = 0; iMid < 4; iMid++ )
            {
                kProd.m_afEntry[ i ] += m_afEntry[ Index( iMid, iRow ) ] * rM.m_afEntry[ Index( iMid, iCol ) ];
            }
        }
    }
    return kProd;
}

Matrix4 Matrix4::TimesTranspose( const Matrix4& rM ) const
{
    // P = A*B^T, P[r][c] = sum_m A[r][m]*B[c][m]
    Matrix4 kProd;
    for( int iRow = 0; iRow < 4; iRow++ )
    {
        for( int iCol = 0; iCol < 4; iCol++ )
        {
            int i = Index( iRow, iCol );
            kProd.m_afEntry[ i ] = 0.0f;
            for( int iMid = 0; iMid < 4; iMid++ )
            {
                kProd.m_afEntry[ i ] += m_afEntry[ Index( iRow, iMid ) ] * rM.m_afEntry[ Index( iCol, iRow ) ];
            }
        }
    }
    return kProd;
}

Vector4 Matrix4::operator * ( const Vector4& rV ) const
{
    Vector4 kProd;
    for( int iRow = 0; iRow < 4; iRow++ )
    {
        kProd[ iRow ] = 0.0f;
        for( int iCol = 0; iCol < 4; iCol++ )
		{
            kProd[ iRow ] += m_afEntry[ Index( iRow, iCol ) ] * rV[ iCol ];
		}
    }
    return kProd;
}

float Matrix4::QForm( const Vector4& rU, const Vector4& rV ) const
{
    return rU.Dot( (*this)*rV );
}

int Matrix4::Index( int iRow, int iCol )
{
    assert( 0 <= iRow && iRow < 4 && 0 <= iCol && iCol < 4 );
    return iCol + 4 * iRow;
}

Matrix4 Matrix4::Translate(float x, float y, float z)
{
	Matrix4 result = IDENTITY;

	result.SetRow(3, Vector4(x, y, z, 1.0f));

	return result;
}

Matrix4 Matrix4::Rotate(float angle, float x, float y, float z)
{
	Matrix4 result;

	const float x2 = x * x;
	const float y2 = y * y;
	const float z2 = z * z;
	float rads = angle * Math::DEG_TO_RAD;
	const float c = Math::Cos(rads);
	const float s = Math::Sin(rads);
	const float omc = 1.0f - c;

	result.SetRow(0, Vector4(x2 * omc + c, y * x * omc + z * s, x * z * omc - y * s, 0.0f));
	result.SetRow(1, Vector4(x * y * omc - z * s, y2 * omc + c, y * z * omc + x * s, 0.0f));
	result.SetRow(2, Vector4(x * z * omc + y * s, y * z * omc - x * s, z2 * omc + c, 0.0f));
	result.SetRow(3, Vector4::UNIT_W);

	return result;
}

Matrix4 Matrix4::Perspective(float fovy, float aspect, float n, float f)
{
	float q = 1.0f / Math::Tan(Math::DEG_TO_RAD * (0.5f * fovy));
	float A = q / aspect;
	float B = (n + f) / (n - f);
	float C = (2.0f * n * f) / (n - f);

	Matrix4 result;

	result.SetRow(0, Vector4(A, 0.0f, 0.0f, 0.0f));
	result.SetRow(1, Vector4(0.0f, q, 0.0f, 0.0f));
	result.SetRow(2, Vector4(0.0f, 0.0f, B, -1.0f));
	result.SetRow(3, Vector4(0.0f, 0.0f, C, 0.0f));

	return result;
}

/*
tvec3<T, P> const f(normalize(center - eye));
tvec3<T, P> const s(normalize(cross(f, up)));
tvec3<T, P> const u(cross(s, f));

tmat4x4<T, P> Result(1);
Result[0][0] = s.x;
Result[1][0] = s.y;
Result[2][0] = s.z;
Result[0][1] = u.x;
Result[1][1] = u.y;
Result[2][1] = u.z;
Result[0][2] =-f.x;
Result[1][2] =-f.y;
Result[2][2] =-f.z;
Result[3][0] =-dot(s, eye);
Result[3][1] =-dot(u, eye);
Result[3][2] = dot(f, eye);
*/
Matrix4 Matrix4::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	Vector3 f = (center - eye);
	f.Normalize();
	//Vector3 upN = up;
	//upN.Normalize();
	const Vector3 s = f.UnitCross(up);
	const Vector3 u = s.UnitCross(f);

	Matrix4 M;

	M.SetRow(0, Vector4(s[0], u[0], -f[0], 0.0f));
	M.SetRow(1, Vector4(s[1], u[1], -f[1], 0.0f));
	M.SetRow(2, Vector4(s[2], u[2], -f[2], 0.0f));
	//M.SetRow(3, Vector4(-eye.X, -eye.Y, -eye.Z, 1.0f));
	M.SetRow(3, Vector4(-s.Dot(eye), -u.Dot(eye), f.Dot(eye), 1.0f));

	//return M * Translate(-eye.X, -eye.Y, -eye.Z);
	return M;
}
