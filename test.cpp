#include <bits/stdc++.h>
#include <algorithm>
using namespace std;
int main(){
    list<string> ilist;
    ilist.push_back("mtg");
    ilist.push_back("hello");
    ilist.push_back("hi");

    list<string>::iterator findIter = std::find(ilist.begin(), ilist.end(), "fff");
    if(findIter == ilist.end()){
        cout << 1 << endl;
    }
    else{
        cout << 0 << endl;
    }
    

}