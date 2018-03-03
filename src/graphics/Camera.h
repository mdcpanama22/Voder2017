#include "../math/mat4.h"
#include "../math/vec3.h"

namespace core { namespace graphics {

	class Camera {
	private:

		math::mat4 projMat;
		math::mat4 viewMat;
		float viewWidth, viewHeight;
		float aspectRatio;
		float fov; //feild of view
		float nearPlane, farPlane;


	public:
		math::vec3 eye; //the point in space the camera is
		math::vec3 lookAt; // the point vector the camera is looking in
		math::vec3 up; //the point vector above the camera

		Camera(const math::vec3& peye, const math::vec3& plookAt, 
			const math::vec3& pup, const float& pfov, const float& pnearPlane,
			const float& pfarPlane, const float& pveiwWidth, const float& pviewHeight);
		~Camera();

		void updateBufferSize(float width, float height);
		const math::mat4& getViewMatrix() const ;
		const math::mat4& getProjectionMatrix() const ;
		void rotateRight(float angle);
		void rotateUp(float angle);
		void forward(float dist);
		void strafeRight(float dist);


	private:

		math::mat4& computeProjMat();
		math::mat4& computeViewMat();

	};

} }