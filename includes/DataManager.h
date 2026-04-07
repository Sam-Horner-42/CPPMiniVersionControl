#ifndef DATAMANAGER_H
#define DATAMANAGER_H

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
    string singleHash(string fileContents);
    void saveData(string repositoryName);
    bool loadData(const string& repositoryName);
};

#endif