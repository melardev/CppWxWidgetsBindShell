#pragma once
#include "Server.h"
#include "ProcessHandler.h"


class Application
{
public:
	Application() = default;
	~Application() = default;

	void Run();
	void OnClientConnected() const;
	void OnMessageReceived(const wxString& command);
	void OnProcOutput(const wxString& processOutput);
	void OnClientDisconnected();
	Server* m_Server;
	ProcessHandler* m_ProcHandler;
};



