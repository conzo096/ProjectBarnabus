#include "pch.h"
#include <GameEngine/Environment.h>

TEST(EnvironmentTest, TestAddEntity)
{
	const std::string testEntity = "TestEntity";

	Level environment("test");
	auto result = environment.AddEntity(testEntity, std::make_unique<Entity>("Test"));

	EXPECT_TRUE(result);
}

TEST(EnvironmentTest, TestAddingExistingEntity)
{
	const std::string testEntity = "TestEntity";

	Level environment("test");
	auto result = environment.AddEntity(testEntity, std::make_unique<Entity>("Test"));

	result = environment.AddEntity(testEntity, std::make_unique<Entity>("Test"));
	EXPECT_FALSE(result);
}

TEST(EnvironmentTest, TestGetEntity)
{
	const std::string testEntity = "TestEntity";

	Level environment("test");
	environment.AddEntity(testEntity, std::make_unique<Entity>("Test"));

	auto result = environment.GetEntity(testEntity);

	EXPECT_TRUE(result);
}

TEST(EnvironmentTest, TestAddLight)
{
	const std::string testLight = "TestLight";

	Level environment("test");
	auto result = environment.AddLight(testLight, std::make_unique<Light>());

	EXPECT_TRUE(result);
}

TEST(EnvironmentTest, TestAddingExistingLight)
{
	const std::string testLight = "TestLight";

	Level environment("test");
	auto result = environment.AddLight(testLight, std::make_unique<Light>());

	result = environment.AddLight(testLight, std::make_unique<Light>());
	EXPECT_FALSE(result);
}

TEST(EnvironmentTest, TestGetLight)
{
	const std::string testLight = "TestLight";

	Level environment("test");
	environment.AddLight(testLight, std::make_unique<Light>());
	
	auto result = environment.GetLight(testLight);

	EXPECT_TRUE(result);
}
