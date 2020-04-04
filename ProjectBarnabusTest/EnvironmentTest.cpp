#include "pch.h"
#include <GameEngine/Environment.h>

TEST(EnvironmentTest, TestAddLight)
{
	const std::string testLight = "TestLight";

	Environment environment;
	auto result = environment.AddLight(testLight, std::make_unique<Light>());

	EXPECT_TRUE(result);
}

TEST(EnvironmentTest, TestAddingExistingLight)
{
	const std::string testLight = "TestLight";

	Environment environment;
	auto result = environment.AddLight(testLight, std::make_unique<Light>());
	EXPECT_TRUE(result);

	result = environment.AddLight(testLight, std::make_unique<Light>());
	EXPECT_FALSE(result);
}

TEST(EnvironmentTest, TestGetLight)
{
	const std::string testLight = "TestLight";

	Environment environment;
	environment.AddLight(testLight, std::make_unique<Light>());
	
	auto result = environment.GetLight(testLight);

	EXPECT_TRUE(result);
}
