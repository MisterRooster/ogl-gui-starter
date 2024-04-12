/*------------------------------------------------------------------------------------------------*\
| ogl-gui-starter
|
| Copyright (c) 2022 MisterRooster (github.com/MisterRooster). All rights reserved.
| Licensed under the MIT license. See LICENSE file for full terms.
| This notice is not to be removed.
\*------------------------------------------------------------------------------------------------*/
#pragma once

#include "Types.h"


namespace nhahn
{
	class Utils
	{
	public:
		static float frand();
		static int irand();

		static float smoothStep(float edge0, float edge1, float x);
		static int stringHash(const char* str);

		/** Write a n*8 bits integer value to memory in little endian. */
		static void intWrite(void* dest, const int n, const unsigned int value);

		/** Read a n*8 bits integer value from memory in little endian. */
		static unsigned int intRead(const void* src, int n);

		/** Convert a hex string to 4 byte integer. */
		static INT32 hexToInt(std::string shex);

		/** Convert a hex string to 3 byte integer. */
		static INT32 hexToInt24(std::string shex);

		/** Convert a hex string to Ascii code. */
		static std::string hexToAscii(std::string sHex);

		/** Split string at the delimiter positions. */
		static std::vector<std::string> split(std::string str, std::string delimiter);
	};
}