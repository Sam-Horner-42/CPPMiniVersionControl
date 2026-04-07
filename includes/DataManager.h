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

// Added to .h
class DataManager {
public:
    std::string DataManager::singleHash(std::string repositoryName, std::string fileName);
    std::string generateId(std::string repositoryName, std::vector<TrackedFile> files);
    void saveData(std::string repositoryName);
    bool loadData(const std::string& repositoryName);
};

#endif