#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "ArgumentManager.h"
using namespace std;

int main(int argc, char* argv[]) {
  map<string, int> mymap;
  set<string> s;
  vector<string> lst;
  string line, key = "", num;
  int count;
  ArgumentManager am(argc, argv);
  const string input = am.get("input");
  const string output = am.get("output");
  ifstream inputfile(input);
  ofstream outputfile(output);
  //ifstream inputfile("input3.txt");
  inputfile >> num;
  getline(inputfile, line);
  getline(inputfile, line);
  for (int i = 0; i < line.length(); i++) {
    if (line[i] == ' ') {
      if (mymap.count(key) == 0) {
        //cout << key << endl;
        mymap[key] = 1;
      } else {
        mymap[key] += 1;
      }
      if (s.find(key) == s.end()) {
        lst.push_back(key);
      }
      s.insert(key);
      key = "";
      continue;
    }
    key += line[i];
  }
  if (mymap.count(key) == 0) {
    mymap[key] = 1;
  } else {
    mymap[key] += 1;
  }
  if (s.find(key) == s.end()) {
    lst.push_back(key);
  }
  // cout << key << endl;
  sort(lst.begin(), lst.end());
  for (int i = 0; i < lst.size(); i++)
    outputfile << lst[i] << ": " << mymap[lst[i]] << endl;
  outputfile.close();
  inputfile.close();
}
