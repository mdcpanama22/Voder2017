#include "mat4.h"
#include "misc_math.h"

namespace core { namespace math {
    
    mat4::mat4() 
    {
        for (int i = 0; i < 16; ++i) {
            elements[i] = 0.0f;
        }
    }
    
    mat4::mat4(const float& diag) {
        for (int i = 0; i < 16; ++i) {
            elements[i] = 0.0f;
        }
        elements[0 + 0 * 4] = diag;
        elements[1 + 1 * 4] = diag;
        elements[2 + 2 * 4] = diag;
        elements[3 + 3 * 4] = diag;
    }
    
    mat4::mat4(const mat4& copy) {
        for (int i = 0; i < 16; ++i) {
            elements[i] = copy.elements[i];
        }
    }
    
    mat4::~mat4() { }
    
    mat4 mat4::identity() {
        return mat4(1.0f);
    }
    
    mat4 mat4::ortho(const float& left, const float& right, const float& bottom, const float& top, const float& near, const float& far) {
        mat4 out = identity();
        
        out.elements[0 + 0 * 4] =  2.0f / (right - left);
        out.elements[1 + 1 * 4] =  2.0f / (top - bottom);
        out.elements[2 + 2 * 4] = -2.0f / (far - near);
        
        out.elements[0 + 3 * 4] = (right + left) / (right - left);
        out.elements[1 + 3 * 4] = (top + bottom) / (top - bottom);
        out.elements[2 + 3 * 4] = (far + near) / (far - near);
        
        return out;
        
    }
    
    mat4 mat4::persp(const float& fov, const float& aspectRatio, const float& near, const float& far) {
        mat4 out = identity();
        
        float q = 1.0f / tan(toRads(0.5f * fov));
        float a = q / aspectRatio;
        float b = (near + far) / (near - far);
        float c = (2.0f * near * far) / (near - far);
        
        out.elements[0 + 0 * 4] = a;
        out.elements[1 + 1 * 4] = q;
        out.elements[2 + 2 * 4] = b;
        out.elements[3 + 2 * 4] = -1.0f;
        out.elements[2 + 3 * 4] = c;
		out.elements[3 + 3 * 4] = 0.0f; //When this is uncommented its a 1st person camera, if not its 3rd person????
        
        return out;
        
    }

	mat4 mat4::lookAt(vec3 eye, vec3 lookAt, vec3 up) {
		mat4 out = identity();

		vec3 f = (lookAt - eye).normalize();
		vec3 s = (f.cross(up)).normalize();
		vec3 u = s.cross(f);

		out.elements[0 + 0 * 4] = s.x;
		out.elements[1 + 0 * 4] = s.y;
		out.elements[2 + 0 * 4] = s.z;
		out.elements[0 + 1 * 4] = u.x;
		out.elements[1 + 1 * 4] = u.y;
		out.elements[2 + 1 * 4] = u.z;
		out.elements[0 + 2 * 4] = -f.x;
		out.elements[1 + 2 * 4] = -f.y;
		out.elements[2 + 2 * 4] = -f.z;
		out.elements[3 + 0 * 4] = -s.dot(eye);
		out.elements[3 + 1 * 4] = -u.dot(eye);
		out.elements[3 + 2 * 4] = f.dot(eye);


		return out;

	}
    
    mat4 mat4::translation(const vec3& trans) {
        mat4 out = identity();
        out.columns[3].x = trans.x;
        out.columns[3].y = trans.y;
        out.columns[3].z = trans.z;
        return out;
    }
    
    mat4 mat4::rotation(float angle, const vec3& axis) {
        mat4 out = identity();
        
        float r = toRads(angle);
        float c = cos(r);
        float s = sin(r);
        float omc = 1.0f - c; //omc = one minus cos(r)
        
        float x = axis.x;
        float y = axis.y;
        float z = axis.z;
        
        out.elements[0 + 0 * 4] = x * x * omc + c;
        out.elements[1 + 0 * 4] = y * x * omc + z * s;
        out.elements[2 + 0 * 4] = x * y * omc - z * s;
        
        out.elements[0 + 1 * 4] = x * y * omc - z * s;
        out.elements[1 + 1 * 4] = y * y * omc + c;
        out.elements[2 + 1 * 4] = y * z * omc + x * s;
        
        out.elements[0 + 2 * 4] = x * y * omc + y * s;
        out.elements[1 + 2 * 4] = y * z * omc - x * s;
        out.elements[2 + 2 * 4] = z * z * omc + c;
        
        return out;
        
    }
    
    mat4 mat4::scale(const vec3& scl) {
        mat4 out = identity();
        
        out.elements[0 + 0 * 4] = scl.x;
        out.elements[1 + 1 * 4] = scl.y;
        out.elements[2 + 2 * 4] = scl.z;
        
        return out;
    }
    
    mat4& mat4::multiply(const mat4& other) {
        float sum;
        for (int y= 0 ; y < 4; ++y) {
            for (int x = 0; x < 4; ++x) {
                sum = 0.0f;
                for (int e = 0; e < 4; ++e) {
                    sum += elements[x + e * 4] * other.elements[e + y * 4];
                }
                elements[x + y * 4] = sum;
            }
        }
        return *this;
    }
    
    mat4 operator*(mat4& left, const mat4& right) {
        return left.multiply(right);
    }
    
    mat4& mat4::operator*=(const mat4& other) {
        return multiply(other);
    }
    
    std::ostream& operator<<(std::ostream& stream, const mat4& matrix) {
        
        stream << "Matrix(4x4):  ";
        for (int i = 0; i < 4; ++i) {
            stream << matrix.elements[ 0 + i] << " ";
            stream << matrix.elements[ 4 + i] << " ";
            stream << matrix.elements[ 8 + i] << " ";
            stream << matrix.elements[12 + i] << " " << "\n\t      ";           
        }
        return stream;
        
    }
 
} }