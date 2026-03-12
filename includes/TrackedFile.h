
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class TrackedFile {

    public:
    void updateContent(string filePath, string status);
    void displayFileInfo();
    string getFileName();
    string getFilePath();

    private:
    string filePath;
    string fileName;
    vector<string> content;
    string status; //The status of a file will be Modified, Staged, or Committed, and i guess NULL when undefined

};