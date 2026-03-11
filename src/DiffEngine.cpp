/*
 * Implementation of the Diffing Engine
 * Authors:
 *  Jacob Dawes - 041169788
 */

#include <vector>
#include "../includes/Commit.hpp"

using namepsace std;

string DiffEngine::computeDiff(const vector<string>& currentContent, const vector<string>& oldContent) {
  // defensive checks
  if(currentContent.size() < 1 || oldContent.size() < 1) return nullptr;

  // find the smallest file for diffing
  int minSize = currentContent.size() < oldContent.size() ? currentContent.size() : oldContent.size();
  int linesModified = 0;

  string diffString = "";

  // compute the diff
  for(auto i = 0; i < minSize; i++) {
    if(currentContent[i] != oldContent[i]) {
      linesModified++;
      diffString += "- " + oldContent[i] + "\n";
      diffString += "+ " + currentContent[i] + "\n";
    }
  }

  // check for different sized files
  // indicate add or removal depending on the file version.
  if(currentContent.size() > oldContent.size()) {
    for(auto i = minSize; i < currentContent.size(); i++) {
      diffString += "+ " + currentContent[i] + "\n";
    }
  }
  else if(currentContent.size() < oldContent.size()) {
    for(auto i = minSize; i < oldContent.size(); i++) {
      diffString += "- " + currentContent[i] + "\n";
    }
  }

  // return computed diff string
  return diffString;
}

vector<string> DiffEngine::displayDiff() {
  // not sure how to structure this yet.
}
