//
// Created by gonzalezext on 09.02.24.
//

#include <iostream>
#include <queue>
#include <mutex>
#include <thread>

#include "gtest/gtest.h"

#include "../server.h"

TEST (StreamTest, TestOutputData) {
    std::istringstream in("LAKJSDFLKASDaCVBLXDKJk");
    std::ostringstream out;

    runServer(in, out);

    ASSERT_EQ("a letter\n", out.str());
}


#define WAITING_DATA (-2)

class my_stringstreambuf : public std::basic_streambuf<char, std::char_traits<char>> {
private:
    std::queue<int> q;
    std::mutex mut;
public:
    explicit my_stringstreambuf(const std::string &initText) {
        std::lock_guard<std::mutex> lock(mut);
        for (auto c: initText) {
            q.push((int) c);
        }
    }

    void appendStr(const std::string &next) {
        std::lock_guard<std::mutex> lock(mut);
        for (auto c: next) {
            q.push(c);
        }
    }

    void finishStream() {
        std::lock_guard<std::mutex> lock(mut);
        q.push(EOF);
    }

protected:
    int uflow() override {
        while (currentChar() == WAITING_DATA) {}
        int c = nextChar();
        return c;
    }

    int currentChar() {
        std::lock_guard<std::mutex> lock(mut);
        return getChar();
    }

    int nextChar() {
        std::lock_guard<std::mutex> lock(mut);
        int c = getChar();
        q.pop();
        return c;
    }

private:
    int getChar() {
        if (q.empty()) {
            return WAITING_DATA;
        }
        return (int) q.front();
    }
};

TEST (StreamTest, TestSyncInputOutputData) {
    // create  our buffer
    auto buf = new my_stringstreambuf("text with a");

    // create the streams
    std::istream in(buf);
    std::ostringstream out;

    // execute server
    std::thread runServerThread(runServer, std::ref(in), std::ref(out));
    while(out.str() != "a letter\n") {} // wait for server to response

    // sent more data to the server thrown the buffer
    buf->appendStr("new text with a");
    while(out.str() != "a letter\na letter\n") {} // wait for server to response

    // send eof to finish stream
    buf->finishStream();
    // wait for thread to finish
    runServerThread.join();
}
