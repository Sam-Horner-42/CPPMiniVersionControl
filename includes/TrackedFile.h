
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class TrackedFile {

    public:
    void updateContent(string filePath);
    void displayFileInfo();
    string getFileName();

    private:
    string filePath;
    string fileName;
    string content;
    string status;

};