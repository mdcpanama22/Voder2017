#pragma once
#include "vec3.h"
#include "vec4.h"
#include <iostream>

namespace core { namespace math {
    
    class mat4 {
    public:

        union {
            float elements[4 * 4]; //4x4 matrix = 16 elements
            vec4 columns[4]; //4 vec4s -> (4 vecs x 4 parts per vec) = 16 elements
        };
    
  
        mat4();
        mat4(const float& diag);
        mat4(const mat4& copy);
        
        ~mat4();
        
        static mat4 identity();
        static mat4 ortho(const float& left, const float& right, const float& bottom, const float& top, const float& near, const float& far);
        static mat4 persp(const float& fov, const float& aspectRatio, const float& near, const float& far);
		static mat4 lookAt(vec3 eye, vec3 lookAt, vec3 up);

        static mat4 translation(const vec3& trans);
        static mat4 rotation(float angle, const vec3& axis);
        static mat4 scale(const vec3& scl);
        
        mat4& multiply(const mat4& other);
        friend mat4 operator*(mat4& left, const mat4& right);
        mat4& operator*=(const mat4& other);
        
        friend std::ostream& operator<<(std::ostream& stream, const mat4& matrix);
  
    };
    
} }