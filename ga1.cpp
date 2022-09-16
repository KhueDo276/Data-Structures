#include <cstdlib>
#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <math.h>
#include <stack>
#include <stdio.h>
#include <string>
#include <vector>

#include "ArgumentManager.h"
#include "LinkedList.h"

using namespace std;

bool isoperator(char i) {
  if (i == '+' || i == '-') {
    return true;
  }
  return false;
}
bool isparen(char i) {
  if (i == '{' || i == '(' || i == '[' || i == '}' || i == ')' || i == ']')
    return true;
  return false;
}
bool isopenparen(char i) {
  if (i == '{' || i == '(' || i == '[') {
    return true;
  }
  return false;
}
int convert(string c) {
  int s, n = 0;
  if (c[0] == '-') {
    s = c.length() - 2;
    n = 1;
  } else {
    s = c.length() - 1;
  }
  int res = 0;
  for (int i = n; i < c.length(); i++) {
    int num = c[i] - '0';
    res += num * pow(10, s);
    s--;
  }
  if (c[0] == '-') {
    return (0 - res);
  }
  return res;
}
// function calculate a small expression
string adding(vector<char> str) {
  vector<string> v;
  for (int i = 0; i < str.size();
       i++) { // put numbers and operators to a vector
    string num = "";
    if (isalnum(str[i])) {
      num += str[i];
      while (isalnum(str[i + 1])) {
        num += str[i + 1];
        i++;
      }
      v.push_back(num);
      continue;
    } else {
      int minus_count = 0;
      if (str[i] == '-') {
        minus_count++;
      }
      while (!isalnum(str[i + 1])) {
        if (str[i + 1] == '-') {
          minus_count++;
          i++;
        }
      }
      if (minus_count % 2 == 1) {
        v.push_back("-");
      } else {
        v.push_back("+");
      }
    }
  }
  // add the numbers together
  int total = 0;
  int n = 0;
  if (v[0] == "-") {
    total = -convert(v[1]);
    n = 2;
  } else if (v[0] == "+") {
    total = convert(v[1]);
    n = 2;
  } else {
    total = convert(v[0]);
    n = 1;
  }
  for (int i = n; i < v.size(); i += 2) {
    if (v[i] == "+") {
      total += convert(v[i + 1]);
    } else {
      total -= convert(v[i + 1]);
    }
  }
  return to_string(total);
}
// function to decode
string find(string str) {
  stack<char> valid;
  valid.push('#');
  map<char, char> mymap;
  mymap['}'] = '{';
  mymap[']'] = '[';
  mymap[')'] = '(';
  stack<char> s;
  s.push('#');

  for (int i = 0; i < str.length();
       i++) { // check if the expression is valid or not
    if (isparen(str[i])) {
      if (isopenparen(str[i])) {
        valid.push(str[i]);
      } else {
        if (valid.top() == mymap[str[i]]) {
          valid.pop();
        } else {
          return "invalid";
        }
      }
    }
    if (isalnum(str[i])) { // if it is a number, open paren, or operator, push
                           // it in the stack
      s.push(str[i]);
    } else if (isoperator(str[i])) {
      s.push(str[i]);
    } else if (isopenparen(str[i])) {
      s.push(str[i]);
    } else if (str[i] == '}' || str[i] == ')' ||
               str[i] == ']') { // if it is a close paren
      /*if(!isalnum(s.top()) && s.top() != mymap[str[i]]){
        return "Invalid";
      } */
      if (s.top() == mymap[str[i]]) {
        s.pop();
      } else {
        vector<char> ex;
        while (s.top() != mymap[str[i]]) { // add the elements of a small
                                           // expression to a vector
          if (s.top() == '#') {
            break;
          }
          ex.insert(ex.begin(), s.top()); // put in front of vector
          s.pop();
        }
        s.pop(); // pop '{'
        string total = adding(ex);
        for (int i = 0; i < total.size(); i++) {
          s.push(total[i]);
        }
      }
    }
  }
  if (valid.top() != '#') {
    return "invalid";
  }
  vector<char> t; // if there is no paren in the expression
  while (s.top() != '#' && !isopenparen(s.top())) { // check
    t.insert(t.begin(), s.top());
    s.pop();
  }
  string total = adding(t);
  for (int i = 0; i < total.size(); i++) {
    s.push(total[i]);
  }
  string res = "";
  vector<char> result;
  while (s.top() != '#' && !isopenparen(s.top())) {
    result.insert(result.begin(), s.top());
    s.pop();
  }
  for (int i = 0; i < result.size(); i++) {
    res += result[i];
  }
  if (s.top() != '#') {
    return "invalid";
  }
  return res;
}

void add(int x, LinkedList *name, vector<int> &negatives) {
  vector<int> test;
  int i = 0;
  if (i == 0) {
    if (x < 0) {
      negatives.push_back(x);
      i++;
    } else {
      name->addInt(name->getHead(), x);
    }
  } else if (i > 0 && x < 0) {
    negatives.push_back(x);
  } else {

    name->addInt(name->getHead(), x);
  }
}

int main(int argc, char* argv[]) {
  
  ArgumentManager am(argc, argv);
  ifstream input(am.get("input"));
  ofstream output(am.get("output"));

  // ------- TAKE IN INPUT ---------
  // ifstream input("input11.txt");
  // ofstream output("output11.txt");


  string line;
  int markSarah;
  string passcode;
  vector<int> negatives;
  vector<int> vec;

  LinkedList Sarah;
  LinkedList Alex;

  // when remove at pos
  int invalidCount = 0; // should increment this when meet invalid expression

  while (getline(input, line)) {
    if (!line.empty()) {
      if (line == "Sarah") {
        markSarah = 1;
        continue;
      } else if (line == "Alex") {
        markSarah = 0;
        continue;
      } else if (line.find("Passcode") != string::npos) {
        markSarah = -1;
        passcode = line.substr(line.find(':') + 1);
        break;
      }
      if (markSarah == 1) {
        if (find(line) != "invalid") {
          add(convert(find(line)), &Sarah, negatives);

        } else {
          invalidCount++;
        }
      } else if (markSarah == 0) {
        if (find(line) != "invalid") {
          add(convert(find(line)), &Alex, negatives);
        } else {
          invalidCount++;
        }
      }
    }
  }
  
  for (int i = 0; i < negatives.size(); i++)
        negatives.at(i) = abs(negatives.at(i));
  
  for (int i = 0; i < negatives.size(); i++) {
    if (Sarah.search(Sarah.getHead(), negatives.at(i))) {
      Sarah.removeNegRec(negatives.at(i));
    }
    else
      Alex.removeNegRec(negatives.at(i));
  }
  output << "Sarah: [";
  Sarah.printRec(Sarah.getHead(), ", ", output);
  output << "]\n";
  output << "Alex: [";
  Alex.printRec(Alex.getHead(), ", ", output);
  output << "]\n";
  Sarah.mergeRec(Sarah.getHead(), Alex.getHead());
  if (invalidCount > 0) { Sarah.remove(invalidCount); }
  
  node* revNode = Sarah.reverseRec(Sarah.getHead());
  
  output << "Decoded passcode: | ";
  Sarah.printRec(revNode," | ", output);
  output << " | " << endl;

  output << "Actual passcode: | ";
  for (int i = 0; i < passcode.size(); i++) {
    if (i != passcode.size() - 1) {
      output << passcode[i] << " | ";
    } else {
      output << passcode[i] << " |";
    }
  }

  
  input.close();
  output.close();

  return 0;
}
