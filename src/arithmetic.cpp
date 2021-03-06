#include <filesystem>

#include "fundot/fundot.h"

namespace fundot {

Object operator+(const Object& lhs, const Object& rhs)
{
    if (lhs.value.type() == typeid(Integer)
        && rhs.value.type() == typeid(Integer)) {
        return {Integer({std::any_cast<const Integer&>(lhs.value).value
                         + std::any_cast<const Integer&>(rhs.value).value})};
    }
    if (lhs.value.type() == typeid(Float)
        && rhs.value.type() == typeid(Float)) {
        return {Float({std::any_cast<const Float&>(lhs.value).value
                       + std::any_cast<const Float&>(rhs.value).value})};
    }
    if (lhs.value.type() == typeid(Integer)
        && rhs.value.type() == typeid(Float)) {
        return {Float({std::any_cast<const Integer&>(lhs.value).value
                       + std::any_cast<const Float&>(rhs.value).value})};
    }
    if (lhs.value.type() == typeid(Float)
        && rhs.value.type() == typeid(Integer)) {
        return {Float({std::any_cast<const Float&>(lhs.value).value
                       + std::any_cast<const Integer&>(rhs.value).value})};
    }
    if (lhs.value.type() == typeid(String)
        && rhs.value.type() == typeid(String)) {
        return {String({std::any_cast<const String&>(lhs.value).value
                        + std::any_cast<const String&>(rhs.value).value})};
    }
    throw std::runtime_error("Invalid operand type.");
}

Object operator-(const Object& lhs, const Object& rhs)
{
    if (lhs.value.type() == typeid(Integer)
        && rhs.value.type() == typeid(Integer)) {
        return {Integer({std::any_cast<const Integer&>(lhs.value).value
                         - std::any_cast<const Integer&>(rhs.value).value})};
    }
    if (lhs.value.type() == typeid(Float)
        && rhs.value.type() == typeid(Float)) {
        return {Float({std::any_cast<const Float&>(lhs.value).value
                       - std::any_cast<const Float&>(rhs.value).value})};
    }
    if (lhs.value.type() == typeid(Integer)
        && rhs.value.type() == typeid(Float)) {
        return {Float({std::any_cast<const Integer&>(lhs.value).value
                       - std::any_cast<const Float&>(rhs.value).value})};
    }
    if (lhs.value.type() == typeid(Float)
        && rhs.value.type() == typeid(Integer)) {
        return {Float({std::any_cast<const Float&>(lhs.value).value
                       - std::any_cast<const Integer&>(rhs.value).value})};
    }
    throw std::runtime_error("Invalid operand type.");
}

Object operator*(const Object& lhs, const Object& rhs)
{
    if (lhs.value.type() == typeid(Integer)
        && rhs.value.type() == typeid(Integer)) {
        return {Integer({std::any_cast<const Integer&>(lhs.value).value
                         * std::any_cast<const Integer&>(rhs.value).value})};
    }
    if (lhs.value.type() == typeid(Float)
        && rhs.value.type() == typeid(Float)) {
        return {Float({std::any_cast<const Float&>(lhs.value).value
                       * std::any_cast<const Float&>(rhs.value).value})};
    }
    if (lhs.value.type() == typeid(Integer)
        && rhs.value.type() == typeid(Float)) {
        return {Float({std::any_cast<const Integer&>(lhs.value).value
                       * std::any_cast<const Float&>(rhs.value).value})};
    }
    if (lhs.value.type() == typeid(Float)
        && rhs.value.type() == typeid(Integer)) {
        return {Float({std::any_cast<const Float&>(lhs.value).value
                       * std::any_cast<const Integer&>(rhs.value).value})};
    }
    throw std::runtime_error("Invalid operand type.");
}

Object operator/(const Object& lhs, const Object& rhs)
{
    if (lhs.value.type() == typeid(Integer)
        && rhs.value.type() == typeid(Integer)) {
        return {Float(
            {static_cast<double>(std::any_cast<const Integer&>(lhs.value).value)
             / std::any_cast<const Integer&>(rhs.value).value})};
    }
    if (lhs.value.type() == typeid(Float)
        && rhs.value.type() == typeid(Float)) {
        return {Float({std::any_cast<const Float&>(lhs.value).value
                       / std::any_cast<const Float&>(rhs.value).value})};
    }
    if (lhs.value.type() == typeid(Integer)
        && rhs.value.type() == typeid(Float)) {
        return {Float({std::any_cast<const Integer&>(lhs.value).value
                       / std::any_cast<const Float&>(rhs.value).value})};
    }
    if (lhs.value.type() == typeid(Float)
        && rhs.value.type() == typeid(Integer)) {
        return {Float({std::any_cast<const Float&>(lhs.value).value
                       / std::any_cast<const Integer&>(rhs.value).value})};
    }
    if (lhs.value.type() == typeid(String)
        && rhs.value.type() == typeid(String)) {
        std::filesystem::path path =
            std::any_cast<const String&>(lhs.value).value;
        path /= std::any_cast<const String&>(rhs.value).value;
        return {String({path.string()})};
    }
    throw std::runtime_error("Invalid operand type.");
}

Object operator%(const Object& lhs, const Object& rhs)
{
    if (lhs.value.type() == typeid(Integer)
        && rhs.value.type() == typeid(Integer)) {
        return {Integer({std::any_cast<const Integer&>(lhs.value).value
                         % std::any_cast<const Integer&>(rhs.value).value})};
    }
    throw std::runtime_error("Invalid operand type.");
}

Object operator-(const Object& object)
{
    if (object.value.type() == typeid(Integer)) {
        return {Integer({-std::any_cast<const Integer&>(object.value).value})};
    }
    if (object.value.type() == typeid(Float)) {
        return {Float({-std::any_cast<const Float&>(object.value).value})};
    }
    throw std::runtime_error("Invalid operand type.");
}

}  // namespace fundot
