#ifndef FILE_LOCK_H
#define FILE_LOCK_H
#ifdef WIN32
#include <windows.h>
#else
#include <sys/file.h>
#include <unistd.h>
#endif
#include <napi.h>
#include <iostream>

class FileLock : public Napi::ObjectWrap<FileLock>
{
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    FileLock(const Napi::CallbackInfo &info);
    ~FileLock();

    static Napi::FunctionReference constructor;

    Napi::Value New(const Napi::CallbackInfo &info);
    Napi::Value Lock(const Napi::CallbackInfo &info);
    Napi::Value Unlock(const Napi::CallbackInfo &info);
    Napi::Value IsLocked(const Napi::CallbackInfo &info);
    Napi::Value GetFilePath(const Napi::CallbackInfo &info);

private:
    bool m_bLocked = false;
    std::string m_sFilePath;

#ifdef _WIN32
    HANDLE m_hFileHandle;
#else
    int m_nFd;
#endif
};

#endif