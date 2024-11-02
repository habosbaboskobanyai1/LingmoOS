/*
 * Compilation Copyright (c) 1995-2019 by Wei Dai.  All rights reserved.
 *	This copyright applies only to this software distribution package
 *	as a compilation, and does not imply a copyright on any particular
 *	file in the package.
 * 
 *	Boost Software License - Version 1.0 - August 17th, 2003
 *
 *	Permission is hereby granted, free of charge, to any person or organization
 *	obtaining a copy of the software and accompanying documentation covered by
 *	this license (the "Software") to use, reproduce, display, distribute,
 *	execute, and transmit the Software, and to prepare derivative works of the
 *	Software, and to permit third-parties to whom the Software is furnished to
 *	do so, all subject to the following:
 * 
 *	The copyright notices in the Software and this entire statement, including
 *	the above license grant, this restriction and the following disclaimer,
 *	must be included in all copies of the Software, in whole or in part, and
 *	all derivative works of the Software, unless such copies or derivative
 *	works are solely in the form of machine-executable object code generated by
 *	a source language processor.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 *	SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 *	FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 *	ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *	DEALINGS IN THE SOFTWARE.
 */

// seed.h - originally written and placed in the public domain by Wei Dai

/// \file seed.h
/// \brief Classes for the SEED block cipher
/// \since Crypto++ 5.6.0

#ifndef CRYPTOPP_SEED_H
#define CRYPTOPP_SEED_H

#include "seckey.h"
#include "secblock.h"

NAMESPACE_BEGIN(CryptoPP)

/// \brief SEED block cipher information
/// \since Crypto++ 5.6.0
struct SEED_Info : public FixedBlockSize<16>, public FixedKeyLength<16>, public FixedRounds<16>
{
	CRYPTOPP_STATIC_CONSTEXPR const char* StaticAlgorithmName() {return "SEED";}
};

/// \brief SEED block cipher
/// \sa <a href="http://www.cryptolounge.org/wiki/SEED">SEED</a>
/// \since Crypto++ 5.6.0
class SEED : public SEED_Info, public BlockCipherDocumentation
{
	class CRYPTOPP_NO_VTABLE Base : public BlockCipherImpl<SEED_Info>
	{
	public:
		void UncheckedSetKey(const byte *key, unsigned int length, const NameValuePairs &params);
		void ProcessAndXorBlock(const byte *inBlock, const byte *xorBlock, byte *outBlock) const;

	protected:
		FixedSizeSecBlock<word32, 32> m_k;
	};

public:
	typedef BlockCipherFinal<ENCRYPTION, Base> Encryption;
	typedef BlockCipherFinal<DECRYPTION, Base> Decryption;
};

NAMESPACE_END

#endif
