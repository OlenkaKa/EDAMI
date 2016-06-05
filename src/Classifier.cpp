#include "Classifier.h"
#include <algorithm>
#include <cmath>
#include <iterator>
#include <random>
#include <stdexcept>
#include <boost/lexical_cast.hpp>

using namespace boost;
using namespace std;

Classifier::Classifier(const Dataset& dataset, const std::string& knn, int minkowskiParam):
        dataset_(dataset), minkowskiParam_(minkowskiParam) {
    
    if (knn.back() == '+') {
        knnNumber_ = lexical_cast<int>(knn.substr(0, knn.size()-1));
        knnPlus_ = true;
    } else {
        knnNumber_ = lexical_cast<int>(knn);
        knnPlus_ = false;
    }
}

string Classifier::classify(const RowPtr& row) const {
    Distances minDistances;

    for (auto &classDataset: dataset_.getClassDatasets()) {
        for (auto &classRow: classDataset.second.getRows()) {
            DistanceElem elem = DistanceElem(classDataset.first, distance_(classRow, row));
            addNewDistance_(minDistances, elem);
            removeOverflowPlus_(minDistances);
        }
    }
    if (!knnPlus_)
        removeOverflowEqual_(minDistances);
    return vote_(minDistances);
}

void Classifier::classify(const SimpleDataset& dataset, list<string>& result) const {
    for (auto &row: dataset.getRows()) {
        result.push_back(classify(row));
    }
}


// private methods

string Classifier::vote_(const Distances& distances) {
    map<string, int> votes;
    // count votes
    for (auto &entry: distances) {
        if (votes.find(entry.first) == votes.end())
            votes[entry.first] = 1;
        else
            ++votes[entry.first];
    }
    
    // find the leader
    // TODO: what about multiple leaders?
    string result;
    int maxVotes = 0;
    for (auto &entry: votes) {
        if (entry.second > maxVotes) {
            result = entry.first;
            maxVotes = entry.second;
        }
    }
    return result;
}

void Classifier::addNewDistance_(Distances& distances, const DistanceElem& distanceElem) {
    bool inserted = false;
    for (auto it = distances.begin(); it != distances.end(); ++it) {
        if (distanceElem.second < it->second) {
            distances.insert(it, distanceElem);
            inserted = true;
            break;
        }
    }
    if (!inserted)
        distances.push_back(distanceElem);
}

void Classifier::setRangeOfLastDistance_(Distances& distances,
        Distances::iterator& start, Distances::iterator& end) const {
    start = distances.begin();
    for (int i = 1; i < knnNumber_ && start != distances.end(); ++i) {
        ++start;
    }

    end = start;
    while (end->second == start->second && end != distances.end()) {
        ++end;
    }
}

void Classifier::removeOverflowEqual_(Distances& distances) const {
    if (distances.size() <= knnNumber_)
        return;

    Distances::iterator startIt, endIt;
    setRangeOfLastDistance_(distances, startIt, endIt);
    
    int remain = rand() % distance(startIt, endIt);
    auto remainIt = startIt;
    for (int i = 0; i < remain; ++i)
        ++remainIt;

    distances.erase(startIt, remainIt);
    distances.erase(++remainIt, distances.end());
}

void Classifier::removeOverflowPlus_(Distances& minDistances) const {
    if (minDistances.size() <= knnNumber_)
        return;

    Distances::iterator startIt, endIt;
    setRangeOfLastDistance_(minDistances, startIt, endIt);
    minDistances.erase(endIt, minDistances.end());
}

long double Classifier::distance_(const RowPtr& r1, const RowPtr& r2) const {
    if (r1->size() != r2->size())
        throw runtime_error("Rows size not equal.");
    long double sum = 0L;
    for (int i = 0; i < r1->size(); ++i) {
        sum += pow(difference_((*r1)[i], (*r2)[i]), minkowskiParam_);
    }
    return abs(sum) < 0.00000000000001 ? 0L : pow(sum, 1.0/minkowskiParam_);
}

long double Classifier::difference_(const Attribute& a1, const Attribute& a2) {
    if (a1.getType() != a2.getType())
        throw runtime_error("Different types of attributes in the same column.");
    switch (a1.getType()) {
        case Attribute::NOMINAL:
            if (get<string>(a1.getValue()) == get<string>(a2.getValue()))
                return 0L;
            return 1L;
        case Attribute::NUMERICAL:
            return abs(get<double>(a1.getValue()) - get<double>(a2.getValue()));
    }
    throw runtime_error("Invalid type of attribute.");
}

