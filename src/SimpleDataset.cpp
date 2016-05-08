#include "SimpleDataset.h"

using namespace std;


SimpleDataset::SimpleDataset() {
}

void SimpleDataset::addRow(const Row& row) {
    rows_.push_back(row);
}

list<Row> SimpleDataset::getRows() const {
    return rows_;
}

size_t SimpleDataset::size() const {
    return rows_.size();
}

size_t SimpleDataset::numberOfColumns() const {
    return rows_.empty() ? 0 : rows_.front().size();
}

