#include "../includes/DataManager.h"


using json = nlohmann::json;
using namespace std;
namespace fs = std::filesystem;

std::string DataManager::singleHash(std::string fileContents){
    std::string result;
    // using the FNV-1a hasing algorithm without extras
    // FowlerNollVo hash function, we need two magic large numbers(given I didn't make them), offset and prime
    // use them based off contents of the files, convert the hash to a string and take the first 10
    // https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
	unsigned long long hash = 14695981039346656037ULL;  // fnv-1a offset
    unsigned long long prime = 1099511628211ULL; // fnv-1a prime
    for (const auto& c : fileContents)
        hash = (hash ^ c) * prime;  // updated so that it just reads a fileContent string and applies hash

    result = to_string(hash);
    return result;  
}

string DataManager::generateId(string repositoryName, std::vector<TrackedFile> files){
	// creates a timestamp as a string
	std::time_t currentTime = std::time(nullptr);
	long longTime = long(currentTime);
    unsigned long long hash = 14695981039346656037ULL;  // fnv-1a offset, not based within the FNV
	unsigned long res = hash ^ longTime;
    // Convert hash to string and take first 10 characters
    string result = to_string(res);
    result = result.substr(0, 10);
    return result;  
}

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
    // removes new line char, adds real ID
	std::time_t currentTime = std::time(nullptr);
	std::string realTime = std::ctime(&currentTime);
	realTime.pop_back();
    
    // if not existing, make
    if (!projectExists) {
        json newProject;
        
        newProject["name"] = repositoryName;
        newProject["id"] = realTime;
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
    string commitPath = repoPath + "/.vcm/snapshots/commits.json";
    
    json existingData;
    json allCommitsData;

     ifstream commitDataIn(commitPath);
     if (commitDataIn.is_open()) {
         commitDataIn >> existingData;
         commitDataIn.close();
     }
    
    for (auto& commit : repo.getRepoCommits()) {
        if(!commit || existingData.contains(dynamic_cast<StandardCommit&>(*commit.get()).getId())) continue;

        json newCommitJson = saveCommit(dynamic_cast<StandardCommit&>(*commit.get()), repositoryName, repoPath);
        allCommitsData["commits"].push_back(newCommitJson);
    }
    
    ofstream commitDataOut(commitPath);
    if (commitDataOut.is_open()) {
        commitDataOut << allCommitsData.dump(4);
        commitDataOut.close();
    }
    
    json metaData;
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

    createCommitDirectory(commit.getId(), commit.getFileSnapshot(), repoPath);
    return tempObject;
}

// snapshots handling/creation
void DataManager::createCommitDirectory(const string& ident, std::map<string,string>& map, std::string repoPath) {
    string dirpath = repoPath + "/.vcm/snapshots/" + ident; // snap folder creation
    if(!fs::create_directories(dirpath)) return;
    for (auto it = map.begin(); it != map.end(); ++it) {
        ofstream outFile(dirpath + "/" + ident + "_" + it->first);
        outFile << it->second;
        outFile.close();
    }
}

json DataManager::saveFile(TrackedFile& file, string& repositoryName) {
    // Create new file entry
    json tempObject;
    tempObject["fileName"] = file.getFileName();
    tempObject["filePath"] = file.getFilePath();
    tempObject["status"] = file.getFileStatus();
    tempObject["edit_count"] = file.getEditCount();
    return tempObject;
}

// use maps, now update snaps and load whatever
bool DataManager::loadData(std::string& repositoryName, Repository& repo) {
	qDebug() << "Load data is running.";
    string pathDataHandler = "data/dataHandler.json";
    std::ifstream in(pathDataHandler);
    if (!in.is_open()) { 
            qDebug() << "cannot open json file"; // for testing, most console outputs are for US not output
            return false; 
        }
    qDebug() << "Datahandler is open";
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
        qDebug() << "Unable to find project"; // us output
        return false;
    }

    repo.setRepoPath(projectPath);

    // handles the metadata.json file, after we know project exists
    string metadataPath = projectPath + "/metadata.json";
	qDebug() << "Meta data path: " << metadataPath;
    ifstream metadataIn(metadataPath);

    if (!metadataIn.is_open()) {
        qDebug() << "Cannot find metadata.json for project";
        return false;
    }

    try
    {
        json projectMetadata;
        metadataIn >> projectMetadata;
        metadataIn.close();

        if(projectMetadata.contains("files")){
            for (const auto& file : projectMetadata["files"]){
                string fileName = file.value("fileName",   "NA");
                string filePath = file.value("filePath",   "NA");
				int fileStatus = file.value("status",   0);
                int editCount = file.value("edit_count", 0);
                
                // file obj creation
                TrackedFile trackedFile(
                    filePath,
                    fileName, 
                    static_cast<TrackedFile::status>(fileStatus), 
                    editCount
                );

				qDebug() << "File Name: " << fileName;
                repo.addToCurrentFiles(trackedFile);   
            }
        }
        
        std::string filePathing = projectPath + "/.vcm/snapshots/commits.json";
        std::ifstream snapsIn(filePathing);
        if (!snapsIn.is_open()) {
            qDebug() << "Failed to open: " << filePathing;
            return false;
        }

		// Check if the file is completely empty before trying to parse
		if (snapsIn.peek() == std::ifstream::traits_type::eof()) {
			qDebug() << "commits.json is completely empty. No commits to load.";
			return false; // or return true, depending on what your function expects for a blank repo
		}
        
        json snapshotsData;
		try {
			snapsIn >> snapshotsData;
		}
		catch (const nlohmann::json::parse_error& e) {
			// Catches malformed JSON so your app doesn't crash
			qDebug() << "JSON parse error in commits.json: " << e.what();
			return false;
		}

		// Check if the parsed JSON is just the literal "null" or an empty array/object
		if (snapshotsData.is_null() || snapshotsData.empty()) {
			qDebug() << "commits.json is null or empty. No commits to load.";
			return false;
		}

        snapsIn.close();
		std::string commitId;

        for (auto& commiti : snapshotsData["commits"]){
			qDebug() << "The commits loop is running.";

			// Extract metadata directly from the current commit object
			commitId = commiti["id"];
			std::string timestamp = commiti["timestamp"];
			std::string parentId = commiti["parentId"];
			std::string author = commiti["author"];
			std::string message = commiti["message"];

			// Add the commit using the parameterized function
			repo.addCommit(commitId, parentId, message, author, timestamp);
        }

		std::string commitDirPath = repo.getRepoPath() + "/.vcm/snapshots/" + commitId;

		// Traverse the directory and populate the fileSnapshot map
		if (fs::exists(commitDirPath) && fs::is_directory(commitDirPath)) {

			for (const auto& entry : fs::directory_iterator(commitDirPath)) {
				// Ensure we are only reading actual files, not sub-directories
				if (fs::is_regular_file(entry.status())) {

					std::string fileName = entry.path().filename().string();
					std::string filePath = entry.path().string();
					std::string fileContent = "";

					// Read actual file content
					std::ifstream txtFile(filePath);
					if (txtFile.is_open()) {
						std::stringstream buffer;
						buffer << txtFile.rdbuf();
						fileContent = buffer.str();
						txtFile.close();
					}
					else {
						qDebug() << "Failed to open file:" << filePath;
					}

					// Add to your snapshot map
					repo.findCommit(commitId)->getFileSnapshot()[fileName] = fileContent;

					// Note: If fileSnapshot is specific to the commit (e.g., inside a standardCommit object), 
					// you would apply it here instead of a global/local map.
					qDebug() << "Added file to snapshot:" << fileName;
				}
			}
			return true;

		}
    }

    catch(const exception& e)
    {
        qDebug() << e.what() << "Bad json pass" << '\n';
        return false;
    }
    return false;
}