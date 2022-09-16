#include "ArgumentManager.h"
#include "Tree.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

struct box {
  int key;
  int index1;
  int index2;
  int count;
};

class HASH {
public:
  int tableSize;
  box *table;
  HASH(int size) {
    tableSize = size;
    table = new box[tableSize];
  }
  ~HASH() { delete[] table; }

  void initializeTable() {
    for (int i = 0; i < tableSize; i++)
      table[i].key = -1;
  }

  string cuckoo(box input[], int inputSize) {
    // initialize table
    initializeTable();
    // start iteration
    string res = "Passed";
    int count = 0;
    for (int i = 0; i < inputSize; i++) {
      // cout << "cuckoo - input.key: " << input[i].key << endl;
      insert(input[i], 0, count, inputSize, res);
      if (res != "Passed") {
        res = "Failed";
        break;
      }
      count = 0;
    }
    return res;
  }

  void insert(box b, int index, int count, int maxIteration, string &res) {
    // void insert(box b, int index, int maxIteration) {
    // cout << "count: " << count << endl;
    if (count == maxIteration) {
      res = "Failed";
      return;
    }

    int pos = 0;
    switch (index) {
    case 0:
      pos = b.index1;
      break;
    case 1:
      pos = b.index2;
      break;
    }
    // cout << "index: " << pos << endl;

    if (table[pos].key != -1) {
      box replace = table[pos];
      // cout << "to replace: " << table[pos].key << endl;
      table[pos] = b;
      insert(replace, 1, count + 1, maxIteration, res);
      // insert(replace,1,maxIteration);
    } else {
      table[pos] = b;
      /*for (int i = 0; i < tableSize; i++)
      cout << table[i].key << " ";
      cout << endl;*/
    }
  }
};

int main(int argc, char *argv[]) {
  ArgumentManager am(argc, argv);
  const string inputF = am.get("input");
  const string outputF = am.get("output");

  ifstream input(inputF);
  ofstream output(outputF);

  int m, n;
  input >> m >> n;
  // cout << m << " " << n << endl;

  box b[m];

  for (int i = 0; i < m; i++) {
    b[i].key = rand() % 10;
    input >> b[i].index1;
    input >> b[i].index2;
  }

  HASH ht(n);

  /*ofstream commandfile("command.txt");
    ht.cuckoo(b,m,commandfile);
    commandfile << "#" << endl;
  commandfile.close();
*/
  // ifstream command("command.txt");
  int size, num, key, value;
  string line;
  tree t;
  input >> size;
  input >> num;
  node *root = t.insert(t.getroot(), num);
  for (int i = 0; i < size - 1; i++) {
    input >> num;
    t.insert(root, num);
  }
  input >> value >> key;

  if (t.count(root, key, value) == 0 || ht.cuckoo(b, m) == "Failed") {
    output << "Failed" << endl;
  } else {
    output << "Passed" << endl;
    output << "Looking for: " << value << endl;
    output << "Starting from: " << key << endl;
    output << t.count(root, key, value) << endl;
    t.search(root, key, value, output);
  }
  input.close();
  output.close();
}
