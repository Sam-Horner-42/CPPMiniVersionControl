#include "../includes/DataManager.h"

#include <iostream>
#include <fstream>


using namespace std;

void DataManager::saveData(string repoName,  vector<TrackedFile> files, vector<unique_ptr<Commit>> commits) {
    json object;
    object["repoName"] = repoName;
    // TODO: Add ways to dump ALL values related to both vectors into the json objects
    json filesJson = json::array();
    for (const auto& file : files) {
        json tempObject;
        object["filePath"] = file->getFilePath();
        object["fileName"] = file->getFileName();
        object["content"] = file->getContent();
        object["status"] = file->getStatus();
        filesJson.push_back(std::move(tempObject));
    }
    object["filesTracked"] = std::move(filesJson);

    json commitsJson = json::array();
    for (const auto& commit : commits) {
        json tempObject;
        object["id"] = commit->getId();
        object["data"] = commit->getDate();
        object["files"] = commit->getFiles();
        object["messages"] = commit->getMessages();
        commitsJson.push_back(std::move(tempObject));
    }
    object["filesTracked"] = std::move(commitsJson);

    // placeholder file to write to, will be replaced with variable input
    // to allow dynamic repos to be saved
    std::ofstream out("data.json");
    if (out.is_open()) {
        out << std::setw(4) << object << std::endl;
        out.close();
    } else {
        std::cerr << "cannot open json file for writing\n";
    }
}

bool DataManager::loadData() {
    // placeholder file to open, will be replaced with variable input
    // to allow dynamic repos to be loaded
    std::ifstream in("data.json");
    // load
    if (!in.is_open()) { 
        std::cerr << "cannot open json file\n"; return false; 
    }
    // placeholder json parsing that serves to test/view json objects, 
    // doesn't work in current state, will be replaced with actual data loading logic
    try {
        json j;
        in >> j;
        cout << j.dump(4) << endl;
    } catch (const std::exception& e) {
        std::cerr << "error parsing json: " << e.what() << "\n";
        return false;
    }
    
    return true;

}