#pragma once

#include "glm/glm.hpp"
#include "Camera.h"

#include <vector>
#include <functional>
#include <iostream>

// Namespace are like packages
// A contract of a class to have certain functions
namespace test {

	class Test
	{
	public:
		Test(){}
		virtual ~Test(){}

		virtual void OnUpdate(){}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		}
	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
	};
}