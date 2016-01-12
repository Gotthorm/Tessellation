// MATRIX4.HPP

#ifndef _MATRIX4_HPP_
#define _MATRIX4_HPP_

#include "Vector3.hpp"
#include "Vector4.hpp"

/** @brief A 4D Matrix class.
  * @author Sean Butterworth
  * @version 1.0
  * @date Oct 2003
**/

/**
	* @todo 
	* Document the public methods
**/

class Matrix4
{
public:
    // Construction
    inline Matrix4();
    Matrix4( bool bZero );
    Matrix4( const Matrix4& rM );

    // Input Mrc is in row r, column c.
    Matrix4( float fM00, float fM01, float fM02, float fM03,
             float fM10, float fM11, float fM12, float fM13,
             float fM20, float fM21, float fM22, float fM23,
             float fM30, float fM31, float fM32, float fM33 );

    // Create a matrix from an array of numbers.  The input array is
    // interpreted based on the Boolean input as
    //   true:  entry[0..15]={m00,m01,m02,m03,m10,m11,m12,m13,m20,m21,m22, m23,m30,m31,m32,m33} [row major]
    //   false: entry[0..15]={m00,m10,m20,m30,m01,m11,m21,m31,m02,m12,m22, m32,m03,m13,m23,m33} [col major]
    Matrix4( const float afEntry[ 16 ], bool bRowMajor );

    // Assignment
    Matrix4& operator = ( const Matrix4& rM );

    // Matrix operations
    Matrix4 Inverse() const;
    Matrix4 Adjoint() const;
    float Determinant() const;

    // Special matrices
    static const Matrix4 ZERO;
    static const Matrix4 IDENTITY;

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

    void SetRow( int iRow, const Vector4& rV );
    Vector4 GetRow( int iRow ) const;
    void SetColumn( int iCol, const Vector4& rV );
    Vector4 GetColumn( int iCol ) const;
    void GetColumnMajor( float* afCMajor ) const;

	// Create a skew symmetric matrix
	//Matrix4( const Vector4& rVector );
	//void MakeSkewSymmetric( const Vector4& rVector );

    // Comparison
    bool operator == ( const Matrix4& rM ) const;
    bool operator != ( const Matrix4& rM ) const;
    bool operator <  ( const Matrix4& rM ) const;
    bool operator <= ( const Matrix4& rM ) const;
    bool operator >  ( const Matrix4& rM ) const;
    bool operator >= ( const Matrix4& rM ) const;

    // Arithmetic operations
    Matrix4 operator + ( const Matrix4& rM ) const;
    Matrix4 operator - ( const Matrix4& rM ) const;
    Matrix4 operator * ( const Matrix4& rM ) const;
    Matrix4 operator * ( float fScalar ) const;
    Matrix4 operator / ( float fScalar ) const;
    Matrix4 operator - () const;

    // Arithmetic updates
    Matrix4& operator += ( const Matrix4& rM );
    Matrix4& operator -= ( const Matrix4& rM );
    Matrix4& operator *= ( float fScalar );
    Matrix4& operator /= ( float fScalar );

    // Matrix products
    Matrix4 Transpose() const;  // M^T
    Matrix4 TransposeTimes( const Matrix4& rM ) const;  // this^T * M
    Matrix4 TimesTranspose( const Matrix4& rM ) const;  // this * M^T

    // Matrix-vector operations
    Vector4 operator * ( const Vector4& rV ) const;  // M * v
    float QForm( const Vector4& rU, const Vector4& rV ) const;  // u^T*M*v

	static Matrix4 Translate(float x, float y, float z);
	static Matrix4 Rotate(float angle, float x, float y, float z);

	static Matrix4 Perspective(float fovy, float aspect, float n, float f);
	static Matrix4 LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

private:
    // For indexing into the 1D array of the matrix, iCol+N*iRow
    static int Index( int nRow, int nCol );

    float m_afEntry[ 4 * 4 ];
};

#endif // _MATRIX4_HPP_
