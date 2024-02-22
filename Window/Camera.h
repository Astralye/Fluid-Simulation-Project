#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Simulation/PhysicsEq.h"

class Camera {
	private:
		struct Projection{
			float left;
			float right;
			float bottom;
			float top;
		};

		Projection m_Projection;
		glm::vec3 m_Position;
		float m_Zoom;

	public:

		Camera(
			Projection proj = {0.0f, 1280.0f, 0.0f, 720.0f},
			//Projection proj = {-77.0f, 100.0f, 0.0f, 100.0f},
			glm::vec3 pos = glm::vec3(0.0f),
			float Zoom = 0.0f)
		: m_Projection(proj),
			m_Position(pos),
			m_Zoom(Zoom)
		{
		}

		enum axis {
			x,
			y
		};

		void move(axis type, bool positive);
		void zoom(bool pos);

		inline float getZoom() { return m_Zoom; }
		inline Projection getProjection() { return m_Projection; }
		inline glm::vec3 getPosition() { return m_Position; }

};

extern Camera camera;

#endif