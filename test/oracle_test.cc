//
// Created by h.teranishi <teranishihiroki@gmail.com>
// Copyright (c) 2017 Hiroki Teranishi. All rights reserved.
//

#include <gtest/gtest.h>

class OracleTest : public ::testing::Test {};

TEST_F(OracleTest, Transition) {
 ASSERT_EQ(2, 1 + 1);
}
