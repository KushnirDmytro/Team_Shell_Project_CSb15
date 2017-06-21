all:runner

runner:runner.o
	g++ socket_server.o -o socket_server

runner.o: socket_server.cpp
	g++ -c socket_server.cpp

	
clean:
	rm -rf *.o runner
