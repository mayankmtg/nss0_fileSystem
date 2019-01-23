#include <bits/stdc++.h>
using namespace std;
int main(){
	char s[100];
	cin.getline(s, 100);
	string s_str = s;
	int pos = s_str.find(' ');
	string sub_fir = s_str.substr(0,pos);
	string sub_lat = s_str.substr(pos+1);
	cout << sub_fir <<endl;
	cout << sub_lat << endl;
	return 0;
}