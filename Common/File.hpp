// FILE.HPP

#ifndef _FILE_HPP_
#define _FILE_HPP_

#include <stdio.h>

#define FILE_NONE		NULL

class File
{
public:
	// Construction
	File();
	virtual ~File();

	// Constants
	enum SEEK_TYPE 
	{ 
		FROM_BEGIN 		= SEEK_SET, 
		FROM_CURRENT	= SEEK_CUR,
		FROM_END		= SEEK_END
	};

	// Constants
	enum OPEN_TYPE 
	{ 
		OPEN_READ		= 0, 
		OPEN_WRITE		= 1,
		OPEN_READ_TEXT	= 2, 
		OPEN_WRITE_TEXT	= 3,
	};

	// Methods
	bool Open(const char *pFileName, OPEN_TYPE nMode);
	bool Close();
	bool Seek(int nOffset, SEEK_TYPE nOrigin);
	unsigned int Read(void *pBuffer, unsigned int nSize);
	unsigned int Write(void *pBuffer, unsigned int nSize);
	unsigned int GetSize();
	int Tell();
	void Flush();
	inline bool IsOpen() { return m_nFileDescriptor != FILE_NONE; }

private:
	
	FILE* m_nFileDescriptor;
	char* m_pName;
};

#endif // _FILE_HPP_
