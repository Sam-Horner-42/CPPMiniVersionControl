/*
 * Definitions for the diff engine
 * Author: Jacob Dawes - 041169788
 */
#include <string>
#include <vector>

#ifndef DIFFENGINE_H
#define DIFFENGINE_H

class DiffEngine {
  std::string diffString;
  int linesModified = 0;
  int linesAdded = 0;
  int linesRemoved = 0;
  public:
    void computeDiff(const std::string& currentContent, const std::string& oldContent);
    std::string displayDiff() const;
};

#endif
