#include "SimpleDataset.h"

using namespace std;


SimpleDataset::SimpleDataset() {
}

void SimpleDataset::addRow(const RowPtr& row) {
    rows_.push_back(row);
}

const Rows& SimpleDataset::getRows() const {
    return rows_;
}

size_t SimpleDataset::size() const {
    return rows_.size();
}

size_t SimpleDataset::numberOfColumns() const {
    return rows_.empty() ? 0 : rows_.front()->size();
}

void SimpleDataset::normalize(const NormalizationParams& params) {
    for (auto &row: rows_) {
        row = RowPtr(new Row(*row)); // deep copy
        for (int i = 0; i < row->size(); ++i) {
            Attribute& attr = (*row)[i];
            if (attr.getType() == Attribute::NUMERICAL) {
                double min = params.getValues(i).getMinValue();
                double diff = params.getValues(i).getMaxValue() - min;
                attr = (get<double>(attr.getValue()) - min) / diff;
                if (get<double>(attr.getValue()) > 1)
                    attr = 1;
                else if (get<double>(attr.getValue()) < 0)
                    attr = 0;
            }
        }
    }
}

void setNormalizationParams(const RowPtr& row, NormalizationParams& params) {
    size_t size = row->size();
    for (int col = 0; col < size; ++col) {
        if (row->at(col).getType() == Attribute::NUMERICAL) {
            double attrVal = boost::get<double>(row->at(col).getValue());
            params.setValue(col, attrVal);
        }
    }
}

ostream& operator<<(ostream& os, const SimpleDataset& dataset) {
    for (auto &row: dataset.getRows()) {
        for (auto &attr: *row) {
            switch (attr.getType()) {
                case Attribute::NOMINAL:
                    os << attr.getValue();
                    break;
                case Attribute::NUMERICAL:
                    os << "!" << attr.getValue();
                    break;
                default:
                    break;
            }
            os << ' ';
        }
        os << endl;
    }
    return os;
}

