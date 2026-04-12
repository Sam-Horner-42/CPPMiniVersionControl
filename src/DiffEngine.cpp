/*
 * Implementation of the Diffing Engine
 * Authors:
 *  Jacob Dawes - 041169788
 */

#include <vector>
#include <iostream>
#include <sstream>

#include "../includes/StandardCommit.h"
#include "../includes/DiffEngine.h"

using namespace std;

void DiffEngine::computeDiff(const std::string& currentContent, const std::string& oldContent) {
    if(currentContent.empty() || oldContent.empty()) return;

    // split both strings into lines
    auto splitLines = [](const std::string& str) {
        std::vector<std::string> lines;
        std::istringstream stream(str);
        std::string line;
        while(getline(stream, line)) {
            lines.push_back(line);
        }
        return lines;
    };

    std::vector<std::string> currentLines = splitLines(currentContent);
    std::vector<std::string> oldLines = splitLines(oldContent);

    std::string result = "";
    int minSize = min(currentLines.size(), oldLines.size());

    for(auto i = 0; i < minSize; i++) {
        if(currentLines[i] != oldLines[i]) {
            linesModified++;
            result += "- " + oldLines[i] + "\n";
            result += "+ " + currentLines[i] + "\n";
        }
    }

    if(currentLines.size() > oldLines.size()) {
        for(auto i = minSize; i < currentLines.size(); i++) {
            linesAdded++;
            result += "+ " + currentLines[i] + "\n";
        }
    }
    else if(currentLines.size() < oldLines.size()) {
        for(auto i = minSize; i < oldLines.size(); i++) {
            linesRemoved++;
            result += "- " + oldLines[i] + "\n";
        }
    }

    result += "[+] Lines Added: " + to_string(linesAdded) + "\n";
    result += "[+] Lines Removed: " + to_string(linesRemoved) + "\n";
    result += "[+] Lines Modified: " + to_string(linesModified) + "\n";

    this->diffString = result;
}

std::string DiffEngine::displayDiff() const {
  return this->diffString;
}

