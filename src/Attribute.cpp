#include "Attribute.h"
#include <boost/lexical_cast.hpp> 

using namespace std;
using namespace boost;

Attribute::Attribute(const string& value):
        type_(NOMINAL), value_(value) {
}

Attribute::Attribute(double value):
        type_(NUMERICAL), value_(value) {
}

Attribute::AttributeType Attribute::getType() const {
    return type_;
}

variant<string, double> Attribute::getValue() const {
    return value_;
}

double& Attribute::operator=(double value) {
    value_ = value;
    type_ = NUMERICAL;
    return get<double>(value_);
}

Attribute Attribute::createAttribute(const std::string& str) {
    try {
        double num = lexical_cast<double>(str);
        return Attribute(num);
    } catch (bad_lexical_cast &e) {
        return Attribute(str);
    }
}

