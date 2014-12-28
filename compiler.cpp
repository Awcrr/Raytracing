#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
using namespace std;
string cmd,s;
char ord[333];

int main(){
	freopen("list","r",stdin);
	cmd = "g++ ";
	while(cin >> s)
		cmd = cmd + s + " ";
	cmd = cmd + "-o raytracing -O2 -g -Wall && size raytracing";
	int l = cmd.length();
	for(int i = 0;i < l;++ i) ord[i] = cmd[i];
	system(ord);
	fclose(stdin);
	return 0;
}
