//Khue Nhat Do
//lab1

#include "ArgumentManager.h"
#include <fstream>
#include <iostream>
#include <string>
#include <bits/stdc++.h>

using namespace std;
void permutation(string str, string result);
ofstream output("output.txt");

int main(int argc, char* argv[]) {
  ArgumentManager am(argc, argv);
  const string input = am.get("input");
  const string output = am.get("output");

  ifstream inputfile(input);
  ofstream outputfile(output);
  //ifstream input("input1.txt");
  string str, line;
  inputfile >> str;
 
  if (str.length() == 0){
    outputfile << "no permutation" << endl;
  }
  else{
    permutation(str, "");
    ifstream output("output.txt");
    while(!output.eof()){
      getline(output, line);
      outputfile << line << endl;
    }
    output.close();
  }
  inputfile.close();
  outputfile.close();
  return 0;
}
void permutation(string str, string result){
  //base case
  if (str.length() == 0)
    output << result << endl;
      
  else{
    for(int i = 0; i < str.length(); i++){
        char a = str[i];
        string left = str.substr(0,i);
        string right = str.substr(i+1);
        string total = left + right;
        permutation(total, result + a);
    }
  }
}


