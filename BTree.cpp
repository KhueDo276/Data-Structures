#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h>
#include <queue>
#include <set>
#include "ArgumentManager.h"
using namespace std;

class Node {
public:
  bool leaf;
  vector<int> keys;
  int size;
  vector<Node *> children;
  Node *parent;
  Node(Node *p, bool l) {
    leaf = l;
    keys.resize(0);
    size = 0;
    children.resize(0);
    parent = p;
  }
};

class BTree {
public:
  Node *root;
  int Tdegree;
  BTree(int degree) {
    root = new Node(nullptr, true);
    Tdegree = degree;
  }

  int height() {
    if (root == nullptr) {
      return 0;
    }
    queue<Node *> q;
    q.push(root);
    int d = 1;
    while (!q.empty()) {
      int size = q.size();
      for (int i = 0; i < size; i++) {
        Node *cur = q.front();
        for (int k = 0; k < (cur->children).size(); k++) {
          Node *next = cur->children[k];
          q.push(next);
        }
        q.pop();
      }
      d += 1;
    }
    return (d - 1);
  }
  void bfs(int target, ofstream &f) {
    if (root == nullptr) {
      return;
    }
    queue<Node *> q;
    q.push(root);
    int d = 1;

    while (!q.empty()) {
      int size = q.size();
 
      for (int i = 0; i < size; i++) {
        Node *cur = q.front();

        if (d < target) {
          for (int k = 0; k < (cur->children).size(); k++) {
            Node *next = cur->children[k];
            q.push(next);
          }
        } else if (d == target) {
          for (int i = 0; i < (cur->keys).size(); i++) {
            f << cur->keys[i] << " ";
          }
        }
        q.pop();
      }
      if (d == target) {
        return;
      }
      d += 1;
    }
  }

  void insertNode(Node *node, int k) {
    if (node->leaf) {
      insertKey(node, k);
    } else {
      int i = 0;
      while (i < node->size && k > node->keys[i]) {
        i++;
      }
      insertNode(node->children[i], k);
    }
  }

  void insertKey(Node *node, int k) {
    node->keys.push_back(k);
    sort(node->keys.begin(), node->keys.end());
    node->size = node->keys.size();
    if (node->size == Tdegree) {
      splitNode(node);
    }
  }

  void splitNode(Node *node) {
    Node *left = new Node(node->parent, node->leaf);
    int size;
    if (Tdegree % 2 == 0) {
      size = Tdegree / 2 - 1;
    } else {
      size = Tdegree / 2;
    }
    for (int i = 0; i < size; i++) {
      left->keys.push_back(node->keys[0]);
      auto it = node->keys.begin();
      node->keys.erase(it);
    }

    int keyPushed = node->keys[0];
    auto it = node->keys.begin();
    node->keys.erase(it);

    if (!node->leaf) {
      for (int i = 0; i < size + 1; i++) {
        left->children.push_back(node->children[0]);
        left->children[i]->parent = left;
        auto it = node->children.begin();
        node->children.erase(it);
      }
      left->size = left->keys.size();
    }

    node->size = Tdegree / 2; // size of right

    if (node->parent == nullptr) {
      Node *newRoot = new Node(nullptr, false);
      newRoot->children.push_back(left);
      newRoot->children[0]->parent = newRoot;
      newRoot->children.push_back(node);
      newRoot->children[1]->parent = newRoot;
      newRoot->keys.push_back(keyPushed);
      newRoot->size++;
      root = newRoot;
    } else {
      Node *parent = node->parent;
      auto it = find(parent->children.begin(), parent->children.end(), node);
      node->parent->children.insert(it, left);
      insertKey(node->parent, keyPushed);
    }
  }
};
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
int main(int argc, char* argv[]) {
  ArgumentManager am(argc,argv);
  const string input = am.get("input");
  const string output = am.get("output");
  const string command = am.get("command");
    
  ifstream inputfile(input);
  ifstream commandfile(command);
  ofstream outputfile(output);
  set<int> s;
  string str, level;
  getline(commandfile, str);
  int degree = stoi(str.substr(str.find('=') + 1));
  BTree t(degree);
  int key;

  while (inputfile >> key) {
    if (s.find(key) == s.end()) {
      t.insertNode(t.root, key);
    }
    s.insert(key);
  }
  outputfile << "Height=" << t.height() << endl;
  while (getline(commandfile, str)) {
    for (int i = 0; i < str.size(); i++) {
      if (str[i] == ' ') {
        for (int j = i + 1; j < str.size(); j++) {
          level += str[j];
        }
      }
    }
    int l = convert(level);
    level = "";
    if (l > t.height()) {
      outputfile << "Empty" << endl;
      continue;
    } else {
      t.bfs(l, outputfile);
      outputfile << endl;
    }
  }
  outputfile.close();
  inputfile.close();
  commandfile.close();
}
