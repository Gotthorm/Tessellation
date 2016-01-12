// FILE.CPP

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "File.hpp"

static char * FILE_MODES[] = { "rb", "wb", "rt", "wt" };

File::File() : m_nFileDescriptor(FILE_NONE), m_pName(NULL)
{
}

File::~File()
{
	if(m_pName) 
		delete m_pName;
}
	
bool File::Open(const char *pFilename, OPEN_TYPE nMode)
{
	assert(pFilename);

	// Check that there isn't a file open already
	if(IsOpen()) 
	{
		// File is already open
		assert(m_pName);
		printf("Open %s request failed,\n %s already open\n", pFilename, m_pName);

		return false;
	}

	// Prepend the host info
	assert(!m_pName);
	m_pName = new char[strlen(pFilename) + 1];
	assert(m_pName);
	sprintf_s(m_pName, strlen(pFilename) + 1, "%s", pFilename);

  // Check if the file failed to open
	if( fopen_s( &m_nFileDescriptor, const_cast<char*>(m_pName), FILE_MODES[nMode]) != 0 )
	{
		// Failure
		
		assert(m_pName);
		printf("Cannot open %s\n", m_pName);
		delete m_pName;
		m_pName = NULL;

		// Indicate failure
		return false;
	}

	// File is open and pointer is at the start of the file
	// Indicate success    
  return true;
}

bool File::Close()
{
	bool ReturnValue = true;
	
	if(m_nFileDescriptor == FILE_NONE) {
		// Warning message
		printf("Close called with no file being open\n");
	}

	// Check that there is an open file
	if(IsOpen()) {
	
		// Close the file and check the results
		if(fclose(m_nFileDescriptor) != 0) 
		{
			// Debug dump of error message
			assert(m_pName);
			printf("Error when closing %s\n", m_pName);

			// There was an error
			ReturnValue = false;
		}

		// Reset the file descriptor		
		m_nFileDescriptor = FILE_NONE;
	}
	
	if(m_pName) 
	{
		delete m_pName;
		m_pName = NULL;
	}

	return ReturnValue;
}
	
bool File::Seek(int nOffset, SEEK_TYPE nOrigin)
{
	assert(nOrigin == FROM_BEGIN || nOrigin == FROM_CURRENT || nOrigin == FROM_END );

	// Check that the file is open
	if(m_nFileDescriptor == FILE_NONE) 
	{
		// There is no file open
	    printf("Seek failed, no file open\n");
	    return false;
	}

	if(fseek(m_nFileDescriptor, nOffset, nOrigin) != 0)
	{
		// Printf error message
		assert(m_pName);
		printf("Seek failed on %s\n", m_pName);
		return false;
	}
  
	return true;
}


unsigned int File::Read(void *pBuffer, unsigned int nSize)
{
	size_t nBytesRead;
	
	// Check that the file is open
	if(m_nFileDescriptor == FILE_NONE) 
	{
		// Debug dump of error message
	    printf("Read failed, no file open\n");
	    return 0;
	}

	assert(pBuffer);
	assert(static_cast<int>(nSize) >= 0);
	
	// Read the data into the given buffer
	nBytesRead = fread(pBuffer, 1, static_cast<int>(nSize), m_nFileDescriptor);

	// Check the reults of the read
	if(nBytesRead < 0) 
	{
		// Read failed
		assert(m_pName);
		printf("Read failed on %s\n", m_pName);
		return 0;
	}
	
	return static_cast<unsigned int>(nBytesRead);
}

unsigned int File::Write(void *pBuffer, unsigned int nSize)
{
	size_t nBytesWritten;
	
	// Check that the file is open
	if(m_nFileDescriptor == FILE_NONE) 
	{
		// No file open
	    printf("Write failed, no file open\n");
	    return 0;
	}
	
	assert(pBuffer);
	
	nBytesWritten = fwrite(pBuffer, 1, static_cast<int>(nSize), m_nFileDescriptor);

	if(nBytesWritten < 0) 
	{
		// Write failed
		assert(m_pName);
		printf("Write failed on %s\n", m_pName);
		return 0;
	}
	
	return static_cast<unsigned int>(nBytesWritten);
}

void File::Flush()
{
	// Unsupported in this base class
	printf("File::Flush() which does nothing in this base class\n");
}

int File::Tell()
{
	// Check that the file is open
	if(m_nFileDescriptor == FILE_NONE) 
	{
		// Debug dump of error message
		printf("Tell failed, no file open\n");
		return -1;
	}

	return ftell(m_nFileDescriptor);
}

unsigned int File::GetSize()
{
	int nSize;

	// Check that the file is open
	if(m_nFileDescriptor == FILE_NONE) 
	{
		// No file open
	    printf("GetSize failed, no file open\n");
	    return 0;
	}

	// Store the current position of the file pointer
	long nCurrentPosition = ftell(m_nFileDescriptor);

	// Calculate the size of the file
	nSize = fseek(m_nFileDescriptor, 0, FROM_END);
	assert(nSize == 0);
	nSize = ftell(m_nFileDescriptor);

	// Check if the seek failed and also attempt to rewind file
	if(nSize == -1 || fseek(m_nFileDescriptor, nCurrentPosition, FROM_BEGIN) != 0)
	{
		// There was an error seeking
		assert(m_pName);
		printf("Seek failed on %s\n", m_pName);
		return 0;
	}

	return static_cast<unsigned int>(nSize);
}   



