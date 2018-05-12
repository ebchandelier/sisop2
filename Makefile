all: client server

client:
	g++ -std=c++11 -o dropboxClient.out -pthread ./src/client/*  ./src/shared/*

server:
	g++ -std=c++11 -o dropboxServer.out -pthread ./src/server/* ./src/shared/*

clean:
	rm ./dropboxClient.out rm ./dropboxServer.out