#include "../includes/DataManager.h"
#include "../includes/nlohmann/json.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <sstream>
#include <filesystem>
#include <ctime>

using json = nlohmann::json;

using namespace std;
namespace fs = std::filesystem;


void DataManager::saveData(string repositoryName,  vector<TrackedFile> files, vector<unique_ptr<Commit>> commits) {

    if (!fs::exists("projects/" + repositoryName)){
        fs::create_directories("projects/" + repositoryName);
    }

    
    // files
    for (auto& file : files){
        string txtFilePath = "projects/" + repositoryName + "/" + file.getFileName();
        ofstream txtFileWrite(txtFilePath);
            if (txtFileWrite.is_open()){
                txtFileWrite << file.getFileContent();
                txtFileWrite.close();
            }
    }
    
    // json
    std::ofstream metadataJson("metadata.json");
    json projectMetadata;
    string metaDataPath = "projects/"+repositoryName+"/" + "metadata.json";

    ifstream metadataIn(metaDataPath);
    if (metadataIn.is_open()) {
        metadataIn >> projectMetadata;
        metadataIn.close();
    }
    // write files
    projectMetadata["repositoryName"] = repositoryName;
    json filesMetadata = json::array();
    time_t timestamp;
    time(&timestamp);
    for (auto& file : files) {
        json tempObject;
        tempObject["fileName"] = file.getFileName();
        tempObject["filePath"] = file.getFilePath();
        tempObject["lastModified"] = ctime(&timestamp);
        filesMetadata.push_back(std::move(tempObject));
    }
    projectMetadata["files"] = filesMetadata;

    // commits
    json commitsData = json::array();
    for (auto& commit : commits){
        json TempObj;
        TempObj["id"] = commit.getId();
        TempObj["date"] = commit.getDate();
        TempObj["message"] = commit.getMessage();
        commitsData.push_back(std::move(TempObj));
    }
       projectMetadata["commits"] = commitsData;
    // metadata saving
    ofstream metadataOut(metaDataPath);
    if (metadataOut.is_open()) {
        metadataOut << setw(4) << projectMetadata << endl;
        metadataOut.close();
    }
    // datahandler.json
    json dataHandler;
    string pathDataHandler = "dataHandler.json";

    ifstream dataHandlerIn(pathDataHandler);
    if(dataHandlerIn.is_open()) {
        metadataIn >> dataHandler;
        metadataIn.close();
    }

    json projectItem;
    projectItem["name"] = repositoryName;
    projectItem["id"] = 12345; // generateId(repositoryName); i need to back something for ids
    projectItem["path"] = "projects/" + repositoryName;
    
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
        // commits log update
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