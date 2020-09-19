#pragma once
#include <gmock/gmock.h>
#include "GameEngine/IRenderer.h"

using namespace testing; 

class MockRenderer : public IRenderer {
public:
	MOCK_METHOD(bool, InitialiseGameEngine, (), (override));
	MOCK_METHOD(bool, ShouldWindowClose, (), (override));
	MOCK_METHOD(GLFWwindow*, GetWindow, (), (override));
};