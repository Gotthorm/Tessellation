// HEIGHTMAP.H

#ifndef _HEIGHTMAP_H_
#define _HEIGHTMAP_H_

class HeightMap
{
public:
	HeightMap();
	virtual ~HeightMap();

	enum Format
	{
		RGB,
		RGBA,
	};

	bool Import( const unsigned char* data, unsigned int width, unsigned int height, unsigned short int heightScale, Format format );
	bool Load( const char* fileName );
	bool Save( const char* fileName );

	bool CreateFromRandom( int size, float rough, unsigned short int heightScale );

	unsigned int GetWidth() { return m_Width; }
	unsigned int GetHeight() { return m_Height; }
	const unsigned short* GetData() { return m_Data; }

private:
	void NormalizeTerrain(float* field,int size);
	unsigned short* m_Data;
	unsigned int m_Width;
	unsigned int m_Height;
};

#endif // _HEIGHTMAP_H_
