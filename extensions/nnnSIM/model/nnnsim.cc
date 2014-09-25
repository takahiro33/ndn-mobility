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
: m_null_p (Create<nnn::NULLp> ())
{
}

NULLp::NULLp (Ptr<nnn::NULLp> null_p)
: m_null_p (null_p)
{
}

Ptr<nnn::NULLp>
NULLp::GetNULLp ()
{
	return m_null_p;
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
NULLp::ToWire (Ptr<const nnn::NULLp> null_p)
{
	Ptr<const Packet> p = null_p->GetWire ();
	if (!p)
	{
		Ptr<Packet> packet = Create<Packet> (*null_p->GetPayload ());
		NULLp wireEncoding (ConstCast<nnn::NULLp> (null_p));
		packet->AddHeader (wireEncoding);
		null_p->SetWire (packet);

		p = packet;
	}

	return p->Copy ();
}

Ptr<nnn::NULLp>
NULLp::FromWire (Ptr<Packet> packet)
{
	Ptr<nnn::NULLp> null_p = Create<nnn::NULLp> ();
	Ptr<Packet> wire = packet->Copy ();

	NULLp wireEncoding (null_p);
	packet->RemoveHeader (wireEncoding);

	null_p->SetPayload (packet);
	null_p->SetWire (wire);

	return null_p;
}

uint32_t
NULLp::GetSerializedSize (void) const
{
	size_t size =
			4 +                                  /* Packetid */
			2 +                                  /* Payload size */
			2 +                                  /* Timestamp */
			m_null_p->GetPayload ()->GetSerializedSize();  /* Payload data */

	NS_LOG_INFO ("Serialize size = " << size);
	return size;
}

void
NULLp::Serialize (Buffer::Iterator start) const
{
	// Get the total size of the serialized packet
	uint32_t totalsize = GetSerializedSize ();
	// Find out the payload size
	uint32_t payloadsize = m_null_p->GetPayload ()->GetSerializedSize ();
	// Create a buffer to be able to serialize the payload
	uint8_t buffer[payloadsize];

	// Serialize packetid
	start.WriteU32(m_null_p->GetPacketId());
	// Get the length of the packet
	start.WriteU16(totalsize - 4); // Minus packetid size of 32 bits

	// Check that the lifetime is within the limits
	NS_ASSERT_MSG (0 <= m_null_p->GetLifetime ().ToInteger (Time::S) &&
			m_null_p->GetLifetime ().ToInteger (Time::S) < 65535,
			"Incorrect Lifetime (should not be smaller than 0 and larger than 65535");

	// Round lifetime to seconds
	start.WriteU16 (static_cast<uint16_t> (m_null_p->GetLifetime ().ToInteger (Time::S)));

	// Serialize the payload and place it in the variable buffer
	m_null_p->GetPayload ()->Serialize (buffer, payloadsize);

	for (int i = 0; i < payloadsize; i++)
	{
		start.WriteU8(buffer[i]);
	}
}

uint32_t
NULLp::Deserialize (Buffer::Iterator start)
{
	Buffer::Iterator i = start;

	// Read packet id
	if (i.ReadU32 () != 0)
		throw new NULLpException ();

	// Read length of packet
	uint16_t packet_len = i.ReadU16 ();

	// Read lifetime of the packet
	m_null_p->SetLifetime (Seconds (i.ReadU16 ()));

	// Update the length. The resulting size is the packet size that
	// we have to deserialize
	packet_len -= 4;

	// Obtain how big the buffer has to be (dividing by 8)
	uint32_t bufsize = packet_len / 8;

	// Create the buffer size
	uint8_t buffer[bufsize];

	// Read 8 bits at a time until the end of the packet
	for (int j = 0; j < bufsize; j++)
	{
		buffer[j] = i.ReadU8 ();
	}

	// Create a packet from the deserialized information
	Ptr<Packet> depacket = Create <Packet> (buffer, packet_len, true);

	// Save the packet information
	m_null_p->SetPayload (depacket);

	NS_ASSERT (GetSerializedSize () == (i.GetDistanceFrom (start)));

	return i.GetDistanceFrom (start);
}

void
NULLp::Print (std::ostream &os) const
{
	m_null_p->Print (os);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


SO::SO ()
: m_so_p (Create<nnn::SO> ())
{
}

SO::SO (Ptr<nnn::SO> so_p)
: m_so_p (so_p)
{
}

Ptr<nnn::SO>
SO::GetSO ()
{
	return m_so_p;
}

Ptr<Packet>
SO::ToWire (Ptr<const nnn::SO> so_p)
{
	Ptr<const Packet> p = so_p->GetWire ();
	if (!p)
	{
		Ptr<Packet> packet = Create<Packet> (*so_p->GetPayload ());
		SO wireEncoding (ConstCast<nnn::SO> (so_p));
		packet->AddHeader (wireEncoding);
		so_p->SetWire (packet);

		p = packet;
	}

	return p->Copy ();
}

Ptr<nnn::SO>
SO::FromWire (Ptr<Packet> packet)
{
	Ptr<nnn::SO> so_p = Create<nnn::SO> ();
	Ptr<Packet> wire = packet->Copy ();

	SO wireEncoding (so_p);
	packet->RemoveHeader (wireEncoding);

	so_p->SetPayload (packet);
	so_p->SetWire (wire);

	return so_p;
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
	size_t size =
			4 +                                  /* Packetid */
			2 +                                  /* Payload size */
			2 +                                  /* Timestamp */
			NnnSim::SerializedSizeName (m_so_p->GetName ()) + /* Name size */
			m_so_p->GetPayload ()->GetSerializedSize();    /* Payload data */

	NS_LOG_INFO ("Serialize size = " << size);
	return size;
}

void
SO::Serialize (Buffer::Iterator start) const
{
	// Get the total size of the serialized packet
	uint32_t totalsize = GetSerializedSize ();
	// Find out the payload size
	uint32_t payloadsize = m_so_p->GetPayload ()->GetSerializedSize ();
	// Create a buffer to be able to serialize the payload
	uint8_t buffer[payloadsize];

	// Serialize packetid
	start.WriteU32(m_so_p->GetPacketId());
	// Get the length of the packet
	start.WriteU16(totalsize - 4); // Minus packetid size of 32 bits
	// Check that the lifetime is within the limits
	NS_ASSERT_MSG (0 <= m_so_p->GetLifetime ().ToInteger (Time::S) &&
			m_so_p->GetLifetime ().ToInteger (Time::S) < 65535,
			"Incorrect Lifetime (should not be smaller than 0 and larger than 65535");

	// Round lifetime to seconds
	start.WriteU16 (static_cast<uint16_t> (m_so_p->GetLifetime ().ToInteger (Time::S)));

	// Serialize
	NnnSim::SerializeName(start, m_so_p->GetName());

	// Serialize the payload and place it in the variable buffer
	m_so_p->GetPayload ()->Serialize (buffer, payloadsize);

	for (int i = 0; i < payloadsize; i++)
	{
		start.WriteU8(buffer[i]);
	}
}

uint32_t
SO::Deserialize (Buffer::Iterator start)
{
	Buffer::Iterator i = start;

	// Read packet id
	if (i.ReadU32 () != 1)
		throw new NULLpException ();

	// Read length of packet
	uint16_t packet_len = i.ReadU16 ();

	// Read lifetime of the packet
	m_so_p->SetLifetime (Seconds (i.ReadU16 ()));

	// Update the length. The resulting size is the packet size that
	// we have to deserialize
	packet_len -= 4;

	// Deserialize the name
	m_so_p->SetName(NnnSim::DeserializeName(i));

	// Substract the size of what we just got
	packet_len -= NnnSim::SerializedSizeName (m_so_p->GetName ());

	// Obtain how big the buffer has to be (dividing by 8)
	uint32_t bufsize = packet_len / 8;

	// Create the buffer size
	uint8_t buffer[bufsize];

	// Read 8 bits at a time until the end of the packet
	for (int j = 0; j < bufsize; j++)
	{
		buffer[j] = i.ReadU8 ();
	}

	// Create a packet from the deserialized information
	Ptr<Packet> depacket = Create <Packet> (buffer, packet_len, true);

	// Save the packet information
	m_so_p->SetPayload (depacket);

	NS_ASSERT (GetSerializedSize () == (i.GetDistanceFrom (start)));

	return i.GetDistanceFrom (start);
}

void
SO::Print (std::ostream &os) const
{
	m_so_p->Print (os);
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
DO::ToWire (Ptr<const nnn::DO> do_p)
{
	Ptr<const Packet> p = do_p->GetWire ();
	if (!p)
	{
		Ptr<Packet> packet = Create<Packet> (*do_p->GetPayload ());
		DO wireEncoding (ConstCast<nnn::DO> (do_p));
		packet->AddHeader (wireEncoding);
		do_p->SetWire (packet);

		p = packet;
	}
	return p->Copy ();
}

Ptr<nnn::DO>
DO::FromWire (Ptr<Packet> packet)
{
	Ptr<nnn::DO> do_p = Create<nnn::DO> ();
	Ptr<Packet> wire = packet->Copy ();

	DO wireEncoding (do_p);
	packet->RemoveHeader (wireEncoding);

	do_p->SetPayload (packet);
	do_p->SetWire (wire);

	return do_p;
}

uint32_t
DO::GetSerializedSize (void) const
{
	size_t size =
			4 +                                  /* Packetid */
			2 +                                  /* Payload size */
			2 +                                  /* Timestamp */
			NnnSim::SerializedSizeName (m_do_p->GetName ()) + /* Name size */
			m_do_p->GetPayload ()->GetSerializedSize();    /* Payload data */

	NS_LOG_INFO ("Serialize size = " << size);
	return size;
}

void
DO::Serialize (Buffer::Iterator start) const
{
	// Get the total size of the serialized packet
	uint32_t totalsize = GetSerializedSize ();
	// Find out the payload size
	uint32_t payloadsize = m_do_p->GetPayload ()->GetSerializedSize ();
	// Create a buffer to be able to serialize the payload
	uint8_t buffer[payloadsize];

	// Serialize packetid
	start.WriteU32(m_do_p->GetPacketId());
	// Get the length of the packet
	start.WriteU16(totalsize - 4); // Minus packetid size of 32 bits
	// Check that the lifetime is within the limits
	NS_ASSERT_MSG (0 <= m_do_p->GetLifetime ().ToInteger (Time::S) &&
			m_do_p->GetLifetime ().ToInteger (Time::S) < 65535,
			"Incorrect Lifetime (should not be smaller than 0 and larger than 65535");

	// Round lifetime to seconds
	start.WriteU16 (static_cast<uint16_t> (m_do_p->GetLifetime ().ToInteger (Time::S)));

	// Serialize
	NnnSim::SerializeName(start, m_do_p->GetName());

	// Serialize the payload and place it in the variable buffer
	m_do_p->GetPayload ()->Serialize (buffer, payloadsize);

	for (int i = 0; i < payloadsize; i++)
	{
		start.WriteU8(buffer[i]);
	}
}

uint32_t
DO::Deserialize (Buffer::Iterator start)
{
	Buffer::Iterator i = start;

	// Read packet id
	if (i.ReadU32 () != 1)
		throw new NULLpException ();

	// Read length of packet
	uint16_t packet_len = i.ReadU16 ();

	// Read lifetime of the packet
	m_do_p->SetLifetime (Seconds (i.ReadU16 ()));

	// Update the length. The resulting size is the packet size that
	// we have to deserialize
	packet_len -= 4;

	// Deserialize the name
	m_do_p->SetName(NnnSim::DeserializeName(i));

	// Substract the size of what we just got
	packet_len -= NnnSim::SerializedSizeName (m_do_p->GetName ());

	// Obtain how big the buffer has to be (dividing by 8)
	uint32_t bufsize = packet_len / 8;

	// Create the buffer size
	uint8_t buffer[bufsize];

	// Read 8 bits at a time until the end of the packet
	for (int j = 0; j < bufsize; j++)
	{
		buffer[j] = i.ReadU8 ();
	}

	// Create a packet from the deserialized information
	Ptr<Packet> depacket = Create <Packet> (buffer, packet_len, true);

	// Save the packet information
	m_do_p->SetPayload (depacket);

	NS_ASSERT (GetSerializedSize () == (i.GetDistanceFrom (start)));

	return i.GetDistanceFrom (start);
}

void
DO::Print (std::ostream &os) const
{
	m_do_p->Print (os);
}

}// nnnSIM
} // wire

NNN_NAMESPACE_END
