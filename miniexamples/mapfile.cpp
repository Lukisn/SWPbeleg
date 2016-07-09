#include <iostream>
#include <map>
#include <fstream>
using namespace std;

int main()
{
    string dbfilename = "mapfile.db";
    map<string, string> dbmap;

    // try opening:
    ifstream infile;
    infile.open(dbfilename);
    if (infile.is_open()) {
        // read file
        cout << "db file exists. reading.." << endl;
    } else {
        // create file
        ofstream outfile;
        outfile.open(dbfilename);
        if (outfile.is_open()) {
            cout << "successfully created." << endl;
        } else {
            cout << "could not create file." << endl;
        }
    }
    infile.close();

    // add data:
    dbmap.insert(make_pair("a/b/file", "data"));
    dbmap.insert(make_pair("a/b/c/file2", "data2"));

    // output data to file:
    ofstream outfile;
    outfile.open(dbfilename);
    for (auto it : dbmap) {
        cout << it.first << "=" << it.second << endl;
        outfile << it.first << "=" << it.second << endl;
    }
    outfile.close();

    cout << "done." << endl;
    return 0;
}
