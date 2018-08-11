// Copyright 2018 yoshitake. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.

#include "CommandLine.h"

#include "log/Log.h"
#include "Command.h"
#include "CommandLineParser.h"

CommandLine::CommandLine() : commandSize(0)
{
}

void CommandLine::Initialize(Stream *stream)
{
    this->stream = stream;
    buf = "";
}

bool CommandLine::AddCommand(Command *command)
{
    if (commandSize >= kCommandCapacity) {
        return false;
    }
    commands[commandSize] = command;
    ++commandSize;
    return true;
}

bool CommandLine::Analyze()
{
    bool bSerialResult = AnalyzeSerial();
    bool bStreamResult = AnalyzeStream();
    return bSerialResult || bStreamResult;
}

bool CommandLine::AnalyzeSerial()
{
    if (!Serial.available()) {
        return false;
    }

    while (Serial.available()) {
        char ch = Serial.read();
        analyzeChar(ch);
    }
    return true;
}

bool CommandLine::AnalyzeStream()
{
    if (!stream) {
        return true;
    }

    while (stream->available()) {
        char ch = stream->read();
        analyzeChar(ch);
    }
    return true;
}

void CommandLine::analyzeChar(char ch)
{
    if (ch == '\r') {
        return;
    }
    Serial.write(ch);
    if (stream) {
        stream->write(ch);
    }

    switch (ch) {
    case '\n':
        if (buf.length() != 0) {
            int result = executeCommandLine(buf.c_str());
            Serial.write("\nresult: ");
            Serial.print(result);
            Serial.write("\n\n");
            if (stream) {
                stream->write("\nresult: ");
                stream->print(result);
                stream->write("\n\n");
            }
        }
        buf = "";
        break;
    default:
        buf += ch;
        break;
    }
}

int CommandLine::executeCommandLine(const char *line)
{
    Log::Info(line);
    CommandLineParser parser(line);
    if (!parser.Parse()) {
        Log::Error("parser: buffer over error");
        return 1;
    }

    for (int i=0; i<commandSize; i++) {
        Command* command = commands[i];
        if (strcmp(parser.GetName(), command->GetName()) == 0) {
            int result = command->Execute(&parser);
            std::string log = command->GetName();
            log += ": returns ";
            log += result;
            Log::Info(log.c_str());
            return result;
        }
    }
    Log::Error("parser: unknown command");
    return 2;
}

