#include "pch.h"
#include <GameEngine/Entity.h>

TEST(EntityTest, TestName)
{
	std::string expected = "EntityName";

	Entity entity;
	entity.SetName(expected);

	EXPECT_EQ(entity.GetName(), expected);
}

TEST(EntityTest, TestActive)
{
	bool expected = true;

	Entity entity;
	
	EXPECT_EQ(entity.IsActive(), expected);

	expected = false;
	entity.SetActive(expected);

	EXPECT_EQ(entity.IsActive(), expected);

}

TEST(EntityTest, TestComponentParent)
{
	std::unique_ptr<Component> expected = std::make_unique<Component>(std::string("Test"));
	Entity entity;

	entity.AddComponent(std::move(expected));

	EXPECT_EQ(entity.GetComponent<Component>().GetParent(), &entity);
}