/*
 * Implementation of the Diffing Engine
 * Authors:
 *  Jacob Dawes - 041169788
 */

#include <vector>
#include <iostream>

#include "../includes/StandardCommit.h"
#include "../includes/DiffEngine.h"

using namespace std;

void DiffEngine::computeDiff(const vector<string>& currentContent, const vector<string>& oldContent) {
    if(currentContent.empty() || oldContent.empty()) return;

    this->diffString = "";

    int minSize = min(currentContent.size(), oldContent.size());
    string diffString = "";

    for(auto i = 0; i < minSize; i++) {
        if(currentContent[i] != oldContent[i]) {
            linesModified++;
            diffString += "- " + oldContent[i] + "\n";
            diffString += "+ " + currentContent[i] + "\n";
        }
    }

    if(currentContent.size() > oldContent.size()) {
        for(auto i = minSize; i < currentContent.size(); i++) {
            linesAdded++;
            diffString += "+ " + currentContent[i] + "\n";
        }
    }
    else if(currentContent.size() < oldContent.size()) {
        for(auto i = minSize; i < oldContent.size(); i++) {
            linesRemoved++;  
            diffString += "- " + oldContent[i] + "\n"; 
        }
    }

    diffString += "[+] Lines Added: " + to_string(linesAdded) + "\n";
    diffString += "[+] Lines Removed: " + to_string(linesRemoved) + "\n";
    diffString += "[+] Lines Modified: " + to_string(linesModified) + "\n";

    this->diffString = diffString;
}

std::string DiffEngine::displayDiff() const {
  return this->diffString;
}

