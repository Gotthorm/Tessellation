// MATRIX3.HPP

#ifndef _MATRIX3_HPP_
#define _MATRIX3_HPP_

#include "Vector3.hpp"

// NOTE.  The (x,y,z) coordinate system is assumed to be right-handed.
// Coordinate axis rotation matrices are of the form
//   RX =    1       0       0
//           0     cos(t) -sin(t)
//           0     sin(t)  cos(t)
// where t > 0 indicates a counterclockwise rotation in the yz-plane
//   RY =  cos(t)    0     sin(t)
//           0       1       0
//        -sin(t)    0     cos(t)
// where t > 0 indicates a counterclockwise rotation in the zx-plane
//   RZ =  cos(t) -sin(t)    0
//         sin(t)  cos(t)    0
//           0       0       1
// where t > 0 indicates a counterclockwise rotation in the xy-plane.



/** @brief A 3D Matrix class.
  * @author Sean Butterworth
  * @version 1.0
  * @date Oct 2003
**/

/**
	* @todo 
	* Document the public methods
**/

class Matrix3
{
public:
    // Construction
    inline Matrix3();
    Matrix3( bool bZero );
    Matrix3( const Matrix3& rM );

    // Input Mrc is in row r, column c.
    Matrix3( float fM00, float fM01, float fM02,
             float fM10, float fM11, float fM12,
             float fM20, float fM21, float fM22 );

    // Create a matrix from an array of numbers.  The input array is
    // interpreted based on the Boolean input as
    //   true:  entry[0..8]={m00,m01,m02,m10,m11,m12,m20,m21,m22} [row major]
    //   false: entry[0..8]={m00,m10,m20,m01,m11,m21,m02,m12,m22} [col major]
    Matrix3( const float afEntry[ 9 ], bool bRowMajor );

    // Create matrices based on vector input.  The Boolean is interpreted as
    //   true: vectors are columns of the matrix
    //   false: vectors are rows of the matrix
    Matrix3( const Vector3& rU, const Vector3& rV, const Vector3& rW, bool bColumns );
    Matrix3( const Vector3* aV, bool bColumns );

    // Create a tensor product U*V^T
    Matrix3( const Vector3& rU, const Vector3& rV );
    void MakeTensorProduct( const Vector3& rU, const Vector3& rV );

    // Create a diagonal matrix
    Matrix3( float fM00, float fM11, float fM22 );
    void MakeDiagonal( float fM00, float fM11, float fM22 );

    // Create rotation matrices (positive angle - counterclockwise).  The
    // angle must be in radians, not degrees.
    Matrix3( const Vector3& rAxis, float fAngle );
    void FromAxisAngle( const Vector3& rAxis, float fAngle );

	// Create a skew symmetric matrix
	Matrix3( const Vector3& rVector );
	void MakeSkewSymmetric( const Vector3& rVector );

    // assignment
    Matrix3& operator = ( const Matrix3& rM );

    // matrix operations
    Matrix3 Inverse() const;
    Matrix3 Adjoint() const;
    float Determinant() const;

    // The matrix must be a rotation for these functions to be valid.  The
    // last function uses Gram-Schmidt orthonormalization applied to the
    // columns of the rotation matrix.  The angle must be in radians, not
    // degrees.
    void ToAxisAngle( Vector3& rAxis, float& rfAngle ) const;
    void Orthonormalize();

    // The matrix must be orthonormal.  The decomposition is yaw*pitch*roll
    // where yaw is rotation about the Up vector, pitch is rotation about the
    // Right axis, and roll is rotation about the Direction axis.
    bool ToEulerAnglesXYZ( float& rfYAngle, float& rfPAngle, float& rfRAngle ) const;
    bool ToEulerAnglesXZY( float& rfYAngle, float& rfPAngle, float& rfRAngle ) const;
    bool ToEulerAnglesYXZ( float& rfYAngle, float& rfPAngle, float& rfRAngle ) const;
    bool ToEulerAnglesYZX( float& rfYAngle, float& rfPAngle, float& rfRAngle ) const;
    bool ToEulerAnglesZXY( float& rfYAngle, float& rfPAngle, float& rfRAngle ) const;
    bool ToEulerAnglesZYX( float& rfYAngle, float& rfPAngle, float& rfRAngle ) const;

    void FromEulerAnglesXYZ( float fYAngle, float fPAngle, float fRAngle );
    void FromEulerAnglesXZY( float fYAngle, float fPAngle, float fRAngle );
    void FromEulerAnglesYXZ( float fYAngle, float fPAngle, float fRAngle );
    void FromEulerAnglesYZX( float fYAngle, float fPAngle, float fRAngle );
    void FromEulerAnglesZXY( float fYAngle, float fPAngle, float fRAngle );
    void FromEulerAnglesZYX( float fYAngle, float fPAngle, float fRAngle );

    // SLERP (spherical linear interpolation) without quaternions.  Computes
    // R(t) = R0*(Transpose(R0)*R1)^t.  If Q is a rotation matrix with
    // unit-length axis U and angle A, then Q^t is a rotation matrix with
    // unit-length axis U and rotation angle t*A.
    static Matrix3 Slerp( float fT, const Matrix3& rR0, const Matrix3& rR1 );

    // The matrix must be symmetric.  Factor M = R * D * R^T where
    // R = [u0|u1|u2] is a rotation matrix with columns u0, u1, and u2 and
    // D = diag(d0,d1,d2) is a diagonal matrix whose diagonal entries are d0,
    // d1, and d2.  The eigenvector u[i] corresponds to eigenvector d[i].
    // The eigenvalues are ordered as d0 <= d1 <= d2.
    void EigenDecomposition( Matrix3& rRot, Matrix3& rDiag ) const;

    // Singular value decomposition, M = L*S*R, where L and R are orthogonal
    // and S is a diagonal matrix whose diagonal entries are nonnegative.
    void SingularValueDecomposition( Matrix3& rL, Matrix3& rS, Matrix3& rR ) const;
    void SingularValueComposition( const Matrix3& rL, const Matrix3& rS, const Matrix3& rR );

    // Factor M = Q*D*U with orthogonal Q, diagonal D, upper triangular U
    void QDUDecomposition( Matrix3& rQ, Matrix3& rD, Matrix3& rU ) const;

    // Special matrices
    static const Matrix3 ZERO;
    static const Matrix3 IDENTITY;

    void MakeZero();
    void MakeIdentity();
    void MakeDiagonal( const float* afDiag );

    // Member access
    operator const float* () const;
    operator float* ();
    const float* operator[] ( int iRow ) const;
    float* operator[] ( int iRow );
    float operator() ( int iRow, int iCol ) const;
    float& operator() ( int iRow, int iCol );

    void SetRow( int iRow, const Vector3& rV );
    Vector3 GetRow( int iRow ) const;
    void SetColumn( int iCol, const Vector3& rV );
    Vector3 GetColumn( int iCol ) const;
    void GetColumnMajor( float* afCMajor ) const;

    // Comparison
    bool operator == ( const Matrix3& rM ) const;
    bool operator != ( const Matrix3& rM ) const;
    bool operator <  ( const Matrix3& rM ) const;
    bool operator <= ( const Matrix3& rM ) const;
    bool operator >  ( const Matrix3& rM ) const;
    bool operator >= ( const Matrix3& rM ) const;

    // Arithmetic operations
    Matrix3 operator + ( const Matrix3& rM ) const;
    Matrix3 operator - ( const Matrix3& rM ) const;
    Matrix3 operator * ( const Matrix3& rM ) const;
    Matrix3 operator * ( float fScalar ) const;
    Matrix3 operator / ( float fScalar ) const;
    Matrix3 operator - () const;

    // Arithmetic updates
    Matrix3& operator += ( const Matrix3& rM );
    Matrix3& operator -= ( const Matrix3& rM );
    Matrix3& operator *= ( float fScalar );
    Matrix3& operator /= ( float fScalar );

    // Matrix products
    Matrix3 Transpose() const;  // M^T
    Matrix3 TransposeTimes( const Matrix3& rM ) const;  // this^T * M
    Matrix3 TimesTranspose( const Matrix3& rM ) const;  // this * M^T

    // Matrix-vector operations
    Vector3 operator * ( const Vector3& rV ) const;  // M * v
    float QForm( const Vector3& rU, const Vector3& rV ) const;  // u^T*M*v

private:
    // Support for eigendecomposition
    void Tridiagonalize( float afDiag[ 3 ], float afSubDiag[ 3 ] );
    bool QLAlgorithm( float afDiag[ 3 ], float afSubDiag[ 3 ] );

    // Support for singular value decomposition
    static void Bidiagonalize( Matrix3& rA, Matrix3& rL, Matrix3& rR );
    static void GolubKahanStep( Matrix3& rA, Matrix3& rL, Matrix3& rR );

    // For indexing into the 1D array of the matrix, iCol+N*iRow
    static int Index( int iRow, int iCol );

    float m_afEntry[ 3 * 3 ];
};

#endif // _MATRIX3_HPP_
