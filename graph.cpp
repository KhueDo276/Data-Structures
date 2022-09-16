#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include "ArgumentManager.h"
using namespace std;

bool dfs(map <int,vector<int>> vertex, int source, int dest, set <int> visited){
  if(visited.find(source) != visited.end())
    return false;
  if(source == dest){
    return true;
  }
  visited.insert(source);
  for(int i = 0; i < vertex[source].size(); i++){
    if(dfs(vertex, vertex[source][i], dest, visited)){
      return true;
    }
  }
  return false;
}

int main(int argc, char* argv[]) {
  //ifstream inputfile("input3.txt");
  ArgumentManager am(argc, argv);
  const string input = am.get("input");
  const string output = am.get("output");
  ifstream inputfile(input);
  ofstream outputfile(output);
  map <int, vector<int>> vertex;
  set <int> visited;
  int num, source, dest, edge, nxtedge;
  inputfile >> num;
  inputfile >> source;
  inputfile >> dest;
  while (inputfile >> edge >> nxtedge) {
    vertex[edge].push_back(nxtedge);
    vertex[nxtedge].push_back(edge);
  }
  
  if(dfs(vertex, source, dest, visited)){
    outputfile << "true" << endl;
  }
  else{
    outputfile << "false" << endl;
  }
  inputfile.close();
  //outputfile.close();
}
