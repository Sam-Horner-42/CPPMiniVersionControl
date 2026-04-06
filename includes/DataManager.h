#pragma once

#include <TrackedFile.h>
#include <Commit.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <sstream>
#include <ctime>
#include <string>
#include <vector>
#include <memory>

using namespace std;

// Added to .h
class DataManager {
public:
    string generateId(string repositoryName, vector<TrackedFile> files);
    void saveData(string repositoryName, vector<TrackedFile> files, vector<unique_ptr<Commit>> commits);
    bool loadData(const string& repositoryName, vector<TrackedFile>& files, vector<unique_ptr<Commit>>& commits);
};

#endif