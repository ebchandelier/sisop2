client: dropboxClient.cpp
	g++ -o dropboxClient dropboxUtil.cpp dropboxClient.cpp datagram.h

server:
	g++ -o dropboxServer dropboxServer.cpp datagram.h
	./dropboxServer

clean:
	rm dropboxClient
