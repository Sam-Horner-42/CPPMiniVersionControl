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

class DataManager {
public:
    void saveData(string repositoryName, vector<TrackedFile> files, vector<unique_ptr<Commit>> commits);
    bool loadData(const string& repositoryName, vector<TrackedFile>& files, vector<unique_ptr<Commit>>& commits);
    string generateId(string repositoryName, vector<TrackedFile> files);
};

#endif