#pragma once
#include <windows.h>
#include <cstdint>
#include <string>
#include <Psapi.h>

namespace NMemory
{
	//bool Initiate();
	//void Terminate();

	namespace NPatternScan
	{
		uint8_t * FindGeneric(const std::string& mask, const uint8_t * start, const uint8_t * end);

		uint8_t * Find(const std::string& ModuleName, const std::string& Mask, size_t offset);
	}
}
