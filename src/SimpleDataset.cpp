#include "SimpleDataset.h"

using namespace std;


SimpleDataset::SimpleDataset() {
}

void SimpleDataset::addRow(Row row) {
    rows_.push_back(row);
}

list<Row> SimpleDataset::getRows() {
    return rows_;
}

