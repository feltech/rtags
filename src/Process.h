#ifndef PROCESS_H
#define PROCESS_H

#include "ByteArray.h"
#include "Path.h"
#include "List.h"
#include <signalslot.h>
#include <deque>

class Process
{
public:
    Process();
    ~Process();

    void setCwd(const Path& cwd);

    void start(const ByteArray& command, const List<ByteArray>& arguments);
    void start(const ByteArray& command, const List<ByteArray>& arguments,
               const List<ByteArray>& environ);

    void write(const ByteArray& data);
    void closeStdIn();

    ByteArray readAllStdOut();
    ByteArray readAllStdErr();

    bool isDone() const { return mPid == -1; }
    int returnCode() const { return mReturn; }

    void stop();

    signalslot::Signal0& readyReadStdOut() { return mReadyReadStdOut; }
    signalslot::Signal0& readyReadStdErr() { return mReadyReadStdErr; }
    signalslot::Signal0& finished() { return mFinished; }

    static List<ByteArray> environment();

    static Path findCommand(const ByteArray& command);

private:
    static void processCallback(int fd, unsigned int flags, void* userData);

    void closeStdOut();
    void closeStdErr();

    void handleInput(int fd);
    void handleOutput(int fd, ByteArray& buffer, int& index, signalslot::Signal0& signal);
    void handleTerminated();

private:
    int mStdIn[2];
    int mStdOut[2];
    int mStdErr[2];

    pid_t mPid;
    int mReturn;

    std::deque<ByteArray> mStdInBuffer;
    ByteArray mStdOutBuffer, mStdErrBuffer;
    int mStdInIndex, mStdOutIndex, mStdErrIndex;

    Path mCwd;

    signalslot::Signal0 mReadyReadStdOut, mReadyReadStdErr, mFinished;
};

#endif
