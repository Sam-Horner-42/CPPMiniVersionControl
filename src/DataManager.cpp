#include "../includes/DataManager.h"
#include "../includes/nlohmann/json.hpp"
//#include "../includes/Repository.h"


using json = nlohmann::json;
using namespace std;
namespace fs = std::filesystem;


string DataManager::singleHash(string fileContents){
    // unsigned long long hash = 14695981039346656037ULL;  // fnv-1a offset
    // unsigned long long prime = 1099511628211ULL; // fnv-1a prime
    // for (const auto& c : fileContents)
    //     hash = (hash ^ c) * prime;  // updated so that it just reads a fileContent string and applies hash

    // string result = to_string(hash);
    return "result";  
}

string DataManager::generateId(string repositoryName, vector<TrackedFile> files){

    // /*
    // using the FNV-1a hasing algorithm without extras
    // FowlerNollVo hash function, we need two magic large numbers(given I didn't make them), offset and prime
    // use them based off contents of the files, convert the hash to a string and take the first 10
    // https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
    // */
    // unsigned long long hash = 14695981039346656037ULL;  // fnv-1a offset
    // unsigned long long prime = 1099511628211ULL; // fnv-1a prime
    // for (const auto& file : files){
    //     string txtFilePath = "projects/" + repositoryName + "/" + file.getFileName();
    //     std::ifstream fileContentRead(txtFilePath, std::ios::binary);
    //     char c;
    //     while (fileContentRead.get(c)) {
    //         hash = (hash ^ c) * prime;  
    //     }
        
    // }
    // // Convert hash to string and take first 10 characters
    // string result = to_string(hash);
    // result = result.substr(0, 10);
    return "result";  
}
// USE maps, read from maps
// we take ONE commit now and we write that to files instead of old system of all commits

void DataManager::saveData(std::string& repositoryName, Commit& currentCommit) {
    // json jObj;
    // // files write
    // for (const auto& file : files){
    //     string txtFilePath = "projects/" + repositoryName + "/" + file.getFileName();
    //     ofstream txtFileWrite(txtFilePath);
    //         if (txtFileWrite.is_open()){
    //             for (auto& line : file.getFileContent()) {
    //             txtFileWrite << line << endl;
    //         }
    //             txtFileWrite.close();
    //         }
    // }
   
    // //metadata writing
    // // file info
    // time_t timestamp;
    // time(&timestamp);
    // for (const auto& file : files) {
    //     json tempObject;
    //     tempObject["fileName"] = file.getFileName();
    //     tempObject["filePath"] = file.getFilePath();
    //     tempObject["lastModified"] = ctime(&timestamp);
    //     jObj["files"].push_back(tempObject);
    // }
     
    // // commits info
    // // for (auto& commit : commits){
    // //     json TempObj;
    // //     TempObj["id"] = commit.getId();
    // //     TempObj["date"] = commit.getDate();
    // //     TempObj["message"] = commit.getMessage();
    // //     jObj["commits"].push_back(TempObj);

    // // }

    // // output
    // string metaDataPath = "projects/"+repositoryName+"/metadata.json";
    // ofstream metadataOut(metaDataPath);
    // if (metadataOut.is_open()) {
    //     metadataOut << jObj.dump(4);
    //     metadataOut.close();
    // }

    // // datahandler.json
    // json dataHandler;
    // string pathDataHandler = "data/dataHandler.json";

    // // check if exist
    // bool projectExists = false;
    // for (const auto& project : dataHandler["projects"]) {
    //     if (project["name"] == repositoryName) {
    //         projectExists = true;
    //         break;
    //     }
    // }
    
    // // if not existing, make
    // if (!projectExists) {
    //     json newProject;
    //     newProject["name"] = repositoryName;
    //     newProject["id"] = generateId(repositoryName, files);
    //     newProject["path"] = "projects/" + repositoryName + "/";
    //     dataHandler["projects"].push_back(newProject);
    // }

    // // out writing
    // ofstream dataHandOut(pathDataHandler);
    // if (dataHandOut.is_open()) {
    //     dataHandOut << dataHandler.dump(4);
    //     dataHandOut.close();
    // }
}


// use maps, now update shitshots and load whatever


bool loadData(std::string& repositoryName, Repository& repo) {
    // string pathDataHandler = "dataHandler.json";
    // std::ifstream in(pathDataHandler);
    // if (!in.is_open()) { 
    //         std::cerr << "cannot open json file\n"; // for testing, most console outputs are for US not output
    //         return false; 
    //     }
    
    // json handlerJson;
    // in >> handlerJson;
    // in.close();

    // // vars for file content
    // string projectPath;
    // string projectId;
    
    // // handles dataHandler content
    // bool foundStat=false;
    // if (handlerJson.contains("projects")){
    //     for (const auto& project : handlerJson["projects"]){
    //         if(project["name"]==repositoryName){
    //             projectPath=project["path"];
    //             projectId=project["id"];
    //             foundStat=true;
    //         }
    //     }
    // }
    // if(!foundStat){
    //     std::cerr << "Unable to find project"; // us output
    //     return false;
    // }

    // // handles the metadata.json file, after we know project exists
    // string metadataPath = projectPath + "/metadata.json";
    // ifstream metadataIn(metadataPath);

    // if (!metadataIn.is_open()) {
    //     cerr << "Cannot find metadata.json for project" << endl;
    //     return false;
    // }

    // try
    // {
    //     json projectMetadata;
    //     metadataIn >> projectMetadata;
    //     metadataIn.close();

    //     if(projectMetadata.contains("files")){
    //         for (const auto& file : projectMetadata["files"]){
    //             string fileName = file.value("fileName", "");
    //             string filePath = file.value("filePath", "");
    //             // Read the actual content from the txt file
    //             string content = "";
    //             ifstream txtFile(filePath);
    //             if (txtFile.is_open()) {
    //                std::stringstream buffer;
    //                 buffer << txtFile.rdbuf();
    //                 content = buffer.str();
    //                 txtFile.close();

    //                 // file obj creation
    //                 TrackedFile trackedFile(filePath, TrackedFile::status::Committed);
    //                 // gets commitvector from repo, goes back, then add file
    //                 // repo->getCommitVector.back()->addTrackedFile(trackedFile);
    //             } else {
    //                 cerr << "unable to read " << fileName << endl;
    //             }          
    //         }
    //     }
    //     // commits log update
    //     // for (auto& commitI : projectMetadata["commits"]){
    //     //     string id = commitI.value("id", "");
    //     //     string date = commitI.value("date", "");
    //     //     string msg = commitI.value("message","");
    //     //     auto commit = make_unique<Commit>(id, date, msg);
    //     //     commits.push_back(std::move(commit));
    //     // }
    //     // finish
    //     return true; 
    // }
    // catch(const exception& e)
    // {
    //     std::cerr << e.what() << "Bad json pass" << '\n';
    //     return false;
    // }
    return false;
}