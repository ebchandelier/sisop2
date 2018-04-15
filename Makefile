client:
	g++ -o dropboxClient dropboxUtil.cpp dropboxClient.cpp
	./dropboxClient

server:
	g++ -o dropboxServer dropboxServer.cpp
	./dropboxServer
