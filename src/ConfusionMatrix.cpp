#include "ConfusionMatrix.h"
#include "Classifier.h"

#include <iomanip>
#include <ostream>
#include <string>

using namespace std;

ConfusionMatrix::ConfusionMatrix(): space_(Classifier::MULTIPLE_CLASSES.length() + 1) {
	matrix_[Classifier::MULTIPLE_CLASSES] = { {Classifier::MULTIPLE_CLASSES, 0} };
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
        }
    }

    // add multiple classes row if not exist
    if (newRow.find(Classifier::MULTIPLE_CLASSES) == newRow.end())
        newRow[Classifier::MULTIPLE_CLASSES] = 0;

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
    int totalSum = 0;
    map<string, int> columnsSum;

    // print classified classes names
    os << setw(matrix.space_) << ' ';
    for (auto &row: matrix.matrix_) {
        os << setw(matrix.space_) << row.first;
    }
    os << setw(matrix.space_) << "_SUM" << endl;

    // print rows
    for (auto &row: matrix.matrix_) {
        os << setw(matrix.space_) << row.first;
        int rowSum = 0;
        for (auto &elem: row.second) {
            os << setw(matrix.space_) << elem.second;

            // add value to row and column sums
            rowSum += elem.second;
            auto columnsSumIt = columnsSum.find(elem.first);
            if (columnsSumIt == columnsSum.end()) {
                columnsSum[elem.first] = elem.second;
            } else {
                columnsSumIt->second += elem.second;
            }
        }
        //print row sum
        os << setw(matrix.space_) << rowSum <<endl;
        totalSum += rowSum;
    }

    // print columns sums
    os << setw(matrix.space_) << "_SUM";
    for (auto &entry: columnsSum) {
        os << setw(matrix.space_) << entry.second;
        totalSum += entry.second;
    }

    os << setw(matrix.space_) << totalSum << endl << endl;
    return os;
}

void ConfusionMatrix::setSpace_(int size) {
    int newSpace = size + 2;
    if (newSpace > space_)
        space_ = newSpace;
}

