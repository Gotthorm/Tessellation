// HMD.CPP

#include <assert.h>
#include <memory.h>
#include <time.h>
#include <stdlib.h>
#include "HeightMap.hpp"
#include "Common\File.hpp"
#include "Common\Math.hpp"

const float HeightMap::VERSION = 2.3f;

HeightMap::HeightMap() : m_HeightMapData( NULL ), m_WidthX( 0 ), m_WidthZ( 0 )
{
}

HeightMap::~HeightMap()
{
}

bool HeightMap::Load( const char* fileName )
{
	assert( fileName );

	bool results = false;

	File inputFile;

	// If data is already loaded, destroy it now
	if( m_HeightMapData )
	{
		delete m_HeightMapData;
		m_HeightMapData = NULL;
	}

	if( inputFile.Open( fileName, File::OPEN_READ ) == true )
	{
		// 
		unsigned char tagBuffer[4];

		if( inputFile.Read( tagBuffer, sizeof( tagBuffer ) ) == sizeof( tagBuffer ) )
		{
			if( tagBuffer[ 0 ] == 'H' && tagBuffer[ 1 ] == 'M' &&tagBuffer[ 2 ] == 'A' &&tagBuffer[ 3 ] == 'P' )
			{
				results = ( inputFile.Read( &m_WidthX, sizeof( m_WidthX ) ) == sizeof( m_WidthX ) );

				if( results == true )
				{
					results = ( inputFile.Read( &m_WidthZ, sizeof( m_WidthZ ) ) == sizeof( m_WidthZ ) );
				}

				if( results == true )
				{
					results = ( m_WidthX > 0 && m_WidthZ > 0 );
				}

				if( results == true )
				{
					m_HeightMapData = new unsigned short[ m_WidthX * m_WidthZ ];
					results = ( inputFile.Read( m_HeightMapData, m_WidthX * m_WidthZ * sizeof( unsigned short ) ) == ( m_WidthX * m_WidthZ * sizeof( unsigned short ) ) );
				
					if( results == false )
					{
						delete m_HeightMapData;
						m_HeightMapData = NULL;
					}
				}
			}
		}
		
		inputFile.Close();
	}

	return results;
}

bool HeightMap::Save( const char* fileName )
{
	assert( fileName );

	bool result = false;

	File outputFile;

	// Make sure we have an image loaded
	if( m_HeightMapData != NULL )
	{
		if( outputFile.Open( fileName, File::OPEN_WRITE ) == true )
		{
			unsigned char tagBuffer[ 16 ] = "HMAP";

			result = ( outputFile.Write( tagBuffer, sizeof( unsigned char ) * 4 ) == sizeof( unsigned char ) * 4 );
			
			if( result == true )
			{
				result = ( outputFile.Write( &m_WidthX, sizeof( m_WidthX ) ) == sizeof( m_WidthX ) );
			}

			if( result == true )
			{
				result = ( outputFile.Write( &m_WidthZ, sizeof( m_WidthZ ) ) == sizeof( m_WidthZ ) );
			}

			result = ( outputFile.Write( m_HeightMapData, m_WidthX * m_WidthZ * sizeof( unsigned short ) ) == ( m_WidthX * m_WidthZ * sizeof( unsigned short ) ) );

			outputFile.Close();
		}
	}
	
	return result;
}

bool HeightMap::Create( unsigned short* data, int widthX, int widthZ )
{	
	if( data == NULL || widthX <= 0 || widthZ <= 0 )
	{
		return false;
	}

	// If data is already loaded, destroy it now
	if( m_HeightMapData != NULL )
	{
		delete m_HeightMapData;
		m_HeightMapData = NULL;
	}

	// Store the attributes
	m_WidthX = widthX;
	m_WidthZ = widthZ;
	
	m_HeightMapData = new unsigned short[ m_WidthX * m_WidthZ ];

	memcpy( m_HeightMapData, data, m_WidthX * m_WidthZ * sizeof( unsigned short ) );

	return true;
}

float RangedRandom(float v1,float v2)
{
	return v1 + (v2-v1)*((float)rand())/((float)RAND_MAX);
}


bool HeightMap::CreateFromRandom( int gridSize, float rough )
{
	// If an old height map exists, delete it
	if( m_HeightMapData )
	{
		delete m_HeightMapData;
		m_HeightMapData = NULL;
	}

	// Reseed the random generator
	srand( (unsigned int)time( NULL ) );

	// Create the temporary float version of the map
	float* pFloatMap = new float[ gridSize * gridSize ];

	int i,j,ni,nj,mi,mj,pmi,pmj,rectSize = gridSize;

	float dh = (float)rectSize/2,r = Math::Pow(2.0f, -1.0f * rough);

	/*
	Since the terrain wraps, all 4 "corners" are represented by the value at 0,0,
		so seeding the heightfield is very straightforward
	Note that it doesn't matter what we use for a seed value, since we're going to
		renormalize the terrain after we're done
	*/
	pFloatMap[0] = 0.0f;


	while(rectSize > 0)
	{
		
		/*
		Diamond step -

		Find the values at the center of the retangles by averaging the values at 
		the corners and adding a random offset:


		a.....b
		.     .  
		.  e  .
		.     .
		c.....d   

		e  = (a+b+c+d)/4 + random

		In the code below:
		a = (i,j)
		b = (ni,j)
		c = (i,nj)
		d = (ni,nj)
		e = (mi,mj)

		*/
		       
		for (i=0;i<gridSize;i+=rectSize)
		for (j=0;j<gridSize;j+=rectSize)
		{
			
			ni = (i+rectSize)%gridSize;
			nj = (j+rectSize)%gridSize;

			mi = (i+rectSize/2);
			mj = (j+rectSize/2);

			pFloatMap[mi+mj*gridSize] = (pFloatMap[i+j*gridSize] + pFloatMap[ni+j*gridSize] + pFloatMap[i+nj*gridSize] + pFloatMap[ni+nj*gridSize])/4 + RangedRandom(-dh/2,dh/2);
		}

		/*
		Square step -

		Find the values on the left and top sides of each rectangle
		The right and bottom sides are the left and top sides of the neighboring rectangles,
		  so we don't need to calculate them

		The height field wraps, so we're never left hanging.  The right side of the last
			rectangle in a row is the left side of the first rectangle in the row.  The bottom
			side of the last rectangle in a column is the top side of the first rectangle in
			the column

              .......
		      .     .
		      .     .
		      .  d  .
		      .     .
		      .     .
		......a..g..b
		.     .     .
		.     .     .
		.  e  h  f  .
		.     .     .
		.     .     .
		......c......

		g = (d+f+a+b)/4 + random
		h = (a+c+e+f)/4 + random
		
		In the code below:
			a = (i,j) 
			b = (ni,j) 
			c = (i,nj) 
			d = (mi,pmj) 
			e = (pmi,mj) 
			f = (mi,mj) 
			g = (mi,j)
			h = (i,mj)

		*/
		for (i=0;i<gridSize;i+=rectSize)
		for (j=0;j<gridSize;j+=rectSize)
		{

			ni = (i+rectSize)%gridSize;
			nj = (j+rectSize)%gridSize;

			mi = (i+rectSize/2);
			mj = (j+rectSize/2);

			pmi = (i-rectSize/2+gridSize)%gridSize;
			pmj = (j-rectSize/2+gridSize)%gridSize;

			
			/*
			Calculate the square value for the top side of the rectangle
			*/
			pFloatMap[mi+j*gridSize] = (pFloatMap[i+j*gridSize] + pFloatMap[ni+j*gridSize] + pFloatMap[mi+pmj*gridSize] + pFloatMap[mi+mj*gridSize])/4 + RangedRandom(-dh/2,dh/2);

			
			/*
			Calculate the square value for the left side of the rectangle
			*/
			pFloatMap[i+mj*gridSize] = (pFloatMap[i+j*gridSize] + pFloatMap[i+nj*gridSize] + pFloatMap[pmi+mj*gridSize] + pFloatMap[mi+mj*gridSize])/4 + RangedRandom(-dh/2,dh/2);

		}

		/*
		Setup values for next iteration
		At this point, the height field has valid values at each of the coordinates that fall on a rectSize/2 boundary
		*/
		rectSize /= 2;
		dh *= r;
	}
	/*
	Normalize terrain so minimum value is 0 and maximum value is 1
	*/
	NormalizeTerrain(pFloatMap, gridSize);

	// Now convert this float map into an integer map
	m_WidthX = m_WidthZ = gridSize;
	m_HeightMapData = new unsigned short[ gridSize * gridSize ];

	for(int Loop = 0; Loop < (gridSize * gridSize); Loop++)
	{
		assert(pFloatMap[Loop] >= 0.0 && pFloatMap[Loop] <= 1.0);
		m_HeightMapData[Loop] = (unsigned short int)(pFloatMap[Loop] * 0xFF);
	}

	delete pFloatMap;

	return true;
}

/*
Given a height field, normalize it so that the minimum altitude
is 0.0 and the maximum altitude is 1.0
*/
void HeightMap::NormalizeTerrain(float* field,int size)
{
	float maxVal,minVal,dh;
	int i;

	/*
	Find the maximum and minimum values in the height field
	*/ 
	maxVal = field[0];
	minVal = field[0];

	for (i=1;i<size*size;i++)
	{
		if (field[i] > maxVal) 
		{
			maxVal = field[i];
		}
		else if (field[i] < minVal) 
		{
			minVal = field[i];
		}
	}

	/*
	Find the altitude range (dh)
	*/
	if (maxVal <= minVal) return;
	dh = maxVal-minVal;

	/*
	Scale all the values so they are in the range 0-1
	*/
	for (i=0;i<size*size;i++)
	{
		field[i] = (field[i]-minVal)/dh;
	}

}
