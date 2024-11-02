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

// blowfish.h - originally written and placed in the public domain by Wei Dai

/// \file blowfish.h
/// \brief Classes for the Blowfish block cipher

#ifndef CRYPTOPP_BLOWFISH_H
#define CRYPTOPP_BLOWFISH_H

#include "seckey.h"
#include "secblock.h"

NAMESPACE_BEGIN(CryptoPP)

/// \brief Blowfish block cipher information
struct Blowfish_Info : public FixedBlockSize<8>, public VariableKeyLength<16, 4, 56>, public FixedRounds<16>
{
	CRYPTOPP_STATIC_CONSTEXPR const char* StaticAlgorithmName() {return "Blowfish";}
};

// <a href="http://www.cryptopp.com/wiki/Blowfish">Blowfish</a>

/// \brief Blowfish block cipher
/// \since Crypto++ 1.0
class Blowfish : public Blowfish_Info, public BlockCipherDocumentation
{
	/// \brief Class specific implementation and overrides used to operate the cipher.
	/// \details Implementations and overrides in \p Base apply to both \p ENCRYPTION and \p DECRYPTION directions
	class CRYPTOPP_NO_VTABLE Base : public BlockCipherImpl<Blowfish_Info>
	{
	public:
		void ProcessAndXorBlock(const byte *inBlock, const byte *xorBlock, byte *outBlock) const;
		void UncheckedSetKey(const byte *key_string, unsigned int keylength, const NameValuePairs &params);

	private:
		void crypt_block(const word32 in[2], word32 out[2]) const;

		static const word32 p_init[ROUNDS+2];
		static const word32 s_init[4*256];

		FixedSizeSecBlock<word32, ROUNDS+2> pbox;
		FixedSizeSecBlock<word32, 4*256> sbox;
	};

public:
	typedef BlockCipherFinal<ENCRYPTION, Base> Encryption;
	typedef BlockCipherFinal<DECRYPTION, Base> Decryption;
};

typedef Blowfish::Encryption BlowfishEncryption;
typedef Blowfish::Decryption BlowfishDecryption;

NAMESPACE_END

#endif
