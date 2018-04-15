client: dropboxClient.cpp
	g++ -o dropboxClient dropboxUtil.cpp dropboxClient.cpp

server:
	g++ -o dropboxServer dropboxServer.cpp
	./dropboxServer

clean:
	rm dropboxClient
