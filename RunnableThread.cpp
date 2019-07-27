#include "RunnableThread.h"

RunnableThread::RunnableThread(std::function<void*()> func): wxThread(wxTHREAD_JOINABLE), m_Runnable(func)
{
	m_Runnable = func;
}

void* RunnableThread::Entry()
{
	return m_Runnable();
}
