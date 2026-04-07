
#include "../includes/StandardCommit.h"
#include "../includes/DataManager.h"

// combination of all getters into a predicable ordered
// vector. ORDER: { ID, MESSAGE, AUTHOR, TIMESTAMP }
// @return: vector of commit data in order.
std::vector<std::string> StandardCommit::getAllAttributes() {
	std::vector<std::string> content;

	content.push_back(getId() + "\n");
	content.push_back(getMessage() + "\n");
	content.push_back(getAuthor() + "\n");
	content.push_back(getTimestamp() + "\n");

	return content;
}


void StandardCommit::createSnapshot(const std::string& filename, const std::vector<std::string>& content) {
	std::string contentstring;

	for (const auto& line : content) {
		contentstring += line + "\n";
	}

	fileSnapshot.insert({ filename,contentstring });
}

void StandardCommit::updateSnapshot(const std::string& filename, const std::vector<std::string>& content) {
	std::string contentstring;

	for (const auto& line : content) {
		contentstring += line + "\n";
	}
	fileSnapshot[filename] = contentstring;
}

// returns a full vector containing all commit info
std::vector<std::string> StandardCommit::displayCommit() {
	return this->getAllAttributes();
}

// returns a small summary commit id and message
std::string StandardCommit::getSummary() {
	return "Commit ID: " + getId() +
		" Parent ID: " + getParentId() +
		" Commit Message:\n" + getMessage();
}



bool StandardCommit::compareHashedFiles(TrackedFile comparingStagedFile) {
	//Mano-TODO: 
	//please make this function to compare the file hash of each file in the TrackedFiles vector
	//with the file hash of each file in the current commit of the repository
	//return true if hashes are different, false if hashes are the same
	DataManager manage;
	std::string currentText = "";
	for (const auto& line : comparingStagedFile.getFileContent()) {
		currentText += line;
	}
	std::string currentHash = manage.singleHash(currentText);

	std::string previousHash = "";
	if (fileSnapshot.count(comparingStagedFile.getFileName())) {
		std::string oldText = fileSnapshot[comparingStagedFile.getFileName()];
		previousHash = manage.singleHash(oldText);
	}
	return (currentHash != previousHash);
}

void StandardCommit::addToCommitVector(bool tf, TrackedFile addingStagedFile) {
	//TODO: This function is supposed to populate the Commit Vector
	// it will take the true or false from the compareHashedFiles
	// if true is passed in then it will add the staged file into the commit vector

	// if truefalse variable is true then add the file into the staged file vector
	if (tf == true) {
		commitVector.push_back(addingStagedFile);
	}
	// if truefalse variable is false then ignore that file
	else if (tf == false) {
		return;
	}

    fileSnapshot[filename] = contentstring;
}

void StandardCommit::createSnapshot(const string& filename, const vector<string>& content) {
    string contentstring;

    for (const auto& line : content) {
        contentstring += line + "\n";
    }

    filesnapshot.insert({filename,contentstring});
}

void StandardCommit::updateSnapshot(const string& filename, const vector<string>& content) {
    string contentstring;

    for (const auto& line : content) {
        contentstring += line + "\n";
    }
            filesnapshot[filename] = contentstring;
}

// returns a full vector containing all commit info
vector<string> StandardCommit::displayCommit() {
    return this->getAllAttributes();
}

// returns a small summary commit id and message
string StandardCommit::getSummary() {
    return "Commit ID: " + getId() +
    " Parent ID: " + getParentId() +
    " Commit Message:\n" + getMessage();
}

bool Commit::checkStagedFiles(std::vector<TrackedFile> fileVector) {
    //TODO: Check TrackedFile vector for which files are staged
    // it should iterate through the whole vector to make sure everything is staged
    // if a file is not staged then we return a false value

    //get vector size for the looping
    int vectorSize = fileVector.size();

    //for loop to iterate through vector
    for (int i = 0; i < vectorSize; i++) {
        // check each file in the vector for if it is staged
        // if not every file is staged then return false and abort the current commit
        // the false return will cause the popup for listing every file that isnt staged and give the user the prompt for if they wish to try and stage those files, if that succeeds it will try to do a new commit
        if (fileVector[i].getFileStatus() != TrackedFile::status::Staged) {
            Commit::clearCommitVector();
            return false;
            break;
        }

        // pass the current iterated file through compareHashedFiles to check
        // nomatter true or false it is a parameter to addToCommitVector
        Commit::addToCommitVector(compareHashedFiles(fileVector[i]), fileVector[i]);
        return true;
    }
}

bool Commit::compareHashedFiles(TrackedFile comparingStagedFile) {
    //Mano-TODO: 
    //please make this function to compare the file hash of each file in the TrackedFiles vector
    //with the file hash of each file in the current commit of the repository
    //return true if hashes are different, false if hashes are the same
    //current file hash
    string currentText = "";
    for (const auto& line : comparingStagedFile.getFileContent()) {
        currentText += line;
    }
    string currentHash = DataManager::singleHash(currentText);

    string previousHash = "";
    if (filesnapshot.count(comparingStagedFile.getFileName())) {
        string oldText = filesnapshot[comparingStagedFile.getFileName()];
        previousHash = DataManager::singleHash(oldText);
    }
return (currentHash != previousHash);
}

void Commit::addToCommitVector(bool tf, TrackedFile addingStagedFile) {
    //TODO: This function is supposed to populate the Commit Vector
    // it will take the true or false from the compareHashedFiles
    // if true is passed in then it will add the staged file into the commit vector

    // if truefalse variable is true then add the file into the staged file vector
    if (tf == true) {
        commitVector.push_back(addingStagedFile);
    }
    // if truefalse variable is false then ignore that file
    else if (tf == false) {
        return;
    }

}

void Commit::clearCommitVector() {
    //TODO: This function will be called to clear out the commit vector entirely
    commitVector.clear();
}

// this function is a getter for the commit vector. The commit vector is just a vector of the tracked files that are specifically getting commit into the repo.
// How I have the committing set up is just that i have a few functions that narrow down the tracked files vector to just transfer staged files that have a different hash to the repo file
// this commitVector just stores those files that have a different hash to the repository old version, and then they all have the status set to Commit.
// its entirely used for another function to get that commitVector during the commit.

// for spencer, what i was thinking was just you take the commitVector, which holds each file object being commit, and put that into the repo as the intended txt structure
std::vector<TrackedFile> StandardCommit::getCommitVector() const {
	return commitVector;
}

bool StandardCommit::hasFile(const std::string& filename) {
	auto it = fileSnapshot.find(filename);
	return it != fileSnapshot.end() ? true : false;
}