#include "../includes/StandardCommit.h"

using namespace std;

// combination of all getters into a predicable ordered
// vector. ORDER: { ID, MESSAGE, AUTHOR, TIMESTAMP }
// @return: vector of commit data in order.
vector<string> StandardCommit::getAllAttributes() {
    vector<string> content;

    content.push_back(getId() + "\n");
    content.push_back(getMessage() + "\n");
    content.push_back(getAuthor() + "\n");
    content.push_back(getTimestamp() + "\n");

    return content;
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

void StandardCommit::createSnapshot(const string& filename, const vector<string>& content) {
    string contentstring;

    for (const auto& line : content) {
        contentstring += line + "\n";
    }

    fileSnapshot.insert({filename, contentstring});
}

void StandardCommit::updateSnapshot(const string& filename, const vector<string>& content) {
    string contentstring;

    for (const auto& line : content) {
        contentstring += line + "\n";
    }

    fileSnapshot[filename] = contentstring;
}

bool StandardCommit::hasFile(const string& filename) {
    auto it = fileSnapshot.find(filename);
    return it != fileSnapshot.end() ? true : false;
}