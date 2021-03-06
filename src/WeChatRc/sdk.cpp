// WeChatRc.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "../common/rpc.h"
#include "../common/rpcdef.h"
#include "../WeChatRs/sdkdef_h.h"
#include "../common/sdkinf.h"

#include <map>;

std::map<DWORD, RPC_WSTR> g_StringBinding;

//WechatRs_IfHandle：对于client端，由于不同端口绑定的统一个WechatRs_IfHandle地址，所以并发区分不同的server
//只能改为每次connect-call-disconnect模式。

//unused
int ConnectSDKServer0(DWORD pid)
{
    RPC_WSTR StringBinding = NULL;
    WCHAR wszEndPoint[MAX_PATH] = { 0 };
    wsprintf(wszEndPoint, L"%ws_%d", g_Endpoint, pid);
    RPC_STATUS status = RpcConnectServer((RPC_WSTR)wszEndPoint, &WechatRs_IfHandle, &StringBinding);
    
    g_StringBinding[pid] = StringBinding;

    return status;
}

//unused
int DisconnectSDKServer0(DWORD pid)
{
    std::map<DWORD, RPC_WSTR>::iterator it = g_StringBinding.find(pid);
    
    if (it != g_StringBinding.end()) {
        RpcDisconnectServer(&WechatRs_IfHandle, &it->second);
    }

    return 0;
}

int ConnectSDKServer(DWORD pid, RPC_WSTR* StringBinding)
{
    WCHAR wszEndPoint[MAX_PATH] = { 0 };
    wsprintf(wszEndPoint, L"%ws_%d", g_Endpoint, pid);
    RPC_STATUS status = RpcConnectServer((RPC_WSTR)wszEndPoint, &WechatRs_IfHandle, StringBinding);
    return status;
}

int DisconnectSDKServer(DWORD pid, RPC_WSTR* StringBinding)
{
    RpcDisconnectServer(&WechatRs_IfHandle, StringBinding);
    return 0;
}

//TODO: edit the file to add interface

int GetSDKInterface(PWechatSDKInterface p)
{
    if (p) {
        p->Initialize = Initialize;
        p->AntiRevokeMsg = AntiRevokeMsg;
        p->UnAntiRevokeMsg = UnAntiRevokeMsg;
        p->StartSaveVoiceMsg = StartSaveVoiceMsg;
        p->StopSaveVoiceMsg = StopSaveVoiceMsg;
        p->SendTextMsg = SendTextMsg;
    }
    
    return 0;
}