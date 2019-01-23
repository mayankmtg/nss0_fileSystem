#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <algorithm>
#include <pthread.h>
using namespace std;

//static int connFd;
static string rootDir = "/home/mayank/Sem-8/NSS/nss0_fileSystem/root";
static string homeDir = "/simple_home";


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


string getFileExtension(string filename) {
	string filename_copy(filename);
	reverse(filename_copy.begin(), filename_copy.end());
	int pos = filename_copy.find('.');
	string sub = filename_copy.substr(0,pos);
	reverse(sub.begin(), sub.end());
	return sub;
}


void *closing_seq (string message, int connFd){
	char test[300];
	send(connFd, (void *)message.c_str(), 300, 0);
	cout << endl << "Closing thread and conn" << endl;
	close(connFd);
	void *returnVal;
	return returnVal;
}

string getUserandGroup(string filename){
	string returnString="";
	ifstream infile;
	filename = filename + ".m";
	infile.open(filename.c_str());
	string userLine;
	while(infile >> userLine){
		returnString +=userLine + " ";
	}
	returnString+="\n";
	return returnString;
}

void *serverHandler (void* dummyPt){
	cout << "Thread No: " << pthread_self() << endl;
	char test[300];
	// 'message' to be sent to the server
	string message="Enter your username: ";
	int connFd =  *((int *)dummyPt);
	send(connFd, (void *)message.c_str(), 300, 0);
	string curr_dir = rootDir + homeDir;
	
	// 'response' received from the server
	bzero(test, 301);
	recv(connFd, test, 300, 0);
	string response = test;
	// 'response' contains username entered by client;
	
	string curr_user = response;
	string curr_group = response; 
	fstream outfile;
	ifstream infile;

	if(!authenticate_user(response)){
		message = "User does not exist. Do you want to create a new one? (yes/no): ";
		send(connFd, (void *)message.c_str(), 300, 0);
		
		bzero(test, 301);
		recv(connFd, test, 300, 0);
		response = test;

		if(response == "yes"){
			string user_file = "/users.txt";
			user_file = rootDir + user_file;
			outfile.open(user_file.c_str(), ofstream::app);
			outfile << curr_user << endl;
			outfile.close();
			
			string group_file = "/groups.txt";
			group_file = rootDir + group_file;
			outfile.open(group_file.c_str(), ofstream::app);
			outfile << curr_group << ":" <<curr_user << endl;
			outfile.close();

			// check = mkdir(rootDir+"/"+curr_user);
			curr_dir = curr_dir + "/" + curr_user;
			if(mkdir(curr_dir.c_str(), 0777) == -1){
				return closing_seq("Error: Driectory Exists.", connFd);
			}
			string meta_file = rootDir + homeDir + "/" + curr_user + ".d";
			outfile.open(meta_file.c_str(), ofstream::app);
			outfile << curr_user << endl;
			outfile << curr_group << endl;
			outfile.close();
		}
		else{
			return closing_seq("Closing Connection", connFd);
		}
	}
	curr_dir = homeDir+"/"+curr_user;
	// curr_dir contains path like /simple-home/user



	// else user name typed exists
	message = curr_dir + "$ ";
	send(connFd, (void *)message.c_str(), 300, 0);


	bool loop = false;
	while(!loop){
		bzero(test, 301);
		recv(connFd, test, 300, 0);
		string response = test;
		int pos = response.find(' ');
		string command = response.substr(0, pos);
		string argument = response.substr(pos+1);
		message = "";
		if(command == "ls"){
			argument = rootDir + homeDir + "/"+curr_user+"/" + argument;
			DIR *dr = opendir(argument.c_str());
			if(dr == NULL){
				message += "Error: Could not find directory\n";
			}
			else{
				struct dirent *de;
				while ((de = readdir(dr)) != NULL){
					string this_inode(de->d_name);
					bool not_relative_dir = this_inode!="." && this_inode!="..";
					if(not_relative_dir){
						if(getFileExtension(this_inode)!="m" && getFileExtension(this_inode)!="d"){
							message += this_inode + " " + getUserandGroup(argument + "/" + this_inode);
						}
					}
				}
			}
		}
		else if(command == "cd"){
			
		}
		else if(command == "fput"){
			
		}
		else if(command == "fget"){
			
		}
		else if(command == "create_dir"){
			
		}
		else if(command == "exit"){
			break;
		}
		else{
			
		}
		message += curr_dir + "$ ";
		send(connFd, (void *)message.c_str(), 300, 0);

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
	int connFd;

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
		pthread_create(&threadA[noThread], NULL, serverHandler, (void *) &connFd); 
		noThread++;
	}

	for(int i = 0; i < 3; i++){
		pthread_join(threadA[i], NULL);
	}
}