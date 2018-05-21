#include <sys/types.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

void read_directory(const std::string& name, vector<string>& v)
{
    DIR* dirp = opendir(name.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
        v.push_back(dp->d_name);
    }
    closedir(dirp);
}

string exec(char* cmd) {
    FILE* pipe = _popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
      if(fgets(buffer, 128, pipe) != NULL)
        result += buffer;
    }
    _pclose(pipe);
    return result;
}
 
int main() {
    vector<string> v;
  read_directory(".", v);
  int i = 0;
  for (; i < v.size(); i++) {
      cerr << v[i] << endl;
  }

    exec("ping -n 1 baidu.com");

    vector<int> error;
    error[1000] = 10;
  return 0;
}