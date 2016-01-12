// MATH.CPP

#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <assert.h>
#include "Math.hpp"

float Math::ACos( float fValue )
{
    if( -1.0f < fValue )
    {
        if( fValue < 1.0f )
		{
            return (float)acos( (double)fValue );
		}
        else
		{
            return 0.0f;
		}
    }
    else
    {
        return PI;
    }
}

float Math::ASin( float fValue )
{
    if ( -1.0f < fValue )
    {
        if( fValue < 1.0f )
		{
            return (float)asin( (double)fValue );
		}
        else
		{
            return -HALF_PI;
		}
    }
    else
    {
        return HALF_PI;
    }
}

float Math::ATan( float fValue )
{
    return (float)atan( (double)fValue );
}

float Math::ATan2( float fY, float fX )
{
    return (float)atan2( (double)fY, (double)fX );
}

float Math::Ceil( float fValue )
{
    return (float)ceil( (double)fValue );
}

float Math::Cos( float fValue )
{
    return (float)cos( (double)fValue );
}

float Math::Exp( float fValue )
{
    return (float)exp( (double)fValue );
}

float Math::FAbs( float fValue )
{
    return (float)fabs( (double)fValue );
}

float Math::Floor( float fValue )
{
    return (float)floor( (double)fValue );
}

float Math::FMod( float fX, float fY )
{
    return (float)fmod( (double)fX, (double)fY );
}

float Math::InvSqrt( float fValue )
{
    return ( 1.0f / (float)sqrt( (double)fValue ) );
}

float Math::Log( float fValue )
{
    return (float)log( (double)fValue );
}

float Math::Pow( float fBase, float fExponent )
{
    return (float)pow( (double)fBase, (double)fExponent );
}

float Math::Sin( float fValue )
{
    return (float)sin( (double)fValue );
}

float Math::Sqr( float fValue )
{
    return fValue * fValue;
}

float Math::Sqrt( float fValue )
{
	static float Sean = 0.0f;

//	if( fValue != 0.0f && (fValue - 0.00000010609966f)  < 0.000000001f )
	{
		Sean = fValue;
//		return 0.0f;
	}

    return (float)sqrt( (double)fValue );
}

float Math::Tan( float fValue )
{
    return (float)tan( (double)fValue );
}

float Math::Max( float fValue1, float fValue2 )
{
	return ( fValue1 >= fValue2 ) ? fValue1 : fValue2;
}

float Math::Min( float fValue1, float fValue2 )
{
	return ( fValue1 <= fValue2 ) ? fValue1 : fValue2;
}

int Math::Sign( int nValue )
{
    if( nValue > 0 )
	{
        return 1;
	}

    if( nValue < 0 )
	{
        return -1;
	}

    return 0;
}

float Math::Sign( float fValue )
{
    if( fValue > 0.0f )
	{
        return 1.0f;
	}

    if( fValue < 0.0f )
	{
        return -1.0f;
	}

    return 0.0f;
}

float Math::UnitRandom( float fSeed )
{
    if( fSeed > 0.0f )
	{
        srand( (unsigned int)fSeed );
	}

    return ( (float)rand() ) / ( (float)RAND_MAX );
}

float Math::SymmetricRandom( float fSeed )
{
    if( fSeed > 0.0f )
	{
        srand( (unsigned int)fSeed );
	}

    float fRatio = ( (float)rand() ) / ( (float)RAND_MAX );

    return ( 2.0f * fRatio - 1.0f );
}

float Math::IntervalRandom( float fMin, float fMax, float fSeed )
{
    if( fSeed > 0.0f )
	{
        srand( (unsigned int)fSeed );
	}

    float fRatio = ( (float)rand() ) / ( (float)RAND_MAX );

    return fMin + ( fMax - fMin ) * fRatio;
}

float Math::FastSin0( float fAngle )
{
    float fASqr = fAngle * fAngle;
    float fResult = 7.61e-03f;
    fResult *= fASqr;
    fResult -= 1.6605e-01f;
    fResult *= fASqr;
    fResult += 1.0f;
    fResult *= fAngle;
    return fResult;
}

float Math::FastSin1( float fAngle )
{
    float fASqr = fAngle * fAngle;
    float fResult = -2.39e-08f;
    fResult *= fASqr;
    fResult += 2.7526e-06f;
    fResult *= fASqr;
    fResult -= 1.98409e-04f;
    fResult *= fASqr;
    fResult += 8.3333315e-03f;
    fResult *= fASqr;
    fResult -= 1.666666664e-01f;
    fResult *= fASqr;
    fResult += 1.0f;
    fResult *= fAngle;
    return fResult;
}

float Math::FastCos0( float fAngle )
{
    float fASqr = fAngle * fAngle;
    float fResult = 3.705e-02f;
    fResult *= fASqr;
    fResult -= 4.967e-01f;
    fResult *= fASqr;
    fResult += 1.0f;
    return fResult;
}

float Math::FastCos1( float fAngle )
{
    float fASqr = fAngle * fAngle;
    float fResult = -2.605e-07f;
    fResult *= fASqr;
    fResult += 2.47609e-05f;
    fResult *= fASqr;
    fResult -= 1.3888397e-03f;
    fResult *= fASqr;
    fResult += 4.16666418e-02f;
    fResult *= fASqr;
    fResult -= 4.999999963e-01f;
    fResult *= fASqr;
    fResult += 1.0f;
    return fResult;
}

float Math::FastTan0( float fAngle )
{
    float fASqr = fAngle * fAngle;
    float fResult = 2.033e-01f;
    fResult *= fASqr;
    fResult += 3.1755e-01f;
    fResult *= fASqr;
    fResult += 1.0f;
    fResult *= fAngle;
    return fResult;
}

float Math::FastTan1( float fAngle )
{
    float fASqr = fAngle * fAngle;
    float fResult = 9.5168091e-03f;
    fResult *= fASqr;
    fResult += 2.900525e-03f;
    fResult *= fASqr;
    fResult += 2.45650893e-02f;
    fResult *= fASqr;
    fResult += 5.33740603e-02f;
    fResult *= fASqr;
    fResult += 1.333923995e-01f;
    fResult *= fASqr;
    fResult += 3.333314036e-01f;
    fResult *= fASqr;
    fResult += 1.0f;
    fResult *= fAngle;
    return fResult;
}

float Math::FastInvSin( float fValue )
{
    float fRoot = Math::Sqrt( 1.0f - fValue );
    float fResult = -0.0187293f;
    fResult *= fValue;
    fResult += 0.0742610f;
    fResult *= fValue;
    fResult -= 0.2121144f;
    fResult *= fValue;
    fResult += 1.5707288f;
    fResult = HALF_PI - fRoot * fResult;
    return fResult;
}

float Math::FastInvCos( float fValue )
{
    float fRoot = Math::Sqrt( 1.0f - fValue );
    float fResult = -0.0187293f;
    fResult *= fValue;
    fResult += 0.0742610f;
    fResult *= fValue;
    fResult -= 0.2121144f;
    fResult *= fValue;
    fResult += 1.5707288f;
    fResult *= fRoot;
    return fResult;
}

float Math::FastInvTan0( float fValue )
{
    float fVSqr = fValue * fValue;
    float fResult = 0.0208351f;
    fResult *= fVSqr;
    fResult -= 0.085133f;
    fResult *= fVSqr;
    fResult += 0.180141f;
    fResult *= fVSqr;
    fResult -= 0.3302995f;
    fResult *= fVSqr;
    fResult += 0.999866f;
    fResult *= fValue;
    return fResult;
}

float Math::FastInvTan1( float fValue )
{
    float fVSqr = fValue * fValue;
    float fResult = 0.0028662257f;
    fResult *= fVSqr;
    fResult -= 0.0161657367f;
    fResult *= fVSqr;
    fResult += 0.0429096138f;
    fResult *= fVSqr;
    fResult -= 0.0752896400f;
    fResult *= fVSqr;
    fResult += 0.1065626393f;
    fResult *= fVSqr;
    fResult -= 0.1420889944f;
    fResult *= fVSqr;
    fResult += 0.1999355085f;
    fResult *= fVSqr;
    fResult -= 0.3333314528f;
    fResult *= fVSqr;
    fResult += 1.0f;
    fResult *= fValue;
    return fResult;
}

float Math::FastInvSqrt( float fValue )
{
    float fFValue = fValue;
    float fHalf = 0.5f * fFValue;
    int i  = *(int*)&fFValue;
    i = 0x5F3759DF - ( i >> 1 );
    fFValue = *(float*)&i;
    fFValue = fFValue * ( 1.5f - fHalf * fFValue * fFValue );
    return fFValue;
}

float Math::LogGamma( float fX )
{
    const float afCoeff[ 6 ] =
    {
         76.18009173f,
        -86.50532033f,
         24.01409822f,
        -1.231739516f,
         0.120858003e-02f,
        -0.536382000e-05f
    };

    fX -= 1.0f;
    float fTmp = fX + 5.5f;
    fTmp -= ( fX + ( 0.5f ) ) * Math::Log( fTmp );
    float fSeries = 1.0f;

    for( int j = 0; j <= 5; j++ )
    {
        fX += 1.0f;
        fSeries += afCoeff[ j ] / fX;
    }

    return -fTmp + Math::Log( 2.50662827465f * fSeries );
}

float Math::Gamma( float fX )
{
    return Math::Exp( LogGamma( fX ) );
}

float Math::IncompleteGammaS( float fA, float fX )
{
    const int iMaxIterations = 100;
    const float fTolerance = 3e-07f;

    if( fX > 0.0f )
    {
        float fAp = fA;
        float fSum = 1.0f / fA;
		float fDel = fSum;

        for( int i = 1; i <= iMaxIterations; i++ )
        {
            fAp += 1.0f;
            fDel *= fX / fAp;
            fSum += fDel;

            if( Math::FAbs( fDel ) < Math::FAbs( fSum ) * fTolerance )
            {
                float fArg = -fX + fA * Math::Log( fX ) - LogGamma( fA );
                return fSum * Math::Exp( fArg );
            }
        }
    }

    if( fX == 0.0f )
	{
        return 0.0f;
	}

    return Math::MAX_FLOAT; // LogGamma not defined for x < 0
}

float Math::IncompleteGammaCF( float fA, float fX )
{
    const int iMaxIterations = 100;
    const float fTolerance = 3e-07f;

    float fA0 = 1.0f;
	float fA1 = fX;
    float fB0 = 0.0f;
	float fB1 = 1.0f;
    float fGold = 0.0f;
	float fFac = 1.0f;

    for( int i = 1; i <= iMaxIterations; i++ )
    {
        float fI = (float)i;
        float fImA = fI - fA;
        fA0 = (fA1 + fA0 * fImA) * fFac;
        fB0 = (fB1 + fB0 * fImA) * fFac;
        float fItF = fI * fFac;
        fA1 = fX * fA0 + fItF * fA1;
        fB1 = fX * fB0 + fItF * fB1;
        if( fA1 != 0.0f )
        {
            fFac = 1.0f / fA1;
            float fG = fB1 * fFac;

            if( Math::FAbs( ( fG - fGold ) / fG ) < fTolerance )
            {
                float fArg = -fX + fA * Math::Log( fX ) - LogGamma( fA );
                return fG * Math::Exp( fArg );
            }

            fGold = fG;
        }
    }

	// Numerical error if you get here
    return Math::MAX_FLOAT;
}

float Math::IncompleteGamma( float fA, float fX )
{
    if( fX < 1.0f + fA )
	{
        return IncompleteGammaS( fA, fX );
	}
    else
	{
        return 1.0f - IncompleteGammaCF( fA, fX );
	}
}

float Math::Erf( float fX )
{
    return 1.0f - Erfc( fX );
}

float Math::Erfc( float fX )
{
    const float afCoeff[ 10 ] =
    {
        -1.26551223f,
         1.00002368f,
         0.37409196f,
         0.09678418f,
        -0.18628806f,
         0.27886807f,
        -1.13520398f,
         1.48851587f,
        -0.82215223f,
         0.17087277f
    };

    float fZ = Math::FAbs( fX );
    float fT = 1.0f / ( 1.0f + 0.5f * fZ );
    float fSum = afCoeff[ 9 ];

    for( int i = 9; i >= 0; i-- )
	{
        fSum = fT * fSum + afCoeff[ i ];
	}

    float fResult = fT * Math::Exp( -fZ * fZ + fSum );

    return ( fX >= 0.0f ? fResult : 2.0f - fResult );
}

float Math::ModBessel0( float fX )
{
    if( fX < 0.0f )  // function is even
	{
        fX = -fX;
	}

    float fT, fResult;
    int i;

    if( fX <= 3.75f )
    {
        const float afCoeff[ 7 ] =
        {
            1.0000000f,
            3.5156229f,
            3.0899424f,
            1.2067492f,
            0.2659732f,
            0.0360768f,
            0.0045813f
        };

        fT = fX / 3.75f;
        float fT2 = fT * fT;
        fResult = afCoeff[ 6 ];

        for( i = 5; i >= 0; i-- )
        {
            fResult *= fT2;
            fResult += afCoeff[ i ];
        }
        // |error| < 1.6e-07
    }
    else
    {
        const float afCoeff[ 9 ] =
        {
             0.39894228f,
             0.01328592f,
             0.00225319f,
            -0.00157565f,
             0.00916281f,
            -0.02057706f,
             0.02635537f,
            -0.01647633f,
             0.00392377f
        };

        fT = fX / 3.75f;
        float fInvT = 1.0f / fT;
        fResult = afCoeff[ 8 ];

        for( i = 7; i >= 0; i-- )
        {
            fResult *= fInvT;
            fResult += afCoeff[ i ];
        }

        fResult *= Math::Exp( fX );
        fResult /= Math::Sqrt( fX );
        // |error| < 1.9e-07
    }

    return fResult;
}

float Math::ModBessel1( float fX )
{
    int iSign;
    if( fX > 0.0f )
    {
        iSign = 1;
    }
    else if( fX < 0.0f )
    {
        fX = -fX;
        iSign = -1;
    }
    else
    {
        return 0.0f;
    }

    float fT, fResult;
    int i;

    if( fX <= 3.75f )
    {
        const float afCoeff[ 7 ] =
        {
            0.50000000f,
            0.87890549f,
            0.51498869f,
            0.15084934f,
            0.02658733f,
            0.00301532f,
            0.00032411f
        };

        fT = fX / 3.75f;
        float fT2 = fT * fT;
        fResult = afCoeff[ 6 ];

        for( i = 5; i >= 0; i-- )
        {
            fResult *= fT2;
            fResult += afCoeff[ i ];
        }

        fResult *= fX;
        // |error| < 8e-09
    }
    else
    {
        const float afCoeff[ 9 ] =
        {
             0.39894228f,
            -0.03988024f,
            -0.00362018f,
             0.00163801f,
            -0.01031555f,
             0.02282967f,
            -0.02895312f,
             0.01787654f,
            -0.00420059f
        };

        fT = fX / 3.75f;
        float fInvT = 1.0f / fT;
        fResult = afCoeff[ 8 ];

        for( i = 7; i >= 0; i-- )
        {
            fResult *= fInvT;
            fResult += afCoeff[ i ];
        }

        fResult *= Math::Exp( fX );
        fResult /= Math::Sqrt( fX );
        // |error| < 2.2e-07
    }

    fResult *= iSign;
    return fResult;
}

int Math::CompareArrays( int nCount, const float* afObject1, const float* afObject2 )
{
    for( int i = 0; i < nCount; i++ )
    {
        unsigned int uiTest0 = *(unsigned int*)&afObject1[ i ];
        unsigned int uiTest1 = *(unsigned int*)&afObject2[ i ];

        if( uiTest0 < uiTest1 )
		{
            return -1;
		}
        if( uiTest0 > uiTest1 )
		{
            return 1;
		}
    }

    return 0;
}

void Math::DecreaseTowardsZero( float& rTargetValue, float fAmount )
{
	assert( fAmount >= 0.0f );

	if( rTargetValue < 0.0f )
	{
		rTargetValue += fAmount;

		if( rTargetValue > 0.0f )
		{
			rTargetValue = 0.0f;
		}
	}
	else if( rTargetValue > 0.0f )
	{
		rTargetValue -= fAmount;

		if( rTargetValue < 0.0f )
		{
			rTargetValue = 0.0f;
		}
	}
}

const float Math::EPSILON = FLT_EPSILON;
const float Math::MAX_FLOAT = FLT_MAX;
const float Math::PI = 4.0f * (float)atan( 1.0 );
const float Math::TWO_PI = 2.0f * Math::PI;
const float Math::HALF_PI = 0.5f * Math::PI;
const float Math::INV_PI = 1.0f / Math::PI;
const float Math::INV_TWO_PI = 1.0f / Math::TWO_PI;
const float Math::DEG_TO_RAD = Math::PI / 180.0f;
const float Math::RAD_TO_DEG = 180.0f / Math::PI;
