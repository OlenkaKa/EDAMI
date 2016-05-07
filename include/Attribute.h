#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <boost/variant.hpp>

class Attribute {
public:
    enum AttributeType {
        SIMPLE,
        NUMERICAL
    };
   
    Attribute(AttributeType type, std::string value);
    Attribute(AttributeType type, double value);
    AttributeType getType();
    boost::variant<std::string, double> getValue();
    
    static Attribute createAttribute(std::string);
    
private:
    AttributeType type_;
    boost::variant<std::string, double> value_;
};


#endif /* ATTRIBUTE_H */

