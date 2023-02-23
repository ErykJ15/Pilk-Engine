#include "Camera.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include <glad.h>
#include <iostream>

Camera::Camera(const glm::vec3 p_pos, const glm::vec3 p_target, const glm::vec3 p_direction, const glm::vec3 p_cameraUp, const float p_width, const float p_height)
{
	m_cameraPos = p_pos;
	m_cameraTarget = p_target;
	m_cameraDirection = p_direction;
	m_cameraUp = p_cameraUp;

	m_projection = glm::perspective(glm::radians(45.0f), p_width / p_height, 0.1f, 100.0f);
	m_cameraRight = glm::normalize(glm::cross(m_cameraUp, m_cameraDirection));
}

Camera::~Camera()
{

}

void Camera::MoveCamera(const Direction p_direction, const float p_distance, const float p_dt)
{
	const float camSpeed = p_distance * p_dt;

	switch (p_direction)
	{
	case Forward:
		m_cameraPos += m_cameraDirection * camSpeed;
		break;
	case Backward:
		m_cameraPos -= m_cameraDirection * camSpeed;
		break;
	case Left:
		m_cameraPos += m_cameraRight * camSpeed;
		break;
	case Right:
		m_cameraPos -= m_cameraRight * camSpeed;
		break;
	default:
		break;
	}

	std::cout << "CamPos: X:" << m_cameraPos.x << " Y: " << m_cameraPos.y << " Z: " << m_cameraPos.z << '\n';
}

void Camera::UpdateCamera(const int p_shaderHandle)
{
	m_cameraTarget = m_cameraPos + m_cameraDirection;

	m_view = glm::lookAt(m_cameraPos, m_cameraTarget, m_cameraUp);


	//glUseProgram(p_shaderHandle);
	glUniformMatrix4fv(glGetUniformLocation(p_shaderHandle, "uView"), 1, GL_FALSE, &m_view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(p_shaderHandle, "uProj"), 1, GL_FALSE, &m_projection[0][0]);

}
