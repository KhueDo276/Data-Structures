#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <set>
#include <vector>
#include "ArgumentManager.h"
using namespace std;

struct node{
  string val;
  node* next;
};

class linked_list{
  public:
    node *head;
    linked_list(){head = nullptr;}
    bool isempty(){
      return (head==nullptr);
    }
    bool isduplicate(string key){
      node *cur = head;
      while(cur != nullptr){
        if(cur->val == key){
          return true;
        }
        cur = cur->next;
      }
      return false;
    }
    void Insert(const vector<string> &lst){
      node **cur = &head;
      for(const auto &value : lst){
        *cur = new node{value, *cur};
        cur = &(*cur)->next;
      }
    }
    void print(){
      node *cur = head;
      while(cur != nullptr){
        cout << cur->val << endl;
        cur = cur->next;
      }
    }
    
    void Add(int i,string key){
      if(isduplicate(key)){return;}
      node *cur = new node;
      cur->val = key;
      cur -> next = nullptr;
      if (isempty() && i > 0){
        return;
      }
      if(i == 0){       
        if(!isempty()){
          cur->next = head;
        }
        head = cur;
      }
      else{
        int count = 0;
        node *temp = head;
        while(temp-> next != nullptr && count != i-1){
          if(temp->val == key){   //duplicate
            return;
          }
          temp = temp->next;
          count++;
        }
        if(i-1 == count){
          cur->next = temp->next;   
          temp->next = cur;
        }
      }
    }
    void Remove(string key){
      if(isempty())
        return;                            
      while(head != nullptr && head->val.find(key) != string::npos){
        head = head->next;
      }
      node *cur = head; 
      node *prev = nullptr;
      while(cur != nullptr){
        string sentence = cur->val;
        if(sentence.find(key) == string::npos){  
          prev = cur;
          cur = cur->next;
        }
        else{
          cur = cur->next;
          prev->next = cur;
        }
      }
    }
    void Sort(string key){
      if(isempty()){return;}
      node *left = head;
      node *right = NULL;
      if(key == "alphabetically"){
        while(right != head->next){
          while(left->next != right){
            if(left->val > left->next->val){
              string temp = left->val;
              left->val = left->next->val;
              left->next->val = temp;
            }
            left = left->next;
          }
          right = left;         
          left = head;
        }
      }
      else if(key == "length"){
        while(right != head->next){
          while(left->next != right){
            if(left->val.length() > left->next->val.length()){
              string temp = left->val;
              left->val = left->next->val;
              left->next->val = temp;
            }
            left = left->next;
          }
          right = left;         
          left = head;
        }
      }
    }
};

string* fun(string line){
  static string arr[3];
  int i = 0;
  string word, num = "";
  while(i<line.length()){
    if (line[i] == 'A'){
      arr[0] = "Add";
      i = 4;
      continue;
    }
    else if (line[i] == 'R'){
      arr[0] = "Remove";
      i = 7;
      continue;
    }
    else if(line[i] == 'S'){
      arr[0] = "Sort";
      i = 5;
      continue;
    }
    else if(line[i] == '('){
      string num = "";
      i++;
      while (i<line.length() && line[i] != ')') {
        num += line[i];
        i++;
      }
      arr[1] = num;
    }
    else if(line[i] == '['){
      string val = "";
      i++;
      while (i<line.length() && line[i] != ']') {
        val += line[i];
        i++;
      }
      arr[2] = val;
    }
    i++;
  }
  return arr;
}


int main(int argc, char* argv[]) {
  ArgumentManager am(argc, argv);
  const string input = am.get("input");
  const string output = am.get("output");
  ifstream inputfile(input);
  ofstream outputfile(output);
  string type, line, line1, commandline;
  string action, key, byway;
  int idx;
  linked_list l;
  set <string> s;
  
  //ifstream inputfile("input23.txt");
  inputfile >> type;
  getline(inputfile, line1);
  int count = 0;
  vector <string> sorted_list;
  while(getline(inputfile,line)){
    if(!line.empty()){
      if(s.find(line) == s.end()){
        sorted_list.push_back(line);
        count++;
      }
      s.insert(line);
    }
  }
  
  if(type == "Alphabetically"){
    sort(sorted_list.begin(),sorted_list.end());
  }
  else if(type=="Length"){
    for(int i = 0; i < count; i++){
      for(int j = i+1; j < count; j++){
        if(sorted_list[i].length()>sorted_list[j].length()){
          swap(sorted_list[i],sorted_list[j]);
        }
      }
    }
  }  
  else if(type == "Beginning"){
    int start = 0;
    int end = count-1;
    while (start < end){
      string temp = sorted_list[start];
      sorted_list[start] = sorted_list[end];
      sorted_list[end] = temp;
      start++;
      end--;
    }
  }
 
  l.Insert(sorted_list);
  const string command = am.get("command");
  ifstream commandfile(command);
  //ifstream commandfile("command23.txt");
  while(getline(commandfile,commandline)){
    if(!commandline.empty()){
      string* res = fun(commandline);
      action = res[0];
      key = res[2];
      if(action == "Add"){        
        idx = stoi(res[1]);
        l.Add(idx,key);
      }
      else if(action == "Remove"){
        l.Remove(key);
      }
      else if(action =="Sort"){
        l.Sort(res[1]);
      }
    }
  }
  //l.print();
  node *cur = l.head;
  while(cur != nullptr){
    outputfile << cur->val << endl;
    cur = cur->next;
  }
  inputfile.close();
  commandfile.close();
  outputfile.close();
  return 0;
}
