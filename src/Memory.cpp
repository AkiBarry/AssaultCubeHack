#include "Memory.hpp"
#include <detours.h>
#include <Windows.h>
#include "Console.hpp"
#include <vector>
#include "MinHook.h"

namespace
{
	std::vector<int16_t> HexToBytes(const std::string mask)
	{
		auto bytes = std::vector<int16_t>{};
		auto start = const_cast<char *>(mask.data());
		auto end = const_cast<char *>(mask.data()) + mask.size();

		for (auto cur = start; cur < end; ++cur)
			if (*cur == '?') {
				++cur;
				if (*cur == '?')
					++cur;
				bytes.push_back(-1);
			}
			else {
				bytes.push_back(static_cast<int16_t>(strtoul(cur, &cur, 16)));
			}

		return bytes;
	}
}

uint8_t * NMemory::NPatternScan::FindGeneric(const std::string& mask, const uint8_t * start, const uint8_t * end)
{
	auto bytes = HexToBytes(mask);

	NConsole::Print(start);

	for (const uint8_t* i = start; i < end - bytes.size(); ++i)
	{
		bool matched = true;
		const uint8_t* temp = i;
		for (auto& A : bytes)
		{
			if (A != *temp && A != -1)
			{
				matched = false;
				break;
			}
			++temp;
		}

		if (matched)
		{
			return const_cast<uint8_t *>(i);
		}
	}

	return NULL;
}

uint8_t * NMemory::NPatternScan::Find(const std::string& ModuleName, const std::string& Mask, size_t offset)
{
	auto module_handle = GetModuleHandleA(ModuleName.c_str());

	auto dosHeader = (PIMAGE_DOS_HEADER)module_handle;
	auto ntHeaders = (PIMAGE_NT_HEADERS)((uint8_t *)module_handle + dosHeader->e_lfanew);

	auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
	auto patternBytes = HexToBytes(Mask);
	auto scanBytes = reinterpret_cast<uint8_t *>(module_handle);

	auto s = patternBytes.size();
	auto d = patternBytes.data();

	for (auto i = 0ul; i < sizeOfImage - s; ++i) {
		bool found = true;
		for (auto j = 0ul; j < s; ++j) {
			if (scanBytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}
		if (found) {
			uint8_t ** temp = (uint8_t **)(&scanBytes[i] + offset);
			return *temp;
		}
	}
	return nullptr;
}

/*bool NMemory::Initiate()
{
	return MH_Initialize() == MH_OK;
}

void NMemory::Terminate()
{
	MH_Uninitialize();
}*/
