// HEIGHTMAP.HPP

#ifndef _HEIGHTMAP_HPP_
#define _HEIGHTMAP_HPP_

/** @brief A HeightMap data class.
  * This class wraps height map data and looks after loading and saving.
  * It also performs version checking.
  * @author Sean Butterworth
  * @version 2.3
  * @date Apr 2009
**/

/**
	* @todo 
	* Document the public methods
**/

class HeightMap
{
public:
	static const float VERSION;

	HeightMap();
	virtual ~HeightMap();

	bool Create( unsigned short* data, int widthX, int widthZ );
	bool Load( const char* fileName );
	bool Save( const char* fileName );

	bool CreateFromRandom( int size, float rough );

	unsigned int GetWidthX() { return m_WidthX; }
	unsigned int GetWidthZ() { return m_WidthZ; }
	const unsigned short* GetData() { return m_HeightMapData; }

private:
	void NormalizeTerrain(float* field,int size);
	unsigned short* m_HeightMapData;
	unsigned int m_WidthX;
	unsigned int m_WidthZ;
};

#endif // _HEIGHTMAP_HPP_
