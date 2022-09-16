//Khue Nhat Do
//hw0
//This code will calculate % of a subset from a
// given matrix

#include <iostream>
#include <fstream>
#include "ArgumentManager.h"
#include <string>
#include <iomanip>

using namespace std;

int main(int argc, char *argv[])
{
    ArgumentManager am(argc, argv);
    const string input = am.get("input");
    const string output = am.get("output");

    ifstream inputfile(input);
    ofstream outputfile(output);

    string str, a;
    char ch;
    int even = 0, same = 0, great = 0, odd = 0, less = 0, n, total = 0;
    float result;
    inputfile >> str;

    const int s = stoi(str);
    int **mat = new int *[s];
    total = s * s;
    for (int i = 0; i < s; i++)
        mat[i] = new int[s];

    for (int i = 0; i < s; i++)
    {
        for (int j = 0; j < s; j++)
        {
            inputfile >> mat[i][j];
        }
    }

    while (getline(inputfile, str))
    {
        for (int i = 0; i < s; i++)
        {
            for (int j = 0; j < s; j++)
            {
                if (str == "e")
                {
                    if (mat[i][j] % 2 == 0)
                        even++;
                }
                else if (str == "o")
                {
                    if (mat[i][j] % 2 != 0)
                        odd++;
                }
                else if (str.find("=") != -1)
                {
                    a = str[1];
                    n = stoi(a);
                    if (mat[i][j] == n)
                        same++;
                }
                else if (str.find(">") != -1)
                {
                    a = str[1];
                    n = stoi(a);
                    if (mat[i][j] > n)
                        great++;
                }
                else if (str.find("<") != -1)
                {
                    a = str[1];
                    n = stoi(a);
                    if (mat[i][j] < n)
                        less++;
                }
            }
        }
    }

    if (str == "e")
    {
        result = (float)even / total;
    }
    else if (str == "o")
    {
        result = (float)odd / total;
    }
    else if (str.find("=") != -1)
    {
        result = (float)same / total;
    }
    else if (str.find(">") != -1)
    {
        result = (float)great / total;
    }
    else if (str.find("<") != -1)
    {
        result = (float)less / total;
    }

    outputfile << fixed << setprecision(2) << result << endl;

    inputfile.close();
    outputfile.close();

    return 0;
}