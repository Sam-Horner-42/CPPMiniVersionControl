/*
 * Implementation of the Diffing Engine
 * Authors:
 *  Jacob Dawes - 041169788
 */

#include <vector>
#include <iostream>

#include "../includes/Commit.h"
#include "../includes/DiffEngine.h"

using namespace std;

void DiffEngine::computeDiff(const vector<string>& currentContent, const vector<string>& oldContent) {
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

  // set computed diff string
  this->diffString = diffString;
}

<<<<<<< HEAD
// Commented out this method because it was not different from header function
/*
std::vector<std::string> DiffEngine::displayDiff() {
  // not sure how to structure this yet.
    cout << "Diff";
=======
string DiffEngine::displayDiff() {
  return this->diffString;
>>>>>>> 9fd035e (changed hpp to h, worked on repo and diffEngine classes - Jacob)
}
*/

