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
		// -50 -> 150 : 200
		// -50 -> 150 : 200

		// 200 x 200 = 1:1 ratio
		// 800 x 800 = 1:1 ratio


		// 1280 x 720 = 16:9 ratio
		// 640 x 360 = 16:9 ratio
		// 320 x 180 = 16:9 ratio

		Camera(
			Projection proj = {-77.7f, 100.0f, 0.0f, 0.0f},
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