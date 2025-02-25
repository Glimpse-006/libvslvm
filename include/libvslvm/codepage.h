/*
 * Codepage definitions for libvslvm
 *
 * Copyright (C) 2014-2023, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _LIBVSLVM_CODEPAGE_H )
#define _LIBVSLVM_CODEPAGE_H

#include <libvslvm/types.h>

#if defined( __cplusplus )
extern "C" {
#endif

/* The codepage definitions
 */
enum LIBVSLVM_CODEPAGES
{
	LIBVSLVM_CODEPAGE_ASCII				= 20127,

	LIBVSLVM_CODEPAGE_ISO_8859_1			= 28591,
	LIBVSLVM_CODEPAGE_ISO_8859_2			= 28592,
	LIBVSLVM_CODEPAGE_ISO_8859_3			= 28593,
	LIBVSLVM_CODEPAGE_ISO_8859_4			= 28594,
	LIBVSLVM_CODEPAGE_ISO_8859_5			= 28595,
	LIBVSLVM_CODEPAGE_ISO_8859_6			= 28596,
	LIBVSLVM_CODEPAGE_ISO_8859_7			= 28597,
	LIBVSLVM_CODEPAGE_ISO_8859_8			= 28598,
	LIBVSLVM_CODEPAGE_ISO_8859_9			= 28599,
	LIBVSLVM_CODEPAGE_ISO_8859_10			= 28600,
	LIBVSLVM_CODEPAGE_ISO_8859_11			= 28601,
	LIBVSLVM_CODEPAGE_ISO_8859_13			= 28603,
	LIBVSLVM_CODEPAGE_ISO_8859_14			= 28604,
	LIBVSLVM_CODEPAGE_ISO_8859_15			= 28605,
	LIBVSLVM_CODEPAGE_ISO_8859_16			= 28606,

	LIBVSLVM_CODEPAGE_KOI8_R			= 20866,
	LIBVSLVM_CODEPAGE_KOI8_U			= 21866,

	LIBVSLVM_CODEPAGE_WINDOWS_874			= 874,
	LIBVSLVM_CODEPAGE_WINDOWS_932			= 932,
	LIBVSLVM_CODEPAGE_WINDOWS_936			= 936,
	LIBVSLVM_CODEPAGE_WINDOWS_949			= 949,
	LIBVSLVM_CODEPAGE_WINDOWS_950			= 950,
	LIBVSLVM_CODEPAGE_WINDOWS_1250			= 1250,
	LIBVSLVM_CODEPAGE_WINDOWS_1251			= 1251,
	LIBVSLVM_CODEPAGE_WINDOWS_1252			= 1252,
	LIBVSLVM_CODEPAGE_WINDOWS_1253			= 1253,
	LIBVSLVM_CODEPAGE_WINDOWS_1254			= 1254,
	LIBVSLVM_CODEPAGE_WINDOWS_1255			= 1255,
	LIBVSLVM_CODEPAGE_WINDOWS_1256			= 1256,
	LIBVSLVM_CODEPAGE_WINDOWS_1257			= 1257,
	LIBVSLVM_CODEPAGE_WINDOWS_1258			= 1258
};

#define LIBVSLVM_CODEPAGE_US_ASCII			LIBVSLVM_CODEPAGE_ASCII

#define LIBVSLVM_CODEPAGE_ISO_WESTERN_EUROPEAN		LIBVSLVM_CODEPAGE_ISO_8859_1
#define LIBVSLVM_CODEPAGE_ISO_CENTRAL_EUROPEAN		LIBVSLVM_CODEPAGE_ISO_8859_2
#define LIBVSLVM_CODEPAGE_ISO_SOUTH_EUROPEAN		LIBVSLVM_CODEPAGE_ISO_8859_3
#define LIBVSLVM_CODEPAGE_ISO_NORTH_EUROPEAN		LIBVSLVM_CODEPAGE_ISO_8859_4
#define LIBVSLVM_CODEPAGE_ISO_CYRILLIC			LIBVSLVM_CODEPAGE_ISO_8859_5
#define LIBVSLVM_CODEPAGE_ISO_ARABIC			LIBVSLVM_CODEPAGE_ISO_8859_6
#define LIBVSLVM_CODEPAGE_ISO_GREEK			LIBVSLVM_CODEPAGE_ISO_8859_7
#define LIBVSLVM_CODEPAGE_ISO_HEBREW			LIBVSLVM_CODEPAGE_ISO_8859_8
#define LIBVSLVM_CODEPAGE_ISO_TURKISH			LIBVSLVM_CODEPAGE_ISO_8859_9
#define LIBVSLVM_CODEPAGE_ISO_NORDIC			LIBVSLVM_CODEPAGE_ISO_8859_10
#define LIBVSLVM_CODEPAGE_ISO_THAI			LIBVSLVM_CODEPAGE_ISO_8859_11
#define LIBVSLVM_CODEPAGE_ISO_BALTIC			LIBVSLVM_CODEPAGE_ISO_8859_13
#define LIBVSLVM_CODEPAGE_ISO_CELTIC			LIBVSLVM_CODEPAGE_ISO_8859_14

#define LIBVSLVM_CODEPAGE_ISO_LATIN_1			LIBVSLVM_CODEPAGE_ISO_8859_1
#define LIBVSLVM_CODEPAGE_ISO_LATIN_2			LIBVSLVM_CODEPAGE_ISO_8859_2
#define LIBVSLVM_CODEPAGE_ISO_LATIN_3			LIBVSLVM_CODEPAGE_ISO_8859_3
#define LIBVSLVM_CODEPAGE_ISO_LATIN_4			LIBVSLVM_CODEPAGE_ISO_8859_4
#define LIBVSLVM_CODEPAGE_ISO_LATIN_5			LIBVSLVM_CODEPAGE_ISO_8859_9
#define LIBVSLVM_CODEPAGE_ISO_LATIN_6			LIBVSLVM_CODEPAGE_ISO_8859_10
#define LIBVSLVM_CODEPAGE_ISO_LATIN_7			LIBVSLVM_CODEPAGE_ISO_8859_13
#define LIBVSLVM_CODEPAGE_ISO_LATIN_8			LIBVSLVM_CODEPAGE_ISO_8859_14
#define LIBVSLVM_CODEPAGE_ISO_LATIN_9			LIBVSLVM_CODEPAGE_ISO_8859_15
#define LIBVSLVM_CODEPAGE_ISO_LATIN_10			LIBVSLVM_CODEPAGE_ISO_8859_16

#define LIBVSLVM_CODEPAGE_KOI8_RUSSIAN			LIBVSLVM_CODEPAGE_KOI8_R
#define LIBVSLVM_CODEPAGE_KOI8_UKRAINIAN		LIBVSLVM_CODEPAGE_KOI8_U

#define LIBVSLVM_CODEPAGE_WINDOWS_THAI			LIBVSLVM_CODEPAGE_WINDOWS_874
#define LIBVSLVM_CODEPAGE_WINDOWS_JAPANESE		LIBVSLVM_CODEPAGE_WINDOWS_932
#define LIBVSLVM_CODEPAGE_WINDOWS_CHINESE_SIMPLIFIED	LIBVSLVM_CODEPAGE_WINDOWS_936
#define LIBVSLVM_CODEPAGE_WINDOWS_KOREAN		LIBVSLVM_CODEPAGE_WINDOWS_949
#define LIBVSLVM_CODEPAGE_WINDOWS_CHINESE_TRADITIONAL	LIBVSLVM_CODEPAGE_WINDOWS_950
#define LIBVSLVM_CODEPAGE_WINDOWS_CENTRAL_EUROPEAN	LIBVSLVM_CODEPAGE_WINDOWS_1250
#define LIBVSLVM_CODEPAGE_WINDOWS_CYRILLIC		LIBVSLVM_CODEPAGE_WINDOWS_1251
#define LIBVSLVM_CODEPAGE_WINDOWS_WESTERN_EUROPEAN	LIBVSLVM_CODEPAGE_WINDOWS_1252
#define LIBVSLVM_CODEPAGE_WINDOWS_GREEK			LIBVSLVM_CODEPAGE_WINDOWS_1253
#define LIBVSLVM_CODEPAGE_WINDOWS_TURKISH		LIBVSLVM_CODEPAGE_WINDOWS_1254
#define LIBVSLVM_CODEPAGE_WINDOWS_HEBREW		LIBVSLVM_CODEPAGE_WINDOWS_1255
#define LIBVSLVM_CODEPAGE_WINDOWS_ARABIC		LIBVSLVM_CODEPAGE_WINDOWS_1256
#define LIBVSLVM_CODEPAGE_WINDOWS_BALTIC		LIBVSLVM_CODEPAGE_WINDOWS_1257
#define LIBVSLVM_CODEPAGE_WINDOWS_VIETNAMESE		LIBVSLVM_CODEPAGE_WINDOWS_1258

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBVSLVM_CODEPAGE_H ) */

