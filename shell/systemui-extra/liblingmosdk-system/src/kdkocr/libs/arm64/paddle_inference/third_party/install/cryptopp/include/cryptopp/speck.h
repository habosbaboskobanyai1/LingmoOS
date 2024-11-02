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

// speck.h - written and placed in the public domain by Jeffrey Walton

/// \file speck.h
/// \brief Classes for the Speck block cipher
/// \details Speck is a block cipher designed by Ray Beaulieu, Douglas Shors, Jason Smith,
///   Stefan Treatman-Clark, Bryan Weeks and Louis Wingers.
/// \sa <A HREF="http://eprint.iacr.org/2013/404">The SIMON and SPECK Families of
///   Lightweight Block Ciphers</A>, <A HREF="http://iadgov.github.io/simon-speck/">
///   The Simon and Speck GitHub</A> and <A HREF="https://www.cryptopp.com/wiki/SPECK">
///   SPECK</A> on the Crypto++ wiki.
/// \since Crypto++ 6.0

#ifndef CRYPTOPP_SPECK_H
#define CRYPTOPP_SPECK_H

#include "config.h"
#include "seckey.h"
#include "secblock.h"

#if CRYPTOPP_BOOL_X64 || CRYPTOPP_BOOL_X32 || CRYPTOPP_BOOL_X86 || \
    CRYPTOPP_BOOL_ARM32 || CRYPTOPP_BOOL_ARMV8 || \
    CRYPTOPP_BOOL_PPC32 || CRYPTOPP_BOOL_PPC64
# ifndef CRYPTOPP_DISABLE_SPECK_SIMD
#  define CRYPTOPP_SPECK64_ADVANCED_PROCESS_BLOCKS 1
# endif
#endif

#if CRYPTOPP_BOOL_X64 || CRYPTOPP_BOOL_X32 || CRYPTOPP_BOOL_X86 || \
    CRYPTOPP_BOOL_ARM32 || CRYPTOPP_BOOL_ARMV8 || \
    CRYPTOPP_BOOL_PPC32 || CRYPTOPP_BOOL_PPC64
# ifndef CRYPTOPP_DISABLE_SPECK_SIMD
#  define CRYPTOPP_SPECK128_ADVANCED_PROCESS_BLOCKS 1
# endif
#endif

// Yet another SunStudio/SunCC workaround. Failed self tests
// in SSE code paths on i386 for SunStudio 12.3 and below.
#if defined(__SUNPRO_CC) && (__SUNPRO_CC <= 0x5120)
# undef CRYPTOPP_SPECK64_ADVANCED_PROCESS_BLOCKS
# undef CRYPTOPP_SPECK128_ADVANCED_PROCESS_BLOCKS
#endif

NAMESPACE_BEGIN(CryptoPP)

/// \brief SPECK block cipher information
/// \tparam L block size of the cipher, in bytes
/// \tparam D default key length, in bytes
/// \tparam N minimum key length, in bytes
/// \tparam M maximum key length, in bytes
/// \since Crypto++ 6.0
template <unsigned int L, unsigned int D, unsigned int N, unsigned int M>
struct SPECK_Info : public FixedBlockSize<L>, VariableKeyLength<D, N, M>
{
    /// \brief The algorithm name
    /// \returns the algorithm name
    /// \details StaticAlgorithmName returns the algorithm's name as a static
    ///   member function.
    static const std::string StaticAlgorithmName()
    {
        // Format is Cipher-Blocksize(Keylength)
        return "SPECK-" + IntToString(L*8);
    }
};

/// \brief SPECK block cipher base class
/// \tparam W the word type
/// \details User code should use SPECK64 or SPECK128
/// \sa SPECK64, SPECK128, <a href="http://www.cryptopp.com/wiki/SPECK">SPECK</a>
/// \since Crypto++ 6.0
template <class W>
struct SPECK_Base
{
    virtual ~SPECK_Base() {}
    SPECK_Base() : m_kwords(0), m_rounds(0) {}

    typedef SecBlock<W, AllocatorWithCleanup<W, true> > AlignedSecBlock;
    mutable AlignedSecBlock m_wspace;  // workspace
    AlignedSecBlock         m_rkeys;   // round keys
    unsigned int            m_kwords;  // number of key words
    unsigned int            m_rounds;  // number of rounds
};

/// \brief SPECK 64-bit block cipher
/// \details Speck is a block cipher designed by Ray Beaulieu, Douglas Shors, Jason Smith,
///   Stefan Treatman-Clark, Bryan Weeks and Louis Wingers.
/// \details SPECK64 provides 64-bit block size. The valid key sizes are 96-bit and 128-bit.
/// \sa SPECK64, SPECK128,  <A HREF="http://eprint.iacr.org/2013/404">The SIMON and SPECK
///   Families of Lightweight Block Ciphers</A>, <A HREF="http://iadgov.github.io/simon-speck/">
///   The Simon and Speck GitHub</A>, <a href="http://www.cryptopp.com/wiki/SPECK">SPECK</a> on the
///   Crypto++ wiki
/// \since Crypto++ 6.0
class CRYPTOPP_NO_VTABLE SPECK64 : public SPECK_Info<8, 12, 12, 16>, public BlockCipherDocumentation
{
public:
    /// \brief SPECK block cipher transformation functions
    /// \details Provides implementation common to encryption and decryption
    /// \since Crypto++ 6.0
    class CRYPTOPP_NO_VTABLE Base : protected SPECK_Base<word32>, public BlockCipherImpl<SPECK_Info<8, 12, 12, 16> >
    {
    public:
        /// \brief The algorithm name
        /// \returns the algorithm name
        /// \details AlgorithmName returns the algorithm's name as a
        ///   member function.
        std::string AlgorithmName() const {
            return StaticAlgorithmName() + (m_kwords == 0 ? "" :
                "(" + IntToString(m_kwords*sizeof(word32)*8) + ")");
        }

        std::string AlgorithmProvider() const;

    protected:
        void UncheckedSetKey(const byte *userKey, unsigned int keyLength, const NameValuePairs &params);
    };

    /// \brief Encryption transformation
    /// \details Enc provides implementation for encryption transformation. All key
    ///   sizes are supported.
    /// \since Crypto++ 6.0
    class CRYPTOPP_NO_VTABLE Enc : public Base
    {
    public:
        void ProcessAndXorBlock(const byte *inBlock, const byte *xorBlock, byte *outBlock) const;
#if CRYPTOPP_SPECK64_ADVANCED_PROCESS_BLOCKS
        size_t AdvancedProcessBlocks(const byte *inBlocks, const byte *xorBlocks, byte *outBlocks, size_t length, word32 flags) const;
#endif
    };

    /// \brief Encryption transformation
    /// \details Dec provides implementation for decryption transformation. All key
    ///   sizes are supported.
    /// \since Crypto++ 6.0
    class CRYPTOPP_NO_VTABLE Dec : public Base
    {
    public:
        void ProcessAndXorBlock(const byte *inBlock, const byte *xorBlock, byte *outBlock) const;
#if CRYPTOPP_SPECK64_ADVANCED_PROCESS_BLOCKS
        size_t AdvancedProcessBlocks(const byte *inBlocks, const byte *xorBlocks, byte *outBlocks, size_t length, word32 flags) const;
#endif
    };

    typedef BlockCipherFinal<ENCRYPTION, Enc> Encryption;
    typedef BlockCipherFinal<DECRYPTION, Dec> Decryption;
};

/// \brief SPECK 128-bit block cipher
/// \details Speck is a block cipher designed by Ray Beaulieu, Douglas Shors, Jason Smith,
///   Stefan Treatman-Clark, Bryan Weeks and Louis Wingers.
/// \details SPECK128 provides 128-bit block size. The valid key sizes are 128-bit, 192-bit and 256-bit.
/// \sa SPECK64, SPECK128,  <A HREF="http://eprint.iacr.org/2013/404">The SIMON and SPECK
///   Families of Lightweight Block Ciphers</A>, <A HREF="http://iadgov.github.io/simon-speck/">
///   The Simon and Speck GitHub</A>, <a href="http://www.cryptopp.com/wiki/SPECK">SPECK</a> on the
///   Crypto++ wiki
/// \since Crypto++ 6.0
class CRYPTOPP_NO_VTABLE SPECK128 : public SPECK_Info<16, 16, 16, 32>, public BlockCipherDocumentation
{
public:
    /// \brief SPECK block cipher transformation functions
    /// \details Provides implementation common to encryption and decryption
    /// \since Crypto++ 6.0
    class CRYPTOPP_NO_VTABLE Base : protected SPECK_Base<word64>, public BlockCipherImpl<SPECK_Info<16, 16, 16, 32> >
    {
    public:
        /// \brief The algorithm name
        /// \returns the algorithm name
        /// \details AlgorithmName returns the algorithm's name as a
        ///   member function.
        std::string AlgorithmName() const {
            return StaticAlgorithmName() + (m_kwords == 0 ? "" :
                "(" + IntToString(m_kwords*sizeof(word64)*8) + ")");
        }

        std::string AlgorithmProvider() const;

    protected:
        void UncheckedSetKey(const byte *userKey, unsigned int keyLength, const NameValuePairs &params);
    };

    /// \brief Encryption transformation
    /// \details Enc provides implementation for encryption transformation. All key
    ///   sizes are supported.
    /// \since Crypto++ 6.0
    class CRYPTOPP_NO_VTABLE Enc : public Base
    {
    public:
        void ProcessAndXorBlock(const byte *inBlock, const byte *xorBlock, byte *outBlock) const;
#if CRYPTOPP_SPECK128_ADVANCED_PROCESS_BLOCKS
        size_t AdvancedProcessBlocks(const byte *inBlocks, const byte *xorBlocks, byte *outBlocks, size_t length, word32 flags) const;
#endif
    };

    /// \brief Encryption transformation
    /// \details Dec provides implementation for decryption transformation. All key
    ///   sizes are supported.
    /// \since Crypto++ 6.0
    class CRYPTOPP_NO_VTABLE Dec : public Base
    {
    public:
        void ProcessAndXorBlock(const byte *inBlock, const byte *xorBlock, byte *outBlock) const;
#if CRYPTOPP_SPECK128_ADVANCED_PROCESS_BLOCKS
        size_t AdvancedProcessBlocks(const byte *inBlocks, const byte *xorBlocks, byte *outBlocks, size_t length, word32 flags) const;
#endif
    };

    typedef BlockCipherFinal<ENCRYPTION, Enc> Encryption;
    typedef BlockCipherFinal<DECRYPTION, Dec> Decryption;
};

NAMESPACE_END

#endif  // CRYPTOPP_SPECK_H
