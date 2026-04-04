#include "../includes/DataManager.h"
#include "../includes/nlohmann/json.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <sstream>


using json = nlohmann::json;

using namespace std;

void DataManager::saveData(string repositoryName,  vector<TrackedFile> files, vector<unique_ptr<Commit>> commits) {
    json object;
    object["repositoryName"] = repositoryName;
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

bool loadData(const std::string& repositoryName, vector<TrackedFile> files, vector<unique_ptr<Commit>> commits) {
    string pathDataHandler = "dataHandler.json";
    std::ifstream in(pathDataHandler);
    if (!in.is_open()) { 
            std::cerr << "cannot open json file\n"; // for testing, most console outputs are for US not output
            return false; 
        }
    
    json handlerJson;
    in >> handlerJson;
    in.close();

    // vars for file content
    string projectPath;
    string projectId;

    // handles dataHandler content
    bool foundStat=false;
    if (handlerJson.contains("projects")){
        for (const auto& project : handlerJson["projects"]){
            if(project["name"]==repositoryName){
                projectPath=project["path"];
                projectId=project["id"];
                foundStat=true;
            }
        }
    }
    if(!foundStat){
        std::cerr << "Unable to find project"; // us output
        return false;
    }

    // handles the metadata.json file, after we know project exists
    string metadataPath = projectPath + "/metadata.json";
    ifstream metadataIn(metadataPath);

    if (!metadataIn.is_open()) {
        cerr << "Cannot find metadata.json for project" << endl;
        return false;
    }

    try
    {
        json projectMetadata;
        metadataIn >> projectMetadata;
        metadataIn.close();
        if(projectMetadata.contains("files")){
            for (const auto& file : projectMetadata["files"]){
                string fileName = file.value("fileName", "");
                string filePath = file.value("filePath", "");
                // Read the actual content from the txt file
                string content = "";
                ifstream txtFile(filePath);
                if (txtFile.is_open()) {
                   std::stringstream buffer;
                    buffer << txtFile.rdbuf();
                    content = buffer.str();
                    txtFile.close();
                } else {
                    cerr << "unable to read " << fileName << endl;
                }
                
                // Create TrackedFile object and add to vector, not sure build order
                TrackedFile trackedFile(fileName, filePath, content, NULL);
                files.push_back(trackedFile);
            }
        }
        // commits log
        for (const auto& commitI : projectMetadata["commits"]){
            int id = commitI.value("id", 0);
            string date = commitI.value("date", "");
            string msg = commitI.value("message","");
            auto commit = make_unique<Commit>(id, date, msg);
            commits.push_back(std::move(commit));
        }
        // finish
        return true; 
    }
    catch(const exception& e)
    {
        std::cerr << e.what() << "Bad json pass" << '\n';
        return false;
    }
}