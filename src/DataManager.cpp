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
    return result;  
}
// USE maps, read from maps
// we take the entire repository and save it to memory based on its current state
bool DataManager::checkProjectExist(std::string repositoryName){
    json dataHandler;
    string pathDataHandler = "data/dataHandler.json";

    ifstream dataHandIn(pathDataHandler);
    if (dataHandIn.is_open()) {
        dataHandIn >> dataHandler;
        dataHandIn.close();
    }
    // check if exist
    for (const auto& project : dataHandler["projects"]) {
        if (project["name"] == repositoryName) {
            return true;
        }
    }
    return false;
}
void DataManager::saveProjectInfo(std::string repositoryName, Repository& repo){
     // datahandler.json
    json dataHandler;
    string pathDataHandler = "data/dataHandler.json";
    
    ifstream dataHandIn(pathDataHandler);
    if (dataHandIn.is_open()) {
        dataHandIn >> dataHandler;
        dataHandIn.close();
    }
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
        newProject["path"] = repo.getRepoPath();
        dataHandler["projects"].push_back(newProject);
    }

    // out writing
    ofstream dataHandOut(pathDataHandler);
    if (dataHandOut.is_open()) {
        dataHandOut << dataHandler.dump(4);
        dataHandOut.close();
    }
}
void DataManager::saveData(Repository& repo, std::string repositoryName, const string repoPath) {
    string metaDataPath = repoPath + "/metadata.json";
    string commitPath = repoPath + "/vcm/snapshots/commits.json";
    
    json allCommitsData;
    ifstream commitDataIn(commitPath);
    if (commitDataIn.is_open()) {
        commitDataIn >> allCommitsData;
        commitDataIn.close();
    }
    
    auto& currentCommit = repo.getCurrentCommit();
    json newCommitJson = saveCommit(currentCommit, repositoryName, repoPath);
    
    // does not work fix
    allCommitsData["commits"].push_back(newCommitJson);
    
    ofstream commitDataOut(commitPath);
    if (commitDataOut.is_open()) {
        commitDataOut << allCommitsData.dump(4);
        commitDataOut.close();
    }
    
    json metaData;
    // ifstream metaDataIn(metaDataPath);
    // if (metaDataIn.is_open()) {
    //     metaDataIn >> metaData;
    //     metaDataIn.close();
    // }
    
    auto& files = repo.getCurrentFiles();
        
    for (auto& file : files) {
        metaData["files"].push_back(saveFile(file, repositoryName));
    }
    
    // Write metadata back
    ofstream metadataOut(metaDataPath);
    if (metadataOut.is_open()) {
        metadataOut << metaData.dump(4);
        metadataOut.close();
    }
    
    saveProjectInfo(repositoryName, repo);
}

json DataManager::saveCommit(StandardCommit& commit, std::string& repositoryName, const string repoPath) {
    json tempObject;
    tempObject["id"] = commit.getId();
    tempObject["parentId"] = commit.getParentId();
    tempObject["message"] = commit.getMessage();
    tempObject["timestamp"] = commit.getTimestamp();
    tempObject["author"] = commit.getAuthor();

    return tempObject;
}

json DataManager::saveFile(TrackedFile& file, string& repositoryName) {
    // Create new file entry
    json tempObject;
    tempObject["fileName"] = file.getFileName();
    tempObject["filePath"] = file.getFilePath();
    tempObject["status"] = file.getFileStatus();
    return tempObject;
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
                std::ifstream txtFile(repo.getRepoPath() + "/" + fileName);
                if (txtFile.is_open()) {
                    std::stringstream buffer;
                    buffer << txtFile.rdbuf();
                    content = buffer.str();
                    txtFile.close();
                }
				c = make_unique<StandardCommit>(
                    commitId,
                    parentId,
                    message,
                    author,
                    timestamp
                );
				// string filename = file.getFileName();
                auto& sc = dynamic_cast<StandardCommit&>(*c);
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