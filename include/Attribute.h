#ifndef EDAMI_ATTRIBUTE_H
#define EDAMI_ATTRIBUTE_H

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
    double& operator=(double value);
    
    static Attribute createAttribute(const std::string& str);
    static Attribute createAttribute(const std::string& str, AttributeType type);

private:
    AttributeType type_;
    boost::variant<std::string, double> value_;
};


#endif /* EDAMI_ATTRIBUTE_H */

