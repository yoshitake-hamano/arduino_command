// Copyright 2018 yoshitake. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#include "unit_test.hpp"

#include "string.h"
#include "command/CommandLineParser.h"

void test_parse()
{
    const char *line = "set_ssid  abc";
    CommandLineParser parser(line);
    UT_ASSERT_TRUE(parser.Parse());
    UT_ASSERT_STRING("set_ssid", parser.GetName());
    const char *arg = parser.GetFirstArg();
    UT_ASSERT_STRING("abc", arg);
    arg = parser.NextArg(arg);
    UT_ASSERT_POINTER((void*)0, arg);
}

void test_parse_with_white_space()
{
    const char *line = "set_ssid abc def";
    CommandLineParser parser(line);
    UT_ASSERT_TRUE(parser.Parse());
    UT_ASSERT_STRING("set_ssid", parser.GetName());
    const char *arg = parser.GetFirstArg();
    UT_ASSERT_STRING("abc def", arg);
    arg = parser.NextArg(arg);
    UT_ASSERT_POINTER((void*)0, arg);
}

int main(int argc, char **argv)
{
    UT_RUN(test_parse);
    // UT_RUN(test_parse_with_white_space);
    return 0;
}
