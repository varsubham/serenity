/*
 * Copyright (c) 2020, the SerenityOS developers.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibTest/TestCase.h>

#include "../CSV.h"
#include "../XSV.h"
#include <LibCore/MemoryStream.h>

TEST_CASE(can_write)
{
    Vector<Vector<int>> data = {
        { 1, 2, 3 },
        { 4, 5, 6 },
        { 7, 8, 9 },
    };

    Core::Stream::AllocatingMemoryStream stream;
    auto csv = Writer::CSV(Core::Stream::Handle<Core::Stream::Stream>(stream), data);
    MUST(csv.generate());

    auto expected_output = R"~(1,2,3
4,5,6
7,8,9
)~"sv;

    auto buffer = MUST(stream.read_until_eof());
    EXPECT_EQ(StringView { buffer.bytes() }, expected_output);
}

TEST_CASE(can_write_with_header)
{
    Vector<Vector<int>> data = {
        { 1, 2, 3 },
        { 4, 5, 6 },
        { 7, 8, 9 },
    };

    Core::Stream::AllocatingMemoryStream stream;
    auto csv = Writer::CSV(Core::Stream::Handle<Core::Stream::Stream>(stream), data, { "A"sv, "B\""sv, "C"sv });
    MUST(csv.generate());

    auto expected_output = R"~(A,"B""",C
1,2,3
4,5,6
7,8,9
)~"sv;

    auto buffer = MUST(stream.read_until_eof());
    EXPECT_EQ(StringView { buffer.bytes() }, expected_output);
}

TEST_CASE(can_write_with_different_behaviors)
{
    Vector<Vector<DeprecatedString>> data = {
        { "Well", "Hello\"", "Friends" },
        { "We\"ll", "Hello,", "   Friends" },
    };

    Core::Stream::AllocatingMemoryStream stream;
    auto csv = Writer::CSV(Core::Stream::Handle<Core::Stream::Stream>(stream), data, { "A"sv, "B\""sv, "C"sv }, Writer::WriterBehavior::QuoteOnlyInFieldStart | Writer::WriterBehavior::WriteHeaders);
    MUST(csv.generate());

    auto expected_output = R"~(A,B",C
Well,Hello",Friends
We"ll,"Hello,",   Friends
)~"sv;

    auto buffer = MUST(stream.read_until_eof());
    EXPECT_EQ(StringView { buffer.bytes() }, expected_output);
}
