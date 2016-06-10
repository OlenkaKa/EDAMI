#include "FileWriter.h"

using namespace std;

FileWriter::FileWriter(const std::string &fileName, const std::string &separator)
        : fileName_(fileName), separator_(separator)
{
    if(fileExists(fileName)) {
        remove(fileName.c_str());   // overwrite
    }
}

FileWriter *FileWriter::addString(const std::string &str) {
    if (str.find(separator_) != string::npos) {
        currentLine_.push_back("\"" + str + "\"");
    } else {
        currentLine_.push_back(str);
    }
    return this;
}

FileWriter *FileWriter::addDouble(const double &value) {
    return addString(to_string(value));
}

FileWriter *FileWriter::addInt(int value) {
    return addString(to_string(value));
}

FileWriter *FileWriter::nextLine() {
    linesToWrite_.push_back(currentLine_);
    currentLine_.clear();
    return this;
}

bool FileWriter::fileExists(const std::string& fileName) {
    ifstream f(fileName.c_str());
    return f.good();
}

void FileWriter::writeToFile() {
    ofstream of;
    of.open(fileName_, ios_base::app);
    for (Line &line : linesToWrite_) {
        writeLine(of, line);
        of << endl;
    }
    if (!currentLine_.empty()) {
        writeLine(of, currentLine_);
    }
}

void FileWriter::writeLine(ofstream &of, Line &line) {
    for (string word : line) {
        of << word << separator_;
    }
}