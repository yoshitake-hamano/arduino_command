// Copyright 2018 yoshitake. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#pragma once

#include <sstream>

class IOStream : public Stream {
public:
    IOStream(std::istream &readStream, std::ostream &writeStream)
        : readStream(readStream), writeStream(writeStream) {}
    virtual ~IOStream() {}

public:
    int available() {
        return readStream.rdbuf()->in_avail();
    }
    int read() {
        if (available() == 0) {
            return -1;
        }

        return readStream.get();
    }
    size_t write(uint8_t ch) {
        writeStream << ch;
        return 1;
    }
    size_t write(const char *str)
    {
        if(str == NULL) {
            return 0;
        }
        int length = strlen(str);
        writeStream << str;
        return length;
    }
    size_t write(const uint8_t *buffer, size_t size) {
        for (size_t i=0; i<size; i++) {
            writeStream << buffer[i];
        }
        return size;
    }
    size_t print(int i) {
        writeStream << i;
        return 1;
    }

private:
    std::istream &readStream;
    std::ostream &writeStream;
};
