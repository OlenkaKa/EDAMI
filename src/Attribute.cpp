#include "Attribute.h"
#include <boost/lexical_cast.hpp> 

using namespace std;
using namespace boost;

Attribute::Attribute(AttributeType type, string value):
        type_(type), value_(value) {
}

Attribute::Attribute(AttributeType type, double value):
        type_(type), value_(value) {
}

Attribute::AttributeType Attribute::getType() {
    return type_;
}

variant<string, double> Attribute::getValue() {
    return value_;
}

Attribute Attribute::createAttribute(std::string str) {
    try {
        double num = lexical_cast<double>(str);
        return Attribute(NUMERICAL, num);
    } catch (bad_lexical_cast &e) {
        return Attribute(SIMPLE, str);
    }
}

