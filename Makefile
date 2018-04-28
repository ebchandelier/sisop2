client:
	g++ -std=c++11 -o dropboxClient.out ./src/client/dropboxClient.cpp ./src/client/ClientConnectionManager.cpp ./src/client/ClientConnectorUDP.cpp ./src/client/ClientUI.cpp ./src/shared/*

server:
	g++ -std=c++11 -o dropboxServer.out -pthread ./src/server/dropboxServer.cpp ./src/shared/* ./src/server/ServerConnectorUDP.cpp ./src/server/ClientHandler.cpp

all:
	make client && make server

clean:
	rm ./dropboxClient.out rm ./dropboxServer.out
