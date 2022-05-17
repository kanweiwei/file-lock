#include "fileLock.h"

std::string UTF8_To_string(const std::string &str)
{
    int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

    wchar_t *pwBuf = new wchar_t[nwLen + 1];
    memset(pwBuf, 0, nwLen * 2 + 2);

    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

    int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

    char *pBuf = new char[nLen + 1];
    memset(pBuf, 0, nLen + 1);

    WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

    std::string retStr = pBuf;

    delete[] pBuf;
    delete[] pwBuf;
    pBuf = NULL;
    pwBuf = NULL;

    return retStr;
}


Napi::Object FileLock::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "FileLock",
                                      {
                                          InstanceMethod("lock", &FileLock::Lock),
                                          InstanceMethod("unlock", &FileLock::Unlock),
                                          InstanceMethod("isLocked", &FileLock::IsLocked),
                                          InstanceMethod("getFilePath", &FileLock::GetFilePath),
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
    this->_filePath = value.Utf8Value();
}

FileLock::~FileLock()
{
#ifdef WIN32
    this->_hFile = NULL;
#endif

    this->_locked = false;
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
#ifdef WIN32
    HANDLE hFile = CreateFileW(s2ws(this->_filePath).c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        Napi::Error::New(info.Env(), "Failed to open file").ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }
    this->_hFile = hFile;
    this->_locked = true;
    return Napi::Boolean::New(info.Env(), true);
#endif
    return Napi::Boolean::New(info.Env(), false);
}

Napi::Value FileLock::Unlock(const Napi::CallbackInfo &info)
{
#ifdef WIN32
    if (this->_hFile != NULL)
    {
        CloseHandle(this->_hFile);
        this->_hFile = NULL;
    }
    return Napi::Boolean::New(info.Env(), true);
#endif
    return Napi::Boolean::New(info.Env(), false);
}

Napi::Value FileLock::IsLocked(const Napi::CallbackInfo &info)
{
    return Napi::Boolean::New(info.Env(), this->_locked);
}

Napi::Value FileLock::GetFilePath(const Napi::CallbackInfo &info)
{
    return Napi::String::New(info.Env(), this->_filePath);
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports)
{
    return FileLock::Init(env, exports);
}

NODE_API_MODULE(filelock, InitAll);
