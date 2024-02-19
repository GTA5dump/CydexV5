#include "nativeinvoker.hpp"
#include "../../cheat/log.hpp"
#include "../../hooking/hooking.hpp"
#include "../../hooking/native/crossmap.hpp"
#include <Windows.h>
#include <unordered_map>

static NativeManagerContext g_context;
static UINT64 g_hash;
void(*scrNativeCallContext::SetVectorResults)(scrNativeCallContext*) = nullptr;

void nativeInit(UINT64 hash) 
{
	g_context.Reset();
	g_hash = hash;
}

void nativePush64(UINT64 value) 
{
	g_context.Push(value);
}

static NativeHandler _Handler(uint64_t origHash)
{
	uint64_t newHash = crossmap::MapNative(origHash);
	if (newHash == 0)
	{
		return nullptr;
	}

	NativeRegistrationNew* table = hooking::m_registrationTable[newHash & 0xFF];

	for (; table; table = table->getNextRegistration())
	{
		for (uint32_t i = 0; i < table->getNumEntries(); i++)
		{
			if (newHash == table->getHash(i))
			{
				return table->handlers[i];
			}
		}
	}
	return nullptr;
}

static std::unordered_map<uint64_t, NativeHandler>	m_handlerCache;
NativeHandler GetNativeHandler(uint64_t origHash)
{
	auto& handler = m_handlerCache[origHash];
	if (handler == nullptr)
	{
		handler = _Handler(origHash);
	}
	return handler;
}

uint64_t * nativeCall()
{
	auto fn = GetNativeHandler(g_hash);
	if (fn != 0) 
	{
		static void* exceptionAddress;
		__try
		{
			fn(&g_context);
			scrNativeCallContext::SetVectorResults(&g_context);
		}
		__except (exceptionAddress = (GetExceptionInformation())->ExceptionRecord->ExceptionAddress, EXCEPTION_EXECUTE_HANDLER)
		{
			Cheat::log::GetLogger()->warn("Executing game native {0} (at address {1}) failed", g_hash, exceptionAddress);
		}
	}
	return reinterpret_cast<uint64_t*>(g_context.GetResultPointer());
}