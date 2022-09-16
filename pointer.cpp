//Khue Nhat Do
//hw1

#include <iostream>
#include <fstream>
#include <string>
#include<bits/stdc++.h>
#include "ArgumentManager.h"

using namespace std;

class Book{
  public:
    string genre, title, author, year;
    Book(string g, string t, string a, string y){
      genre = g;
      title = t;
      author = a;
      year = y;
    }
};

Book* library(string line){
  string genre,title,author,year;
  genre = title = author = year = "NULL";
  string name = "", info = "";
  bool isfound = true;
  int i = 0;
  //loop through each line and get information 
  while (line[i] != '}'){
    if (line[i] == ' ' || line[i] == '{'){
      i++;
      continue;
    }
    if(line[i]==','){
      if(name=="genre"){
        genre= info;
      }
      if(name=="title"){
        title = info;
      }
      if(name=="author"){
        if(title=="NULL"){
          return NULL;
        }
        author = info;
      }
      if(name=="year"){
        if(author=="NULL"){
          return NULL;
        }
        year=info;
      }
      name="";
      info="";
      isfound= true;
    }
    else if(line[i]==':'){
      isfound = false;
    }
    else{
      if(isfound)
        name+=line[i];
      else
        info+=line[i];
    }
    i++;
  }
  if(name=="year"){
    if(author=="NULL")
      return NULL;
    year = info;
  }
  if(genre == "NULL" || title == "NULL"){
     return NULL;
  }
  Book *book = new Book(genre, title, author, year);
  return book;
  
}

//pair name and information
pair<string, string> Command(string info){
  string name="", val ="";
  bool isfound = true;
  for(int i = 0; i < info.length(); i++){
    if(info[i] == ' ')
      continue;
    if(info[i] == ':'){
      isfound = false;
      continue;
    }
    if(isfound)
      name += info[i];
    else
      val += info[i];
  }
  return make_pair(name,val);
}

int main(int argc, char* argv[]) {
  ArgumentManager am(argc, argv);
  const string input = am.get("input");
  const string output = am.get("output");
  ifstream inputfile(input);
  ofstream outputfile(output);
  //ifstream inputfile("input11.txt");
  Book* books[10000];
  string row;
  int size = 0;
  //store information of books in class Book array
  while(getline(inputfile,row)){
    if(row.length() < 1)
      continue;
    Book *book = library(row);
    if (book != NULL){
      books[size] = book;
      size++;
    }
  }

  const string command = am.get("command");
  ifstream commandfile(command);
  //ifstream commandfile("command11.txt");
  string info;
  set <string> genre_com, title_com, author_com, year_com;
  //pair name and information
  while(getline(commandfile, info)){
    pair<string, string> line = Command(info);
    if(line.first == "genre")
      genre_com.insert(line.second);
    if(line.first == "title")
      title_com.insert(line.second);
    if(line.first == "author")
      author_com.insert(line.second);
    if(line.first == "year")
      year_com.insert(line.second);
  }

  //store books that matched the information from command files
  Book* items[10000];
  int s = 0;
  for (int i =0; i< size; i++){
    if(books[i]->genre != "NULL" && genre_com.count(books[i]->genre)== 0 && genre_com.size() >0)
      continue;
    if(title_com.size() >0 && title_com.count(books[i]->author)==0)
      continue;
    if(year_com.size()>0 && year_com.count(books[i]->year)==0)
      continue;
    items[s] = books[i];
    s++;
  }

  for(int i=0; i<s; i++){
    if(items[i]->genre != "NULL")
      outputfile << "{genre:" << items[i]->genre << ",title:" << items[i]->title << ",author:" << items[i]->author << ",year:" << items[i]->year << "}" << endl;
    else
      outputfile << "{title:" << items[i]->title << ",author:" << items[i]->author << ",year:" << items[i]->year << "}" << endl;
  }
  
  inputfile.close();
  outputfile.close();
  commandfile.close();
  
  return 0;

}
