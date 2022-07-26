#include "file_lock.h"

Napi::Object FileLock::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "FileLock",
                                      {
                                          InstanceMethod("lock", &FileLock::Lock),
                                          InstanceMethod("unlock", &FileLock::Unlock),
                                          InstanceMethod("isLocked", &FileLock::IsLocked),
                                          InstanceMethod("getFilePath", &FileLock::GetFilePath),
                                          InstanceMethod("setFilePath", &FileLock::SetFilePath),
                                      });

    Napi::FunctionReference *constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);

    exports.Set("FileLock", func);
    return exports;
}

FileLock::FileLock(const Napi::CallbackInfo &info) : Napi::ObjectWrap<FileLock>(info)
{
    Napi::String value = info[0].As<Napi::String>();
    this->m_sFilePath = value.Utf8Value();
}

FileLock::~FileLock()
{
#ifdef _WIN32
    if (this->m_bLocked && this->m_hFileHandle != NULL)
    {
        CloseHandle(this->m_hFileHandle);
    }
    this->m_hFileHandle = NULL;
#else
    if (this->m_bLocked && this->m_nFd)
    {
        flock(this->m_nFd, LOCK_UN);
        close(this->m_nFd);
    }
#endif
    this->m_bLocked = false;
}

#ifdef WIN32
std::wstring s2ws(const std::string &s, bool isUtf8 = true)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(isUtf8 ? CP_UTF8 : CP_ACP, 0, s.c_str(), slength, 0, 0);
    std::wstring buf;
    buf.resize(len);
    MultiByteToWideChar(isUtf8 ? CP_UTF8 : CP_ACP, 0, s.c_str(), slength,
                        const_cast<wchar_t *>(buf.c_str()), len);
    return buf;
}
#endif

Napi::Value FileLock::Lock(const Napi::CallbackInfo &info)
{
#ifdef _WIN32
    HANDLE hFile = CreateFileW(s2ws(this->m_sFilePath).c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        Napi::Error::New(info.Env(), "Failed to open file").ThrowAsJavaScriptException();
        return Napi::Boolean::New(info.Env(), false);
    }
    this->m_hFileHandle = hFile;
    this->m_bLocked = true;
    return Napi::Boolean::New(info.Env(), true);
#else
    int fd = open(this->m_sFilePath.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    int res = flock(fd, LOCK_EX | LOCK_NB);
    if (res == -1)
    {
        Napi::Error::New(info.Env(), "Failed to lock file").ThrowAsJavaScriptException();
        return Napi::Boolean::New(info.Env(), false);
    }
    this->m_bLocked = true;
    this->m_nFd = fd;
    return Napi::Boolean::New(info.Env(), true);
#endif
    return Napi::Boolean::New(info.Env(), false);
}

Napi::Value FileLock::Unlock(const Napi::CallbackInfo &info)
{
#ifdef _WIN32
    if (this->m_bLocked && this->m_hFileHandle != NULL)
    {
        CloseHandle(this->m_hFileHandle);
        this->m_hFileHandle = NULL;
    }
    this->m_bLocked = false;
    return Napi::Boolean::New(info.Env(), true);
#else
    if (this->m_bLocked && this->m_nFd)
    {
        int r = flock(this->m_nFd, LOCK_UN);
        if (r == -1)
        {
            Napi::Error::New(info.Env(), "Failed to unlock file").ThrowAsJavaScriptException();
            return Napi::Boolean::New(info.Env(), false);
        }
        close(m_nFd);
    }
    this->m_bLocked = false;
    return Napi::Boolean::New(info.Env(), true);
#endif
    return Napi::Boolean::New(info.Env(), false);
}

Napi::Value FileLock::IsLocked(const Napi::CallbackInfo &info)
{
    return Napi::Boolean::New(info.Env(), this->m_bLocked);
}

Napi::Value FileLock::GetFilePath(const Napi::CallbackInfo &info)
{
    if (this->m_sFilePath.empty())
    {
        return info.Env().Undefined();
    }
    return Napi::String::New(info.Env(), this->m_sFilePath);
}

Napi::Value FileLock::SetFilePath(const Napi::CallbackInfo &info)
{
    Napi::String value = info[0].As<Napi::String>();
    this->m_sFilePath = value.Utf8Value();
    return Napi::Boolean::New(info.Env(), true);
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
    return FileLock::Init(env, exports);
}

NODE_API_MODULE(filelock, InitAll);
