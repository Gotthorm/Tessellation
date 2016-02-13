// HMD.CPP

#include <assert.h>
#include <memory.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "HeightMap.h"

HeightMap::HeightMap() : m_Data( NULL ), m_Width( 0 ), m_Height( 0 )
{
}

HeightMap::~HeightMap()
{
}

bool HeightMap::Load( const char* fileName )
{
	assert( fileName );

	bool results = false;

	//File inputFile;

	// If data is already loaded, destroy it now
	if( m_Data )
	{
		delete m_Data;
		m_Data = NULL;
	}

	//if( inputFile.Open( fileName, File::OPEN_READ ) == true )
	//{
	//	// 
	//	unsigned char tagBuffer[4];

	//	if( inputFile.Read( tagBuffer, sizeof( tagBuffer ) ) == sizeof( tagBuffer ) )
	//	{
	//		if( tagBuffer[ 0 ] == 'H' && tagBuffer[ 1 ] == 'M' &&tagBuffer[ 2 ] == 'A' &&tagBuffer[ 3 ] == 'P' )
	//		{
	//			results = ( inputFile.Read( &m_Width, sizeof( m_Width ) ) == sizeof( m_Width ) );

	//			if( results == true )
	//			{
	//				results = ( inputFile.Read( &m_Height, sizeof( m_Height ) ) == sizeof( m_Height ) );
	//			}

	//			if( results == true )
	//			{
	//				results = ( m_Width > 0 && m_Height > 0 );
	//			}

	//			if( results == true )
	//			{
	//				m_Data = new unsigned short[ m_Width * m_Height ];
	//				results = ( inputFile.Read( m_Data, m_Width * m_Height * sizeof( unsigned short ) ) == ( m_Width * m_Height * sizeof( unsigned short ) ) );
	//			
	//				if( results == false )
	//				{
	//					delete m_Data;
	//					m_Data = NULL;
	//				}
	//			}
	//		}
	//	}
	//	
	//	inputFile.Close();
	//}

	return results;
}

bool HeightMap::Save( const char* fileName )
{
	assert( fileName );

	bool result = false;

	//File outputFile;

	// Make sure we have an image loaded
	if( m_Data != NULL )
	{
		//if( outputFile.Open( fileName, File::OPEN_WRITE ) == true )
		//{
		//	unsigned char tagBuffer[ 16 ] = "HMAP";

		//	result = ( outputFile.Write( tagBuffer, sizeof( unsigned char ) * 4 ) == sizeof( unsigned char ) * 4 );
		//	
		//	if( result == true )
		//	{
		//		result = ( outputFile.Write( &m_Width, sizeof( m_Width ) ) == sizeof( m_Width ) );
		//	}

		//	if( result == true )
		//	{
		//		result = ( outputFile.Write( &m_Height, sizeof( m_Height ) ) == sizeof( m_Height ) );
		//	}

		//	result = ( outputFile.Write( m_Data, m_Width * m_Height * sizeof( unsigned short ) ) == ( m_Width * m_Height * sizeof( unsigned short ) ) );

		//	outputFile.Close();
		//}
	}
	
	return result;
}

bool HeightMap::Import( const unsigned char* importData, unsigned int width, unsigned int height, unsigned short int heightScale, Format format )
{	
	if( importData != NULL )
	{
		// If data is already loaded, destroy it now
		if( m_Data != NULL )
		{
			delete [] m_Data;
		}

		// Store the attributes
		m_Width = width;
		m_Height = height;

		unsigned int totalEntries = m_Width * m_Height;

		m_Data = new unsigned short[ totalEntries ];

		//unsigned char* importPosition = importData;
		unsigned char incrementStep = ( format == RGB ) ? 3 : 4;

		for( unsigned int index = 0; index < totalEntries; ++index )
		{
			float redValue = (float)*importData;
			float normalizedValue = redValue / UCHAR_MAX;
			m_Data[ index ] = ( unsigned short )( normalizedValue * heightScale );
			importData += incrementStep;
		}

		return true;
	}

	return false;
}

float RangedRandom(float v1,float v2)
{
	return v1 + (v2-v1)*((float)rand())/((float)RAND_MAX);
}


bool HeightMap::CreateFromRandom( int gridSize, float rough, unsigned short int heightScale )
{
	// If an old height map exists, delete it
	if( m_Data )
	{
		delete m_Data;
		m_Data = NULL;
	}

	// Reseed the random generator
	srand( (unsigned int)time( NULL ) );

	// Create the temporary float version of the map
	float* floatMap = new float[ gridSize * gridSize ];

	int i,j,ni,nj,mi,mj,pmi,pmj,rectSize = gridSize;

	float dh = rectSize / 2.0f;
	float r = (float)pow(2.0f, -1.0f * rough);

	/*
	Since the terrain wraps, all 4 "corners" are represented by the value at 0,0,
		so seeding the heightfield is very straightforward
	Note that it doesn't matter what we use for a seed value, since we're going to
		renormalize the terrain after we're done
	*/
	floatMap[0] = 0.0f;


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

			floatMap[mi+mj*gridSize] = (floatMap[i+j*gridSize] + floatMap[ni+j*gridSize] + floatMap[i+nj*gridSize] + floatMap[ni+nj*gridSize])/4 + RangedRandom(-dh/2,dh/2);
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
			floatMap[mi+j*gridSize] = (floatMap[i+j*gridSize] + floatMap[ni+j*gridSize] + floatMap[mi+pmj*gridSize] + floatMap[mi+mj*gridSize])/4 + RangedRandom(-dh/2,dh/2);

			
			/*
			Calculate the square value for the left side of the rectangle
			*/
			floatMap[i+mj*gridSize] = (floatMap[i+j*gridSize] + floatMap[i+nj*gridSize] + floatMap[pmi+mj*gridSize] + floatMap[mi+mj*gridSize])/4 + RangedRandom(-dh/2,dh/2);

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
	NormalizeTerrain(floatMap, gridSize);

	// Now convert this float map into an integer map
	m_Width = m_Height = gridSize;
	m_Data = new unsigned short[ gridSize * gridSize ];

	for(int Loop = 0; Loop < (gridSize * gridSize); Loop++)
	{
		assert(floatMap[Loop] >= 0.0 && floatMap[Loop] <= 1.0);
		m_Data[Loop] = (unsigned short int)(floatMap[Loop] * heightScale);
	}

	delete floatMap;

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
