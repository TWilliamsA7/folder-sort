// tests/config/ConditionFactoryTests.cpp

#include <gtest/gtest.h>
#include "config/ConditionFactory.hpp"

TEST(ConditionFactoryTests, CreatesExpectedSizeCondition) {
    const char* yaml = R"(
        size: "> 10MB"
    )";

    YAML::Node root = YAML::Load(yaml);
    std::vector<std::unique_ptr<Condition>> conditions = ConditionFactory::build(root);

    ASSERT_EQ(conditions.size(), 1);

    SizeCondition* sizeCond = dynamic_cast<SizeCondition*>(conditions.at(0).get());

    ASSERT_NE(sizeCond, nullptr);

    EXPECT_EQ(sizeCond->comparison(), Comparison::GREATER);
    EXPECT_EQ(sizeCond->size(), 10 * 1000 * 1000);

}

TEST(ConditionFactoryTests, CreatesExpectedExtensionCondition) {
    const char* yaml = R"(
        extension: [".png", ".jpg"]
    )";

    YAML::Node root = YAML::Load(yaml);
    std::vector<std::unique_ptr<Condition>> conditions = ConditionFactory::build(root);

    ASSERT_EQ(conditions.size(), 1);

    ExtensionCondition* extCond = dynamic_cast<ExtensionCondition*>(conditions.at(0).get());

    ASSERT_NE(extCond, nullptr);

    EXPECT_EQ(extCond->extensions().size(), 2);
}

TEST(ConditionFactoryTests, ThrowsForInvalidSize) {
    const char* yaml = R"(
        size: "> 10A"
    )";

    YAML::Node root = YAML::Load(yaml);
    EXPECT_ANY_THROW(ConditionFactory::build(root));
}

TEST(ConditionFactoryTests, CreatesMultipleConditions) {
    const char* yaml = R"(
        extension: [".jpg", ".PNG"]
        size: "< 1.5GB"
    )";

    YAML::Node root = YAML::Load(yaml);
    std::vector<std::unique_ptr<Condition>> conditions = ConditionFactory::build(root);

    ASSERT_EQ(conditions.size(), 2);
}