#include "vec4.h"

namespace core { namespace math {
    
    vec4::vec4()
        : x(0), y(0), z(0), w(0)
    {
    }
    
    vec4::vec4(const float& px, const float& py, const float& pz, const float& pw)
        : x(px), y(py), z(pz), w(pw)
    {
    }
    
    vec4::vec4(const vec4& copy)
        : x(copy.x), y(copy.y), z(copy.z), w(copy.w)
    {
    }
    
    vec4::~vec4() { }
    
    vec4& vec4::add(const vec4& other) {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        this->w += other.w;
        return *this;
    }
    
    vec4& vec4::subtract(const vec4& other) {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
        this->w -= other.w;
        return *this;
    }
    
    float vec4::dot(const vec4& other) {
        return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
    }
    
    vec4& vec4::scale(const float& scaler) {
        this->x *= scaler;
        this->y *= scaler;
        this->z *= scaler;
        this->w *= scaler;
        return *this;
    }
    
    vec4& operator+(vec4& left, const vec4& right) {
        return left.add(right);
    }
    
    vec4& operator-(vec4& left, const vec4& right) {
        return left.subtract(right);
    }
    
    float operator*(vec4& left, const vec4& right) {
        return left.dot(right);
    }
    
    vec4& operator*(const float& scaler, vec4& vector) {
        return vector.scale(scaler);
    }
    
    vec4& operator*(vec4& vector, const float& scaler) {
        return vector.scale(scaler);
    }

	bool operator==(const vec4& left, const vec4& right) {
		return (left.x == right.x) && (left.y == right.y) && (left.z == right.z) && (left.w == right.w);
	}
    
    std::ostream& operator<<(std::ostream& stream, const vec4& v) {
        stream << "vec4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
        return stream;
    }
    
} } 