#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;
int main(){
    string my_str;
    cin >> my_str;
    while(my_str.find(',') != string::npos){
        int pos = my_str.find(',');
        cout << my_str.substr(0,my_str.find(','))<< endl;
        my_str = my_str.substr(my_str.find(',')+1);
        usleep(1000000);

    }
    cout << my_str<< endl;
    return 0;
}