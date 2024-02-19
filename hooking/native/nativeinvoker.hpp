#pragma once
#include <windows.h>
#include <string>

#pragma warning( disable : 6001)
#pragma warning( disable : 26451)
#pragma warning( disable : 26495)

class scrNativeCallContext {
protected:

	void *		m_pReturn;
	uint32_t	m_nArgCount;
	void *		m_pArgs;
	uint32_t	m_nDataCount;
	alignas(uintptr_t)uint8_t m_vectorSpace[192];

public:

	template<typename T>
	inline T GetArgument( int idx ) {

		intptr_t * arguments = (intptr_t*)m_pArgs;
		return *(T*)&arguments[idx];
	}

	template<typename T>
	inline void SetResult( int idx, T value ) {

		intptr_t * returnValues = (intptr_t*)m_pReturn;
		*(T*)&returnValues[idx] = value;
	}

	inline int GetArgumentCount() {

		return m_nArgCount;
	}

	template<typename T>
	inline T GetResult( int idx ) {

		intptr_t * returnValues = (intptr_t*)m_pReturn;
		return *(T*)&returnValues[idx];
	}

	static void(*SetVectorResults)(scrNativeCallContext*);
};

class NativeContext : public scrNativeCallContext {
private:

	// Configuration
	enum {
		MaxNativeParams = 16,
		ArgSize = 8,
	};

	// Anything temporary that we need
	uint8_t m_TempStack[MaxNativeParams * ArgSize];

public:

	inline NativeContext() {

		m_pArgs = &m_TempStack;
		m_pReturn = &m_TempStack;		// It's okay to point both args and return at
		// the same pointer. The game should handle this.
		m_nArgCount = 0;
		m_nDataCount = 0;
	}

	template <typename T>
	inline void Push( T value ) {

		if ( sizeof( T ) > ArgSize ) {
			throw "Argument has an invalid size";
		} else if ( sizeof( T ) < ArgSize ) {
			// Ensure we don't have any stray data
			*reinterpret_cast<uintptr_t*>( m_TempStack + ArgSize * m_nArgCount ) = 0;
		}

		*reinterpret_cast<T*>( m_TempStack + ArgSize * m_nArgCount ) = value;
		m_nArgCount++;
	}

	inline void Reverse() {

		uintptr_t tempValues[MaxNativeParams];
		uintptr_t * args = (uintptr_t*)m_pArgs;

		for ( uint32_t i = 0; i < m_nArgCount; i++ ) {

			int target = m_nArgCount - i - 1;
			tempValues[target] = args[i];
		}

		memcpy(m_TempStack, tempValues, sizeof(m_TempStack));
	}

	template <typename T>
	inline T GetResult() {

		return *reinterpret_cast<T*>( m_TempStack );
	}
};

struct pass {
	template<typename ...T> pass( T... ) {}
};

class NativeManagerContext : public NativeContext {
public:

	NativeManagerContext()
		: NativeContext() {
	}

	void Reset() {

		m_nArgCount = 0;
		m_nDataCount = 0;
	}

	inline void* GetResultPointer() {

		return m_pReturn;
	}
};

void nativeInit(UINT64 hash);
void nativePush64(UINT64 val);
PUINT64 nativeCall();


#pragma warning (push)
#pragma warning (disable: 4244)
typedef void(__cdecl* NativeHandler)(scrNativeCallContext* context);
struct NativeRegistrationNew
{
	uint64_t nextRegistration1;
	uint64_t nextRegistration2;
	NativeHandler handlers[7];
	uint32_t numEntries1;
	uint32_t numEntries2;
	uint64_t hashes;

	inline NativeRegistrationNew* getNextRegistration()
	{
		uintptr_t result;
		auto v5 = reinterpret_cast<uintptr_t>(&nextRegistration1);
		auto v12 = 2i64;
		auto v13 = v5 ^ nextRegistration2;
		auto v14 = (char*)&result - v5;
		do
		{
			*(DWORD*)&v14[v5] = v13 ^ *(DWORD*)v5;
			v5 += 4i64;
			--v12;
		} while (v12);

		return reinterpret_cast<NativeRegistrationNew*>(result);
	}

	inline uint32_t getNumEntries()
	{
		return ((uintptr_t)&numEntries1) ^ numEntries1 ^ numEntries2;
	}

	inline uint64_t getHash(uint32_t index)
	{
		auto naddr = 16 * index + reinterpret_cast<uintptr_t>(&nextRegistration1) + 0x54;
		auto v8 = 2i64;
		uint64_t nResult;
		auto v11 = (char*)&nResult - naddr;
		auto v10 = naddr ^ *(DWORD*)(naddr + 8);
		do
		{
			*(DWORD*)&v11[naddr] = v10 ^ *(DWORD*)(naddr);
			naddr += 4i64;
			--v8;
		} while (v8);

		return nResult;
	}
};
#pragma warning (pop)