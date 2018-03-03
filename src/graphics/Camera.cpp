#include "Camera.h"

namespace core { namespace graphics {

	Camera::Camera(const math::vec3& peye, const math::vec3& plookAt,
		const math::vec3& pup, const float& pfov, const float& pnearPlane,
		const float& pfarPlane, const float& pviewWidth, const float& pviewHeight)
		: eye(peye), lookAt(plookAt), up(pup), fov(pfov), nearPlane(pnearPlane),
		farPlane(pfarPlane), viewWidth(pviewWidth), viewHeight(pviewHeight)
	{
		aspectRatio = viewWidth / viewHeight;
		computeProjMat();
		computeViewMat();
	}

	Camera::~Camera() 
	{

	}

	void Camera::updateBufferSize(float width, float height)
	{
		viewWidth = width;
		viewHeight = height;
		computeViewMat();
	}

	const math::mat4& Camera::getViewMatrix() const 
	{
		return viewMat;
	}

	const math::mat4& Camera::getProjectionMatrix() const
	{
		return projMat;
	}

	void Camera::rotateRight(float angle) 
	{
		
	}

	void Camera::rotateUp(float angle) 
	{
		
	}

	void Camera::forward(float dist) 
	{
		math::vec3 forward = dist * (lookAt - eye).normalize();
		eye = eye + forward;
		lookAt = lookAt + forward;
		computeViewMat();
	}

	void Camera::strafeRight(float dist)
	{
		math::vec3 forward = lookAt - eye;
		math::vec3 right = dist * (forward.cross(up)).normalize();

		eye = eye + right;
		lookAt = lookAt + right;
		computeViewMat();
	}


	math::mat4& Camera::computeProjMat() 
	{
		projMat = math::mat4::persp(fov, aspectRatio, nearPlane, farPlane);
		return projMat;
	}

	math::mat4& Camera::computeViewMat() 
	{
		viewMat = math::mat4::lookAt(eye, lookAt, up);
		return viewMat;
	}

} }