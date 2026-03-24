#include "../includes/Commit.h"

using namespace std;
//functions
void Commit::displayCommit() {
    //TODO: DISPLAY COMMIT
}

void Commit::getSummary() {
    //TODO: GET COMMIT SUMMARY
}

bool Commit::checkStagedFiles(vector<TrackedFile> filesVector) {
    //TODO: Check TrackedFile vector for which files are staged
    // it should iterate through the whole vector to make sure everything is staged
    // if a file is not staged then we return a false value
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
    
}

void Commit::clearCommitVector() {
    //TODO: This function will be called to delete and reinitialize the commit vector entirely

}

vector<TrackedFile> Commit::commitToRepo() {
    //TODO: This function will take the commit vector and fully commit the files into the repository

}

//functions to be used in Repository.cpp
string const Commit::getId() {
    return commitID;
}

string const Commit::getDate() {
    return timestamp;
}

vector<TrackedFile> const Commit::getFiles() {
    return files;
}

string const Commit::getMessages() {
    return message;
}