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

// eax.h - originally written and placed in the public domain by Wei Dai

/// \file eax.h
/// \brief EAX block cipher mode of operation

#ifndef CRYPTOPP_EAX_H
#define CRYPTOPP_EAX_H

#include "authenc.h"
#include "modes.h"
#include "cmac.h"

NAMESPACE_BEGIN(CryptoPP)

/// \brief EAX block cipher base implementation
/// \details Base implementation of the AuthenticatedSymmetricCipher interface
/// \since Crypto++ 5.6.0
class CRYPTOPP_NO_VTABLE EAX_Base : public AuthenticatedSymmetricCipherBase
{
public:
	// AuthenticatedSymmetricCipher
	std::string AlgorithmName() const
		{return GetMAC().GetCipher().AlgorithmName() + std::string("/EAX");}
	std::string AlgorithmProvider() const
		{return GetMAC().GetCipher().AlgorithmProvider();}
	size_t MinKeyLength() const
		{return GetMAC().MinKeyLength();}
	size_t MaxKeyLength() const
		{return GetMAC().MaxKeyLength();}
	size_t DefaultKeyLength() const
		{return GetMAC().DefaultKeyLength();}
	size_t GetValidKeyLength(size_t n) const
		{return GetMAC().GetValidKeyLength(n);}
	bool IsValidKeyLength(size_t n) const
		{return GetMAC().IsValidKeyLength(n);}
	unsigned int OptimalDataAlignment() const
		{return GetMAC().OptimalDataAlignment();}
	IV_Requirement IVRequirement() const
		{return UNIQUE_IV;}
	unsigned int IVSize() const
		{return GetMAC().TagSize();}
	unsigned int MinIVLength() const
		{return 0;}
	unsigned int MaxIVLength() const
		{return UINT_MAX;}
	unsigned int DigestSize() const
		{return GetMAC().TagSize();}
	lword MaxHeaderLength() const
		{return LWORD_MAX;}
	lword MaxMessageLength() const
		{return LWORD_MAX;}

protected:
	// AuthenticatedSymmetricCipherBase
	bool AuthenticationIsOnPlaintext() const
		{return false;}
	unsigned int AuthenticationBlockSize() const
		{return 1;}
	void SetKeyWithoutResync(const byte *userKey, size_t keylength, const NameValuePairs &params);
	void Resync(const byte *iv, size_t len);
	size_t AuthenticateBlocks(const byte *data, size_t len);
	void AuthenticateLastHeaderBlock();
	void AuthenticateLastFooterBlock(byte *mac, size_t macSize);
	SymmetricCipher & AccessSymmetricCipher() {return m_ctr;}
	const CMAC_Base & GetMAC() const {return const_cast<EAX_Base *>(this)->AccessMAC();}
	virtual CMAC_Base & AccessMAC() =0;

	CTR_Mode_ExternalCipher::Encryption m_ctr;
};

/// \brief EAX block cipher final implementation
/// \tparam T_BlockCipher block cipher
/// \tparam T_IsEncryption direction in which to operate the cipher
/// \since Crypto++ 5.6.0
template <class T_BlockCipher, bool T_IsEncryption>
class EAX_Final : public EAX_Base
{
public:
	static std::string StaticAlgorithmName()
		{return T_BlockCipher::StaticAlgorithmName() + std::string("/EAX");}
	std::string AlgorithmProvider() const
		{return m_cmac.AlgorithmProvider();}
	bool IsForwardTransformation() const
		{return T_IsEncryption;}

private:
	CMAC_Base & AccessMAC() {return m_cmac;}
	CMAC<T_BlockCipher> m_cmac;
};

#ifdef EAX	// EAX is defined to 11 on GCC 3.4.3, OpenSolaris 8.11
#undef EAX
#endif

/// \brief EAX block cipher mode of operation
/// \tparam T_BlockCipher block cipher
/// \details \p EAX provides the \p Encryption and \p Decryption typedef. See EAX_Base
///   and EAX_Final for the AuthenticatedSymmetricCipher implementation.
/// \sa <a href="http://www.cryptopp.com/wiki/EAX_Mode">EAX Mode</a> and
///   <A HREF="http://www.cryptopp.com/wiki/Modes_of_Operation">Modes of Operation</A>
///   on the Crypto++ wiki.
/// \since Crypto++ 5.6.0
template <class T_BlockCipher>
struct EAX : public AuthenticatedSymmetricCipherDocumentation
{
	typedef EAX_Final<T_BlockCipher, true> Encryption;
	typedef EAX_Final<T_BlockCipher, false> Decryption;
};

NAMESPACE_END

#endif
