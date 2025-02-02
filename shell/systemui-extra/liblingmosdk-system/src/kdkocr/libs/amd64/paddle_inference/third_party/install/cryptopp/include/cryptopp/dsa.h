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

// dsa.h - originally written and placed in the public domain by Wei Dai

/// \file dsa.h
/// \brief Classes for the DSA signature algorithm

#ifndef CRYPTOPP_DSA_H
#define CRYPTOPP_DSA_H

#include "cryptlib.h"
#include "gfpcrypt.h"

NAMESPACE_BEGIN(CryptoPP)

/// \brief DSA Signature Format
/// \details The DSA signature format used by Crypto++ is as defined by IEEE P1363.
///   Java and .Net use the DER format, and OpenPGP uses the OpenPGP format.
enum DSASignatureFormat {
	/// \brief Crypto++ native signature encoding format
	DSA_P1363,
	/// \brief signature encoding format used by Java and .Net
	DSA_DER,
	/// \brief OpenPGP signature encoding format
	DSA_OPENPGP
};

/// \brief Converts between signature encoding formats
/// \param buffer byte buffer for the converted signature encoding
/// \param bufferSize the length of the converted signature encoding buffer
/// \param toFormat the source signature format
/// \param signature byte buffer for the existing signature encoding
/// \param signatureLen the length of the existing signature encoding buffer
/// \param fromFormat the source signature format
/// \details This function converts between these formats, and returns length
///   of signature in the target format. If <tt>toFormat == DSA_P1363</tt>, then
///   <tt>bufferSize</tt> must equal <tt>publicKey.SignatureLength()</tt>
/// \since Crypto++ 1.0
size_t DSAConvertSignatureFormat(byte *buffer, size_t bufferSize, DSASignatureFormat toFormat,
	const byte *signature, size_t signatureLen, DSASignatureFormat fromFormat);

NAMESPACE_END

#endif
