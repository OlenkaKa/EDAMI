#ifndef EDAMI_CONFUSIONMATRIX_H
#define EDAMI_CONFUSIONMATRIX_H

#include <map>
#include <list>
#include <string>

class ConfusionMatrix {
public:
    ConfusionMatrix();
    void addResults(const std::string& cls, const std::list<std::string>& results);
    friend std::ostream& operator<<(std::ostream& os, const ConfusionMatrix& matrix);

private:
    typedef std::map<std::string, int> Row;
    
    std::map<std::string, Row> matrix_;
    int space_;
    
    void setSpace_(int size);
};

#endif /* EDAMI_CONFUSIONMATRIX_H */

