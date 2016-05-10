#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <boost/variant.hpp>

class Attribute {
public:
    enum AttributeType {
        NOMINAL,
        NUMERICAL
    };
   
    Attribute(const std::string& value);
    Attribute(double value);
    AttributeType getType() const;
    boost::variant<std::string, double> getValue() const;
    
    static Attribute createAttribute(const std::string& str);
    
private:
    AttributeType type_;
    boost::variant<std::string, double> value_;
};


#endif /* ATTRIBUTE_H */

