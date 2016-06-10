#ifndef EDAMI_FILEWRITER_H
#define EDAMI_FILEWRITER_H

#include <string>
#include <vector>
#include <list>
#include <fstream>

class FileWriter {
public:
    FileWriter(const std::string& fileName, const std::string& separator = ",");
    FileWriter* addString(const std::string &str);
    FileWriter* addDouble(const double &value);
    FileWriter* addInt(int value);
    FileWriter* nextLine();
    void writeToFile();

private:
    std::string fileName_;
    std::string separator_;

    typedef std::vector<std::string> Line;
    Line currentLine_;
    std::list<Line> linesToWrite_;

    bool fileExists(const std::string& fileName);
    void writeLine(std::ofstream &of, Line &line);
};

#endif //EDAMI_FILEWRITER_H
