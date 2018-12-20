#pragma once

#define QUEUE_BUFFER_SIZE	8192

class CQueue
{
public:
	CQueue(void);
	~CQueue(void);

	char* m_bufQueue[QUEUE_BUFFER_SIZE];
	int m_nStart, m_nEnd;

	void ClearQueue();

	void Put(char *buf, int nSize);
	BOOL Get(char *buf, int nSize);
	int GetSize();
};

