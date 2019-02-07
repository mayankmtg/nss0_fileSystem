all: clientmake servermake
clientmake: client.cpp
	g++ -pthread client.cpp -o client
servermake: server.cpp
	g++ -pthread server.cpp -o server