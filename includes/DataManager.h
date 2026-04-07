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

// Added to .h
class DataManager {
public:
    std::string generateId(std::string repositoryName, std::vector<TrackedFile> files);
    std::string singleHash(std::string fileContents);
    void saveData(std::string repositoryName);
    bool loadData(const std::string& repositoryName);
};

#endif