#include "vec3.h"
#include <cmath>

namespace core { namespace math {
    
    vec3::vec3() 
        : x(0), y(0), z(0)
    {        
    }
    
    vec3::vec3(const float& px, const float& py, const float& pz)
        : x(px), y(py), z(pz)
    {
    }
    
    vec3::vec3(const vec3& copy) 
        :x(copy.x), y(copy.y), z(copy.z)
    {
    }
    
    vec3::~vec3() { }
    
    vec3& vec3::add(const vec3& other) {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        return *this;
    }
    
    vec3& vec3::subtract(const vec3& other) {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        return *this;
    }
    
    float vec3::dot(const vec3& other) {
        return (x * other.x) + (y * other.y) + (z * other.z);
    }
    
    vec3 vec3::cross(const vec3& other) {
        vec3 out(0.0f, 0.0f, 0.0f);
        out.x = (y * other.z) - (z * other.y);
        out.y = (z * other.x) - (x * other.z);
        out.z = (x * other.y) - (y * other.x);
        return out;
    }
    
    vec3& vec3::scale(const float& scaler) {
        this->x *= scaler;
        this->y *= scaler;
        this->z *= scaler;
        return *this;
    }

	float vec3::length() const {
		return sqrt((x * x) + (y * y) + (z * z));
	}

	vec3& vec3::normalize() {
		float len = this->length();

		x = x / len;
		y = y / len;
		z = z / len;

        return *this;
	}
    
    vec3& operator+(vec3& left, const vec3& right) {
        return left.add(right);
    }
    
    vec3& operator-(vec3& left, const vec3& right) {
        return left.subtract(right);
    }

	vec3 operator-(vec3& vector) {
		vec3 out(0.0f, 0.0f, 0.0f);
		return out.subtract(vector);
	}
    
    vec3 operator^(vec3& left, const vec3& right) {
        return left.cross(right);
    }
    
    float operator*(vec3& left, const vec3& right) {
        return left.dot(right);
    }
    
    vec3& operator*(const float& scaler, vec3& vector) {
        return vector.scale(scaler);
    }
    
    vec3& operator*(vec3& vector, const float& scaler) {
        return vector.scale(scaler);
    }
    
    std::ostream& operator<<(std::ostream& stream, const vec3& v) {
        stream << "vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
        return stream;
    }
    
} }