#include <gtest/gtest.h>
#include <filesystem>
#include <iostream>
#include "messageManager.h"

namespace fs = std::filesystem;

class MessageFileTest : public ::testing::Test {
protected:
    std::string file = "test.bin";

    void SetUp() override {
        if (fs::exists(file)) fs::remove(file);
    }

    void TearDown() override {
        if (fs::exists(file)) fs::remove(file);
    }
};

TEST_F(MessageFileTest, WriteAndReadSingleMessage) {
    bool ok = writeMessage(file, "PrivetTest");
    EXPECT_TRUE(ok);

    auto messages = readMessages(file);
    ASSERT_EQ(messages.size(), 1);
    EXPECT_EQ(messages[0].text, "PrivetTest");
}

TEST_F(MessageFileTest, WriteMultipleMessages) {
    writeMessage(file, "test1");
    writeMessage(file, "TEST2");
    writeMessage(file, "testtest");

    auto messages = readMessages(file);
    ASSERT_EQ(messages.size(), 3);
    EXPECT_EQ(messages[0].text, "test1");
    EXPECT_EQ(messages[1].text, "TEST2");
    EXPECT_EQ(messages[2].text, "testtest");
}

TEST_F(MessageFileTest, ClearFileRemovesMessages) {
    writeMessage(file, "Test");
    clearFile(file);

    auto messages = readMessages(file);
    EXPECT_TRUE(messages.empty());
}

TEST_F(MessageFileTest, MessageIsTrimmedTo20Chars) {
    std::string longMsg(50, 'X');
    writeMessage(file, longMsg);

    auto messages = readMessages(file);
    ASSERT_EQ(messages.size(), 1);
    EXPECT_EQ(messages[0].text.size(), 20);
}
