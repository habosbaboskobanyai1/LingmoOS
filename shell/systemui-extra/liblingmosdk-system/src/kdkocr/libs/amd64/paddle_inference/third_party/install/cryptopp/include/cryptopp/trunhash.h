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

#ifndef CRYPTOPP_TRUNHASH_H
#define CRYPTOPP_TRUNHASH_H

#include "cryptlib.h"

NAMESPACE_BEGIN(CryptoPP)

class NullHash : public HashTransformation
{
public:
	void Update(const byte *input, size_t length)
		{CRYPTOPP_UNUSED(input);CRYPTOPP_UNUSED(length);}
	unsigned int DigestSize() const
		{return 0;}
	void TruncatedFinal(byte *digest, size_t digestSize)
		{CRYPTOPP_UNUSED(digest);CRYPTOPP_UNUSED(digestSize);}
	bool TruncatedVerify(const byte *digest, size_t digestLength)
		{CRYPTOPP_UNUSED(digest);CRYPTOPP_UNUSED(digestLength);return true;}
};

/// construct new HashModule with smaller DigestSize() from existing one
template <class T>
class TruncatedHashTemplate : public HashTransformation
{
public:
	TruncatedHashTemplate(T hm, unsigned int digestSize)
		: m_hm(hm), m_digestSize(digestSize) {}
	TruncatedHashTemplate(const byte *key, size_t keyLength, unsigned int digestSize)
		: m_hm(key, keyLength), m_digestSize(digestSize) {}
	TruncatedHashTemplate(size_t digestSize)
		: m_digestSize(digestSize) {}

	void Restart()
		{m_hm.Restart();}
	void Update(const byte *input, size_t length)
		{m_hm.Update(input, length);}
	unsigned int DigestSize() const {return m_digestSize;}
	void TruncatedFinal(byte *digest, size_t digestSize)
		{m_hm.TruncatedFinal(digest, digestSize);}
	bool TruncatedVerify(const byte *digest, size_t digestLength)
		{return m_hm.TruncatedVerify(digest, digestLength);}

private:
	T m_hm;
	unsigned int m_digestSize;
};

typedef TruncatedHashTemplate<HashTransformation &> TruncatedHashModule;

NAMESPACE_END

#endif
