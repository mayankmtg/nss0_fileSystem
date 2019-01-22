#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>
using namespace std;

static int connFd;

void send_string(char* char_array, string send_string, int size){
	bzero(char_array, size+1);
	strcpy(char_array, send_string.c_str());
	write(connFd,char_array,size);
}
string recv_string(char* char_array, int size){
	bzero(char_array, size+1);
	read(connFd, char_array, size);
	string curr_string(char_array);
	return curr_string;
}


void *serverHandler (void *dummyPt){
	cout << "Thread No: " << pthread_self() << endl;
	char test[300];
	string login_string="Enter your username: ";
	send_string(test, login_string, 300);

	bool loop = false;
	while(!loop){	
		string tester = recv_string(test, 300);

		if(tester == "ls"){
			cout << "tested" <<endl;
		}
		else if(tester == "fput"){
			cout << tester <<endl;
		}
		else if(tester == "fget"){
			cout << tester <<endl;
		}
		else if(tester == "create_dir"){
			cout << tester <<endl;
		}
		else if(tester == "cd"){
			cout << tester <<endl;
		}
		else if(tester == "exit"){
			break;
		}
		else{
			cout << tester << endl;
		}
	}

	cout << endl << "Closing thread and conn" << endl;
	close(connFd);
}

int main(int argc, char* argv[]) {
	int pId, portNo, listenFd;
	socklen_t len;
	bool loop = false;
	struct sockaddr_in svrAdd, clntAdd;
	pthread_t threadA[3];

	if (argc < 2){
		cerr << "Syntax : ./server <port>" << endl;
		return 0;
	}
	portNo = atoi(argv[1]);
	if((portNo > 65535) || (portNo < 2000)){
		cerr << "Please enter a port number between 2000-65535" << endl;
		return 0;
	}

	//create socket
	listenFd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenFd < 0){
		cerr << "Cannot Open Socket" << endl;
		return 0;
	}

	bzero((char*) &svrAdd, sizeof(svrAdd));

	svrAdd.sin_family = AF_INET;
	svrAdd.sin_addr.s_addr = INADDR_ANY;
	svrAdd.sin_port = htons(portNo);

	//bind socket
	if(bind(listenFd, (struct sockaddr *)&svrAdd, sizeof(svrAdd)) < 0){
		cerr << "Cannot bind" << endl;
		return 0;
	}

	listen(listenFd, 5);
	len = sizeof(clntAdd);
	int noThread = 0;
	while (noThread < 3){
		cout << "Listening" << endl;

		connFd = accept(listenFd, (struct sockaddr *)&clntAdd, &len);
		if (connFd < 0){
			cerr << "Cannot Accept Connection" << endl;
			return 0;
		}
		else{
			cout << "Connection Successful" << endl;
		}
		pthread_create(&threadA[noThread], NULL, serverHandler, NULL); 
		noThread++;
	}

	for(int i = 0; i < 3; i++){
		pthread_join(threadA[i], NULL);
	}
}