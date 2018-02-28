#include "vec2.h"

namespace core { namespace math {
   
    vec2::vec2() 
        : x(0), y(0)
    {
    }
    
    vec2::vec2(const float& px, const float& py) 
        :x(px), y(py)
    {
    }
    
    vec2::vec2(const vec2& copy)
        :x(copy.x), y(copy.y)
    {
    }
    
    vec2::~vec2() { }
    
    vec2& vec2::add(const vec2& other) {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }
    
    vec2& vec2::subtract(const vec2& other) {
        this->x -= other.x;
        this->y -= other.y;
        return *this;
    }
    
    float vec2::dot(const vec2& other) {
        return (x * other.x) + (y * other.y);
    }
    
    vec2& vec2::scale(const float& scaler) {
        this->x *= scaler;
        this->y *= scaler;
        return *this;
    }
    
    vec2& operator+(vec2& left, const vec2& right) {
        return left.add(right);
    }
    
    vec2& operator-(vec2& left, const vec2& right) {
        return left.subtract(right);
    }
    
    float operator*(vec2& left, const vec2& right) {
        return left.dot(right);
    }
    
    vec2& operator*(const float& scaler, vec2& vector) {
        return vector.scale(scaler);
    }
    
    vec2& operator*(vec2& vector, const float& scaler) {
        return vector.scale(scaler);
    }
    
    std::ostream& operator<<(std::ostream& stream, const vec2& v) {
        stream << "vec2(" << v.x << ", " << v.y << ")";
        return stream;
    }
 
} }