#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <GameEngine/BarnabusGameEngine.h>
#include "MockRenderer.h"
namespace
{
	using namespace testing;
}

TEST(BarnabusGameEngineTest, CheckRendererIsInitialisedOnStartGame)
{
	auto mockRenderer = std::make_unique<MockRenderer>();
	EXPECT_CALL(*mockRenderer, InitialiseGameEngine).Times(1);

	BarnabusGameEngine::Get().StartGame(std::move(mockRenderer));
}

TEST(BarnabusGameEngineTest, EngineHandlesNoRenderPresent)
{
	EXPECT_FALSE(BarnabusGameEngine::Get().StartGame(nullptr));
}

TEST(BarnabusGameEngineTest, CheckStartReturnsFalseWhenRendererFails)
{
	auto mockRenderer = std::make_unique<MockRenderer>();
	EXPECT_CALL(*mockRenderer, InitialiseGameEngine).WillOnce(Return(false));

	EXPECT_FALSE(BarnabusGameEngine::Get().StartGame(std::move(mockRenderer)));
}

