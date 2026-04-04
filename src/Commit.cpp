#include "../includes/Commit.h"

//functions
void Commit::displayCommit() {
    //TODO: DISPLAY COMMIT
}

void Commit::getSummary() {
    //TODO: GET COMMIT SUMMARY
}

bool Commit::checkStagedFiles(vector<TrackedFile> fileVector) {
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
        if (fileVector[i].getFileStatus() != "Staged") {
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

// for Sam
// the when we are fully confirming the commit to be pushed to the repository we will call this function
// as of right now it stores all the committed file names into a string vector and passes that back since jacob had suggested for that to be the case
// i will change this function as needed since im sure there must be more to it than this
vector<string> Commit::commitToRepo(vector<TrackedFile> commitFiles) {
    //TODO: This function will take the commit vector and fully commit the files into the repository

    //apparently it should for now just return all the names from the commitfiles vector

    int commitSize = commitFiles.size();

    vector<string> commitNames;

    for (int i = 0; i < commitSize; i++) {
        commitNames.push_back(commitFiles[i].getFileName());
    }

    return commitNames;
}

//functions to be used in Repository.cpp
std::string const Commit::getId() {
    return commitId;
}

std::string const Commit::getDate() {
    return timestamp;
}

std::vector<TrackedFile> const Commit::getFiles() {
    return files;
}

std::string const Commit::getMessages() {
    return message;
}