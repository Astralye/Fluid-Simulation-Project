#include "Camera.h"

#include <iostream>

void Camera::move(axis type, bool positive) {

	// movement is relative to zoom
	float moveIncrement = 0.02f * m_Projection.right;

	moveIncrement = (positive) ? moveIncrement : -moveIncrement;
	
	switch (type) {
	case x:
		m_Position.x += moveIncrement;
		break;
	case y:
		m_Position.y += moveIncrement;
		break;
	}
}

void Camera::zoom(bool pos) {

	// Delete this after debugging

	//float increment = 10.0f;
	//m_Zoom = (pos) ? increment : -increment;

	//float projZoom = m_Zoom / 2;

	//m_Projection.left += projZoom;
	//m_Projection.right -= projZoom;

	//m_Projection.bottom += projZoom;
	//m_Projection.top -= projZoom;


	glm::vec2 aspectRatio_16_9 = { 16,9 };
	glm::vec2 projZoom;

	projZoom = ((pos) ? aspectRatio_16_9 : -aspectRatio_16_9);
	projZoom.x /= 2;
	projZoom.y /= 2;

	m_Projection.left += projZoom.x;
	m_Projection.right -= projZoom.x;

	m_Projection.bottom += projZoom.y;
	m_Projection.top -= projZoom.y;

}

Camera camera;
