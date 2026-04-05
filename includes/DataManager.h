#include <TrackedFile.h>
#include <Commit.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <sstream>
#include <filesystem>
#include <ctime>
#include <string>

class DataManager {
    public:
    void saveData(string repositoryName,  vector<TrackedFile> files, vector<unique_ptr<Commit>> commits);
    bool loadData(const std::string& repositoryName, vector<TrackedFile> files, vector<unique_ptr<Commit>> commits);
    string generateId(string repositoryName, vector<TrackedFile> files);
};