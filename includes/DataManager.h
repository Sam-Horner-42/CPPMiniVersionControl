#pragma once

#include "TrackedFile.h"
#include "Commit.h"
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
    void saveData(string repositoryName,  vector<TrackedFile> files, vector<unique_ptr<Commit>> commits);
    bool loadData(const std::string& repositoryName, vector<TrackedFile> files, vector<unique_ptr<Commit>> commits);
};
