#pragma once
#include <wx/event.h>

class Application;
class wxSocketServer;
class wxSocketEvent;
class wxSocketBase;

class Server : public wxEvtHandler
{
public:
	Server(Application* app);

	void OnSocketEvent(wxSocketEvent& event);

	void ReadFromSocket(wxSocketBase* sock);

	void Start();
	void OnServerSocketEvent(wxSocketEvent& event);

	void SendData(const wxString& data) const;

	void FinishSync() const;

private:
	wxSocketServer* m_ServerSocket;
	char receivingBuffer[1024];
	Application* app;
	wxSocketBase* m_Client;
wxDECLARE_EVENT_TABLE();
};
