#include "Camera.h"

#include <iostream>

void Camera::move(axis type, bool positive) {

	// movement is relative to zoom
	float moveIncrement = 0.02 * m_Projection.right;

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

	int increment = 10;
	m_Zoom = (pos) ? increment : -increment;

	float projZoom = m_Zoom / 2;

	m_Projection.left += projZoom;
	m_Projection.right -= projZoom;

	m_Projection.bottom += projZoom;
	m_Projection.top -= projZoom;
}

Camera camera;
