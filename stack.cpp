#include <fstream>
#include "prefixpostfix.h"
#include "stack.h"
#include "ArgumentManager.h"
using namespace std;

struct node {
  string type;
  string val;
  node *next;
  node *prev;
};

class dll {
private:
  node *head;
  node *tail;

public:
  dll() {
    head = nullptr;
    tail = nullptr;
  }
  bool isempty() { 
    return head == nullptr; 
  }
  node* gethead(){return head;}
  node* gettail(){return tail;}
  int size() {
    if (isempty())
      return 0;
    int count = 0;
    node *cur = head;
    while (cur != nullptr) {
      cur = cur->next;
      count++;
    }
    return count;
  }
  node* create(string type, string val){
    node* cur = new node;
    cur->val = val;
    cur->type = type;
    cur->next = nullptr;
    cur->prev = nullptr;
    return cur;
  }
  void insert(node* cur){
    if(isempty()){
      head = cur;
      tail = cur;
      return;
    }
    tail->next = cur;
    cur->next = nullptr;
    cur->prev = tail;
    tail = cur;
  }
  void insertatpos(node* cur, int pos){
    if (isempty() && pos > 0){
      return;
    }
    if(pos <= 0){       
      if(isempty()){
        head = cur;
        return;
      }
      cur->next = head;
      head = cur;
      head->prev = nullptr;
    }
    else if(pos >= size()){
      tail->next = cur;
      cur->prev = tail;
      tail = cur;
    }
    else{
      int count = 0;
      node *temp = head;
      while(temp->next != nullptr && count != pos-1){
        temp = temp->next;
        count++;
      }
      if(pos-1 == count){
        cur->next = temp->next;   
        temp->next = cur;
        cur->next->prev = cur;
        cur->prev = temp;
      }
    }
  }
  void deletenode(node* cur){
    if(cur == head && cur == tail){
      node* temp = head;
      head = nullptr;
      delete temp;
      return;
    }
    if(cur == head){
      node* temp = head;
      head = head->next;
      head->prev = nullptr;
      delete temp;
    }
    else if(cur == tail){
      node* temp = tail;
      tail = tail->prev;
      tail->next = nullptr;
      delete temp;
    }
    else{
      node* temp = cur->prev;
      node* dnode = cur;
      temp->next = cur->next;
      cur->next->prev = temp;
      delete dnode;
    }
  }
  int converttoint(string c){   
    int s, n=0;
    if(c[0] == '-'){
      s = c.length()-2;
      n = 1;
    }
    else{
      s = c.length()-1;
    }
    int res = 0;
    for(int i = n; i < c.length(); i++){
      int num = c[i] - '0';
      res += num*pow(10,s);
      s--;
    }
    if(c[0] == '-'){
      return (0-res);
    }
    return res;
  }
  string convert(string type, string newtype, string val) {
    string temp;
    if (type == "prefix" && newtype == "postfix") {
      temp = preToPost(val);
    } else if (type == "postfix" && newtype == "prefix") {
      temp = postToPre(val);
    } else if (type == newtype) {
      return val;
    }
    return temp;
  }
  void position(node* cur) {
    if (cur->type == "postfix") {
      cur->val = convert(cur->type, "prefix", cur->val);
      cur->type = "prefix";
    } else if (cur->type == "prefix") {
      cur->val = convert(cur->type, "postfix", cur->val);
      cur->type = "postfix";
    }
  }

  void convertList(string type) { // postfix/prefix/position/all
    if (isempty()) {
      return;
    }
    node *cur = head;
    if (type == "postfix") {
      while (cur != nullptr) {
        if (cur->type == type) {
          cur->val = convert(cur->type, "prefix", cur->val);
          cur->type = "prefix";
        }
        cur = cur->next;
      }
    } 
    else if (type == "prefix") {
      while (cur != nullptr) {
        if (cur->type == type) {
          cur->val = convert(cur->type, "postfix", cur->val);
          cur->type = "postfix";
        }
        cur = cur->next;
      }
    } 
    else if (type == "all") {
      while (cur != nullptr) {
        position(cur);
        cur = cur->next;
      }
    } 
    else {
      //cout << "position" << endl;
      int num = converttoint(type);
      if (num <= 0) {
        position(head);
      } else if (num == size() -1) {
        position(tail);
      }
      else if (num >= size()){
        return;
      }
      else{
        int index = 0;
        while (num != index) {
          cur = cur->next;
          index++;
        }
        position(cur);
      }
    }
  }

  void removeList(string type){
    if(isempty())
      return;
    node* cur = head;
    if(type == "postfix" || type == "prefix"){
      while(cur != nullptr){
        if(cur->type == type){
          deletenode(cur);
        }
        cur = cur->next;
      }
    }
    else if(type == "all"){
      while(cur != nullptr){
        deletenode(cur);
        cur = cur->next;
      }
    } 
    else{
      int num = converttoint(type);
      if (num <= 0) {
        deletenode(head);
      } else if (num == size()-1) {
        deletenode(tail);
      }
        else if (num >= size()){
        return;
      }
      else{
        int index = 0;
        while (num != index) {
          cur = cur->next;
          index++;
        }
        deletenode(cur);
      }
    }
  }

  ::stack <string> pushReserve(string type, ::stack <string> s){
    if(isempty())
      return s;
    node *cur = head;
    if (type == "postfix" || type == "prefix") {
      while (cur != nullptr) {
        if(cur->type == type){
          s.pushNode(cur->val, cur->type);
          deletenode(cur);
        }
        cur = cur->next;
      }
    }
    else if(type == "all"){
      while(cur != nullptr){
        s.pushNode(cur->val, cur->type);
        deletenode(cur);
        cur = cur->next;
      }
    }
    else{
      int num = converttoint(type);
      if (num <= 0) {
        s.pushNode(head->val, head->type);
        deletenode(head);
      } else if (num == size()-1) {
        s.pushNode(tail->val, tail->type);
        deletenode(tail);
      }
        else if (num >= size()){
        return s;
      }
      else{
        int index = 0;
        while (num != index) {
          cur = cur->next;
          index++;
        }
        s.pushNode(cur->val, cur->type);
        deletenode(cur);
      }
    }
    return s;
  }

  ::stack <string> flipReserve(::stack <string> s){
    ::stack <string> news;
    while(!s.isempty()){
      news.pushNode(s.top(), s.typetop());
      s.pop();
    }
    return news;
  }

  void convertReserve(::stack <string> s){
    if(s.isempty()){
      return;
    }
    string type;
    if(s.typetop() == "postfix"){
      type = "prefix";
    }
    else{
      type = "postfix";
    }
    string val = convert(s.typetop(), type, s.top());
    s.pop();
    s.pushNode(val, type);
  }

  void emptyReserve(int pos, ::stack <string> s){
    if(s.isempty()){
      return;}
    while(!s.isempty()){
      node* cur = create(s.typetop(), s.top());
      if(pos <= 0){
        insertatpos(cur,0);
      }
      else if (pos >= size()){
        insertatpos(cur,size());
      }
      else{
        insertatpos(cur,pos);
      }
      s.pop();
    }
  }
};


int main(int argc, char* argv[]) {
    ArgumentManager am(argc, argv);
    const string input = am.get("input");
    const string output = am.get("output");
    ifstream inputfile(input);
    ofstream outputfile(output);
  dll l;
  ::stack <string> s;
  
  string line, cline, type = "", val = "";
  bool isval = false;
  //ifstream inputfile("input32.txt");
  while(getline(inputfile, line)){
    if (!line.empty()){
      for(int i = 0; i < line.length(); i++){
        if(line[i] == ' '){
          continue;
        }
        if(line[i] == ':'){
          isval = true;
          continue;
        }
        if(isval){
          val += line[i];
        }
        else{
          type += line[i];
        }
      }
      node* cur = l.create(type, val);
      l.insert(cur);
      type = "";
      val = "";
      isval = false;
    }
  }
    const string command = am.get("command");
    ifstream commandfile(command);
  string cm = "", pos = "";
  bool ispos = false;
  //ifstream commandfile("command32.txt");
  while(getline(commandfile, line)){
    if(!line.empty()){
      for(int i = 0; i < line.length(); i++){
        if(line[i] == ' ' || line[i] == '('){
          ispos = true;
          continue;
        }
        if(line[i] == ')')
          break;
        if(ispos){
          pos += line[i];
        }
        else{
          cm += line[i];
        }
      }
      //cout << cm<< pos << endl;
      if(cm == "convertList"){
        l.convertList(pos);
      }
        
      else if(cm == "removeList"){
        l.removeList(pos);
      }
        
      else if(cm == "printList"){
          outputfile << "List:"<< endl;
        if(l.isempty()){
          outputfile << "EMPTY" << endl;
          outputfile << endl;
        }
        else{
          node* cur = l.gethead();
          while(cur != nullptr){
            outputfile << cur->type << ":" << cur->val << endl;
            cur= cur->next;
          }
          outputfile << endl;
        }
      }
    
      else if(cm == "printListBackwards"){
          outputfile << "Reversed List:"<< endl;
        if(l.isempty()){
            outputfile << "EMPTY" << endl;
            outputfile << endl;
        }
        else{
          node* cur = l.gettail();
          while(cur != nullptr){
            outputfile << cur->type << ":" << cur->val << endl;
            cur= cur->prev;
          }
            outputfile << endl;
        }
      }
        
      else if(cm == "pushReserve"){
        ::stack <string> news = l.pushReserve(pos, s);
        s = news;
      }
        
      else if(cm == "popReserve"){
        int num = l.converttoint(pos);
        if(!s.isempty()){
          string val = s.top();
          string type = s.typetop();
            s.pop();
            node* temp = l.create(type, val);
        l.insertatpos(temp, num);
        }        
      }
        
      else if(cm == "flipReserve"){
        ::stack <string> news = l.flipReserve(s);
        s = news;
      }
        
      else if(cm == "printReserveSize"){
        if(s.isempty()){
            outputfile << "Reserve Size: " << 0 << endl;
            outputfile << endl;
        }
        else{
          ::stack <string> news;
          int count = 0;
          while(!s.isempty()){
            count++;
            news.pushNode(s.top(), s.typetop());
            s.pop();
          }
          while(!news.isempty()){
            s.pushNode(news.top(), news.typetop());
            news.pop();
          }
            outputfile << "Reserve Size: " << count << endl;
            outputfile << endl;
        }
      }
        
      else if(cm == "convertReserve"){
        l.convertReserve(s);
      }
        
      else if(cm == "printReserveTop"){
        if(s.isempty()){
            outputfile << "Top of Reserve: EMPTY" << endl;
            outputfile << endl;
        }
        else{
            outputfile <<"Top of Reserve: " << s.typetop() << ":" << s.top() << endl;
            outputfile << endl;
        }
      }
        
      else if(cm == "emptyReserve"){
        int num = l.converttoint(pos);
        l.emptyReserve(num,s);
      }
      
      cm = "";
      pos = "";
      ispos = false;
    }
  }
    outputfile.close();
  inputfile.close();
  commandfile.close();
}
