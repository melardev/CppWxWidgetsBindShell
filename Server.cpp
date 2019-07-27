#include "Server.h"
#include "Application.h"
#include <wx/socket.h>
#include <wx/wxcrtvararg.h>


wxDECLARE_EVENT(ClientSocketId, wxSocketEvent);
wxDEFINE_EVENT(ClientSocketId, wxSocketEvent);

int ServerSocketId = wxNewId();

wxBEGIN_EVENT_TABLE(Server, wxEvtHandler)
		EVT_SOCKET(ServerSocketId, Server::OnServerSocketEvent)
		EVT_SOCKET(ClientSocketId, Server::OnSocketEvent)
wxEND_EVENT_TABLE()

Server::Server(Application* app) : m_ServerSocket(nullptr), receivingBuffer{0}, app(app)
{
}

void Server::Start()
{
	wxIPV4address address;
	address.Service(3002);

	m_ServerSocket = new wxSocketServer(address);
	if (m_ServerSocket->IsOk())
	{
		wxIPV4address addressBoundTo;
		m_ServerSocket->GetLocal(addressBoundTo);

		wxPrintf("Server listening on %s -> %s", addressBoundTo.Hostname(), addressBoundTo.IPAddress());

		m_ServerSocket->SetEventHandler(*this, ServerSocketId);
		m_ServerSocket->SetNotify(wxSOCKET_CONNECTION_FLAG);
		m_ServerSocket->Notify(true);
	}
}

void Server::OnServerSocketEvent(wxSocketEvent& event)
{
	switch (event.GetSocketEvent())
	{
	case wxSOCKET_CONNECTION:

		break;
	default:
		// wxString::Format("Unknown socket event: %d\n",event.GetSocketEvent());
		break;
	}

	// Only one client allowed
	if (m_Client != nullptr)
		return;

	// Accept new connection if there is one in the pending
	// connections queue, else exit. We use Accept(false) for
	// non-blocking accept (although if we got here, there
	// should ALWAYS be a pending connection).

	m_Client = m_ServerSocket->Accept(false);

	if (m_Client)
	{
		wxIPV4address addr;


		if (m_Client->GetPeer(addr))
		{
			wxPuts(wxString::Format("New connection from: %s\n", addr.IPAddress()));
		}

		m_Client->SetEventHandler(*this, ClientSocketId);

		// clientSocket->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
		// Since we use this class for both server clients, and Client project
		// add wxSOCKET_CONNECTION_FLAG
		m_Client->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
		m_Client->Notify(true);
		app->OnClientConnected();
	}
	else
	{
		wxLogMessage("Error: couldn't accept a new connection");
		return;
	}
}

void Server::OnSocketEvent(wxSocketEvent& event)
{
	wxString s = "OnSocketEvent: ";
	wxSocketBase* sock = event.GetSocket();

	// m_text->AppendText(s);

	// Now we process the event
	switch (event.GetSocketEvent())
	{
	case wxSOCKET_INPUT:
		{
			ReadFromSocket(sock);
			break;
		}
	case wxSOCKET_LOST:
		{
			if (m_Client != nullptr)
			{
				m_Client->Shutdown();
				m_Client->Close();
			}
			app->OnClientDisconnected();
			m_Client = nullptr;
			break;
		}
	default: ;
	}
}

void Server::ReadFromSocket(wxSocketBase* sock)
{
	// We disable input events, so that the test doesn't trigger
	// wxSocketEvent again.
	sock->SetNotify(wxSOCKET_LOST_FLAG);


	sock->Read(receivingBuffer, 1024);
	const wxUint32 readSoFar = sock->GetLastIOReadSize();
	app->OnMessageReceived(wxString(receivingBuffer, 0, readSoFar));
	sock->SetNotify(wxSOCKET_LOST_FLAG | wxSOCKET_INPUT_FLAG);
}

void Server::SendData(const wxString& data) const
{
	m_Client->Write(data.c_str(), data.size());
	const wxUint32 lastWrite = m_Client->LastWriteCount();
	assert(lastWrite == data.size());
}

void Server::FinishSync() const
{
	m_ServerSocket->Close();
}
