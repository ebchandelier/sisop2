all: dropboxClient.out dropboxServer.out

dropboxClient.out: ./src/client/*  ./src/shared/*
	g++ -std=c++11 -o dropboxClient.out -pthread ./src/client/*  ./src/shared/*

dropboxServer.out: ./src/server/* ./src/shared/*
	g++ -std=c++11 -o dropboxServer.out -pthread ./src/server/* ./src/shared/*

clean: ./dropboxClient.out ./dropboxServer.out
	rm ./dropboxClient.out ./dropboxServer.out