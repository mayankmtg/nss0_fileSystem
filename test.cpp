#include <bits/stdc++.h>
using namespace std;
int main(){
    fstream outfile;

	string user_file = "/home/mayank/Sem-8/NSS/nss0_fileSystem/Assumptions.txt";
	outfile.open(user_file.c_str(), ofstream::app);
    string line;
    while (getline(cin, line)){
        if (line == "Q"){
            break;
        }
        outfile << line << endl;
    }
	outfile.close();
    return 0;
}