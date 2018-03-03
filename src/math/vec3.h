#pragma once 
#include <iostream>

namespace core { namespace math {
   
    class vec3 {
    public:

        float x, y, z;

        vec3();
        vec3(const float& x, const float& y, const float& z);
        vec3(const vec3& copy);
        
        ~vec3();
        
        vec3& add(const vec3& other);
        vec3& subtract(const vec3& other);
        float dot(const vec3& other);
        vec3 cross(const vec3& other);
        vec3& scale(const float& scaler);

		float length() const ;
		vec3& normalize();
        
        friend vec3& operator+(vec3& left, const vec3& right);
        friend vec3& operator-(vec3& left, const vec3& right);
		friend vec3 operator-(vec3& vector);
        
        friend vec3 operator^(vec3& left, const vec3& right); //cross product overload
        friend float operator*(vec3& left, const vec3& right); //dot product overload
        
        friend vec3& operator*(const float& scaler, vec3& vector); //scale overloads
        friend vec3& operator*(vec3& vector, const float& scaler);
        
        friend std::ostream& operator<<(std::ostream& stream, const vec3& v); //print
  
    };
 
} }