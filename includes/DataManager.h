#pragma once

#include "TrackedFile.h"
#include "StandardCommit.h"
#include "Repository.h"
#include "../includes/nlohmann/json.hpp"

using json = nlohmann::json;

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
    void saveData(Repository& repo, std::string repositoryName, const std::string repoPath);
    bool loadData(std::string& repositoryName, Repository& repo);
	json saveCommit(StandardCommit& commit, std::string& repositoryName, const std::string repoPath);
	json saveFile(TrackedFile& file,std::string& repositoryName);
    void saveProjectInfo(std::string repositoryName, Repository& repo);
    bool checkProjectExist(std::string repositoryName);
};