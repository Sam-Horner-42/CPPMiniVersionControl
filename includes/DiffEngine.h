/*
 * Definitions for the diff engine
 * Author: Jacob Dawes - 041169788
 */

#ifndef DIFF_ENGINE_H
#define DIFF_ENGINE_H

class DiffEngine {
  public:
    string computeDiff(const std::vector<std::string>& currentContent, const std::vector<std::string>& oldContent);
    void displayDiff();
};

#endif
