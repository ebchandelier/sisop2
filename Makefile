client:
	g++ -std=c++11 -o dropboxClient.out ./src/client/dropboxClient.cpp ./src/client/ClientConnectionManager.cpp ./src/client/ClientConnectorUDP.cpp ./src/client/ClientUI.cpp ./src/shared/datagram.h ./src/shared/DatagramStringifier.cpp

server:
	g++ -std=c++11 -o dropboxServer.out -pthread ./src/server/dropboxServer.cpp ./src/shared/datagram.h ./src/server/ServerConnectorUDP.cpp ./src/shared/DatagramStringifier.cpp ./src/server/ClientHandler.cpp

clean:
	rm ./dropboxClient.out rm ./dropboxServer.out
