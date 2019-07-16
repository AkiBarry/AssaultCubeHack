#include "Cursor.hpp"
#include <Windows.h>

namespace
{
	HHOOK _hook;
	KBDLLHOOKSTRUCT kbdStruct;

	LRESULT __stdcall HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode >= 0)
		{
			// the action is valid: HC_ACTION.
			if (wParam == WM_KEYDOWN)
			{
				// lParam is the pointer to the struct containing the data needed, so cast and assign it to kdbStruct.
				kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
				// a key (non-system) is pressed.
				if (kbdStruct.vkCode == VK_F1)
				{
					// F1 is pressed!
					MessageBox(NULL, "F1 is pressed!", "key pressed", MB_ICONINFORMATION);
				}
			}
		}

		return CallNextHookEx(_hook, nCode, wParam, lParam);
	};
}

namespace NCursor
{
	bool Initiate()
	{
		//SetWindowsHookEx(WH_KEYBOARD, 0);
		return false;
	}

	void LockScreen()
	{
		
	}
}
