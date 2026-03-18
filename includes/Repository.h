#pragma once;

/*
 * class definitions for the Repository class
 * Author: Jacob Dawes - 041169788, Spencer Scarlett 041151395, Ethan Geary 041032340
 */

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include <TrackedFile.h>
#include <Commit.h>

using namespace std;


class Repository {

    private:
    string repositoryName;
    vector<TrackedFile> files;
    vector<unique_ptr<Commit>> commits;
    enum class fileStatus {Added, Modified, Staged, Committed};
    
    public:
    
    Repository::Repository();
    Repository::~Repository();

    bool initRepository(const string& repoName);
    TrackedFile& Repository::getTrackedFile(const string& filepath); // helper class to get the tracked file used in addFile
    void addFile(const std::string& filepath);
    void stageFile(const std::string& filepath);
    bool commitChanges();
    vector<string> getCommitHistory();
    void updateFileStatus(TrackedFile& file, enum fileStatus);
    bool fileIsTracked(const string& filepath);
    };