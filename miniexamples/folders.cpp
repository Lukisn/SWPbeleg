#include <iostream>
#include <fstream>
using namespace std;
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

int main()
{
    // existing path:
    fs::path mypath_existing = "./folders.cpp";
    bool result_existing = fs::exists(mypath_existing);

    // non existing path:
    fs::path mypath_nonexisting = "./nonexisting_folder/nonexisting.file";
    bool result_nonexisting = fs::exists(mypath_nonexisting);

    cout << "path: " << mypath_existing << " exists? -> " << result_existing << endl;
    cout << "path: " << mypath_nonexisting << " exists? -> " << result_nonexisting << endl;


    fs::path mypath = "./a/b/file";
    cout << "mypath: " << mypath << endl;
    cout << "parent_path: " << mypath.parent_path() <<  " / filename: " << mypath.filename() << endl;

    if (fs::exists(mypath)) {
        cout << "exists!" << endl;
    } else {
        cout << "does not exist! creating.." << endl;
        fs::create_directories(mypath.parent_path());
        ofstream outfile;
        outfile.open(mypath);
        outfile << "data!" << endl;
        outfile.close();
    }

    cout << "checking again" << endl;
    if (fs::exists(mypath)) {
        cout << "exists!" << endl;
    } else {
        cout << "error." << endl;
    }

    cout << "done." << endl;
    return 0;
}
