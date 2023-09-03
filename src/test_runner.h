#pragma once

#include "gtest/gtest.h"
#include "macros.h"

namespace merle {

class Application;

class TestRunner : public ::testing::Test {
 public:
  TestRunner();

  ~TestRunner();

  bool Run(Application& application) const;

  void SetUp() override;

  void TearDown() override;

 private:
  MERLE_DISALLOW_COPY_AND_ASSIGN(TestRunner);
};

}  // namespace merle
