#pragma once

#include "gtest/gtest.h"
#include "macros.h"

namespace ns {

class Application;

class TestRunner : public ::testing::Test {
 public:
  TestRunner();

  ~TestRunner();

  bool Run(Application& application) const;

  void SetUp() override;

  void TearDown() override;

 private:
  NS_DISALLOW_COPY_AND_ASSIGN(TestRunner);
};

}  // namespace ns
