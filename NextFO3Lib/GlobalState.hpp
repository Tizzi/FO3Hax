#pragma once

#include "Util.hpp"
#include "Types.hpp"
#include "Hook32.hpp"

namespace GlobalState
{
	class Settings {
	public:
		bool IsInjected = false;
		bool IsConsoleActivate = false;
		Util::Console* Console = new Util::Console();

		~Settings()
		{
			if (Console != nullptr)
			{
				delete Console;
			}
		}
	};

	class KeyBoard {
	public:
		class KeyStatus {
		private:
			SHORT _flags = 0;
			SHORT _prev = 0;

		public:
			bool IsUp()
			{
				return !IsDown();
			}

			bool IsDown()
			{
				return (_flags & 0x8000) != 0;
			}

			bool IsPressedOnce()
			{
				return (_flags & 0x1) != (_prev & 0x1);
			}

			void operator= (SHORT flags)
			{
				_prev = _flags;
				_flags = flags;
			}
		};

		KeyStatus Shift;
		KeyStatus LeftControl;
		KeyStatus Space;
	};

	class GlobalObjects {
	public:
		HMODULE Dll = nullptr;

		Types::PFOClient Client = nullptr;

		std::string FormatBuffer;

		Hooks::Hook32* OneHexHook = new Hooks::Hook32(true);

		Hooks::Hook32* SetActionHook = new Hooks::Hook32(true);
		unsigned int SetActionArgs[7] = { 0 };

		~GlobalObjects()
		{
			delete OneHexHook;
			delete SetActionHook;
		}
	};
}