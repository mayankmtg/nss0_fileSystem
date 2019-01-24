#include <limits.h> /* PATH_MAX */
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>

using namespace std;
static string rootDir = "/home/mayank/Sem-8/NSS/nss0_fileSystem/direc/root";
int main(void) {
    char buf[PATH_MAX + 1];
    char *res = realpath("/home/mayank/Sem-8/NSS/nss0_fileSystem/direc/root/simple_home/amit", buf);
    if (res) {
		string stt = buf;
        cout << stt << endl;
		cout << stt.substr(rootDir.length(), stt.length()) << endl;
    } else {
        perror("realpath");
        exit(EXIT_FAILURE);
    }
    return 0;
}