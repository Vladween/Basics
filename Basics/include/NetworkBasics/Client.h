#pragma once
#ifndef NETWORK_BASICS_CLIENT_H
#define NETWORK_BASICS_CLIENT_H

#include "AppBasics\AppUpdatables.h"
#include "Network.h"

begin_basics_namespace(network)

class Client : public Updatable
{
public:
	sf::Socket::Status connect(sf::IpAddress ip, unsigned short port)
	{
		sf::Socket::Status status = m_socket.connect(ip, port);
		if (status == sf::Socket::Done)
		{
			m_socket.setBlocking(false);
			m_isConnected = true;
			std::thread receive_thread(&Client::ReceiveData, this);
			receive_thread.detach();
			onConnect();
		}
		return status;
	}
	void disconnect()
	{
		onDisconnect();
		m_isConnected = false;
		m_socket.disconnect();
	}
	bool isConnected() const
	{
		return m_isConnected;
	}

	sf::Socket::Status receive(sf::Packet& packet)
	{
		return m_socket.receive(packet);
	}
	sf::Socket::Status send(sf::Packet& packet)
	{
		return m_socket.send(packet);
	}

	virtual void update(AppInfo& app) = 0;
protected:
	virtual void onConnect()
	{
	}
	virtual void onReceive(sf::Packet& packet) = 0;
	virtual void onDisconnect()
	{
	}
private:
	sf::TcpSocket m_socket;
	bool m_isConnected = false;

	void ReceiveData()
	{
		while (m_isConnected)
		{
			sf::Packet packet;
			sf::Socket::Status status = m_socket.receive(packet);

			if (status == sf::Socket::Done)
			{
				onReceive(packet);
			}
			if (status == sf::Socket::Disconnected)
			{
				disconnect();
			}
		}
	}
};

end_basics_namespace(network)

#endif