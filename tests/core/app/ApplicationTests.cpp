// tests/core/app/ApplicationTests.cpp

#include <gtest/gtest.h>
#include "core/app/Application.hpp"
#include "../tests/helpers/TestFilesystem.hpp"

TEST(ApplicationTest, DoesNothingOnDryRun) {
    TestTree temp("temp");
    auto search_dir = temp.dir("search_dir");
    temp.file(search_dir, "x.jpg");
    temp.file(search_dir, "y.png");
    temp.file(search_dir, "z.pdf");

    auto rules = R"(
        rules:
            - name: "Archive images"
              when: 
                extension: [".png", ".jpg"]
              then:
                - action: move
                  to: "Images/"
    )";

    auto rules_file = temp.root / "rules.yaml";
    std::ofstream(rules_file) << rules;

    AppConfig config;
    config.dry_run = true;
    config.verbose = true;
    config.log_dir = temp.dir("log");
    config.root_dir = search_dir;
    config.rules_file = rules_file;

    Application app(config);
    int result = app.run();

    EXPECT_EQ(result, 0);
    EXPECT_TRUE(std::filesystem::exists(search_dir / "x.jpg"));
    std::filesystem::remove_all(temp.root);
}