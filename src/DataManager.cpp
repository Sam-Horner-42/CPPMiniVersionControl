#include "../includes/DataManager.h"


using json = nlohmann::json;
using namespace std;
namespace fs = std::filesystem;

std::string DataManager::singleHash(std::string fileContents){
    std::string result;
	unsigned long long hash = 14695981039346656037ULL;  // fnv-1a offset
    unsigned long long prime = 1099511628211ULL; // fnv-1a prime
    for (const auto& c : fileContents)
        hash = (hash ^ c) * prime;  // updated so that it just reads a fileContent string and applies hash

    result = to_string(hash);
    return result;  
}

string DataManager::generateId(string repositoryName, std::vector<TrackedFile> files){
    // using the FNV-1a hasing algorithm without extras
    // FowlerNollVo hash function, we need two magic large numbers(given I didn't make them), offset and prime
    // use them based off contents of the files, convert the hash to a string and take the first 10
    // https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
    unsigned long long hash = 14695981039346656037ULL;  // fnv-1a offset
    unsigned long long prime = 1099511628211ULL; // fnv-1a prime
    for (const auto& file : files){
        string txtFilePath = "projects/" + repositoryName + "/" + file.getFileName();
        std::ifstream fileContentRead(txtFilePath, std::ios::binary);
        char c;
        while (fileContentRead.get(c)) {
            hash = (hash ^ c) * prime;  
        }
        
    }
    // Convert hash to string and take first 10 characters
    string result = to_string(hash);
    result = result.substr(0, 10);
    return "result";  
}
// USE maps, read from maps
// we take the entire repository and save it to memory based on its current state
void DataManager::saveData(Repository& repo, std::string& repositoryName) {

    json jObj;

    auto& commits = repo.getRepoCommits();

    for (auto& c : commits) {
        auto& sc = static_cast<StandardCommit&>(*c);
        jObj["commit"].push_back(saveCommit(sc,repositoryName));
    }

    // datahandler.json
    json dataHandler;
    string pathDataHandler = "data/dataHandler.json";

    // check if exist
    bool projectExists = false;
    for (const auto& project : dataHandler["projects"]) {
        if (project["name"] == repositoryName) {
            projectExists = true;
            break;
        }
    }
    
    // if not existing, make
    if (!projectExists) {
        json newProject;
        newProject["name"] = repositoryName;
        newProject["id"] = generateId(repositoryName, repo.getCurrentFiles());
        newProject["path"] = "projects/" + repositoryName + "/";
        dataHandler["projects"].push_back(newProject);
    }

    // out writing
    ofstream dataHandOut(pathDataHandler);
    if (dataHandOut.is_open()) {
        dataHandOut << dataHandler.dump(4);
        dataHandOut.close();
    }
}

// Function to save all the commits to different directories
// The commit hash will be the name of the folder
// The commit snapshot will be saved by commit
json DataManager::saveCommit(StandardCommit& commit, std::string& repositoryName) {
    json commitData;
    commitData["id"] = commit.getId();
    commitData["parentId"] = commit.getParentId();
    commitData["message"] = commit.getMessage();
    commitData["timestamp"] = commit.getTimestamp();
    commitData["author"] = commit.getAuthor();

    for (auto& file : commit.getIncomingFiles()) {
        commitData["files"].push_back(saveFile(file,repositoryName));
    }

    return commitData;
}

json DataManager::saveFile(TrackedFile& file,string& repositoryName) {
    json jObj;
   
    time_t timestamp;
    time(&timestamp);
    json tempObject;
    tempObject["fileName"] = file.getFileName();
    tempObject["filePath"] = file.getFilePath();
    tempObject["lastModified"] = ctime(&timestamp);
    tempObject["status"] = file.getFileStatus();
    jObj["files"].push_back(tempObject);
    
    string metaDataPath = "projects/"+ repositoryName +"/metadata.json";
    ofstream metadataOut(metaDataPath);
    if (metadataOut.is_open()) {
        metadataOut << jObj.dump(4);
        metadataOut.close();
    }
    return jObj;
}


// use maps, now update shitshots and load whatever
bool DataManager::loadData(std::string& repositoryName, Repository& repo) {
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

                    // file obj creation
                    TrackedFile trackedFile(filePath, TrackedFile::status::Committed);
                    // gets commitvector from repo, goes back, then add file
                    // repo->getCommitVector.back()->addTrackedFile(trackedFile);
                } else {
                    cerr << "unable to read " << fileName << endl;
                }          
            }
        }
        


        std::string filePathing = projectPath + "/snapshots/commits.json";
        std::ifstream snapsIn(filePathing);
        if (!snapsIn.is_open()) {
            std::cerr << "Failed to open: " << filePathing << std::endl;
            return false;
        }
        
        json snapshotsData;
        snapsIn >> snapshotsData;
        snapsIn.close();
        
        for (auto& commiti : snapshotsData["commits"]){
            std::unique_ptr<StandardCommit> c;
            for (auto& file : commiti["files"]){
                string commitId = commiti["id"];
                string timestamp = commiti["timestamp"];
                string parentId = commiti["parentId"];
                string author = commiti["author"];
                string message = commiti["message"];
                string fileName = file["fileName"];
                
                // Read actual file content
                std::string content = " ";
                std::ifstream txtFile("snapshot/" + commitId + "/" + fileName);
                if (txtFile.is_open()) {
                    std::stringstream buffer;
                    buffer << txtFile.rdbuf();
                    content = buffer.str();
                    txtFile.close();
                }
                auto& sc = dynamic_cast<StandardCommit&>(*c);
				c = make_unique<StandardCommit>(
                    commitId,
                    parentId,
                    message,
                    author,
                    timestamp
                );
				// string filename = file.getFileName();
                sc.addToSnapshot(fileName, content);
            }
            repo.getRepoCommits().push_back(move(c));
        }
        return true; 
    }
    catch(const exception& e)
    {
        std::cerr << e.what() << "Bad json pass" << '\n';
        return false;
    }
    return false;
}