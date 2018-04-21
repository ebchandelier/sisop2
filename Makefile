client:
	g++ -o dropboxClient.out ./src/client/dropboxClient.cpp ./src/client/dropboxUtil.cpp ./src/shared/datagram.h

server:
	g++ -o dropboxServer.out ./src/server/dropboxServer.cpp ./src/shared/datagram.h ./src/server/ServerConnectorUDP.cpp

clean:
	rm ./dropboxClient.out rm ./dropboxServer.out
