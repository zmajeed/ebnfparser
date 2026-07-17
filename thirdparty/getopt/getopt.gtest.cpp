// getopt/getopt.gtest.cpp

/*
MIT License

Copyright (c) 2024-2026 Zartaj Majeed

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "getopt.h"

#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;
using namespace ::testing;

namespace getoptx::testing {

TEST(Getopt, test_0000) {
  const char* argv[] = {
    "prog",
    "-h",
    0
  };
  int argc = sizeof(argv) / sizeof(*argv);
  optind = 1;

  EXPECT_EQ((getopt(argc, (char* const *)argv, "h")), 'h');
  EXPECT_EQ((getopt(argc, (char* const *)argv, "h")), -1);
  EXPECT_EQ(optind, 2);
}

TEST(Getopt, test_0001) {
  optind = 1;
  const char* argv[] = {
    "prog",
    "-a",
    "-c",
    0
  };
  int argc = sizeof(argv) / sizeof(*argv);
  const char* opts = "ac";
  optind = 1;

  EXPECT_EQ((getopt(argc, (char* const *)argv, opts)), 'a');
  EXPECT_EQ((getopt(argc, (char* const *)argv, opts)), 'c');
  EXPECT_EQ((getopt(argc, (char* const *)argv, "h")), -1);
  EXPECT_EQ(optind, 3);
}

TEST(Getopt, test_0002) {
  const char* argv[] = {
    "prog",
    "-ab",
    0
  };
  int argc = sizeof(argv) / sizeof(*argv);
  const char* opts = "ab";
  optind = 1;

  EXPECT_EQ((getopt(argc, (char* const *)argv, opts)), 'a');
  EXPECT_EQ((getopt(argc, (char* const *)argv, opts)), 'b');

  EXPECT_EQ((getopt(argc, (char* const *)argv, "h")), -1);
  EXPECT_EQ(optind, 2);
}

TEST(Getopt, test_0003) {
  const char* argv[] = {
    "prog",
    "-a",
    "optval",
    0
  };
  int argc = sizeof(argv) / sizeof(*argv);
  const char* opts = "a:";
  optind = 1;

  EXPECT_EQ((getopt(argc, (char* const *)argv, opts)), 'a');
  EXPECT_STREQ(optarg, "optval");

  EXPECT_EQ((getopt(argc, (char* const *)argv, "h")), -1);
  EXPECT_EQ(optind, 3);
}

TEST(Getopt, test_0004) {
  const char* argv[] = {
    "prog",
    "-aoptval",
    0
  };
  int argc = sizeof(argv) / sizeof(*argv);
  const char* opts = "a:";
  optind = 1;

  EXPECT_EQ((getopt(argc, (char* const *)argv, opts)), 'a');
  EXPECT_STREQ(optarg, "optval");

  EXPECT_EQ((getopt(argc, (char* const *)argv, "h")), -1);
  EXPECT_EQ(optind, 2);
}

TEST(Getopt, test_0005) {
  const char* argv[] = {
    "prog",
    "-b",
    0
  };
  int argc = sizeof(argv) / sizeof(*argv);
  const char* opts = "a";
  optind = 1;

  EXPECT_EQ((getopt(argc, (char* const *)argv, opts)), '?');
  EXPECT_EQ(optopt, 'b');

  EXPECT_EQ((getopt(argc, (char* const *)argv, "h")), -1);
  EXPECT_EQ(optind, 2);
}

TEST(GetoptLong, test_0000) {
  const char* argv[] = {
    "prog",
    "-a",
    "123",
    "--opt_a",
    "456",
    0
  };
  int argc = sizeof(argv) / sizeof(*argv);
  const char* opts = "a:h";

  int opt_a = 0;

  option longOpts[] = {
    {"opt_a", required_argument, &opt_a, 1},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
  };

  optind = 1;

  EXPECT_EQ((getopt_long(argc, (char* const *)argv, opts, longOpts, 0)), 'a');
  EXPECT_STREQ(optarg, "123");
  EXPECT_EQ(opt_a, 0);

  EXPECT_EQ((getopt_long(argc, (char* const *)argv, opts, longOpts, 0)), 0);
  EXPECT_STREQ(optarg, "456");
  EXPECT_EQ(opt_a, 1);

  EXPECT_EQ((getopt(argc, (char* const *)argv, "h")), -1);
  EXPECT_EQ(optind, 5);
}

}
