/*
 * Definitions for the diff engine
 * Author: Jacob Dawes - 041169788
 */

#ifndef DIFFENGINE_H
#define DIFFENGINE_H

class DiffEngine {
  std::string diffString;
  public:
    void computeDiff(const std::vector<std::string>& currentContent, const std::vector<std::string>& oldContent);
    std::string displayDiff() const;
};

#endif
