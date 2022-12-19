#include <gtest/gtest.h>
#include "application.h"
#include "fixtures_location.h"
#include "test_runner.h"

namespace ns {

using NeonSandboxTest = TestRunner;

TEST_F(NeonSandboxTest, Setup) {
  Application application;
  ASSERT_TRUE(Run(application));
}

}  // namespace ns
