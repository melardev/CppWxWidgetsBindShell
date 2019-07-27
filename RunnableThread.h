#pragma once
#include <functional>
#include <wx/thread.h>

class RunnableThread : public wxThread
{
public:

	RunnableThread(std::function<void*()> func);

protected:
	virtual void* Entry() override;

private:
	std::function<void*()> m_Runnable;
};
