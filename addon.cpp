#include <napi.h>
#include <windows.h>
#include <lm.h>
#pragma comment(lib, "netapi32.lib")

Napi::String CheckUserPrivilege(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "Expected a string argument").ThrowAsJavaScriptException();
        return Napi::String::New(env, "");
    }

    std::string userName = info[0].As<Napi::String>().Utf8Value();
    std::wstring wUserName(userName.begin(), userName.end());
    
    LPUSER_INFO_2 pBuf = nullptr;
    NET_API_STATUS nStatus = NetUserGetInfo(NULL, wUserName.c_str(), 2, (LPBYTE*)&pBuf);

    if (nStatus != NERR_Success) {
        return Napi::String::New(env, "User not found");
    }

    std::string privilege;
    switch (pBuf->usri2_priv) {
        case USER_PRIV_GUEST:
            privilege = "Guest";
            break;
        case USER_PRIV_USER:
            privilege = "User";
            break;
        case USER_PRIV_ADMIN:
            privilege = "Administrator";
            break;
        default:
            privilege = "Unknown";
            break;
    }

    NetApiBufferFree(pBuf);

    return Napi::String::New(env, privilege);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "checkUserPrivilege"), Napi::Function::New(env, CheckUserPrivilege));
    return exports;
}

NODE_API_MODULE(addon, Init)
