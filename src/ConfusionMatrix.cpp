#include "ConfusionMatrix.h"

#include <iostream>
#include <iomanip>
#include <ostream>
#include <string>

using namespace std;

ConfusionMatrix::ConfusionMatrix(): space_(0) {
}

void ConfusionMatrix::addResults(const string& correctClass, const list<string>& classifiedClasses) {
    setSpace_(correctClass.size());
    Row newRow;

    for (auto &resultClass: classifiedClasses) {
        setSpace_(resultClass.size());

        // fill new row with results
        auto newRowIt = newRow.find(resultClass);
        if (newRowIt == newRow.end()) {
            newRow[resultClass] = 1;
        } else {
            ++newRowIt->second;
        }

        // add result class to other rows
        for (auto &matrixRow: matrix_) {
            if (matrixRow.second.find(resultClass) == matrixRow.second.end()) {
                matrixRow.second[resultClass] = 0;
            }
            if (newRow.find(matrixRow.first) == newRow.end()) {
                newRow[matrixRow.first] = 0;
            }
        }
    }
    
    // get other classes to new row
    for (auto &matrixRow: matrix_) {
        if (newRow.find(matrixRow.first) == newRow.end()) {
            newRow[matrixRow.first] = 0;
        }
    }
    
    // add new row or merge with existing one
    auto matrixIt = matrix_.find(correctClass);
    if (matrixIt == matrix_.end()) {
        matrix_[correctClass] = newRow;
    } else {
        for (auto &elem: newRow) {
            matrixIt->second[elem.first] += elem.second;
        }
    }
}

ostream& operator<<(ostream& os, const ConfusionMatrix& matrix) {
    os << setw(matrix.space_) << ' ';
    for (auto &row: matrix.matrix_) {
        os << setw(matrix.space_) << row.first;
    }
    os << endl;

    for (auto &row: matrix.matrix_) {
        os << setw(matrix.space_) << row.first;
        for (auto &elem: row.second)
            os << setw(matrix.space_) << elem.second;
        os << endl;
    }
    os << endl;

    return os;
}

void ConfusionMatrix::setSpace_(int size) {
    int newSpace = size + 2;
    if (newSpace > space_)
        space_ = newSpace;
}

