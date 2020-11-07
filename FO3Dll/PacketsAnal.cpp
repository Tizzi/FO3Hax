#include "PacketsAnal.h"
#include <ws2def.h>
#include <WinSock2.h>

#define STACKPOS__COUNT_OF_BUFS 2

HookSetter* hs_send = nullptr;
LPWSABUF _buffer = nullptr;
PDWORD _stack = nullptr;
bool* _outputP = nullptr;
void hkWSASend() {
	_asm
	{
		mov dword ptr ds : [_buffer] , eax
		mov dword ptr ds : [_stack], esp
	}

	if (_buffer != nullptr && *_outputP)
	{
		printf_s(("[SEND] count=%d adr=%p | adr_buf=%p | len=%d\n"),
			_stack[STACKPOS__COUNT_OF_BUFS], &_buffer->buf, _buffer->buf, _buffer[0].len
		);
		__printer((PBYTE)_buffer[0].buf, _buffer[0].len);
		printf_s(("===========================[WSASendEnd]======================\n\n"));
	}


	typedef  void(*fTmp)();
	fTmp __exit = fTmp(hs_send->OriginalOps);
	__exit();
}

bool* _inputP = nullptr;
HookSetter* hs_netprocess = nullptr;
void hkNetProcess() {
	if (*_inputP)
		printf_s(("*************************************************************\n"));
	typedef  void(*fTmp)();
	fTmp __exit = fTmp(hs_netprocess->OriginalOps);
	__exit();
}

HookSetter* hs_recv = nullptr;
void hkWSARecvEnd() {
	if (*_inputP)
		printf_s(("\n===========================[WSARecvEnd]======================\n"));
	typedef  void(*fTmp)();
	fTmp __exit = fTmp(hs_recv->OriginalOps);
	__exit();
}

void PacketsAnal::SetInlineHook(bool *input, bool *output)
{
	_outputP = output;
	_inputP = input;
	//if (input) {
		hs_netprocess = CrtHookSetter((PBYTE)GET_ADR(FO3_NETPROCESS), (DWORD)hkNetProcess, 6);
		SetHookSetter(hs_netprocess);
		hs_recv = CrtHookSetter((PBYTE)GET_ADR(FO3_WSARECVEND), (DWORD)hkWSARecvEnd, 5);
		SetHookSetter(hs_recv);
	//}
	
	//if (output) {
		hs_send = CrtHookSetter((PBYTE)GET_ADR(FO3_WSASEND), (DWORD)hkWSASend, 6);
		SetHookSetter(hs_send);
	//}
}

void PacketsAnal::Unset()
{
	UnsetHook(hs_send);
	UnsetHook(hs_recv);
	UnsetHook(hs_netprocess);
}
