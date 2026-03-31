/*
 * Definitions for the diff engine
 * Author: Jacob Dawes - 041169788
 */

<<<<<<< HEAD
#ifndef DIFF_ENGINE_H
#define DIFF_ENGINE_H

class DiffEngine {
  public:
    string computeDiff(const std::vector<std::string>& currentContent, const std::vector<std::string>& oldContent);
    void displayDiff();
=======
#ifndef DIFFENGINE_H
#define DIFFENGINE_H

class DiffEngine {
  std::string diffString;
  public:
    void computeDiff(const std::vector<std::string>& currentContent, const std::vector<std::string>& oldContent);
    std::string displayDiff() const;
>>>>>>> 9fd035e (changed hpp to h, worked on repo and diffEngine classes - Jacob)
};

#endif
