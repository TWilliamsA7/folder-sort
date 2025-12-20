// tests/core/actions/ActionExecutorTests.cpp

#include <gtest/gtest.h>
#include "core/actions/ActionExecutor.hpp"
#include "core/actions/MoveAction.hpp"

TEST(ActionExecutorTests, DryRunDoesNotThrowOrModify) {
    ActionExecutor executor(/* dry_run = */true);

    FileInfo file;
    file.path = "fake.txt";

    std::vector<std::unique_ptr<Action>> actions;

    actions.push_back(
        std::make_unique<MoveAction>(file, "dest/")
    );

    EXPECT_NO_THROW(executor.execute(std::move(actions)));
}