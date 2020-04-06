#include "pch.h"
#include <GameEngine/Environment.h>

TEST(EnvironmentTest, TestAddEntity)
{
	const std::string testEntity = "TestEntity";

	Environment environment("test");
	auto result = environment.AddEntity(testEntity, std::make_unique<Entity>());

	EXPECT_TRUE(result);
}

TEST(EnvironmentTest, TestAddingExistingEntity)
{
	const std::string testEntity = "TestEntity";

	Environment environment("test");
	auto result = environment.AddEntity(testEntity, std::make_unique<Entity>());

	result = environment.AddEntity(testEntity, std::make_unique<Entity>());
	EXPECT_FALSE(result);
}

TEST(EnvironmentTest, TestGetEntity)
{
	const std::string testEntity = "TestEntity";

	Environment environment("test");
	environment.AddEntity(testEntity, std::make_unique<Entity>());

	auto result = environment.GetEntity(testEntity);

	EXPECT_TRUE(result);
}

TEST(EnvironmentTest, TestAddLight)
{
	const std::string testLight = "TestLight";

	Environment environment("test");
	auto result = environment.AddLight(testLight, std::make_unique<Light>());

	EXPECT_TRUE(result);
}

TEST(EnvironmentTest, TestAddingExistingLight)
{
	const std::string testLight = "TestLight";

	Environment environment("test");
	auto result = environment.AddLight(testLight, std::make_unique<Light>());

	result = environment.AddLight(testLight, std::make_unique<Light>());
	EXPECT_FALSE(result);
}

TEST(EnvironmentTest, TestGetLight)
{
	const std::string testLight = "TestLight";

	Environment environment("test");
	environment.AddLight(testLight, std::make_unique<Light>());
	
	auto result = environment.GetLight(testLight);

	EXPECT_TRUE(result);
}
