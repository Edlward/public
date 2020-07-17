#ifndef INT512_HPP
#define INT512_HPP

/* INT512 - int512_t, uint512_t
// Int512.hpp
//
// Author: Jan Ringos, http://Tringi.TrimCore.cz, tringi@trimcore.cz
// Version: 1.0
// Description: Implements 512-bit Integer using double_integer template
//
// License:
//   This software is provided 'as-is', without any express or implied warranty.
//   In no event will the author be held liable for any damages arising from the
//   use of this software. 
// 
//   Permission is granted to anyone to use this software for any purpose,
//   including commercial applications, and to alter it and redistribute it
//   freely, subject to the following restrictions: 
//
//       1. The origin of this software must not be misrepresented; you must not
//          claim that you wrote the original software. If you use this software
//          in a product, an acknowledgment in the product documentation would
//          be appreciated but is not required.
//
//       2. Altered source versions must be plainly marked as such, and must not
//          be misrepresented as being the original software.
//
//       3. This notice may not be removed or altered from any source
//          distribution.
*/

#include "double_integer.hpp"
#include "int256.hpp"

typedef double_integer <uint256_t,  int256_t>  int512_t;
typedef double_integer <uint256_t, uint256_t> uint512_t;

#endif
