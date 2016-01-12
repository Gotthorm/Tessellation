// MATRIX3.CPP

#include <math.h>
#include <memory.h>
#include <assert.h>
#include "Matrix3.hpp"
#include "Math.hpp"

const Matrix3 Matrix3::ZERO( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f );
const Matrix3 Matrix3::IDENTITY( 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f );

Matrix3::Matrix3()
{
    // the matrix is uninitialized
}

Matrix3::Matrix3( const Matrix3& rM )
{
    memcpy( m_afEntry, rM.m_afEntry, 9 * sizeof( float ) );
}

Matrix3::Matrix3( bool bZero )
{
    memset( m_afEntry, 0, 9 * sizeof( float ) );

    if( !bZero )
    {
        for( int i = 0; i < 3; i++ )
		{
            m_afEntry[ Index( i, i ) ] = 1.0f;
		}
    }
}

Matrix3& Matrix3::operator = ( const Matrix3& rM )
{
    memcpy( m_afEntry, rM.m_afEntry, 9 * sizeof( float ) );
    return *this;
}

Matrix3::Matrix3 (float fM00, float fM01, float fM02, float fM10, float fM11, float fM12, float fM20, float fM21, float fM22 )
{
    m_afEntry[ 0 ] = fM00;
    m_afEntry[ 1 ] = fM01;
    m_afEntry[ 2 ] = fM02;
    m_afEntry[ 3 ] = fM10;
    m_afEntry[ 4 ] = fM11;
    m_afEntry[ 5 ] = fM12;
    m_afEntry[ 6 ] = fM20;
    m_afEntry[ 7 ] = fM21;
    m_afEntry[ 8 ] = fM22;
}

Matrix3::Matrix3( const float afEntry[ 9 ], bool bRowMajor )
{
    if( bRowMajor )
    {
        memcpy( m_afEntry, afEntry, 9 * sizeof( float ) );
    }
    else
    {
        m_afEntry[ 0 ] = afEntry[ 0 ];
        m_afEntry[ 1 ] = afEntry[ 3 ];
        m_afEntry[ 2 ] = afEntry[ 6 ];
        m_afEntry[ 3 ] = afEntry[ 1 ];
        m_afEntry[ 4 ] = afEntry[ 4 ];
        m_afEntry[ 5 ] = afEntry[ 7 ];
        m_afEntry[ 6 ] = afEntry[ 2 ];
        m_afEntry[ 7 ] = afEntry[ 5 ];
        m_afEntry[ 8 ] = afEntry[ 8 ];
    }
}

Matrix3::Matrix3( const Vector3& rU, const Vector3& rV, const Vector3& rW, bool bColumns )
{
    if( bColumns )
    {
        m_afEntry[ 0 ] = rU[ 0 ];
        m_afEntry[ 1 ] = rV[ 0 ];
        m_afEntry[ 2 ] = rW[ 0 ];
        m_afEntry[ 3 ] = rU[ 1 ];
        m_afEntry[ 4 ] = rV[ 1 ];
        m_afEntry[ 5 ] = rW[ 1 ];
        m_afEntry[ 6 ] = rU[ 2 ];
        m_afEntry[ 7 ] = rV[ 2 ];
        m_afEntry[ 8 ] = rW[ 2 ];
    }
    else
    {
        m_afEntry[ 0 ] = rU[ 0 ];
        m_afEntry[ 1 ] = rU[ 1 ];
        m_afEntry[ 2 ] = rU[ 2 ];
        m_afEntry[ 3 ] = rV[ 0 ];
        m_afEntry[ 4 ] = rV[ 1 ];
        m_afEntry[ 5 ] = rV[ 2 ];
        m_afEntry[ 6 ] = rW[ 0 ];
        m_afEntry[ 7 ] = rW[ 1 ];
        m_afEntry[ 8 ] = rW[ 2 ];
    }
}

Matrix3::Matrix3( const Vector3* aV, bool bColumns )
{
    if ( bColumns )
    {
        m_afEntry[ 0 ] = aV[ 0 ][ 0 ];
        m_afEntry[ 1 ] = aV[ 1 ][ 0 ];
        m_afEntry[ 2 ] = aV[ 2 ][ 0 ];
        m_afEntry[ 3 ] = aV[ 0 ][ 1 ];
        m_afEntry[ 4 ] = aV[ 1 ][ 1 ];
        m_afEntry[ 5 ] = aV[ 2 ][ 1 ];
        m_afEntry[ 6 ] = aV[ 0 ][ 2 ];
        m_afEntry[ 7 ] = aV[ 1 ][ 2 ];
        m_afEntry[ 8 ] = aV[ 2 ][ 2 ];
    }
    else
    {
        m_afEntry[ 0 ] = aV[ 0 ][ 0 ];
        m_afEntry[ 1 ] = aV[ 0 ][ 1 ];
        m_afEntry[ 2 ] = aV[ 0 ][ 2 ];
        m_afEntry[ 3 ] = aV[ 1 ][ 0 ];
        m_afEntry[ 4 ] = aV[ 1 ][ 1 ];
        m_afEntry[ 5 ] = aV[ 1 ][ 2 ];
        m_afEntry[ 6 ] = aV[ 2 ][ 0 ];
        m_afEntry[ 7 ] = aV[ 2 ][ 1 ];
        m_afEntry[ 8 ] = aV[ 2 ][ 2 ];
    }
}

Matrix3::Matrix3( const Vector3& rU, const Vector3& rV )
{
    MakeTensorProduct( rU,rV );
}

void Matrix3::MakeTensorProduct( const Vector3& rU, const Vector3& rV )
{
    m_afEntry[ 0 ] = rU[ 0 ] * rV[ 0 ];
    m_afEntry[ 1 ] = rU[ 0 ] * rV[ 1 ];
    m_afEntry[ 2 ] = rU[ 0 ] * rV[ 2 ];
    m_afEntry[ 3 ] = rU[ 1 ] * rV[ 0 ];
    m_afEntry[ 4 ] = rU[ 1 ] * rV[ 1 ];
    m_afEntry[ 5 ] = rU[ 1 ] * rV[ 2 ];
    m_afEntry[ 6 ] = rU[ 2 ] * rV[ 0 ];
    m_afEntry[ 7 ] = rU[ 2 ] * rV[ 1 ];
    m_afEntry[ 8 ] = rU[ 2 ] * rV[ 2 ];
}

Matrix3::Matrix3( float fM00, float fM11, float fM22 )
{
    MakeDiagonal( fM00, fM11, fM22 );
}

void Matrix3::MakeDiagonal( float fM00, float fM11, float fM22 )
{
    m_afEntry[ 0 ] = fM00;
    m_afEntry[ 1 ] = 0.0f;
    m_afEntry[ 2 ] = 0.0f;
    m_afEntry[ 3 ] = 0.0f;
    m_afEntry[ 4 ] = fM11;
    m_afEntry[ 5 ] = 0.0f;
    m_afEntry[ 6 ] = 0.0f;
    m_afEntry[ 7 ] = 0.0f;
    m_afEntry[ 8 ] = fM22;
}

Matrix3::Matrix3( const Vector3& rAxis, float fAngle )
{
    FromAxisAngle( rAxis, fAngle );
}

void Matrix3::FromAxisAngle( const Vector3& rAxis, float fAngle )
{
    float fCos = Math::Cos( fAngle );
    float fSin = Math::Sin( fAngle );
    float fOneMinusCos = 1.0f - fCos;
    float fX2 = rAxis[ 0 ] * rAxis[ 0 ];
    float fY2 = rAxis[ 1 ] * rAxis[ 1 ];
    float fZ2 = rAxis[ 2 ] * rAxis[ 2 ];

    float fXYM = rAxis[ 0 ] * rAxis[ 1 ] * fOneMinusCos;
    float fXZM = rAxis[ 0 ] * rAxis[ 2 ] * fOneMinusCos;
    float fYZM = rAxis[ 1 ] * rAxis[ 2 ] * fOneMinusCos;

    float fXSin = rAxis[ 0 ] * fSin;
    float fYSin = rAxis[ 1 ] * fSin;
    float fZSin = rAxis[ 2 ] * fSin;
    
    m_afEntry[ 0 ] = fX2 * fOneMinusCos + fCos;
    m_afEntry[ 1 ] = fXYM - fZSin;
    m_afEntry[ 2 ] = fXZM + fYSin;
    m_afEntry[ 3 ] = fXYM + fZSin;
    m_afEntry[ 4 ] = fY2 * fOneMinusCos + fCos;
    m_afEntry[ 5 ] = fYZM - fXSin;
    m_afEntry[ 6 ] = fXZM - fYSin;
    m_afEntry[ 7 ] = fYZM + fXSin;
    m_afEntry[ 8 ] = fZ2 * fOneMinusCos + fCos;
}

// Create a skew symmetric matrix
Matrix3::Matrix3( const Vector3& rVector )
{
	MakeSkewSymmetric( rVector );
}

void Matrix3::MakeSkewSymmetric( const Vector3& rVector )
{
    m_afEntry[ 0 ] = 0.0f;
    m_afEntry[ 1 ] = -rVector.Z;
    m_afEntry[ 2 ] = rVector.Y;
    m_afEntry[ 3 ] = rVector.Z;
    m_afEntry[ 4 ] = 0.0f;
    m_afEntry[ 5 ] = -rVector.X;
    m_afEntry[ 6 ] = -rVector.Y;
    m_afEntry[ 7 ] = rVector.X;
    m_afEntry[ 8 ] = 0.0f;
}

Matrix3 Matrix3::Inverse() const
{
    // Invert a 3x3 using cofactors.  This is faster than using a generic
    // Gaussian elimination because of the loop overhead of such a method.

    Matrix3 kInverse;

    kInverse[ 0 ][ 0 ] = m_afEntry[ 4 ] * m_afEntry[ 8 ] - m_afEntry[ 5 ] * m_afEntry[ 7 ];
    kInverse[ 0 ][ 1 ] = m_afEntry[ 2 ] * m_afEntry[ 7 ] - m_afEntry[ 1 ] * m_afEntry[ 8 ];
    kInverse[ 0 ][ 2 ] = m_afEntry[ 1 ] * m_afEntry[ 5 ] - m_afEntry[ 2 ] * m_afEntry[ 4 ];
    kInverse[ 1 ][ 0 ] = m_afEntry[ 5 ] * m_afEntry[ 6 ] - m_afEntry[ 3 ] * m_afEntry[ 8 ];
    kInverse[ 1 ][ 1 ] = m_afEntry[ 0 ] * m_afEntry[ 8 ] - m_afEntry[ 2 ] * m_afEntry[ 6 ];
    kInverse[ 1 ][ 2 ] = m_afEntry[ 2 ] * m_afEntry[ 3 ] - m_afEntry[ 0 ] * m_afEntry[ 5 ];
    kInverse[ 2 ][ 0 ] = m_afEntry[ 3 ] * m_afEntry[ 7 ] - m_afEntry[ 4 ] * m_afEntry[ 6 ];
    kInverse[ 2 ][ 1 ] = m_afEntry[ 1 ] * m_afEntry[ 6 ] - m_afEntry[ 0 ] * m_afEntry[ 7 ];
    kInverse[ 2 ][ 2 ] = m_afEntry[ 0 ] * m_afEntry[ 4 ] - m_afEntry[ 1 ] * m_afEntry[ 3 ];

    float fDet = m_afEntry[ 0 ] * kInverse[ 0 ][ 0 ] + m_afEntry[ 1 ] * kInverse[ 1 ][ 0 ] + m_afEntry[ 2 ] * kInverse[ 2 ][ 0 ];

    if( Math::FAbs( fDet ) <= Math::EPSILON )
	{
        return Matrix3::ZERO;
	}

    kInverse /= fDet;

    return kInverse;
}

Matrix3 Matrix3::Adjoint() const
{
    Matrix3 kAdjoint;

    kAdjoint[ 0 ][ 0 ] = m_afEntry[ 4 ] * m_afEntry[ 8 ] - m_afEntry[ 5 ] * m_afEntry[ 7 ];
    kAdjoint[ 0 ][ 1 ] = m_afEntry[ 2 ] * m_afEntry[ 7 ] - m_afEntry[ 1 ] * m_afEntry[ 8 ];
    kAdjoint[ 0 ][ 2 ] = m_afEntry[ 1 ] * m_afEntry[ 5 ] - m_afEntry[ 2 ] * m_afEntry[ 4 ];
    kAdjoint[ 1 ][ 0 ] = m_afEntry[ 5 ] * m_afEntry[ 6 ] - m_afEntry[ 3 ] * m_afEntry[ 8 ];
    kAdjoint[ 1 ][ 1 ] = m_afEntry[ 0 ] * m_afEntry[ 8 ] - m_afEntry[ 2 ] * m_afEntry[ 6 ];
    kAdjoint[ 1 ][ 2 ] = m_afEntry[ 2 ] * m_afEntry[ 3 ] - m_afEntry[ 0 ] * m_afEntry[ 5 ];
    kAdjoint[ 2 ][ 0 ] = m_afEntry[ 3 ] * m_afEntry[ 7 ] - m_afEntry[ 4 ] * m_afEntry[ 6 ];
    kAdjoint[ 2 ][ 1 ] = m_afEntry[ 1 ] * m_afEntry[ 6 ] - m_afEntry[ 0 ] * m_afEntry[ 7 ];
    kAdjoint[ 2 ][ 2 ] = m_afEntry[ 0 ] * m_afEntry[ 4 ] - m_afEntry[ 1 ] * m_afEntry[ 3 ];

    return kAdjoint;
}

float Matrix3::Determinant() const
{
    float fCo00 = m_afEntry[ 4 ] * m_afEntry[ 8 ] - m_afEntry[ 5 ] * m_afEntry[ 7 ];
    float fCo10 = m_afEntry[ 5 ] * m_afEntry[ 6 ] - m_afEntry[ 3 ] * m_afEntry[ 8 ];
    float fCo20 = m_afEntry[ 3 ] * m_afEntry[ 7 ] - m_afEntry[ 4 ] * m_afEntry[ 6 ];
    float fDet = m_afEntry[ 0 ] * fCo00 + m_afEntry[ 1 ] * fCo10 + m_afEntry[ 2 ] * fCo20;
    return fDet;
}

void Matrix3::ToAxisAngle( Vector3& rAxis, float& rfAngle ) const
{
    // Let (x,y,z) be the unit-length axis and let A be an angle of rotation.
    // The rotation matrix is R = I + sin(A)*P + (1-cos(A))*P^2 where
    // I is the identity and
    //
    //       +-        -+
    //   P = |  0 -z +y |
    //       | +z  0 -x |
    //       | -y +x  0 |
    //       +-        -+
    //
    // If A > 0, R represents a counterclockwise rotation about the axis in
    // the sense of looking from the tip of the axis vector towards the
    // origin.  Some algebra will show that
    //
    //   cos(A) = (trace(R)-1)/2  and  R - R^t = 2*sin(A)*P
    //
    // In the event that A = pi, R-R^t = 0 which prevents us from extracting
    // the axis through P.  Instead note that R = I+2*P^2 when A = pi, so
    // P^2 = (R-I)/2.  The diagonal entries of P^2 are x^2-1, y^2-1, and
    // z^2-1.  We can solve these for axis (x,y,z).  Because the angle is pi,
    // it does not matter which sign you choose on the square roots.

    float fTrace = m_afEntry[ 0 ] + m_afEntry[ 4 ] + m_afEntry[ 8 ];
    float fCos = 0.5f * ( fTrace - 1.0f );
    rfAngle = Math::ACos( fCos );  // in [0,PI]

    if( rfAngle > 0.0f )
    {
        if( rfAngle < Math::PI )
        {
            rAxis[ 0 ] = m_afEntry[ 7 ] - m_afEntry[ 5 ];
            rAxis[ 1 ] = m_afEntry[ 2 ] - m_afEntry[ 6 ];
            rAxis[ 2 ] = m_afEntry[ 3 ] - m_afEntry[ 1 ];
            rAxis.Normalize();
        }
        else
        {
            // angle is PI
            float fHalfInverse;
            if( m_afEntry[ 0 ] >= m_afEntry[ 4 ] )
            {
                // r00 >= r11
                if( m_afEntry[ 0 ] >= m_afEntry[ 8 ] )
                {
                    // r00 is maximum diagonal term
                    rAxis[ 0 ] = 0.5f * Math::Sqrt( m_afEntry[ 0 ] - m_afEntry[ 4 ] - m_afEntry[ 8 ] + 1.0f );
                    fHalfInverse = 0.5f / rAxis[ 0 ];
                    rAxis[ 1 ] = fHalfInverse * m_afEntry[ 1 ];
                    rAxis[ 2 ] = fHalfInverse * m_afEntry[ 2 ];
                }
                else
                {
                    // r22 is maximum diagonal term
                    rAxis[ 2 ] = 0.5f * Math::Sqrt( m_afEntry[ 8 ] - m_afEntry[ 0 ] - m_afEntry[ 4 ] + 1.0f );
                    fHalfInverse = 0.5f / rAxis[ 2 ];
                    rAxis[ 0 ] = fHalfInverse * m_afEntry[ 2 ];
                    rAxis[ 1 ] = fHalfInverse * m_afEntry[ 5 ];
                }
            }
            else
            {
                // r11 > r00
                if( m_afEntry[ 4 ] >= m_afEntry[ 8 ] )
                {
                    // r11 is maximum diagonal term
                    rAxis[ 1 ] = 0.5f * Math::Sqrt( m_afEntry[ 4 ] - m_afEntry[ 0 ] - m_afEntry[ 8 ] + 1.0f );
                    fHalfInverse  = 0.5f / rAxis[ 1 ];
                    rAxis[ 0 ] = fHalfInverse * m_afEntry[ 1 ];
                    rAxis[ 2 ] = fHalfInverse * m_afEntry[ 5 ];
                }
                else
                {
                    // r22 is maximum diagonal term
                    rAxis[ 2 ] = 0.5f * Math::Sqrt( m_afEntry[ 8 ] - m_afEntry[ 0 ] - m_afEntry[ 4 ] + 1.0f );
                    fHalfInverse = 0.5f / rAxis[ 2 ];
                    rAxis[ 0 ] = fHalfInverse * m_afEntry[ 2 ];
                    rAxis[ 1 ] = fHalfInverse * m_afEntry[ 5 ];
                }
            }
        }
    }
    else
    {
        // The angle is 0 and the matrix is the identity.  Any axis will
        // wor, so just use the x-axis.
        rAxis[ 0 ] = 1.0f;
        rAxis[ 1 ] = 0.0f;
        rAxis[ 2 ] = 0.0f;
    }
}

void Matrix3::Orthonormalize()
{
    // Algorithm uses Gram-Schmidt orthogonalization.  If 'this' matrix is
    // M = [m0|m1|m2], then orthonormal output matrix is Q = [q0|q1|q2],
    //
    //   q0 = m0/|m0|
    //   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
    //   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
    //
    // where |V| indicates length of vector V and A*B indicates dot
    // product of vectors A and B.

    // compute q0
    float fInvLength = Math::InvSqrt( m_afEntry[ 0 ] * m_afEntry[ 0 ] + m_afEntry[ 3 ] * m_afEntry[ 3 ] + m_afEntry[ 6 ] * m_afEntry[ 6 ] );

    m_afEntry[ 0 ] *= fInvLength;
    m_afEntry[ 3 ] *= fInvLength;
    m_afEntry[ 6 ] *= fInvLength;

    // compute q1
    float fDot0 = m_afEntry[ 0 ] * m_afEntry[ 1 ] + m_afEntry[ 3 ] * m_afEntry[ 4 ] + m_afEntry[ 6 ] * m_afEntry[ 7 ];

    m_afEntry[ 1 ] -= fDot0 * m_afEntry[ 0 ];
    m_afEntry[ 4 ] -= fDot0 * m_afEntry[ 3 ];
    m_afEntry[ 7 ] -= fDot0 * m_afEntry[ 6 ];

    fInvLength = Math::InvSqrt( m_afEntry[ 1 ] * m_afEntry[ 1 ] + m_afEntry[ 4 ] * m_afEntry[ 4 ] + m_afEntry[ 7 ] * m_afEntry[ 7 ] );

    m_afEntry[ 1 ] *= fInvLength;
    m_afEntry[ 4 ] *= fInvLength;
    m_afEntry[ 7 ] *= fInvLength;

    // compute q2
    float fDot1 = m_afEntry[ 1 ] * m_afEntry[ 2 ] + m_afEntry[ 4 ] * m_afEntry[ 5 ] + m_afEntry[ 7 ] * m_afEntry[ 8 ];

    fDot0 = m_afEntry[ 0 ] * m_afEntry[ 2 ] + m_afEntry[ 3 ] * m_afEntry[ 5 ] + m_afEntry[ 6 ] * m_afEntry[ 8 ];

    m_afEntry[ 2 ] -= fDot0 * m_afEntry[ 0 ] + fDot1 * m_afEntry[ 1 ];
    m_afEntry[ 5 ] -= fDot0 * m_afEntry[ 3 ] + fDot1 * m_afEntry[ 4 ];
    m_afEntry[ 8 ] -= fDot0 * m_afEntry[ 6 ] + fDot1 * m_afEntry[ 7 ];

    fInvLength = Math::InvSqrt( m_afEntry[ 2 ] * m_afEntry[ 2 ] + m_afEntry[ 5 ] * m_afEntry[ 5 ] + m_afEntry[ 8 ] * m_afEntry[ 8 ] );

    m_afEntry[ 2 ] *= fInvLength;
    m_afEntry[ 5 ] *= fInvLength;
    m_afEntry[ 8 ] *= fInvLength;
}

bool Matrix3::ToEulerAnglesXYZ( float& rfXAngle, float& rfYAngle, float& rfZAngle ) const
{
    // rot =  cy*cz          -cy*sz           sy
    //        cz*sx*sy+cx*sz  cx*cz-sx*sy*sz -cy*sx
    //       -cx*cz*sy+sx*sz  cz*sx+cx*sy*sz  cx*cy

    if( m_afEntry[ 2 ] < 1.0f )
    {
        if( m_afEntry[ 2 ] > -1.0f )
        {
            rfXAngle = Math::ATan2( -m_afEntry[ 5 ], m_afEntry[ 8 ] );
            rfYAngle = (float)asin( (double)m_afEntry[ 2 ] );
            rfZAngle = Math::ATan2( -m_afEntry[ 1 ], m_afEntry[ 0 ] );
            return true;
        }
        else
        {
            // WARNING.  Not unique.  XA - ZA = -atan2(r10,r11)
            rfXAngle = -Math::ATan2( m_afEntry[ 3 ], m_afEntry[ 4 ] );
            rfYAngle = -Math::HALF_PI;
            rfZAngle = 0.0f;
            return false;
        }
    }
    else
    {
        // WARNING.  Not unique.  XAngle + ZAngle = atan2(r10,r11)
        rfXAngle = Math::ATan2( m_afEntry[ 3 ], m_afEntry[ 4 ] );
        rfYAngle = Math::HALF_PI;
        rfZAngle = 0.0f;
        return false;
    }
}

bool Matrix3::ToEulerAnglesXZY( float& rfXAngle, float& rfZAngle, float& rfYAngle ) const
{
    // rot =  cy*cz          -sz              cz*sy
    //        sx*sy+cx*cy*sz  cx*cz          -cy*sx+cx*sy*sz
    //       -cx*sy+cy*sx*sz  cz*sx           cx*cy+sx*sy*sz

    if( m_afEntry[ 1 ] < 1.0f )
    {
        if( m_afEntry[ 1 ] > -1.0f )
        {
            rfXAngle = Math::ATan2( m_afEntry[ 7 ], m_afEntry[ 4 ] );
            rfZAngle = (float)asin( -(double)m_afEntry[ 1 ] );
            rfYAngle = Math::ATan2( m_afEntry[ 2 ], m_afEntry[ 0 ] );
            return true;
        }
        else
        {
            // WARNING.  Not unique.  XA - YA = atan2(r20,r22)
            rfXAngle = Math::ATan2( m_afEntry[ 6 ], m_afEntry[ 8 ] );
            rfZAngle = Math::HALF_PI;
            rfYAngle = 0.0f;
            return false;
        }
    }
    else
    {
        // WARNING.  Not unique.  XA + YA = atan2(-r20,r22)
        rfXAngle = Math::ATan2( -m_afEntry[ 6 ], m_afEntry[ 8 ] );
        rfZAngle = -Math::HALF_PI;
        rfYAngle = 0.0f;
        return false;
    }
}

bool Matrix3::ToEulerAnglesYXZ (float& rfYAngle, float& rfXAngle, float& rfZAngle) const
{
    // rot =  cy*cz+sx*sy*sz  cz*sx*sy-cy*sz  cx*sy
    //        cx*sz           cx*cz          -sx
    //       -cz*sy+cy*sx*sz  cy*cz*sx+sy*sz  cx*cy

    if( m_afEntry[ 5 ] < 1.0f )
    {
        if( m_afEntry[ 5 ] > -1.0f )
        {
            rfYAngle = Math::ATan2( m_afEntry[ 2 ], m_afEntry[ 8 ] );
            rfXAngle = (float)asin( -(double)m_afEntry[ 5 ] );
            rfZAngle = Math::ATan2( m_afEntry[ 3 ], m_afEntry[ 4 ] );
            return true;
        }
        else
        {
            // WARNING.  Not unique.  YA - ZA = atan2(r01,r00)
            rfYAngle = Math::ATan2( m_afEntry[ 1 ], m_afEntry[ 0 ] );
            rfXAngle = Math::HALF_PI;
            rfZAngle = 0.0f;
            return false;
        }
    }
    else
    {
        // WARNING.  Not unique.  YA + ZA = atan2(-r01,r00)
        rfYAngle = Math::ATan2( -m_afEntry[ 1 ], m_afEntry[ 0 ] );
        rfXAngle = -Math::HALF_PI;
        rfZAngle = 0.0f;
        return false;
    }
}

bool Matrix3::ToEulerAnglesYZX( float& rfYAngle, float& rfZAngle, float& rfXAngle ) const
{
    // rot =  cy*cz           sx*sy-cx*cy*sz  cx*sy+cy*sx*sz
    //        sz              cx*cz          -cz*sx
    //       -cz*sy           cy*sx+cx*sy*sz  cx*cy-sx*sy*sz

    if( m_afEntry[ 3 ] < 1.0f )
    {
        if( m_afEntry[ 3 ] > -1.0f )
        {
            rfYAngle = Math::ATan2( -m_afEntry[ 6 ], m_afEntry[ 0 ] );
            rfZAngle = (float)asin( (double)m_afEntry[ 3 ] );
            rfXAngle = Math::ATan2( -m_afEntry[ 5 ], m_afEntry[ 4 ] );
            return true;
        }
        else
        {
            // WARNING.  Not unique.  YA - XA = -atan2(r21,r22);
            rfYAngle = -Math::ATan2( m_afEntry[ 7 ], m_afEntry[ 8 ] );
            rfZAngle = -Math::HALF_PI;
            rfXAngle = 0.0f;
            return false;
        }
    }
    else
    {
        // WARNING.  Not unique.  YA + XA = atan2(r21,r22)
        rfYAngle = Math::ATan2( m_afEntry[ 7 ], m_afEntry[ 8 ] );
        rfZAngle = Math::HALF_PI;
        rfXAngle = 0.0f;
        return false;
    }
}

bool Matrix3::ToEulerAnglesZXY( float& rfZAngle, float& rfXAngle, float& rfYAngle ) const
{
    // rot =  cy*cz-sx*sy*sz -cx*sz           cz*sy+cy*sx*sz
    //        cz*sx*sy+cy*sz  cx*cz          -cy*cz*sx+sy*sz
    //       -cx*sy           sx              cx*cy

    if( m_afEntry[ 7 ] < 1.0f )
    {
        if( m_afEntry[ 7 ] > -1.0f )
        {
            rfZAngle = Math::ATan2( -m_afEntry[ 1 ], m_afEntry[ 4 ] );
            rfXAngle = (float)asin( (double)m_afEntry[ 7 ] );
            rfYAngle = Math::ATan2( -m_afEntry[ 6 ], m_afEntry[ 8 ] );
            return true;
        }
        else
        {
            // WARNING.  Not unique.  ZA - YA = -atan(r02,r00)
            rfZAngle = -Math::ATan2( m_afEntry[ 2 ], m_afEntry[ 0 ] );
            rfXAngle = -Math::HALF_PI;
            rfYAngle = 0.0f;
            return false;
        }
    }
    else
    {
        // WARNING.  Not unique.  ZA + YA = atan2(r02,r00)
        rfZAngle = Math::ATan2( m_afEntry[ 2 ], m_afEntry[ 0 ] );
        rfXAngle = Math::HALF_PI;
        rfYAngle = 0.0f;
        return false;
    }
}

bool Matrix3::ToEulerAnglesZYX( float& rfZAngle, float& rfYAngle, float& rfXAngle ) const
{
    // rot =  cy*cz           cz*sx*sy-cx*sz  cx*cz*sy+sx*sz
    //        cy*sz           cx*cz+sx*sy*sz -cz*sx+cx*sy*sz
    //       -sy              cy*sx           cx*cy

    if( m_afEntry[ 6 ] < 1.0f )
    {
        if( m_afEntry[ 6 ] > -1.0f )
        {
            rfZAngle = Math::ATan2( m_afEntry[ 3 ], m_afEntry[ 0 ] );
            rfYAngle = (float)asin( -(double)m_afEntry[ 6 ] );
            rfXAngle = Math::ATan2( m_afEntry[ 7 ], m_afEntry[ 8 ] );
            return true;
        }
        else
        {
            // WARNING.  Not unique.  ZA - XA = -atan2(r01,r02)
            rfZAngle = -Math::ATan2( m_afEntry[ 1 ], m_afEntry[ 2 ] );
            rfYAngle = Math::HALF_PI;
            rfXAngle = 0.0f;
            return false;
        }
    }
    else
    {
        // WARNING.  Not unique.  ZA + XA = atan2(-r01,-r02)
        rfZAngle = Math::ATan2( -m_afEntry[ 1 ], -m_afEntry[ 2 ] );
        rfYAngle = -Math::HALF_PI;
        rfXAngle = 0.0f;
        return false;
    }
}

void Matrix3::FromEulerAnglesXYZ( float fYAngle, float fPAngle, float fRAngle )
{
    float fCos, fSin;

    fCos = Math::Cos( fYAngle );
    fSin = Math::Sin( fYAngle );
    Matrix3 kXMat( 1.0f, 0.0f, 0.0f, 0.0f, fCos, -fSin, 0.0f, fSin, fCos );

    fCos = Math::Cos( fPAngle );
    fSin = Math::Sin( fPAngle );
    Matrix3 kYMat( fCos, 0.0f, fSin, 0.0f, 1.0f, 0.0f, -fSin, 0.0f, fCos );

    fCos = Math::Cos( fRAngle );
    fSin = Math::Sin( fRAngle );
    Matrix3 kZMat( fCos, -fSin, 0.0f, fSin, fCos, 0.0f, 0.0f, 0.0f, 1.0f );

    *this = kXMat * ( kYMat * kZMat );
}

void Matrix3::FromEulerAnglesXZY( float fYAngle, float fPAngle, float fRAngle )
{
    float fCos, fSin;

    fCos = Math::Cos( fYAngle );
    fSin = Math::Sin( fYAngle );
    Matrix3 kXMat( 1.0f, 0.0f, 0.0f, 0.0f, fCos, -fSin, 0.0f, fSin, fCos );

    fCos = Math::Cos( fPAngle );
    fSin = Math::Sin( fPAngle );
    Matrix3 kZMat( fCos, -fSin, 0.0f, fSin, fCos, 0.0f, 0.0f, 0.0f, 1.0f );

    fCos = Math::Cos( fRAngle );
    fSin = Math::Sin( fRAngle );
    Matrix3 kYMat( fCos, 0.0f, fSin, 0.0f, 1.0f, 0.0f, -fSin, 0.0f, fCos );

    *this = kXMat * ( kZMat * kYMat );
}

void Matrix3::FromEulerAnglesYXZ( float fYAngle, float fPAngle, float fRAngle )
{
    float fCos, fSin;

    fCos = Math::Cos( fYAngle );
    fSin = Math::Sin( fYAngle );
    Matrix3 kYMat( fCos, 0.0f, fSin, 0.0f, 1.0f, 0.0f, -fSin, 0.0f, fCos );

    fCos = Math::Cos( fPAngle );
    fSin = Math::Sin( fPAngle );
    Matrix3 kXMat( 1.0f, 0.0f, 0.0f, 0.0f, fCos, -fSin, 0.0f, fSin, fCos );

    fCos = Math::Cos( fRAngle );
    fSin = Math::Sin( fRAngle );
    Matrix3 kZMat( fCos, -fSin, 0.0f, fSin, fCos, 0.0f, 0.0f, 0.0f, 1.0f );

    *this = kYMat * ( kXMat * kZMat );
}

void Matrix3::FromEulerAnglesYZX( float fYAngle, float fPAngle, float fRAngle )
{
    float fCos, fSin;

    fCos = Math::Cos( fYAngle );
    fSin = Math::Sin( fYAngle );
    Matrix3 kYMat( fCos, 0.0f, fSin, 0.0f, 1.0f, 0.0f, -fSin, 0.0f, fCos );

    fCos = Math::Cos( fPAngle );
    fSin = Math::Sin( fPAngle );
    Matrix3 kZMat( fCos, -fSin, 0.0f, fSin, fCos, 0.0f, 0.0f, 0.0f, 1.0f );

    fCos = Math::Cos( fRAngle );
    fSin = Math::Sin( fRAngle );
    Matrix3 kXMat( 1.0f, 0.0f, 0.0f, 0.0f, fCos, -fSin, 0.0f, fSin, fCos );

    *this = kYMat * ( kZMat * kXMat );
}

void Matrix3::FromEulerAnglesZXY( float fYAngle, float fPAngle, float fRAngle )
{
    float fCos, fSin;

    fCos = Math::Cos( fYAngle );
    fSin = Math::Sin( fYAngle );
    Matrix3 kZMat( fCos, -fSin, 0.0f, fSin, fCos, 0.0f, 0.0f, 0.0f, 1.0f );

    fCos = Math::Cos( fPAngle );
    fSin = Math::Sin( fPAngle );
    Matrix3 kXMat( 1.0f, 0.0f, 0.0f, 0.0f, fCos, -fSin, 0.0f, fSin, fCos );

    fCos = Math::Cos( fRAngle );
    fSin = Math::Sin( fRAngle );
    Matrix3 kYMat( fCos, 0.0f, fSin, 0.0f, 1.0f, 0.0f, -fSin, 0.0f, fCos );

    *this = kZMat * ( kXMat * kYMat );
}

void Matrix3::FromEulerAnglesZYX( float fYAngle, float fPAngle, float fRAngle )
{
    float fCos, fSin;

    fCos = Math::Cos( fYAngle );
    fSin = Math::Sin( fYAngle );
    Matrix3 kZMat( fCos, -fSin, 0.0f, fSin, fCos, 0.0f, 0.0f, 0.0f, 1.0f );

    fCos = Math::Cos( fPAngle );
    fSin = Math::Sin( fPAngle );
    Matrix3 kYMat( fCos, 0.0f, fSin, 0.0f, 1.0f, 0.0f, -fSin, 0.0f, fCos );

    fCos = Math::Cos( fRAngle );
    fSin = Math::Sin( fRAngle );
    Matrix3 kXMat( 1.0f, 0.0f, 0.0f, 0.0f, fCos, -fSin, 0.0f, fSin, fCos );

    *this = kZMat * ( kYMat * kXMat );
}

Matrix3 Matrix3::Slerp( float fT, const Matrix3& rR0, const Matrix3& rR1 )
{
    Vector3 kAxis;
    float fAngle;
    Matrix3 kProd = rR0.TransposeTimes( rR1 );
    kProd.ToAxisAngle( kAxis, fAngle );
    return Matrix3( kAxis, fT * fAngle );
}

void Matrix3::Tridiagonalize( float afDiag[ 3 ], float afSubDiag[ 3 ] )
{
    // Householder reduction T = Q^t M Q
    //   Input:   
    //     mat, symmetric 3x3 matrix M
    //   Output:  
    //     mat, orthogonal matrix Q (a reflection)
    //     diag, diagonal entries of T
    //     subd, subdiagonal entries of T (T is symmetric)

    float fA = m_afEntry[ 0 ];
    float fB = m_afEntry[ 1 ];
    float fC = m_afEntry[ 2 ];
    float fD = m_afEntry[ 4 ];
    float fE = m_afEntry[ 5 ];
    float fF = m_afEntry[ 8 ];

    afDiag[ 0 ] = fA;
    afSubDiag[ 2 ] = 0.0f;

    if( Math::FAbs( fC ) >= Math::EPSILON )
    {
        float fLength = Math::Sqrt( fB * fB + fC * fC );
        float fInvLength = 1.0f / fLength;
        fB *= fInvLength;
        fC *= fInvLength;
        float fQ = 2.0f * fB * fE + fC * ( fF - fD );
        afDiag[ 1 ] = fD + fC * fQ;
        afDiag[ 2 ] = fF - fC * fQ;
        afSubDiag[ 0 ] = fLength;
        afSubDiag[ 1 ] = fE - fB * fQ;

        m_afEntry[ 0 ] = 1.0f;
        m_afEntry[ 1 ] = 0.0f;
        m_afEntry[ 2 ] = 0.0f;
        m_afEntry[ 3 ] = 0.0f;
        m_afEntry[ 4 ] = fB;
        m_afEntry[ 5 ] = fC;
        m_afEntry[ 6 ] = 0.0f;
        m_afEntry[ 7 ] = fC;
        m_afEntry[ 8 ] = -fB;
    }
    else
    {
        afDiag[ 1 ] = fD;
        afDiag[ 2 ] = fF;
        afSubDiag[ 0 ] = fB;
        afSubDiag[ 1 ] = fE;

        m_afEntry[ 0 ] = 1.0f;
        m_afEntry[ 1 ] = 0.0f;
        m_afEntry[ 2 ] = 0.0f;
        m_afEntry[ 3 ] = 0.0f;
        m_afEntry[ 4 ] = 1.0f;
        m_afEntry[ 5 ] = 0.0f;
        m_afEntry[ 6 ] = 0.0f;
        m_afEntry[ 7 ] = 0.0f;
        m_afEntry[ 8 ] = -1.0f;
    }
}

bool Matrix3::QLAlgorithm( float afDiag[ 3 ], float afSubDiag[ 3 ] )
{
    // QL iteration with implicit shifting to reduce matrix from tridiagonal
    // to diagonal

    for( int i0 = 0; i0 < 3; i0++ )
    {
        const int iMaxIter = 32;
        int iIter;

        for( iIter = 0; iIter < iMaxIter; iIter++ )
        {
            int i1;
            for( i1 = i0; i1 <= 1; i1++ )
            {
                float fSum = Math::FAbs( afDiag[ i1 ] ) + Math::FAbs( afDiag[ i1 + 1 ] );
                if( Math::FAbs( afSubDiag[ i1 ]) + fSum == fSum )
				{
                    break;
				}
            }

            if( i1 == i0 )
			{
                break;
			}

            float fTmp0 = ( afDiag[ i0 + 1 ] - afDiag[ i0 ] ) / (2.0f * afSubDiag[ i0 ] );
            float fTmp1 = Math::Sqrt( fTmp0 * fTmp0 + 1.0f );

            if( fTmp0 < 0.0f )
			{
                fTmp0 = afDiag[ i1 ] - afDiag[ i0 ] + afSubDiag[ i0 ] / ( fTmp0 - fTmp1 );
			}
            else
			{
                fTmp0 = afDiag[ i1 ] - afDiag[ i0 ] + afSubDiag[ i0 ] / ( fTmp0 + fTmp1 );
			}

            float fSin = 1.0f;
            float fCos = 1.0f;
            float fTmp2 = 0.0f;
            for( int i2 = i1-1; i2 >= i0; i2-- )
            {
                float fTmp3 = fSin * afSubDiag[ i2 ];
                float fTmp4 = fCos * afSubDiag[ i2 ];

                if( Math::FAbs( fTmp3 ) >= Math::FAbs( fTmp0 ) )
                {
                    fCos = fTmp0 / fTmp3;
                    fTmp1 = Math::Sqrt( fCos * fCos + 1.0f );
                    afSubDiag[ i2 + 1 ] = fTmp3 * fTmp1;
                    fSin = 1.0f / fTmp1;
                    fCos *= fSin;
                }
                else
                {
                    fSin = fTmp3 / fTmp0;
                    fTmp1 = Math::Sqrt( fSin * fSin + 1.0f );
                    afSubDiag[ i2 + 1 ] = fTmp0 * fTmp1;
                    fCos = 1.0f / fTmp1;
                    fSin *= fCos;
                }
                fTmp0 = afDiag[ i2 + 1 ] - fTmp2;
                fTmp1 = ( afDiag[ i2 ] - fTmp0 ) * fSin + 2.0f * fTmp4 * fCos;
                fTmp2 = fSin * fTmp1;
                afDiag[ i2 + 1 ] = fTmp0 + fTmp2;
                fTmp0 = fCos * fTmp1 - fTmp4;

                for( int iRow = 0; iRow < 3; iRow++ )
                {
                    fTmp3 = m_afEntry[ Index( iRow, i2 + 1 ) ];
                    m_afEntry[ Index( iRow, i2 + 1 ) ] = fSin * m_afEntry[ Index( iRow, i2 ) ] + fCos * fTmp3;
                    m_afEntry[ Index( iRow, i2 ) ] = fCos * m_afEntry[ Index( iRow, i2 ) ] - fSin * fTmp3;
                }
            }
            afDiag[ i0 ] -= fTmp2;
            afSubDiag[ i0 ] = fTmp0;
            afSubDiag[ i1 ] = 0.0f;
        }

        if( iIter == iMaxIter )
        {
            // Should not get here under normal circumstances
            assert( false );
            return false;
        }
    }

    return true;
}

void Matrix3::EigenDecomposition( Matrix3& rRot, Matrix3& rDiag ) const
{
    // Factor M = R*D*R^T.  The columns of R are the eigenvectors.  The
    // diagonal entries of D are the corresponding eigenvalues.
    float afDiag[ 3 ], afSubDiag[ 3 ];
    rRot = *this;
    rRot.Tridiagonalize( afDiag, afSubDiag );
    rRot.QLAlgorithm( afDiag, afSubDiag );

    // The Householder transformation is a reflection.  Make the eigenvectors
    // a right--handed system by changing sign on the last column.
    rRot[ 2 ][ 0 ] = -rRot[ 2 ][ 0 ];
    rRot[ 2 ][ 1 ] = -rRot[ 2 ][ 1 ];
    rRot[ 2 ][ 2 ] = -rRot[ 2 ][ 2 ];

    // (insertion) sort eigenvalues in increasing order, d0 <= d1 <= d2
    int i;
    float fSave;

    if( afDiag[ 1 ] < afDiag[ 0 ] )
    {
        // swap d0 and d1
        fSave = afDiag[ 0 ];
        afDiag[ 0 ] = afDiag[ 1 ];
        afDiag[ 1 ] = fSave;

        // swap V0 and V1
        for( i = 0; i < 3; i++ )
        {
            fSave = rRot[ i ][ 0 ];
            rRot[ i ][ 0 ] = rRot[ i ][ 1 ];
            rRot[ i ][ 1 ] = fSave;
        }
    }

    if( afDiag[ 2 ] < afDiag[ 1 ] )
    {
        // swap d1 and d2
        fSave = afDiag[ 1 ];
        afDiag[ 1 ] = afDiag[ 2 ];
        afDiag[ 2 ] = fSave;

        // swap V1 and V2
        for( i = 0; i < 3; i++ )
        {
            fSave = rRot[ i ][ 1 ];
            rRot[ i ][ 1 ] = rRot[ i ][ 2 ];
            rRot[ i ][ 2 ] = fSave;
        }
    }

    if( afDiag[ 1 ] < afDiag[ 0 ] )
    {
        // swap d0 and d1
        fSave = afDiag[ 0 ];
        afDiag[ 0 ] = afDiag[ 1 ];
        afDiag[ 1 ] = fSave;

        // swap V0 and V1
        for( i = 0; i < 3; i++ ) 
        {
            fSave = rRot[ i ][ 0 ];
            rRot[ i ][ 0 ] = rRot[ i ][ 1 ];
            rRot[ i ][ 1 ] = fSave;
        }
    }

    rDiag.MakeDiagonal( afDiag[ 0 ], afDiag[ 1 ], afDiag[ 2 ] );
}

void Matrix3::Bidiagonalize( Matrix3& rA, Matrix3& rL, Matrix3& rR )
{
    float afV[ 3 ], afW[ 3 ];
    float fLength, fSign, fT1, fInvT1, fT2;
    bool bIdentity;

    // Map first column to (*,0,0)
    fLength = Math::Sqrt( rA[ 0 ][ 0 ] * rA[ 0 ][ 0 ] + rA[ 1 ][ 0 ] * rA[ 1 ][ 0 ] + rA[ 2 ][ 0 ] * rA[ 2 ][ 0 ] );
    
	if( fLength > 0.0f )
    {
        fSign = ( rA[ 0 ][ 0 ] > 0.0f ? 1.0f : -1.0f );
        fT1 = rA[ 0 ][ 0 ] + fSign * fLength;
        fInvT1 = 1.0f / fT1;
        afV[ 1 ] = rA[ 1 ][ 0 ] * fInvT1;
        afV[ 2 ] = rA[ 2 ][ 0 ] * fInvT1;

        fT2 = -2.0f / ( 1.0f + afV[ 1 ] * afV[ 1 ] + afV[ 2 ] * afV[ 2 ] );
        afW[ 0 ] = fT2 * ( rA[ 0 ][ 0 ] + rA[ 1 ][ 0 ] * afV[ 1 ] + rA[ 2 ][ 0 ] * afV[ 2 ] );
        afW[ 1 ] = fT2 * ( rA[ 0 ][ 1 ] + rA[ 1 ][ 1 ] * afV[ 1 ] + rA[ 2 ][ 1 ] * afV[ 2 ] );
        afW[ 2 ] = fT2 * ( rA[ 0 ][ 2 ] + rA[ 1 ][ 2 ] * afV[ 1 ] + rA[ 2 ][ 2 ] * afV[ 2 ] );
        rA[ 0 ][ 0 ] += afW[ 0 ];
        rA[ 0 ][ 1 ] += afW[ 1 ];
        rA[ 0 ][ 2 ] += afW[ 2 ];
        rA[ 1 ][ 1 ] += afV[ 1 ] * afW[ 1 ];
        rA[ 1 ][ 2 ] += afV[ 1 ] * afW[ 2 ];
        rA[ 2 ][ 1 ] += afV[ 2 ] * afW[ 1 ];
        rA[ 2 ][ 2 ] += afV[ 2 ] * afW[ 2 ];

        rL[ 0 ][ 0 ] = 1.0f + fT2;
        rL[ 0 ][ 1 ] = fT2 * afV[ 1 ];
        rL[ 1 ][ 0 ] = rL[ 0 ][ 1 ];
        rL[ 0 ][ 2 ] = fT2 * afV[ 2 ];
        rL[ 2 ][ 0 ] = rL[ 0 ][ 2 ];
        rL[ 1 ][ 1 ] = 1.0f + fT2 * afV[ 1 ] * afV[ 1 ];
        rL[ 1 ][ 2 ] = fT2 * afV[ 1 ] * afV[ 2 ];
        rL[ 2 ][ 1 ] = rL[ 1 ][ 2 ];
        rL[ 2 ][ 2 ] = 1.0f + fT2 * afV[ 2 ] * afV[ 2 ];
        bIdentity = false;
    }
    else
    {
        rL = Matrix3::IDENTITY;
        bIdentity = true;
    }

    // Map first row to (*,*,0)
    fLength = Math::Sqrt( rA[ 0 ][ 1 ] * rA[ 0 ][ 1 ] + rA[ 0 ][ 2 ] * rA[ 0 ][ 2 ] );

    if( fLength > 0.0f )
    {
        fSign = ( rA[ 0 ][ 1 ] > 0.0f ? 1.0f : -1.0f );
        fT1 = rA[ 0 ][ 1 ] + fSign * fLength;
        afV[ 2 ] = rA[ 0 ][ 2 ] / fT1;

        fT2 = -2.0f / ( 1.0f + afV[ 2 ] * afV[ 2 ] );
        afW[ 0 ] = fT2 * ( rA[ 0 ][ 1 ] + rA[ 0 ][ 2 ] * afV[ 2 ] );
        afW[ 1 ] = fT2 * ( rA[ 1 ][ 1 ] + rA[ 1 ][ 2 ] * afV[ 2 ] );
        afW[ 2 ] = fT2 * ( rA[ 2 ][ 1 ] + rA[ 2 ][ 2 ] * afV[ 2 ] );
        rA[ 0 ][ 1 ] += afW[ 0 ];
        rA[ 1 ][ 1 ] += afW[ 1 ];
        rA[ 1 ][ 2 ] += afW[ 1 ] * afV[ 2 ];
        rA[ 2 ][ 1 ] += afW[ 2 ];
        rA[ 2 ][ 2 ] += afW[ 2 ] * afV[ 2 ];
        
        rR[ 0 ][ 0 ] = 1.0f;
        rR[ 0 ][ 1 ] = 0.0f;
        rR[ 1 ][ 0 ] = 0.0f;
        rR[ 0 ][ 2 ] = 0.0f;
        rR[ 2 ][ 0 ] = 0.0f;
        rR[ 1 ][ 1 ] = 1.0f + fT2;
        rR[ 1 ][ 2 ] = fT2 * afV[ 2 ];
        rR[ 2 ][ 1 ] = rR[ 1 ][ 2 ];
        rR[ 2 ][ 2 ] = 1.0f + fT2 * afV[ 2 ] * afV[ 2 ];
    }
    else
    {
        rR = Matrix3::IDENTITY;
    }

    // map second column to (*,*,0)
    fLength = Math::Sqrt( rA[ 1 ][ 1 ] * rA[ 1 ][ 1 ] + rA[ 2 ][ 1 ] * rA[ 2 ][ 1 ] );

    if( fLength > 0.0f )
    {
        fSign = ( rA[ 1 ][ 1 ] > 0.0f ? 1.0f : -1.0f );
        fT1 = rA[ 1 ][ 1 ] + fSign * fLength;
        afV[ 2 ] = rA[ 2 ][ 1 ] / fT1;

        fT2 = -2.0f / ( 1.0f + afV[ 2 ] * afV[ 2 ] );
        afW[ 1 ] = fT2 * ( rA[ 1 ][ 1 ] + rA[ 2 ][ 1 ] * afV[ 2 ] );
        afW[ 2 ] = fT2 * ( rA[ 1 ][ 2 ] + rA[ 2 ][ 2 ] * afV[ 2 ] );
        rA[ 1 ][ 1 ] += afW[ 1 ];
        rA[ 1 ][ 2 ] += afW[ 2 ];
        rA[ 2 ][ 2 ] += afV[ 2 ] * afW[ 2 ];

        float fA = 1.0f + fT2;
        float fB = fT2 * afV[ 2 ];
        float fC = 1.0f + fB * afV[ 2 ];

        if( bIdentity )
        {
            rL[ 0 ][ 0 ] = 1.0f;
            rL[ 0 ][ 1 ] = 0.0f;
            rL[ 1 ][ 0 ] = 0.0f;
            rL[ 0 ][ 2 ] = 0.0f;
            rL[ 2 ][ 0 ] = 0.0f;
            rL[ 1 ][ 1 ] = fA;
            rL[ 1 ][ 2 ] = fB;
            rL[ 2 ][ 1 ] = fB;
            rL[ 2 ][ 2 ] = fC;
        }
        else
        {
            for( int iRow = 0; iRow < 3; iRow++ )
            {
                float fTmp0 = rL[ iRow ][ 1 ];
                float fTmp1 = rL[ iRow ][ 2 ];
                rL[ iRow ][ 1 ] = fA * fTmp0 + fB * fTmp1;
                rL[ iRow ][ 2 ] = fB * fTmp0 + fC * fTmp1;
            }
        }
    }
}

void Matrix3::GolubKahanStep( Matrix3& rA, Matrix3& rL, Matrix3& rR )
{
    float fT11 = rA[ 0 ][ 1 ] * rA[ 0 ][ 1 ] + rA[ 1 ][ 1 ] * rA[ 1 ][ 1 ];
    float fT22 = rA[ 1 ][ 2 ] * rA[ 1 ][ 2 ] + rA[ 2 ][ 2 ] * rA[ 2 ][ 2 ];
    float fT12 = rA[ 1 ][ 1 ] * rA[ 1 ][ 2 ];
    float fTrace = fT11 + fT22;
    float fDiff = fT11 - fT22;
    float fDiscr = Math::Sqrt( fDiff * fDiff + 4.0f * fT12 * fT12 );
    float fRoot1 = 0.5f * ( fTrace + fDiscr );
    float fRoot2 = 0.5f * ( fTrace - fDiscr );

    // Adjust right
    float fY = rA[ 0 ][ 0 ] - ( Math::FAbs( fRoot1 - fT22 ) <= Math::FAbs( fRoot2 - fT22 ) ? fRoot1 : fRoot2 );
    float fZ = rA[ 0 ][ 1 ];
    float fInvLength = Math::InvSqrt( fY * fY + fZ * fZ );
    float fSin = fZ * fInvLength;
    float fCos = -fY * fInvLength;

    float fTmp0 = rA[ 0 ][ 0 ];
    float fTmp1 = rA[ 0 ][ 1 ];
    rA[ 0 ][ 0 ] = fCos * fTmp0 - fSin * fTmp1;
    rA[ 0 ][ 1 ] = fSin * fTmp0 + fCos * fTmp1;
    rA[ 1 ][ 0 ] = -fSin * rA[ 1 ][ 1 ];
    rA[ 1 ][ 1 ] *= fCos;

    int iRow;
    for( iRow = 0; iRow < 3; iRow++ )
    {
        fTmp0 = rR[ 0 ][ iRow ];
        fTmp1 = rR[ 1 ][ iRow ];
        rR[ 0 ][ iRow ] = fCos * fTmp0 - fSin * fTmp1;
        rR[ 1 ][ iRow ] = fSin * fTmp0 + fCos * fTmp1;
    }

    // Adjust left
    fY = rA[ 0 ][ 0 ];
    fZ = rA[ 1 ][ 0 ];
    fInvLength = Math::InvSqrt( fY * fY + fZ * fZ );
    fSin = fZ * fInvLength;
    fCos = -fY * fInvLength;

    rA[ 0 ][ 0 ] = fCos * rA[ 0 ][ 0 ] - fSin * rA[ 1 ][ 0 ];
    fTmp0 = rA[ 0 ][ 1 ];
    fTmp1 = rA[ 1 ][ 1 ];
    rA[ 0 ][ 1 ] = fCos * fTmp0 - fSin * fTmp1;
    rA[ 1 ][ 1 ] = fSin * fTmp0 + fCos * fTmp1;
    rA[ 0 ][ 2 ] = -fSin * rA[ 1 ][ 2 ];
    rA[ 1 ][ 2 ] *= fCos;

    int iCol;
    for( iCol = 0; iCol < 3; iCol++ )
    {
        fTmp0 = rL[ iCol ][ 0 ];
        fTmp1 = rL[ iCol ][ 1 ];
        rL[ iCol ][ 0 ] = fCos * fTmp0 - fSin * fTmp1;
        rL[ iCol ][ 1 ] = fSin * fTmp0 + fCos * fTmp1;
    }

    // adjust right
    fY = rA[ 0 ][ 1 ];
    fZ = rA[ 0 ][ 2 ];
    fInvLength = Math::InvSqrt( fY * fY + fZ * fZ );
    fSin = fZ * fInvLength;
    fCos = -fY * fInvLength;

    rA[ 0 ][ 1 ] = fCos * rA[ 0 ][ 1 ] - fSin * rA[ 0 ][ 2 ];
    fTmp0 = rA[ 1 ][ 1 ];
    fTmp1 = rA[ 1 ][ 2 ];
    rA[ 1 ][ 1 ] = fCos * fTmp0 - fSin * fTmp1;
    rA[ 1 ][ 2 ] = fSin * fTmp0 + fCos * fTmp1;
    rA[ 2 ][ 1 ] = -fSin * rA[ 2 ][ 2 ];
    rA[ 2 ][ 2 ] *= fCos;

    for( iRow = 0; iRow < 3; iRow++ )
    {
        fTmp0 = rR[ 1 ][ iRow ];
        fTmp1 = rR[ 2 ][ iRow ];
        rR[ 1 ][ iRow ] = fCos * fTmp0 - fSin * fTmp1;
        rR[ 2 ][ iRow ] = fSin * fTmp0 + fCos * fTmp1;
    }

    // adjust left
    fY = rA[ 1 ][ 1 ];
    fZ = rA[ 2 ][ 1 ];
    fInvLength = Math::InvSqrt( fY * fY + fZ * fZ );
    fSin = fZ * fInvLength;
    fCos = -fY * fInvLength;

    rA[ 1 ][ 1 ] = fCos * rA[ 1 ][ 1 ] - fSin * rA[ 2 ][ 1 ];
    fTmp0 = rA[ 1 ][ 2 ];
    fTmp1 = rA[ 2 ][ 2 ];
    rA[ 1 ][ 2 ] = fCos * fTmp0 - fSin * fTmp1;
    rA[ 2 ][ 2 ] = fSin * fTmp0 + fCos * fTmp1;

    for( iCol = 0; iCol < 3; iCol++ )
    {
        fTmp0 = rL[ iCol ][ 1 ];
        fTmp1 = rL[ iCol ][ 2 ];
        rL[ iCol ][ 1 ] = fCos * fTmp0 - fSin * fTmp1;
        rL[ iCol ][ 2 ] = fSin * fTmp0 + fCos * fTmp1;
    }
}

void Matrix3::SingularValueDecomposition( Matrix3& rL, Matrix3& rS, Matrix3& rR ) const
{
    int iRow, iCol;

    Matrix3 kA = *this;
    Bidiagonalize( kA, rL, rR );
    rS.MakeZero();

    const int iMax = 32;
    const float fEpsilon = 1e-04f;
    for( int i = 0; i < iMax; i++ )
    {
        float fTmp, fTmp0, fTmp1;
        float fSin0, fCos0, fTan0;
        float fSin1, fCos1, fTan1;

        bool bTest1 = ( Math::FAbs( kA[ 0 ][ 1 ] ) <= fEpsilon * ( Math::FAbs( kA[ 0 ][ 0 ]) + Math::FAbs( kA[ 1 ][ 1 ] ) ) );
        bool bTest2 = ( Math::FAbs( kA[ 1 ][ 2 ] ) <= fEpsilon * ( Math::FAbs( kA[ 1 ][ 1 ]) + Math::FAbs( kA[ 2 ][ 2 ] ) ) );

        if( bTest1 )
        {
            if( bTest2 )
            {
                rS[ 0 ][ 0 ] = kA[ 0 ][ 0 ];
                rS[ 1 ][ 1 ] = kA[ 1 ][ 1 ];
                rS[ 2 ][ 2 ] = kA[ 2 ][ 2 ];
                break;
            }
            else
            {
                // 2x2 closed form factorization
                fTmp = ( kA[ 1 ][ 1 ] * kA[ 1 ][ 1 ] - kA[ 2 ][ 2 ] * kA[ 2 ][ 2 ] + kA[ 1 ][ 2 ] * kA[ 1 ][ 2 ] ) / ( kA[ 1 ][ 2 ] * kA[ 2 ][ 2 ] );
                fTan0 = 0.5f * ( fTmp + Math::Sqrt( fTmp * fTmp + 4.0f ) );
                fCos0 = Math::InvSqrt( 1.0f + fTan0 * fTan0 );
                fSin0 = fTan0 * fCos0;

                for( iCol = 0; iCol < 3; iCol++ )
                {
                    fTmp0 = rL[ iCol ][ 1 ];
                    fTmp1 = rL[ iCol ][ 2 ];
                    rL[ iCol ][ 1 ] = fCos0 * fTmp0 - fSin0 * fTmp1;
                    rL[ iCol ][ 2 ] = fSin0 * fTmp0 + fCos0 * fTmp1;
                }
                
                fTan1 = ( kA[ 1 ][ 2 ] - kA[ 2 ][ 2 ] * fTan0 ) / kA[ 1 ][ 1 ];
                fCos1 = Math::InvSqrt( 1.0f + fTan1 * fTan1 );
                fSin1 = -fTan1 * fCos1;

                for( iRow = 0; iRow < 3; iRow++ )
                {
                    fTmp0 = rR[ 1 ][ iRow ];
                    fTmp1 = rR[ 2 ][ iRow ];
                    rR[ 1 ][ iRow ] = fCos1 * fTmp0 - fSin1 * fTmp1;
                    rR[ 2 ][ iRow ] = fSin1 * fTmp0 + fCos1 * fTmp1;
                }

                rS[ 0 ][ 0 ] = kA[ 0 ][ 0 ];
                rS[ 1 ][ 1 ] = fCos0 * fCos1 * kA[ 1 ][ 1 ] - fSin1 * ( fCos0 * kA[ 1 ][ 2 ] - fSin0 * kA[ 2 ][ 2 ] );
                rS[ 2 ][ 2 ] = fSin0 * fSin1 * kA[ 1 ][ 1 ] + fCos1 * ( fSin0 * kA[ 1 ][ 2 ] + fCos0 * kA[ 2 ][ 2 ] );
                break;
            }
        }
        else 
        {
            if( bTest2 )
            {
                // 2x2 closed form factorization 
                fTmp = ( kA[ 0 ][ 0 ] * kA[ 0 ][ 0 ] + kA[ 1 ][ 1 ] * kA[ 1 ][ 1 ] - kA[ 0 ][ 1 ] * kA[ 0 ][ 1 ] ) / ( kA[ 0 ][ 1 ] * kA[ 1 ][ 1 ] );
                fTan0 = 0.5f * ( -fTmp + Math::Sqrt( fTmp * fTmp + 4.0f ) );
                fCos0 = Math::InvSqrt( 1.0f + fTan0 * fTan0 );
                fSin0 = fTan0 * fCos0;

                for( iCol = 0; iCol < 3; iCol++ )
                {
                    fTmp0 = rL[ iCol ][ 0 ];
                    fTmp1 = rL[ iCol ][ 1 ];
                    rL[ iCol ][ 0 ] = fCos0 * fTmp0 - fSin0 * fTmp1;
                    rL[ iCol ][ 1 ] = fSin0 * fTmp0 + fCos0 * fTmp1;
                }
                
                fTan1 = ( kA[ 0 ][ 1 ] - kA[ 1 ][ 1 ] * fTan0 ) / kA[ 0 ][ 0 ];
                fCos1 = Math::InvSqrt( 1.0f + fTan1 * fTan1 );
                fSin1 = -fTan1 * fCos1;

                for( iRow = 0; iRow < 3; iRow++ )
                {
                    fTmp0 = rR[ 0 ][ iRow ];
                    fTmp1 = rR[ 1 ][ iRow ];
                    rR[ 0 ][ iRow ] = fCos1 * fTmp0 - fSin1 * fTmp1;
                    rR[ 1 ][ iRow ] = fSin1 * fTmp0 + fCos1 * fTmp1;
                }

                rS[ 0 ][ 0 ] = fCos0 * fCos1 * kA[ 0 ][ 0 ] - fSin1 * ( fCos0 * kA[ 0 ][ 1 ] - fSin0 * kA[ 1 ][ 1 ] );
                rS[ 1 ][ 1 ] = fSin0 * fSin1 * kA[ 0 ][ 0 ] + fCos1 * ( fSin0 * kA[ 0 ][ 1 ] + fCos0 * kA[ 1 ][ 1 ] );
                rS[ 2 ][ 2 ] = kA[ 2 ][ 2 ];
                break;
            }
            else
            {
                GolubKahanStep( kA, rL, rR );
            }
        }
    }

    // Positize diagonal
    for( iRow = 0; iRow < 3; iRow++ )
    {
        if( rS[ iRow ][ iRow ] < 0.0f )
        {
            rS[ iRow ][ iRow ] = -rS[ iRow ][ iRow ];
            for( iCol = 0; iCol < 3; iCol++ )
			{
                rR[ iRow ][ iCol ] = -rR[ iRow ][ iCol ];
			}
        }
    }
}

void Matrix3::SingularValueComposition( const Matrix3& rL, const Matrix3& rS, const Matrix3& rR )
{
    *this = rL * ( rS * rR );
}

void Matrix3::QDUDecomposition( Matrix3& rQ, Matrix3& rD, Matrix3& rU ) const
{
    // Factor M = QR = QDU where Q is orthogonal (rotation), D is diagonal
    // (scaling),  and U is upper triangular with ones on its diagonal
    // (shear).  Algorithm uses Gram-Schmidt orthogonalization (the QR
    // algorithm).
    //
    // If M = [ m0 | m1 | m2 ] and Q = [ q0 | q1 | q2 ], then
    //
    //   q0 = m0/|m0|
    //   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
    //   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
    //
    // where |V| indicates length of vector V and A*B indicates dot
    // product of vectors A and B.  The matrix R has entries
    //
    //   r00 = q0*m0  r01 = q0*m1  r02 = q0*m2
    //   r10 = 0      r11 = q1*m1  r12 = q1*m2
    //   r20 = 0      r21 = 0      r22 = q2*m2
    //
    // so D = diag(r00,r11,r22) and U has entries u01 = r01/r00,
    // u02 = r02/r00, and u12 = r12/r11.

    // build orthogonal matrix Q
    float fInvLength = Math::InvSqrt( m_afEntry[ 0 ] * m_afEntry[ 0 ] + m_afEntry[ 3 ] * m_afEntry[ 3 ] + m_afEntry[ 6 ] * m_afEntry[ 6 ] );
    rQ[ 0 ][ 0 ] = m_afEntry[ 0 ] * fInvLength;
    rQ[ 1 ][ 0 ] = m_afEntry[ 3 ] * fInvLength;
    rQ[ 2 ][ 0 ] = m_afEntry[ 6 ] * fInvLength;

    float fDot = rQ[ 0 ][ 0 ] * m_afEntry[ 1 ] + rQ[ 1 ][ 0 ] * m_afEntry[ 4 ] + rQ[ 2 ][ 0 ] * m_afEntry[ 7 ];
    rQ[ 0 ][ 1 ] = m_afEntry[ 1 ] - fDot * rQ[ 0 ][ 0 ];
    rQ[ 1 ][ 1 ] = m_afEntry[ 4 ] - fDot * rQ[ 1 ][ 0 ];
    rQ[ 2 ][ 1 ] = m_afEntry[ 7 ] - fDot * rQ[ 2 ][ 0 ];
    fInvLength = Math::InvSqrt( rQ[ 0 ][ 1 ] * rQ[ 0 ][ 1 ] + rQ[ 1 ][ 1 ] * rQ[ 1 ][ 1 ] + rQ[ 2 ][ 1 ] * rQ[ 2 ][ 1 ] );
    rQ[ 0 ][ 1 ] *= fInvLength;
    rQ[ 1 ][ 1 ] *= fInvLength;
    rQ[ 2 ][ 1 ] *= fInvLength;

    fDot = rQ[ 0 ][ 0 ] * m_afEntry[ 2 ] + rQ[ 1 ][ 0 ] * m_afEntry[ 5 ] + rQ[ 2 ][ 0 ] * m_afEntry[ 8 ];
    rQ[ 0 ][ 2 ] = m_afEntry[ 2 ] - fDot * rQ[ 0 ][ 0 ];
    rQ[ 1 ][ 2 ] = m_afEntry[ 5 ] - fDot * rQ[ 1 ][ 0 ];
    rQ[ 2 ][ 2 ] = m_afEntry[ 8 ] - fDot * rQ[ 2 ][ 0 ];
    fDot = rQ[ 0 ][ 1 ] * m_afEntry[ 2 ] + rQ[ 1 ][ 1 ] * m_afEntry[ 5 ] + rQ[ 2 ][ 1 ] * m_afEntry[ 8 ];
    rQ[ 0 ][ 2 ] -= fDot * rQ[ 0 ][ 1 ];
    rQ[ 1 ][ 2 ] -= fDot * rQ[ 1 ][ 1 ];
    rQ[ 2 ][ 2 ] -= fDot * rQ[ 2 ][ 1 ];
    fInvLength = Math::InvSqrt( rQ[ 0 ][ 2 ] * rQ[ 0 ][ 2 ] + rQ[ 1 ][ 2 ] * rQ[ 1 ][ 2 ] + rQ[ 2 ][ 2 ] * rQ[ 2 ][ 2 ] );
    rQ[ 0 ][ 2 ] *= fInvLength;
    rQ[ 1 ][ 2 ] *= fInvLength;
    rQ[ 2 ][ 2 ] *= fInvLength;

    // Guarantee that orthogonal matrix has determinant 1 (no reflections)
    float fDet =  rQ[ 0 ][ 0 ] * rQ[ 1 ][ 1 ] * rQ[ 2 ][ 2 ] + rQ[ 0 ][ 1 ] * rQ[ 1 ][ 2 ] * rQ[ 2 ][ 0 ]
				+ rQ[ 0 ][ 2 ] * rQ[ 1 ][ 0 ] * rQ[ 2 ][ 1 ] - rQ[ 0 ][ 2 ] * rQ[ 1 ][ 1 ] * rQ[ 2 ][ 0 ]
				- rQ[ 0 ][ 1 ] * rQ[ 1 ][ 0 ] * rQ[ 2 ][ 2 ] - rQ[ 0 ][ 0 ] * rQ[ 1 ][ 2 ] * rQ[ 2 ][ 1 ];

    if( fDet < 0.0f )
    {
        for( int iRow = 0; iRow < 3; iRow++ )
        {
            for( int iCol = 0; iCol < 3; iCol++ )
			{
                rQ[ iRow ][ iCol ] = -rQ[ iRow ][ iCol ];
			}
        }
    }

    // build "right" matrix R
    Matrix3 kR;
    kR[ 0 ][ 0 ] = rQ[ 0 ][ 0 ] * m_afEntry[ 0 ] + rQ[ 1 ][ 0 ] * m_afEntry[ 3 ] + rQ[ 2 ][ 0 ] * m_afEntry[ 6 ];
    kR[ 0 ][ 1 ] = rQ[ 0 ][ 0 ] * m_afEntry[ 1 ] + rQ[ 1 ][ 0 ] * m_afEntry[ 4 ] + rQ[ 2 ][ 0 ] * m_afEntry[ 7 ];
    kR[ 1 ][ 1 ] = rQ[ 0 ][ 1 ] * m_afEntry[ 1 ] + rQ[ 1 ][ 1 ] * m_afEntry[ 4 ] + rQ[ 2 ][ 1 ] * m_afEntry[ 7 ];
    kR[ 0 ][ 2 ] = rQ[ 0 ][ 0 ] * m_afEntry[ 2 ] + rQ[ 1 ][ 0 ] * m_afEntry[ 5 ] + rQ[ 2 ][ 0 ] * m_afEntry[ 8 ];
    kR[ 1 ][ 2 ] = rQ[ 0 ][ 1 ] * m_afEntry[ 2 ] + rQ[ 1 ][ 1 ] * m_afEntry[ 5 ] + rQ[ 2 ][ 1 ] * m_afEntry[ 8 ];
    kR[ 2 ][ 2 ] = rQ[ 0 ][ 2 ] * m_afEntry[ 2 ] + rQ[ 1 ][ 2 ] * m_afEntry[ 5 ] + rQ[ 2 ][ 2 ] * m_afEntry[ 8 ];

    // the scaling component
    rD.MakeDiagonal( kR[ 0 ][ 0 ], kR[ 1 ][ 1 ], kR[ 2 ][ 2 ] );

    // the shear component
    float fInvD0 = 1.0f / rD[ 0 ][ 0 ];
    rU[ 0 ][ 0 ] = 1.0f;
    rU[ 0 ][ 1 ] = kR[ 0 ][ 1 ] * fInvD0;
    rU[ 0 ][ 2 ] = kR[ 0 ][ 2 ] * fInvD0;
    rU[ 1 ][ 0 ] = 0.0f;
    rU[ 1 ][ 1 ] = 1.0f;
    rU[ 1 ][ 2 ] = kR[ 1 ][ 2 ] / rD[ 1 ][ 1 ];
    rU[ 2 ][ 0 ] = 0.0f;
    rU[ 2 ][ 1 ] = 0.0f;
    rU[ 2 ][ 2 ] = 1.0f;
}

void Matrix3::MakeZero ()
{
    memset( m_afEntry, 0, 9 * sizeof( float ) );
}

void Matrix3::MakeIdentity()
{
    memset( m_afEntry, 0, 9 * sizeof( float ) );
    for( int i = 0; i < 3; i++ )
	{
        m_afEntry[ Index( i, i ) ] = 1.0f;
	}
}

void Matrix3::MakeDiagonal( const float* afDiag )
{
    memset( m_afEntry, 0, 9 * sizeof( float ) );
    for( int i = 0; i < 3; i++ )
	{
        m_afEntry[ Index( i, i ) ] = afDiag[ i ];
	}
}

Matrix3::operator const float* () const
{
    return m_afEntry;
}

Matrix3::operator float* ()
{
    return m_afEntry;
}

const float* Matrix3::operator[] ( int iRow ) const
{
    assert( 0 <= iRow && iRow < 3 );
    return &m_afEntry[ 3 * iRow ];
}

float* Matrix3::operator[] ( int iRow )
{
    assert( 0 <= iRow && iRow < 3 );
    return &m_afEntry[ 3 * iRow ];
}

float Matrix3::operator() ( int iRow, int iCol ) const
{
    return m_afEntry[ Index( iRow, iCol ) ];
}

float& Matrix3::operator() ( int iRow, int iCol )
{
    return m_afEntry[ Index( iRow, iCol ) ];
}

void Matrix3::SetRow( int iRow, const Vector3& rV )
{
    assert( 0 <= iRow && iRow < 3 );
    for( int iCol = 0, i = 3 * iRow; iCol < 3; iCol++, i++ )
	{
        m_afEntry[ i ] = rV[ iCol ];
	}
}

Vector3 Matrix3::GetRow( int iRow ) const
{
    assert( 0 <= iRow && iRow < 3 );
    Vector3 kV;
    for( int iCol = 0, i = 3*iRow; iCol < 3; iCol++, i++ )
	{
        kV[ iCol ] = m_afEntry[ i ];
	}

    return kV;
}

void Matrix3::SetColumn( int iCol, const Vector3& rV )
{
    assert( 0 <= iCol && iCol < 3 );
    for( int iRow = 0, i = iCol; iRow < 3; iRow++, i += 3 )
	{
        m_afEntry[ i ] = rV[ iRow ];
	}
}

Vector3 Matrix3::GetColumn( int iCol ) const
{
    assert( 0 <= iCol && iCol < 3 );
    Vector3 kV;
    for( int iRow = 0, i = iCol; iRow < 3; iRow++, i += 3 )
	{
        kV[ iRow ] = m_afEntry[ i ];
	}

    return kV;
}

void Matrix3::GetColumnMajor( float* afCMajor ) const
{
    for( int iRow = 0, i = 0; iRow < 3; iRow++ )
    {
        for( int iCol = 0; iCol < 3; iCol++ )
		{
            afCMajor[ i++ ] = m_afEntry[ Index( iCol, iRow ) ];
		}
    }
}

bool Matrix3::operator == ( const Matrix3& rM ) const
{
    return memcmp( m_afEntry, rM.m_afEntry, 9 * sizeof( float ) ) == 0;
}

bool Matrix3::operator != ( const Matrix3& rM ) const
{
    return memcmp( m_afEntry, rM.m_afEntry, 9 * sizeof( float ) ) != 0;
}

bool Matrix3::operator < ( const Matrix3& rM ) const
{
    return Math::CompareArrays( 3, *this, rM ) < 0;
}

bool Matrix3::operator <= ( const Matrix3& rM ) const
{
    return Math::CompareArrays( 3, *this, rM ) <= 0;
}

bool Matrix3::operator > ( const Matrix3& rM ) const
{
    return Math::CompareArrays( 3, *this, rM ) > 0;
}

bool Matrix3::operator >= ( const Matrix3& rM ) const
{
    return Math::CompareArrays( 3, *this, rM ) >= 0;
}

Matrix3 Matrix3::operator + ( const Matrix3& rM ) const
{
    Matrix3 kSum;
    for( int i = 0; i < 9; i++ )
	{
        kSum.m_afEntry[ i ] = m_afEntry[ i ] + rM.m_afEntry[ i ];
	}

    return kSum;
}

Matrix3 Matrix3::operator - ( const Matrix3& rM ) const
{
    Matrix3 kDiff;
    for( int i = 0; i < 9; i++ )
	{
        kDiff.m_afEntry[ i ] = m_afEntry[ i ] - rM.m_afEntry[ i ];
	}

    return kDiff;
}

Matrix3 Matrix3::operator * ( const Matrix3& rM ) const
{
    Matrix3 kProd;
    for( int iRow = 0; iRow < 3; iRow++ )
    {
        for( int iCol = 0; iCol < 3; iCol++ )
        {
            int i = Index( iRow, iCol );
            kProd.m_afEntry[ i ] = 0.0f;
            for( int iMid = 0; iMid < 3; iMid++ )
            {
                kProd.m_afEntry[ i ] += m_afEntry[ Index( iRow, iMid ) ] * rM.m_afEntry[ Index( iMid, iCol ) ];
            }
        }
    }
    return kProd;
}

Matrix3 Matrix3::operator * ( float fScalar ) const
{
    Matrix3 kProd;
    for( int i = 0; i < 9; i++ )
	{
        kProd.m_afEntry[ i ] = fScalar * m_afEntry[ i ];
	}
    return kProd;
}

Matrix3 Matrix3::operator / ( float fScalar ) const
{
    Matrix3 kQuot;
    int i;

    if( fScalar != 0.0f )
    {
        float fInvScalar = 1.0f / fScalar;
        for( i = 0; i < 9; i++ )
		{
            kQuot.m_afEntry[ i ] = fInvScalar * m_afEntry[ i ];
		}
    }
    else
    {
        for( i = 0; i < 9; i++ )
		{
            kQuot.m_afEntry[ i ] = Math::MAX_FLOAT;
		}
    }

    return kQuot;
}

Matrix3 Matrix3::operator - () const
{
    Matrix3 kNeg;
    for( int i = 0; i < 9; i++ )
	{
        kNeg.m_afEntry[ i ] = -m_afEntry[ i ];
	}
    return kNeg;
}

Matrix3& Matrix3::operator += ( const Matrix3& rM )
{
    for( int i = 0; i < 9; i++ )
	{
        m_afEntry[ i ] += rM.m_afEntry[ i ];
	}
    return *this;
}

Matrix3& Matrix3::operator -= ( const Matrix3& rM )
{
    for( int i = 0; i < 9; i++ )
	{
        m_afEntry[ i ] -= rM.m_afEntry[ i ];
	}
    return *this;
}

Matrix3& Matrix3::operator *= ( float fScalar )
{
    for( int i = 0; i < 9; i++ )
	{
        m_afEntry[ i ] *= fScalar;
	}
    return *this;
}

Matrix3& Matrix3::operator /= ( float fScalar )
{
    int i;

    if( fScalar != 0.0f )
    {
        float fInvScalar = 1.0f / fScalar;
        for( i = 0; i < 9; i++ )
		{
            m_afEntry[ i ] *= fInvScalar;
		}
    }
    else
    {
        for( i = 0; i < 9; i++ )
		{
            m_afEntry[ i ] = Math::MAX_FLOAT;
		}
    }

    return *this;
}

Matrix3 Matrix3::Transpose() const
{
    Matrix3 kTranspose;
    for( int iRow = 0; iRow < 3; iRow++ )
    {
        for( int iCol = 0; iCol < 3; iCol++ )
		{
            kTranspose.m_afEntry[ Index( iRow, iCol ) ] = m_afEntry[ Index( iCol, iRow ) ];
		}
    }
    return kTranspose;
}

Matrix3 Matrix3::TransposeTimes( const Matrix3& rM ) const
{
    // P = A^T*B, P[r][c] = sum_m A[m][r]*B[m][c]
    Matrix3 kProd;
    for( int iRow = 0; iRow < 3; iRow++ )
    {
        for( int iCol = 0; iCol < 3; iCol++ )
        {
            int i = Index( iRow, iCol );
            kProd.m_afEntry[ i ] = 0.0f;
            for( int iMid = 0; iMid < 3; iMid++ )
            {
                kProd.m_afEntry[ i ] += m_afEntry[ Index( iMid, iRow ) ] * rM.m_afEntry[ Index( iMid, iCol ) ];
            }
        }
    }
    return kProd;
}

Matrix3 Matrix3::TimesTranspose( const Matrix3& rM ) const
{
    // P = A*B^T, P[r][c] = sum_m A[r][m]*B[c][m]
    Matrix3 kProd;
    for( int iRow = 0; iRow < 3; iRow++ )
    {
        for( int iCol = 0; iCol < 3; iCol++ )
        {
            int i = Index( iRow, iCol );
            kProd.m_afEntry[ i ] = 0.0f;
            for( int iMid = 0; iMid < 3; iMid++ )
            {
                kProd.m_afEntry[ i ] += m_afEntry[ Index( iRow, iMid ) ] * rM.m_afEntry[ Index( iCol, iRow ) ];
            }
        }
    }
    return kProd;
}

Vector3 Matrix3::operator * ( const Vector3& rV ) const
{
    Vector3 kProd;
    for( int iRow = 0; iRow < 3; iRow++ )
    {
        kProd[ iRow ] = 0.0f;
        for( int iCol = 0; iCol < 3; iCol++ )
		{
            kProd[ iRow ] += m_afEntry[ Index( iRow, iCol ) ] * rV[ iCol ];
		}
    }
    return kProd;
}

float Matrix3::QForm( const Vector3& rU, const Vector3& rV ) const
{
    return rU.Dot( (*this)*rV );
}

int Matrix3::Index( int iRow, int iCol )
{
    assert( 0 <= iRow && iRow < 3 && 0 <= iCol && iCol < 3 );
    return iCol + 3 * iRow;
}

