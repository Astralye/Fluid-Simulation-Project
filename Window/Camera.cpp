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

	// TODO:

	// This code is being very buggy.
	// It needs to scale with aspect ratio but for some reason I cant seem
	// To get it to worK

	float increment = 10.0f;
	m_Zoom = (pos) ? increment : -increment;

	float projZoom = m_Zoom / 2;

	m_Projection.left += projZoom;
	m_Projection.right -= projZoom;

	m_Projection.bottom += projZoom / 1.77f;
	m_Projection.top -= projZoom / 1.77f;
}

Camera camera;
