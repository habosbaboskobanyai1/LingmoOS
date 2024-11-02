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

// channels.h - originally written and placed in the public domain by Wei Dai

/// \file channels.h
/// \brief Classes for multiple named channels

#ifndef CRYPTOPP_CHANNELS_H
#define CRYPTOPP_CHANNELS_H

#include "cryptlib.h"
#include "simple.h"
#include "smartptr.h"
#include "stdcpp.h"

#if CRYPTOPP_MSC_VERSION
# pragma warning(push)
# pragma warning(disable: 4355)
#endif

NAMESPACE_BEGIN(CryptoPP)

#if 0
/// Route input on default channel to different and/or multiple channels based on message sequence number
class MessageSwitch : public Sink
{
public:
	void AddDefaultRoute(BufferedTransformation &destination, const std::string &channel);
	void AddRoute(unsigned int begin, unsigned int end, BufferedTransformation &destination, const std::string &channel);

	void Put(byte inByte);
	void Put(const byte *inString, unsigned int length);

	void Flush(bool completeFlush, int propagation=-1);
	void MessageEnd(int propagation=-1);
	void PutMessageEnd(const byte *inString, unsigned int length, int propagation=-1);
	void MessageSeriesEnd(int propagation=-1);

private:
	typedef std::pair<BufferedTransformation *, std::string> Route;
	struct RangeRoute
	{
		RangeRoute(unsigned int begin, unsigned int end, const Route &route)
			: begin(begin), end(end), route(route) {}
		bool operator<(const RangeRoute &rhs) const {return begin < rhs.begin;}
		unsigned int begin, end;
		Route route;
	};

	typedef std::list<RangeRoute> RouteList;
	typedef std::list<Route> DefaultRouteList;

	RouteList m_routes;
	DefaultRouteList m_defaultRoutes;
	unsigned int m_nCurrentMessage;
};
#endif

class ChannelSwitchTypedefs
{
public:
	typedef std::pair<BufferedTransformation *, std::string> Route;
	typedef std::multimap<std::string, Route> RouteMap;

	typedef std::pair<BufferedTransformation *, value_ptr<std::string> > DefaultRoute;
	typedef std::list<DefaultRoute> DefaultRouteList;

	// SunCC workaround: can't use const_iterator here
	typedef RouteMap::iterator MapIterator;
	typedef DefaultRouteList::iterator ListIterator;
};

class ChannelSwitch;

class ChannelRouteIterator : public ChannelSwitchTypedefs
{
public:
	ChannelRouteIterator(ChannelSwitch &cs) : m_cs(cs), m_useDefault(false) {}

	void Reset(const std::string &channel);
	bool End() const;
	void Next();
	BufferedTransformation & Destination();
	const std::string & Channel();

	ChannelSwitch& m_cs;
	std::string m_channel;
	bool m_useDefault;
	MapIterator m_itMapCurrent, m_itMapEnd;
	ListIterator m_itListCurrent, m_itListEnd;

protected:
	// Hide this to see if we break something...
	ChannelRouteIterator();
};

/// Route input to different and/or multiple channels based on channel ID
class CRYPTOPP_DLL ChannelSwitch : public Multichannel<Sink>, public ChannelSwitchTypedefs
{
public:
	ChannelSwitch() : m_it(*this), m_blocked(false) {}
	ChannelSwitch(BufferedTransformation &destination) : m_it(*this), m_blocked(false)
	{
		AddDefaultRoute(destination);
	}
	ChannelSwitch(BufferedTransformation &destination, const std::string &outChannel) : m_it(*this), m_blocked(false)
	{
		AddDefaultRoute(destination, outChannel);
	}

	void IsolatedInitialize(const NameValuePairs &parameters=g_nullNameValuePairs);

	size_t ChannelPut2(const std::string &channel, const byte *begin, size_t length, int messageEnd, bool blocking);
	size_t ChannelPutModifiable2(const std::string &channel, byte *begin, size_t length, int messageEnd, bool blocking);

	bool ChannelFlush(const std::string &channel, bool completeFlush, int propagation=-1, bool blocking=true);
	bool ChannelMessageSeriesEnd(const std::string &channel, int propagation=-1, bool blocking=true);

	byte * ChannelCreatePutSpace(const std::string &channel, size_t &size);

	void AddDefaultRoute(BufferedTransformation &destination);
	void RemoveDefaultRoute(BufferedTransformation &destination);
	void AddDefaultRoute(BufferedTransformation &destination, const std::string &outChannel);
	void RemoveDefaultRoute(BufferedTransformation &destination, const std::string &outChannel);
	void AddRoute(const std::string &inChannel, BufferedTransformation &destination, const std::string &outChannel);
	void RemoveRoute(const std::string &inChannel, BufferedTransformation &destination, const std::string &outChannel);

private:
	RouteMap m_routeMap;
	DefaultRouteList m_defaultRoutes;

	ChannelRouteIterator m_it;
	bool m_blocked;

	friend class ChannelRouteIterator;
};

NAMESPACE_END

#if CRYPTOPP_MSC_VERSION
# pragma warning(pop)
#endif

#endif
