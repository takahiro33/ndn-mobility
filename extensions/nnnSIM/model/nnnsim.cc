/* -*- Mode:C++; c-file-style:"gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnnsim.cc is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnnsim.cc is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnnsim.cc.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "nnnsim.h"

using namespace std;

#include <ns3-dev/ns3/header.h>
#include <ns3-dev/ns3/packet.h>
#include <ns3-dev/ns3/log.h>

#include "wire-nnnsim.h"

NS_LOG_COMPONENT_DEFINE ("nnn.wire.nnnSIM");

NNN_NAMESPACE_BEGIN

namespace wire {
namespace nnnSIM {

NS_OBJECT_ENSURE_REGISTERED (NULLp);
NS_OBJECT_ENSURE_REGISTERED (SO);
NS_OBJECT_ENSURE_REGISTERED (DO);

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

NULLp::NULLp ()
: m_nullp (Create<nnn::NULLp> ())
{
}

NULLp::NULLp (Ptr<nnn::NULLp> nullp)
: m_nullp (nullp)
{
}

Ptr<nnn::NULLp>
NULLp::GetNULLp ()
{
	return m_nullp;
}

TypeId
NULLp::GetTypeId (void)
{
	static TypeId tid = TypeId ("ns3::nnn::NULLp::nnnSIM")
    		.SetGroupName ("Nnn")
    		.SetParent<Header> ()
    		.AddConstructor<NULLp> ()
    		;
	return tid;
}

TypeId
NULLp::GetInstanceTypeId (void) const
{
	return GetTypeId ();
}

Ptr<Packet>
NULLp::ToWire (Ptr<const nnn::NULLp> interest)
{
//	Ptr<const Packet> p = interest->GetWire ();
//	if (!p)
//	{
//		Ptr<Packet> packet = Create<Packet> (*interest->GetPayload ());
//		Interest wireEncoding (ConstCast<ndn::Interest> (interest));
//		packet->AddHeader (wireEncoding);
//		interest->SetWire (packet);
//
//		p = packet;
//	}
//
//	return p->Copy ();
}

Ptr<nnn::NULLp>
NULLp::FromWire (Ptr<Packet> packet)
{
//	Ptr<ndn::Interest> interest = Create<ndn::Interest> ();
//	Ptr<Packet> wire = packet->Copy ();
//
//	Interest wireEncoding (interest);
//	packet->RemoveHeader (wireEncoding);
//
//	interest->SetPayload (packet);
//	interest->SetWire (wire);
//
//	return interest;
}

uint32_t
NULLp::GetSerializedSize (void) const
{
//	size_t size =
//			1/*version*/ + 1 /*type*/ + 2/*length*/ +
//			(4/*nonce*/ + 1/*scope*/ + 1/*nack type*/ + 2/*timestamp*/ +
//					NdnSim::SerializedSizeName (m_interest->GetName ()) +
//
//					(2 +
//							(m_interest->GetExclude () == 0 ? 0 : (1 + NdnSim::SerializedSizeExclude (*m_interest->GetExclude ())))
//					)/* selectors */ +
//
//					(2 + 0)/* options */);
//
//	NS_LOG_INFO ("Serialize size = " << size);
//	return size;
}

void
NULLp::Serialize (Buffer::Iterator start) const
{
//	start.WriteU8 (0x80); // version
//	start.WriteU8 (0x00); // packet type
//
//	start.WriteU16 (GetSerializedSize () - 4);
//
//	start.WriteU32 (m_interest->GetNonce ());
//	start.WriteU8 (m_interest->GetScope ());
//	start.WriteU8 (m_interest->GetNack ());
//
//	NS_ASSERT_MSG (0 <= m_interest->GetInterestLifetime ().ToInteger (Time::S) && m_interest->GetInterestLifetime ().ToInteger (Time::S) < 65535,
//			"Incorrect InterestLifetime (should not be smaller than 0 and larger than 65535");
//
//	// rounding timestamp value to seconds
//	start.WriteU16 (static_cast<uint16_t> (m_interest->GetInterestLifetime ().ToInteger (Time::S)));
//
//	NdnSim::SerializeName (start, m_interest->GetName ());
//
//	if (m_interest->GetExclude () == 0)
//	{
//		start.WriteU16 (0); // no selectors
//	}
//	else
//	{
//		start.WriteU16 (1 + NdnSim::SerializedSizeExclude (*m_interest->GetExclude ()));
//		start.WriteU8 (0x01);
//		NdnSim::SerializeExclude (start, *m_interest->GetExclude ());
//	}
//
//	start.WriteU16 (0); // no options
}

uint32_t
NULLp::Deserialize (Buffer::Iterator start)
{
//	Buffer::Iterator i = start;
//
//	if (i.ReadU8 () != 0x80)
//		throw new InterestException ();
//
//	if (i.ReadU8 () != 0x00)
//		throw new InterestException ();
//
//	i.ReadU16 (); // length, don't need it right now
//
//	m_interest->SetNonce (i.ReadU32 ());
//	m_interest->SetScope (i.ReadU8 ());
//	m_interest->SetNack (i.ReadU8 ());
//
//	m_interest->SetInterestLifetime (Seconds (i.ReadU16 ()));
//
//	m_interest->SetName (NdnSim::DeserializeName (i));
//
//	uint32_t selectorsLen = i.ReadU16 ();
//	if (selectorsLen > 0)
//	{
//		if (i.ReadU8 () != 0x01) // exclude filter only
//			throw InterestException ();
//
//		m_interest->SetExclude (NdnSim::DeserializeExclude (i));
//	}
//	i.ReadU16 ();
//
//	NS_ASSERT (GetSerializedSize () == (i.GetDistanceFrom (start)));
//
//	return i.GetDistanceFrom (start);
}

void
NULLp::Print (std::ostream &os) const
{
	m_nullp->Print (os);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


SO::SO ()
: m_so (Create<nnn::SO> ())
{
}

SO::SO (Ptr<nnn::SO> so)
: m_so (so)
{
}

Ptr<nnn::SO>
SO::GetSO ()
{
	return m_so;
}

Ptr<Packet>
SO::ToWire (Ptr<const nnn::SO> data)
{

}

Ptr<nnn::SO>
SO::FromWire (Ptr<Packet> packet)
{

}

TypeId
SO::GetTypeId (void)
{
	static TypeId tid = TypeId ("ns3::nnn::SO::nnnSIM")
    		.SetGroupName ("Nnn")
    		.SetParent<Header> ()
    		.AddConstructor<SO> ()
    		;
	return tid;
}

TypeId
SO::GetInstanceTypeId (void) const
{
	return GetTypeId ();
}

uint32_t
SO::GetSerializedSize () const
{

}

void
SO::Serialize (Buffer::Iterator start) const
{

}

uint32_t
SO::Deserialize (Buffer::Iterator start)
{

}

void
SO::Print (std::ostream &os) const
{
	m_so->Print (os);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

DO::DO ()
: m_do_p (Create<nnn::DO> ())
{
}

DO::DO (Ptr<nnn::DO> do_p)
: m_do_p (do_p)
{
}

Ptr<nnn::DO>
DO::GetDO ()
{
	return m_do_p;
}

TypeId
DO::GetTypeId (void)
{
	static TypeId tid = TypeId ("ns3::nnn::DO::nnnSIM")
    		.SetGroupName ("Nnn")
    		.SetParent<Header> ()
    		.AddConstructor<NULLp> ()
    		;
	return tid;
}

TypeId
DO::GetInstanceTypeId (void) const
{
	return GetTypeId ();
}

Ptr<Packet>
DO::ToWire (Ptr<const nnn::DO> interest)
{

}

Ptr<nnn::DO>
DO::FromWire (Ptr<Packet> packet)
{

}

uint32_t
DO::GetSerializedSize (void) const
{

}

void
DO::Serialize (Buffer::Iterator start) const
{

}

uint32_t
DO::Deserialize (Buffer::Iterator start)
{

}

void
DO::Print (std::ostream &os) const
{
	m_do_p->Print (os);
}

}// nnnSIM
} // wire
NNN_NAMESPACE_END
