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

#ifndef CRYPTOPP_XTRCRYPT_H
#define CRYPTOPP_XTRCRYPT_H

/// \file
/// \brief XTR public key system
/// \sa  "The XTR public key system" by Arjen K. Lenstra and Eric R. Verheul

#include "cryptlib.h"
#include "xtr.h"
#include "integer.h"

NAMESPACE_BEGIN(CryptoPP)

/// \brief XTR-DH with key validation
class XTR_DH : public SimpleKeyAgreementDomain, public CryptoParameters
{
	typedef XTR_DH ThisClass;

public:
	XTR_DH(const Integer &p, const Integer &q, const GFP2Element &g);
	XTR_DH(RandomNumberGenerator &rng, unsigned int pbits, unsigned int qbits);
	XTR_DH(BufferedTransformation &domainParams);

	void DEREncode(BufferedTransformation &domainParams) const;

	bool Validate(RandomNumberGenerator &rng, unsigned int level) const;
	bool GetVoidValue(const char *name, const std::type_info &valueType, void *pValue) const;
	void AssignFrom(const NameValuePairs &source);
	CryptoParameters & AccessCryptoParameters() {return *this;}
	unsigned int AgreedValueLength() const {return 2*m_p.ByteCount();}
	unsigned int PrivateKeyLength() const {return m_q.ByteCount();}
	unsigned int PublicKeyLength() const {return 2*m_p.ByteCount();}

	void GeneratePrivateKey(RandomNumberGenerator &rng, byte *privateKey) const;
	void GeneratePublicKey(RandomNumberGenerator &rng, const byte *privateKey, byte *publicKey) const;
	bool Agree(byte *agreedValue, const byte *privateKey, const byte *otherPublicKey, bool validateOtherPublicKey=true) const;

	const Integer &GetModulus() const {return m_p;}
	const Integer &GetSubgroupOrder() const {return m_q;}
	const GFP2Element &GetSubgroupGenerator() const {return m_g;}

	void SetModulus(const Integer &p) {m_p = p;}
	void SetSubgroupOrder(const Integer &q) {m_q = q;}
	void SetSubgroupGenerator(const GFP2Element &g) {m_g = g;}

private:
	unsigned int ExponentBitLength() const;

	Integer m_p, m_q;
	GFP2Element m_g;
};

NAMESPACE_END

#endif
