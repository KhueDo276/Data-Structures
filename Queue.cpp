#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
#include <string.h>
#include "ArgumentManager.h"
using namespace std;

struct node{
  string val;
  float priority;
  node* next;
};
node* newNode(string value, float key){
  node* temp = (node*)malloc(sizeof(node));
  temp->val = value;
  temp->priority = key;
  temp->next = nullptr;
  return temp;
}

string peek(node** head){
  return (*head)->val;
}

void pop(node** head){
  node* temp = *head;
  (*head) = (*head)->next;
  free(temp);
}

void push(node** head, string value, float key){
  node *start = (*head);
  node* temp = newNode(value, key);
  if((*head)->priority > key){
    temp->next = *head;
    (*head) = temp;
  }
  else{
    while(start->next != nullptr && start->next->priority <= key){
      start = start->next;
    }
    temp->next = start->next;
    start->next = temp;
  }
}

int isEmpty(node** head){
  return (*head) == nullptr;
}

int main(int argc, char* argv[]) {
  string line;
  //ifstream inputfile("input1.txt");
  ArgumentManager am(argc, argv);
  const string input = am.get("input");
  const string output = am.get("output");
  ifstream inputfile(input);
  ofstream outputfile(output);
  int flag = 1;
  node *pq;
  while(getline(inputfile, line)){
    int i;
    for(i = line.length()-1; line[i] != ' ' ; i--);
    float num = stof(line.substr(i+1, line.length()-1));
    line = line.substr(0,i);
    if(flag ==  1){
      pq = newNode(line, num);
      flag = 0;
    }
    else{
      push(&pq, line, num);
    }
  }

  while(!isEmpty(&pq)){
    outputfile << peek(&pq) << endl;
    pop(&pq);
  }
  inputfile.close();
  outputfile.close();
  return 0;
}
