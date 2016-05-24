#ifndef EDAMI_CLASSIFIER_H
#define EDAMI_CLASSIFIER_H

#include "Dataset.h"
#include <list>


class Classifier {
public:
    Classifier(const Dataset& dataset, const std::string& knn, int minkowskiParam);
    std::string classify(const RowPtr& row) const;
    void classify(const SimpleDataset& dataset, std::list<std::string>& result) const;

private:
    Dataset dataset_;
    int minkowskiParam_;
    int knnNumber_;
    bool knnPlus_;

    // table of distances consisting of pairs class-distance
    typedef std::pair<std::string, double> DistanceElem;
    typedef std::list<DistanceElem> Distances;

    // counting votes and determining the winner class
    static std::string vote_(const Distances& distances);

    // adding new distance to distances table
    static void addNewDistance_(Distances& distances, const DistanceElem& distanceElem);

    // finding range of last same distances
    void setRangeOfLastDistance_(Distances& distances,
            Distances::iterator& start, Distances::iterator& end) const;

    // removing overflow from distance table
    void removeOverflowEqual_(Distances& distances) const;
    void removeOverflowPlus_(Distances& distances) const;

    // calculating distance between rows
    double distance_(const RowPtr& r1, const RowPtr& r2) const;
    static double difference_(const Attribute& a1, const Attribute& a2);
};

#endif /* EDAMI_CLASSIFIER_H */

