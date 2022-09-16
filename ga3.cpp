#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <vector>
#include "ArgumentManager.h"

using namespace std;
struct node {
  vector<int> adj;
};

class graph {
public:
  int numV;
  node *matrix;

  graph(int num) {
    numV = num;
    matrix = new node[numV];
  }

  void add_ver(int x, int y) {
    matrix[x].adj.push_back(y);
    matrix[y].adj.push_back(x);
  }

  void add_edge(int x, int y, int w) {
    matrix[x].adj.push_back(y);
    matrix[y].adj.push_back(x);
  }

  void maxLocations(int &loc, vector<int> &locations) {
    // vector<int> locations;
    for (int i = 0; i < numV; i++) {
      if (matrix[i].adj.size() > 2) {
        locations.push_back(i);
      }
    }
    if (locations.size() > 0) {
      /*  cout << "2. Yes (";*/
      /*  for (int i = 0; i < locations.size() - 1; i++)
          cout << "Location " << locations[i] << ", ";
        cout << "Location " << locations[locations.size() - 1] << ")" << endl;*/
      loc = locations[0];
    } else {
      /* cout << "2. No" << endl;*/
      loc = 0;
    }
  }

  bool dfs(int source, int dest, vector<bool> &visited) {
    if (visited[source] == true)
      return false;
    if (source == dest) {
      return true;
    }
    visited[source] = true;
    for (int i = 0; i < matrix[source].adj.size(); i++) {
      if (dfs(matrix[source].adj[i], dest, visited)) {
        return true;
      }
    }
    return false;
  }
  bool check() {
    vector<bool> v(numV, false);
    v[0] = true;
    for (int i = 0; i < matrix[0].adj.size(); i++) {
      v[matrix[0].adj[i]] = true;
    }
    for (int j = 0; j < numV; j++) {
      if (!v[j]) {
        vector<bool> visited(numV, false);
        if (!dfs(0, j, visited)) {
          return false;
        }
      }
    }
    return true;
  }
};

int primMST(int size, int source, vector<vector<pair<int, int>>> edges) {
  // cout << endl;
  vector<bool> v(size, false);
  v[source] = true;
  int weight = 0;
  int count = 0;

  while (count < size - 1) {
    // cout << "Source: " << source << endl;
    int i;
    for (i = 1; i < edges[source].size(); i++) {
      if (!v[edges[source][i].first]) {
        break;
      }
    }
    weight += edges[source][i].second;
    // cout << "New source: " << edges[source][i].first << endl;
    // cout << "New Weight " << i << ": " << weight << endl;
    v[edges[source][i].first] = true;
    source = edges[source][i].first;
    count++;
  }
  return weight;
}
bool sortbysec(const pair<int, int> &a, const pair<int, int> &b) {
  return (a.second < b.second);
}

vector<int> dijkstraSPT(int source, int size,
                        vector<vector<pair<int, int>>> edges,
                        vector<int> &path) {
  vector<bool> v(size, false);
  vector<int> tempWeight(size, INT_MAX);
  for (int i = 0; i < size; i++) {
    tempWeight[i] = INT_MAX;
    v[i] = false;
  }

  tempWeight[source] = 0;
  v[source] = true;
  path[source] = source;
  int weight = 0;
  int count = 0;
  while (count < size - 1) {
    int j;
    int min_num = 0, min_indx = 0;
    // cout << "Source: " << source << endl;
    for (int i = 1; i < edges[source].size(); i++) {
      if (!v[edges[source][i].first]) {
        // cout << "Temp weight at index " << edges[source][i].first << ": "
        //  << tempWeight[edges[source][i].first] << endl;
        int u = edges[source][i].second + tempWeight[source];
        if (tempWeight[edges[source][i].first] == 0) {
          path[edges[source][i].first] = source;
        } else if (u < tempWeight[edges[source][i].first]) {
          path[edges[source][i].first] = source;
        }
        tempWeight[edges[source][i].first] =
            min(u, tempWeight[edges[source][i].first]);
        // cout << "Min weight: " << tempWeight[edges[source][i].first] << endl;
        min_num = tempWeight[edges[source][i].first];
      }
    }
    for (j = 0; j < tempWeight.size(); j++) {
      if (tempWeight[j] == 0 || v[j]) {
        continue;
      }
      if (tempWeight[j] <= min_num) {
        // cout << "j " << j << endl;
        min_num = tempWeight[j];
        min_indx = j;
      }
    }
    weight += min_num;
    source = min_indx;
    path.push_back(source);
    v[min_indx] = true;
    // cout << "New source: " << min_indx << endl;
    // cout << "Weight " << count << ": " << weight << endl;
    // cout << endl;
    count++;
  }
  return path;
}

int main(int argc, char *argv[]) {
  ArgumentManager am(argc, argv);
  const string inputF = am.get("input");
  const string outputF = am.get("output");

  ifstream input(inputF);
  ofstream output(outputF);
  
  //ifstream input("input32.txt");
  //ofstream output("output32.txt");
    
  int numVer;
  input >> numVer;
  vector<vector<pair<int, int>>> edges;
  for (int i = 0; i < numVer; i++) {
    vector<pair<int, int>> v;
    v.push_back(make_pair(0, 0));
    edges.push_back(v);
  }

  int x, y, w;
  graph g(numVer);

  while (!input.eof()) {
    input >> x >> y >> w;
    g.add_ver(x, y);
    edges[x].push_back(make_pair(y, w));
    edges[y].push_back(make_pair(x, w));
  }
  for (int i = 0; i < edges.size(); i++) {
    sort(edges[i].begin(), edges[i].end(), sortbysec);
  }

  // ------ NHÁP -------
  /*for (int i = 0; i < edges.size(); i++) {
    cout << "Index " << i << ": "
         << "[";
    for (int j = 0; j < edges[i].size(); j++) {
      pair<int, int> p = edges[i][j];
      cout << "(" << p.first << ", " << p.second << "), ";
    }
    cout << "]" << endl;

  cout << endl;
  int loc = 0;
  g.maxLocations(loc);
  }*/
  //------
  vector <int> okay;
  
  if (g.check()) {
    vector<int> locations;
    int loc = 0;
    g.maxLocations(loc, locations);
    int prim = primMST(numVer, loc, edges);
    vector<int> path(numVer, -1);
    dijkstraSPT(loc, numVer, edges, path);
    /*for (int i = 0; i < numVer; i++) {
      cout << "Index " << i << ": " << path[i] << endl;
    }*/
    set<pair<int, int>> s;
    vector<bool> mark(numVer, false);
    mark[loc] = true;
    int weight = 0;
    for (int i = 0; i < numVer; i++) {
      if (!mark[i]) {
        int idx = i;
        vector<int> way;
        way.push_back(i);
        mark[i] = true;
        while (i != loc) {
          way.push_back(path[i]);
          i = path[i];
          mark[i] = true;
        }
        // cout << "Way " << idx << ": ";
        /*for (int j = 0; j < way.size(); j++) {
          cout << way[j] << " ";
        }
        cout << endl;*/
        for (int j = way.size() - 1; j > 0; j--) {
          // cout << "Pair: " << way[j] << ", " <<way[j-1] << endl;
          pair<int, int> p1 = make_pair(way[j], way[j - 1]);
          pair<int, int> p2 = make_pair(way[j - 1], way[j]);
          if (s.find(p1) != s.end() || s.find(p2) != s.end()) {
            // cout << "In set" << endl;
            continue;
          } else {
            s.insert(p1);
            s.insert(p2);
            for (int k = 1; k < edges[way[j]].size(); k++) {
              if (edges[way[j]][k].first == way[j - 1]) {
                // cout << "Weight of pair = " << edges[way[j]][k].second <<
                // endl;
                weight += edges[way[j]][k].second;
              }
            }
            // cout << "Weight: " << weight << endl;
          }
        }
      }
      // cout << endl;
    }
    output << "1. Yes" << endl;
    okay.push_back(1);
    if (locations.size() > 0) {
      okay.push_back(1);
      output << "2. Yes (";
      for (int i = 0; i < locations.size() - 1; i++)
        output << "Location " << locations[i] << ", ";
      output << "Location " << locations[locations.size() - 1] << ")" << endl;
    } else {
      output << "2. No" << endl;
      
    }

    if ((abs(prim - weight) <= 10)) {
      output << "3. Yes (MST=" << prim << ", SPT=" << weight << ")" << endl;
      //output << "Good" << endl;
      okay.push_back(1);
    } else {
      output << "3. No (MST=" << prim << ", SPT=" << weight << ")" << endl;
      //output << "Fair" << endl;
    }

  } else {
    output << "1. No" << endl;
    //output << "Bad" << endl;
  }
  
  if (okay.size() == 3) {
    output << "Good" << endl;
  }
  else if (okay.size() >= 1 && okay.size() < 3) {
    output << "Fair" << endl;
  }
  else {
    output << "Bad" << endl;
  }

  input.close();
  output.close();
  return 0;
}
