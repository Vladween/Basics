#pragma once
#ifndef NETWORK_BASICS_SERVER_H
#define NETWORK_BASICS_SERVER_H

#include "Network.h"

#define NO_CONNECTION_LIMIT 0

begin_basics_namespace(network)

std::ostream server_stream(std::cout.rdbuf());

class Server
{
public:
	bool isRunning() const
	{
		return m_isRunning;
	}
	void stop()
	{
		m_isRunning = false;
	}
	void SendPacket(sf::Packet& packet, size_t clientIndex)
	{
		m_connections[clientIndex]->send(packet);
	}
	void BroadcastPacket(sf::Packet& packet, size_t currentIndex)
	{
		for (size_t i = 0; i < m_connections.size(); i++)
		{
			if (i == currentIndex) continue;
				SendPacket(packet, i);
		}
	}
	void setMaxConnectionsCount(int count)
	{
		m_maxConCount = count;
	}
	size_t getMaxConnectionsCount() const
	{
		return m_maxConCount;
	}

	void run(unsigned short port, sf::IpAddress ip = sf::IpAddress::Any)
	{
		sf::Socket::Status status = m_listener.listen(port, ip);
		if (status == sf::Socket::Done)
		{
			m_isRunning = true;
			std::thread accept_thread(&Server::AcceptConnections, this);
			accept_thread.detach();

			ManagePackets();
		}
	}
	void start(unsigned short port, sf::IpAddress ip = sf::IpAddress::Any)
	{
		std::thread run_thread(&Server::run, this, port, ip);
		run_thread.detach();
	}

	virtual ~Server()
	{
		for (size_t i = 0; i < m_connections.size(); i++)
			DisconnectClient(i);
	}
protected:
	virtual void onReceive(sf::TcpSocket* socket, size_t index, sf::Packet& packet) = 0;
private:
	void DisconnectClient(size_t i)
	{
		server_stream << "Client [" << m_connections[i]->getRemoteAddress() << ":" << m_connections[i]->getRemotePort() << "] disconnected\n";
		m_connections[i]->disconnect();
		delete m_connections[i];
		m_connections.erase(m_connections.begin() + i);
	}

	void AcceptConnections()
	{
		while (m_isRunning)
		{
			sf::TcpSocket* new_con = new sf::TcpSocket();
			if (m_listener.accept(*new_con) == sf::Socket::Done)
			{
				if (m_connections.size() > m_maxConCount && m_maxConCount != NO_CONNECTION_LIMIT)
				{
					new_con->disconnect();
					server_stream << "Client tried to connect, but server is already full!\n";
					continue;
				}

				server_stream << "Accepted new client [" << new_con->getRemoteAddress() << ":" << new_con->getRemotePort() << "] in slot " << m_connections.size() << "\n";
				new_con->setBlocking(false);
				m_connections.push_back(new_con);
			}
			else
			{
				delete new_con;
				m_isRunning = false;
			}
		}
	}
	void ManagePackets()
	{
		while (m_isRunning)
		{
			for (size_t i = 0; i < m_connections.size(); i++)
			{
				sf::Packet packet;
				sf::Socket::Status status = m_connections[i]->receive(packet);
				if (status == sf::Socket::Done)
				{
					onReceive(m_connections[i], i, packet);
				}
				else if (status == sf::Socket::Disconnected)
				{
					DisconnectClient(i);
				}
			}
		}
	}

	sf::TcpListener m_listener;
	std::vector<sf::TcpSocket*> m_connections;
	bool m_isRunning = false;
	size_t m_maxConCount = NO_CONNECTION_LIMIT;
};
	
end_basics_namespace(network)

#endif