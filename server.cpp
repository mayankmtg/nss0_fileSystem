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
#include <sys/stat.h>
#include <pthread.h>
using namespace std;

static int connFd;
static string rootDir = "/home/mayank/Sem-8/NSS/nss0_fileSystem/root";
static string homeDir = "/simple_home";
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

bool authenticate_user(string curr_user){
	ifstream infile;
	string user_file = "/users.txt";
	user_file = rootDir + user_file;
	infile.open(user_file.c_str());
	string userLine;
	while(infile >> userLine){
		if(userLine == curr_user){
			return true;
		}
	}
	return false;

}

void *closing_seq (string message){
	char test[300];
	send_string(test, message, 300);
	cout << endl << "Closing thread and conn" << endl;
	close(connFd);
	void *returnVal;
	return returnVal;
}

void *serverHandler (void *dummyPt){
	cout << "Thread No: " << pthread_self() << endl;
	char test[300];
	// 'message' to be sent to the server
	string message="Enter your username: ";
	send_string(test, message, 300);
	string curr_dir = rootDir + homeDir;
	
	// 'response' received from the server
	string response = recv_string(test, 300);
	// 'response' contains username entered by client;
	
	string curr_user = response;
	string curr_group = response; 
	fstream outfile;
	ifstream infile;

	if(!authenticate_user(response)){
		message = "User does not exist. Do you want to create a new one? (yes/no): ";
		send_string(test, message, 300);
		response = recv_string(test, 300);

		if(response == "yes"){
			string user_file = "/users.txt";
			user_file = rootDir + user_file;
			outfile.open(user_file.c_str(), ofstream::app);
			outfile << curr_user << endl;
			outfile.close();
			// check = mkdir(rootDir+"/"+curr_user);
			curr_dir = curr_dir + "/" + curr_user;
			if(mkdir(curr_dir.c_str(), 0777) == -1){
				return closing_seq("Error: Driectory Exists.");
			}
			string meta_file = rootDir + homeDir + "/" + curr_user + ".d";
			outfile.open(meta_file.c_str(), ofstream::app);
			outfile << curr_user << endl;
			outfile << curr_group << endl;
			outfile.close();
		}
		else{
			return closing_seq("Closing Connection");
		}
	}
	curr_dir = homeDir+"/"+curr_user;
	// else user name typed exists
	message = curr_dir + "$ ";
	send_string(test, message, 300);


	bool loop = false;
	while(!loop){	
		response = recv_string(test, 300);

		if(response == "ls"){
			cout << "tested" <<endl;
		}
		else if(response == "fput"){
			cout << response <<endl;
		}
		else if(response == "fget"){
			cout << response <<endl;
		}
		else if(response == "create_dir"){
			cout << response <<endl;
		}
		else if(response == "cd"){
			cout << response <<endl;
		}
		else if(response == "exit"){
			break;
		}
		else{
			cout << response << endl;
		}
	}

	cout << "Closing thread and conn" << endl;
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