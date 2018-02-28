#pragma once
#include <iostream>

namespace core { namespace math {
    
    class vec2 {
    public:
    
        float x, y;

        vec2();
        vec2(const float& x, const float& y);
        vec2(const vec2& other);
        
        ~vec2();
        
        vec2& add(const vec2& other);
        vec2& subtract(const vec2& other);
        float dot(const vec2& other);
        vec2& scale(const float& scaler);
        
        friend vec2& operator+(vec2& left, const vec2& right);
        friend vec2& operator-(vec2& left, const vec2& right);
        
        friend float operator*(vec2& left, const vec2& right); //dot product overload
        
        friend vec2& operator*(const float& scaler, vec2& vector); //scale overloads
        friend vec2& operator*(vec2& vector, const float& scaler);
        
        friend std::ostream& operator<<(std::ostream& stream, const vec2& v); //print
  
  
    };
 
} }