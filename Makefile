client:
	g++ -std=c++11 -o dropboxClient.out ./src/client/dropboxClient.cpp ./src/client/ClientConnectionManager.cpp ./src/client/ClientConnectorUDP.cpp ./src/client/dropboxUtil.cpp ./src/shared/datagram.h

server:
	g++ -std=c++11 -o dropboxServer.out ./src/server/dropboxServer.cpp ./src/shared/datagram.h ./src/server/ServerConnectorUDP.cpp

clean:
	rm ./dropboxClient.out rm ./dropboxServer.out
