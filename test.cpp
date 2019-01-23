#include <stdio.h> 
#include <dirent.h>
#include <bits/stdc++.h> 
using namespace std;




string getFileExtension(string filename) {
	// int pos = filename.find('.');
	// string sub = filename.substr(pos+1);
	// return sub;
	string filename_copy(filename);
	reverse(filename_copy.begin(), filename_copy.end());
	int pos = filename_copy.find('.');
	string sub = filename_copy.substr(0,pos);
	reverse(sub.begin(), sub.end());
	return sub;
}

int main(void) 
{ 
    struct dirent *de;  // Pointer for directory entry 
  
    // opendir() returns a pointer of DIR type.  
    DIR *dr = opendir("/home/mayank/Sem-8/NSS/nss0_fileSystem/root"); 
  
    if (dr == NULL)  // opendir returns NULL if couldn't open directory 
    { 
        printf("Could not open current directory" ); 
        return 0; 
    }
    while ((de = readdir(dr)) != NULL){

        string this_inode(de->d_name);

        bool m = this_inode!="." && this_inode!="..";
        if(m){
            cout << this_inode<< endl;
            cout << getFileExtension(this_inode)<< endl;

        }
    }
  
    closedir(dr);     
    return 0; 
} 