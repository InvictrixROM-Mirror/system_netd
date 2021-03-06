/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <array>
#include <cstdint>

#include <gtest/gtest.h>

#include "netdutils/Slice.h"
#include "netdutils/Status.h"
#include "netdutils/StatusOr.h"

namespace android {
namespace netdutils {

class SliceTest : public testing::Test {
  protected:
    std::array<char, 256> mRaw = {};
};

TEST_F(SliceTest, smoke) {
    Slice s1 = makeSlice(mRaw);
    Slice s2 = makeSlice(mRaw);
    auto p = split(s1, 14);
    std::stringstream ss;
    ss << Slice();
    EXPECT_EQ("Slice[base: 0x0, limit: 0x0, size: 0x0]", ss.str());
    constexpr size_t kBytes = 14;
    EXPECT_EQ(s1.base(), take(s1, kBytes).base());
    EXPECT_EQ(kBytes, take(s1, kBytes).size());
    EXPECT_EQ(s1.base() + kBytes, drop(s1, kBytes).base());
    EXPECT_EQ(s1.size() - kBytes, drop(s1, kBytes).size());
    double a = 0;
    double b = 0;
    int c = 0;
    EXPECT_EQ(sizeof(a), extract(s1, a));
    EXPECT_EQ(sizeof(a) + sizeof(b), extract(s1, a, b));
    EXPECT_EQ(sizeof(a) + sizeof(b) + sizeof(c), extract(s1, a, b, c));
}

TEST_F(SliceTest, constructor) {
    // Expect the following lines to compile
    Slice s1 = makeSlice(mRaw);
    Slice s2(s1);
    Slice s3 = s2;
    const Slice s4(s3);
    const Slice s5 = s4;
    s3 = s5;
    Slice s6(mRaw.data(), mRaw.size());
    Slice s7(mRaw.data(), mRaw.data() + mRaw.size());
    struct {
      int a;
      double b;
      float c;
    } anon;
    makeSlice(anon);
    EXPECT_EQ(reinterpret_cast<uint8_t*>(mRaw.data()), s1.base());
    EXPECT_EQ(reinterpret_cast<uint8_t*>(mRaw.data()) + mRaw.size(), s1.limit());
    EXPECT_EQ(mRaw.size(), s1.size());
    EXPECT_FALSE(mRaw.empty());
    EXPECT_TRUE(Slice().empty());
    EXPECT_TRUE(Slice(nullptr, static_cast<size_t>(0)).empty());
    EXPECT_TRUE(Slice(nullptr, nullptr).empty());
}

}  // namespace netdutils
}  // namespace android
