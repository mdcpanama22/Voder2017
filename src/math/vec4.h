#pragma once
#include <iostream>

namespace core { namespace math { 

    class vec4 {
    public:  
        
        float x, y, z, w;
        
        vec4();
        vec4(const float& px, const float& py, const float& pz, const float& pw);
        vec4(const vec4& copy);
        
        ~vec4();
        
        vec4& add(const vec4& other);
        vec4& subtract(const vec4& other);
        float dot(const vec4& other);
        vec4& scale(const float& scaler);
        
        friend vec4& operator+(vec4& left, const vec4& right);
        friend vec4& operator-(vec4& left, const vec4& right);
        
        friend float operator*(vec4& left, const vec4& right); //dot product overload
        
        friend vec4& operator*(const float& scaler, vec4& vector); //scale overloads
        friend vec4& operator*(vec4& vector, const float& scaler);

		friend bool operator==(const vec4& left, const vec4& right);
        
        friend std::ostream& operator<<(std::ostream& stream, const vec4& v); //print
   
    };

} }