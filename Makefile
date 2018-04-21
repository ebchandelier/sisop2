client:
	g++ -o dropboxClient.out ./src/client/dropboxUtil.cpp ./src/client/dropboxClient.cpp ./src/shared/datagram.h

server:
	g++ -o dropboxServer.out ./src/server/dropboxServer.cpp ./src/shared/datagram.h

clean:
	rm dropboxClient
