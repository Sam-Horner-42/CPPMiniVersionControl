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
    public:
    bool initRepository(std::string repoName);
    void addFile(const std::string& filepath);
    void stageFile(const std::string& filepath);
    bool commitChanges();
    void getCommitHistory();

    };