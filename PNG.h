#ifndef __PNG_H__
#define __PNG_H__

class PNG
{
public:
	PNG();
	virtual ~PNG();

	bool Load( const char* fileName );

	unsigned int GetHeight() { return m_Height; }
	unsigned int GetWidth() { return m_Width; }

	bool GetHasAlpha() { return m_HasAlpha; }

	unsigned int GetSize() { return m_Size; }
	unsigned char* GetData() { return m_Data; }

private:
	unsigned int m_Height;
	unsigned int m_Width;
	bool m_HasAlpha;

	unsigned int m_Size;
	unsigned char* m_Data;
};

#endif /* __PNG_H__ */
