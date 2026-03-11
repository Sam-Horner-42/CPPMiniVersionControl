/*
 * Definitions for the diff engine
 * Author: Jacob Dawes - 041169788
 */

class DiffEngine {
  public:
    string computeDiff(const std::vector<std::string>& currentContent, const std::vector<std::string>& oldContent);
    void displayDiff();
};
