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
#include <ns3-dev/ns3/tag-buffer.h>

#include "wire-nnnsim.h"

NS_LOG_COMPONENT_DEFINE ("nnn.wire.nnnSIM");

NNN_NAMESPACE_BEGIN

namespace wire {
namespace nnnSIM {

NS_OBJECT_ENSURE_REGISTERED (NULLp);
NS_OBJECT_ENSURE_REGISTERED (SO);
NS_OBJECT_ENSURE_REGISTERED (DO);
NS_OBJECT_ENSURE_REGISTERED (EN);
NS_OBJECT_ENSURE_REGISTERED (AEN);
NS_OBJECT_ENSURE_REGISTERED (REN);
NS_OBJECT_ENSURE_REGISTERED (INF);

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
			2 +                                  /* Length of packet */
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
			2 +                                  /* Length of packet */
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
    		.AddConstructor<DO> ()
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
			2 +                                  /* Length of packet */
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

	// Serialize NNN address
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

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

EN::EN ()
: m_en_p (Create<nnn::EN> ())
{
}

EN::EN (Ptr<nnn::EN> en_p)
: m_en_p (en_p)
{
}

Ptr<nnn::EN>
EN::GetEN ()
{
	return m_en_p;
}

TypeId
EN::GetTypeId (void)
{
	static TypeId tid = TypeId ("ns3::nnn::EN::nnnSIM")
    		.SetGroupName ("Nnn")
    		.SetParent<Header> ()
    		.AddConstructor<EN> ()
    		;
	return tid;
}

TypeId
EN::GetInstanceTypeId (void) const
{
	return GetTypeId ();
}

Ptr<Packet>
EN::ToWire (Ptr<const nnn::EN> en_p)
{
	Ptr<const Packet> p = en_p->GetWire ();
	if (!p)
	{
		// Mechanism packets have no payload, make an empty packet
		Ptr<Packet> packet = Create<Packet> ();
		EN wireEncoding (ConstCast<nnn::EN> (en_p));
		packet->AddHeader (wireEncoding);
		en_p->SetWire (packet);

		p = packet;
	}
	return p->Copy ();
}

Ptr<nnn::EN>
EN::FromWire (Ptr<Packet> packet)
{
	Ptr<nnn::EN> en_p = Create<nnn::EN> ();
	Ptr<Packet> wire = packet->Copy ();

	EN wireEncoding (en_p);
	packet->RemoveHeader (wireEncoding);

	// Mechanism packets have no payload, make an empty packet
	en_p->SetWire (wire);

	return en_p;
}

uint32_t
EN::GetSerializedSize (void) const
{
	uint16_t poatype = m_en_p->GetPoaType ();
	size_t poatype_size = 0;
	size_t poa_num = 0;

	if (poatype == 0)
	{
		poa_num = m_en_p->GetNumPoa ();
		poatype_size = 6; // Hardcoded size of a Mac48Address
	}

	size_t size =
			4 +                                  /* Packetid */
			2 +                                  /* Length of packet */
			2 +                                  /* Timestamp */
			2 +                                  /* PoA Type */
			2 +                                  /* Number of PoAs */
			poa_num * poatype_size;              /* Total size of PoAs */

	NS_LOG_INFO ("Serialize size = " << size);
	return size;
}

void
EN::Serialize (Buffer::Iterator start) const
{
	// Get the total size of the serialized packet
	uint32_t totalsize = GetSerializedSize ();
	// Find out the PoA total size
	uint32_t totalpoabufsize = (totalsize -4 - 8);

	uint16_t poatype = m_en_p->GetPoaType ();
	size_t bufsize = 0;

	if (poatype == 0)
		bufsize = 6; // Hardcoded Mac48Address size

	// Create a buffer to be able to serialize PoAs
	uint8_t buffer[bufsize];

	uint32_t totalpoas = m_en_p->GetNumPoa();

	NS_ASSERT_MSG (totalpoabufsize == (totalpoas * bufsize),
			"Incorrect number of PoAs or serialization sizes" );

	// Serialize packetid
	start.WriteU32(m_en_p->GetPacketId());
	// Get the length of the packet
	start.WriteU16(totalsize - 4); // Minus packetid size of 32 bits
	// Check that the lifetime is within the limits
	NS_ASSERT_MSG (0 <= m_en_p->GetLifetime ().ToInteger (Time::S) &&
			m_en_p->GetLifetime ().ToInteger (Time::S) < 65535,
			"Incorrect Lifetime (should not be smaller than 0 and larger than 65535");

	// Round lifetime to seconds
	start.WriteU16 (static_cast<uint16_t> (m_en_p->GetLifetime ().ToInteger (Time::S)));

	// Serialize the PoA Type
	start.WriteU16(poatype);

	// Serialize the Number of PoAs
	start.WriteU16(totalpoas);

	// Go through the PoA list
	for (int i = 0; i < totalpoas; i++)
	{
		// Use the CopyTo function to get the bit representation
		m_en_p->GetOnePoa(i).CopyTo(buffer);

		// Since the bit representation is in 8 bit chunks, serialize it
		// accordingly
		for (int j = 0; j < bufsize; j++)
			start.WriteU8(buffer[j]);
	}
}

uint32_t
EN::Deserialize (Buffer::Iterator start)
{
	Buffer::Iterator i = start;

	// Read packet id
	if (i.ReadU32 () != 1)
		throw new ENException ();

	// Read length of packet
	uint16_t packet_len = i.ReadU16 ();

	// Read lifetime of the packet
	m_en_p->SetLifetime (Seconds (i.ReadU16 ()));

	// Read the PoA Type
	m_en_p->SetPoaType(i.ReadU16 ());

	size_t bufsize = 0;

	// Obtain how big the buffer has to be (dividing by 8)
	if (m_en_p->GetPoaType () == 0)
	{
		bufsize = 6;
	}

	// Read the number of PoAs the packet is holding
	uint32_t num_poa = i.ReadU16 ();

	// Update the length. The resulting size is the packet size that
	// we have to deserialize
	packet_len -= 8;

	// Create the buffer size
	uint8_t buffer[bufsize];

	// For the number of PoAs, read the 8 bits until the end of the PoA size
	for (int k = 0; k < num_poa; k++)
	{
		for (int j = 0; j < bufsize; j++)
		{
			buffer[j] = i.ReadU8 ();
		}

		if (m_en_p->GetPoaType () == 0)
		{
			Mac48Address tmp = Mac48Address ();
			tmp.CopyFrom (buffer);

			m_en_p->AddPoa (tmp);
		}
	}

	NS_ASSERT (GetSerializedSize () == (i.GetDistanceFrom (start)));

	return i.GetDistanceFrom (start);
}

void
EN::Print (std::ostream &os) const
{
	m_en_p->Print (os);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

AEN::AEN ()
: m_aen_p (Create<nnn::AEN> ())
{
}

AEN::AEN (Ptr<nnn::AEN> aen_p)
: m_aen_p (aen_p)
{
}

Ptr<nnn::AEN>
AEN::GetAEN ()
{
	return m_aen_p;
}

TypeId
AEN::GetTypeId (void)
{
	static TypeId tid = TypeId ("ns3::nnn::AEN::nnnSIM")
    		.SetGroupName ("Nnn")
    		.SetParent<Header> ()
    		.AddConstructor<AEN> ()
    		;
	return tid;
}

TypeId
AEN::GetInstanceTypeId (void) const
{
	return GetTypeId ();
}

Ptr<Packet>
AEN::ToWire (Ptr<const nnn::AEN> aen_p)
{
	Ptr<const Packet> p = aen_p->GetWire ();
	if (!p)
	{
		// Mechanism packets have no payload, make an empty packet
		Ptr<Packet> packet = Create<Packet> ();
		AEN wireEncoding (ConstCast<nnn::AEN> (aen_p));
		packet->AddHeader (wireEncoding);
		aen_p->SetWire (packet);

		p = packet;
	}
	return p->Copy ();
}

Ptr<nnn::AEN>
AEN::FromWire (Ptr<Packet> packet)
{
	Ptr<nnn::AEN> aen_p = Create<nnn::AEN> ();
	Ptr<Packet> wire = packet->Copy ();

	AEN wireEncoding (aen_p);
	packet->RemoveHeader (wireEncoding);

	// Mechanism packets have no payload, make an empty packet
	aen_p->SetWire (wire);

	return aen_p;
}

uint32_t
AEN::GetSerializedSize (void) const
{
	size_t size =
			4 +                                  /* Packetid */
			2 +                                  /* Length of packet */
			2 +                                  /* Timestamp */
			2 +                                  /* Lease time */
			NnnSim::SerializedSizeName (m_aen_p->GetName ()); /* Name size */


	NS_LOG_INFO ("Serialize size = " << size);
	return size;
}

void
AEN::Serialize (Buffer::Iterator start) const
{
	// Get the total size of the serialized packet
	uint32_t totalsize = GetSerializedSize ();

	// Serialize packetid
	start.WriteU32(m_aen_p->GetPacketId());

	// Get the length of the packet
	start.WriteU16(totalsize - 4); // Minus packetid size of 32 bits

	// Check that the lifetime is within the limits
	NS_ASSERT_MSG (0 <= m_aen_p->GetLifetime ().ToInteger (Time::S) &&
			m_aen_p->GetLifetime ().ToInteger (Time::S) < 65535,
			"Incorrect Lifetime (should not be smaller than 0 and larger than 65535");

	// Round lifetime to seconds
	start.WriteU16 (static_cast<uint16_t> (m_aen_p->GetLifetime ().ToInteger (Time::S)));

	NS_ASSERT_MSG (0 <= m_aen_p->GetLeasetime ().ToInteger (Time::S) &&
			m_aen_p->GetLeasetime ().ToInteger (Time::S) < 65535,
			"Incorrect Lease time (should not be smaller than 0 and larger than 65535");

	// Round lease time to seconds
	start.WriteU16 (static_cast<uint16_t> (m_aen_p->GetLeasetime ().ToInteger (Time::S)));

	// Serialize NNN address
	NnnSim::SerializeName(start, m_aen_p->GetName());
}

uint32_t
AEN::Deserialize (Buffer::Iterator start)
{
	Buffer::Iterator i = start;

	// Read packet id
	if (i.ReadU32 () != 1)
		throw new AENException ();

	// Read length of packet
	uint16_t packet_len = i.ReadU16 ();

	// Read lifetime of the packet
	m_aen_p->SetLifetime (Seconds (i.ReadU16 ()));

	// Read the PoA Type
	m_aen_p->SetLeasetime (Seconds (i.ReadU16 ()));

	// Deserialize the name
	m_aen_p->SetName(NnnSim::DeserializeName(i));

	NS_ASSERT (GetSerializedSize () == (i.GetDistanceFrom (start)));

	return i.GetDistanceFrom (start);
}

void
AEN::Print (std::ostream &os) const
{
	m_aen_p->Print (os);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

REN::REN ()
: m_ren_p (Create<nnn::REN> ())
{
}

REN::REN (Ptr<nnn::REN> ren_p)
: m_ren_p (ren_p)
{
}

Ptr<nnn::REN>
REN::GetREN()
{
	return m_ren_p;
}

TypeId
REN::GetTypeId (void)
{
	static TypeId tid = TypeId ("ns3::nnn::REN::nnnSIM")
    		.SetGroupName ("Nnn")
    		.SetParent<Header> ()
    		.AddConstructor<REN> ()
    		;
	return tid;
}

TypeId
REN::GetInstanceTypeId (void) const
{
	return GetTypeId ();
}

Ptr<Packet>
REN::ToWire (Ptr<const nnn::REN> ren_p)
{
	Ptr<const Packet> p = ren_p->GetWire ();
	if (!p)
	{
		// Mechanism packets have no payload, make an empty packet
		Ptr<Packet> packet = Create<Packet> ();
		REN wireEncoding (ConstCast<nnn::REN> (ren_p));
		packet->AddHeader (wireEncoding);
		ren_p->SetWire (packet);

		p = packet;
	}
	return p->Copy ();
}

Ptr<nnn::REN>
REN::FromWire (Ptr<Packet> packet)
{
	Ptr<nnn::REN> ren_p = Create<nnn::REN> ();
	Ptr<Packet> wire = packet->Copy ();

	REN wireEncoding (ren_p);
	packet->RemoveHeader (wireEncoding);

	// Mechanism packets have no payload, make an empty packet
	ren_p->SetWire (wire);

	return ren_p;
}

uint32_t
REN::GetSerializedSize (void) const
{
	uint16_t poatype = m_ren_p->GetPoaType ();
	size_t poatype_size = 0;
	size_t poa_num = 0;

	if (poatype == 0)
	{
		poa_num = m_ren_p->GetNumPoa ();
		poatype_size = 6; // Hardcoded size of a Mac48Address
	}

	size_t size =
			4 +                                  /* Packetid */
			2 +                                  /* Length of packet */
			2 +                                  /* Timestamp */
			2 +                                  /* PoA Type */
			2 +                                  /* Number of PoAs */
			poa_num * poatype_size +             /* Total size of PoAs */
			2 +                                  /* Lease time */
			NnnSim::SerializedSizeName (m_ren_p->GetName ()); /* Name size */

	NS_LOG_INFO ("Serialize size = " << size);
	return size;
}

void
REN::Serialize (Buffer::Iterator start) const
{
	// Get the total size of the serialized packet
	uint32_t totalsize = GetSerializedSize ();

	// Serialize packetid
	start.WriteU32(m_ren_p->GetPacketId());

	// Get the length of the packet
	start.WriteU16(totalsize - 4); // Minus packetid size of 32 bits

	// Check that the lifetime is within the limits
	NS_ASSERT_MSG (0 <= m_ren_p->GetLifetime ().ToInteger (Time::S) &&
			m_ren_p->GetLifetime ().ToInteger (Time::S) < 65535,
			"Incorrect Lifetime (should not be smaller than 0 and larger than 65535");

	// Round lifetime to seconds
	start.WriteU16 (static_cast<uint16_t> (m_ren_p->GetLifetime ().ToInteger (Time::S)));

	uint16_t poatype = m_ren_p->GetPoaType ();
	size_t bufsize = 0;

	if (poatype == 0)
		bufsize = 6; // Hardcoded Mac48Address size

	// Create a buffer to be able to serialize PoAs
	uint8_t buffer[bufsize];

	uint32_t totalpoas = m_ren_p->GetNumPoa();

	// Serialize the PoA Type
	start.WriteU16(poatype);

	// Serialize the Number of PoAs
	start.WriteU16(totalpoas);

	// Go through the PoA list
	for (int i = 0; i < totalpoas; i++)
	{
		// Use the CopyTo function to get the bit representation
		m_ren_p->GetOnePoa(i).CopyTo(buffer);

		// Since the bit representation is in 8 bit chunks, serialize it
		// accordingly
		for (int j = 0; j < bufsize; j++)
			start.WriteU8(buffer[j]);
	}

	NS_ASSERT_MSG (0 <= m_ren_p->GetRemainLease ().ToInteger (Time::S) &&
			m_ren_p->GetRemainLease ().ToInteger (Time::S) < 65535,
			"Incorrect Lease time (should not be smaller than 0 and larger than 65535");

	// Round lease time to seconds
	start.WriteU16 (static_cast<uint16_t> (m_ren_p->GetRemainLease ().ToInteger (Time::S)));

	// Serialize NNN address
	NnnSim::SerializeName(start, m_ren_p->GetName());
}

uint32_t
REN::Deserialize (Buffer::Iterator start)
{
	Buffer::Iterator i = start;

	// Read packet id
	if (i.ReadU32 () != 1)
		throw new INFException ();

	// Read length of packet
	uint16_t packet_len = i.ReadU16 ();

	// Read lifetime of the packet
	m_ren_p->SetLifetime (Seconds (i.ReadU16 ()));

	// Read the PoA Type
	m_ren_p->SetPoaType (i.ReadU16 ());

	// Read the number of PoAs
	size_t bufsize = 0;

	// Obtain how big the buffer has to be (dividing by 8)
	if (m_ren_p->GetPoaType () == 0)
	{
		bufsize = 6;
	}

	// Read the number of PoAs the packet is holding
	uint32_t num_poa = i.ReadU16 ();

	// Create the buffer size
	uint8_t buffer[bufsize];

	// For the number of PoAs, read the 8 bits until the end of the PoA size
	for (int k = 0; k < num_poa; k++)
	{
		for (int j = 0; j < bufsize; j++)
		{
			buffer[j] = i.ReadU8 ();
		}

		if (m_ren_p->GetPoaType () == 0)
		{
			Mac48Address tmp = Mac48Address ();
			tmp.CopyFrom (buffer);

			m_ren_p->AddPoa (tmp);
		}
	}

	// Read the least time
	m_ren_p->SetRemainLease (Seconds (i.ReadU16 ()));

	// Deserialize the old name
	m_ren_p->SetName(NnnSim::DeserializeName(i));

	NS_ASSERT (GetSerializedSize () == (i.GetDistanceFrom (start)));

	return i.GetDistanceFrom (start);
}

void
REN::Print (std::ostream &os) const
{
	m_ren_p->Print (os);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

INF::INF ()
: m_inf_p (Create<nnn::INF> ())
{
}

INF::INF (Ptr<nnn::INF> inf_p)
: m_inf_p (inf_p)
{
}

Ptr<nnn::INF>
INF::GetINF()
{
	return m_inf_p;
}

TypeId
INF::GetTypeId (void)
{
	static TypeId tid = TypeId ("ns3::nnn::INF::nnnSIM")
    		.SetGroupName ("Nnn")
    		.SetParent<Header> ()
    		.AddConstructor<INF> ()
    		;
	return tid;
}

TypeId
INF::GetInstanceTypeId (void) const
{
	return GetTypeId ();
}

Ptr<Packet>
INF::ToWire (Ptr<const nnn::INF> inf_p)
{
	Ptr<const Packet> p = inf_p->GetWire ();
	if (!p)
	{
		// Mechanism packets have no payload, make an empty packet
		Ptr<Packet> packet = Create<Packet> ();
		INF wireEncoding (ConstCast<nnn::INF> (inf_p));
		packet->AddHeader (wireEncoding);
		inf_p->SetWire (packet);

		p = packet;
	}
	return p->Copy ();
}

Ptr<nnn::INF>
INF::FromWire (Ptr<Packet> packet)
{
	Ptr<nnn::INF> inf_p = Create<nnn::INF> ();
	Ptr<Packet> wire = packet->Copy ();

	INF wireEncoding (inf_p);
	packet->RemoveHeader (wireEncoding);

	// Mechanism packets have no payload, make an empty packet
	inf_p->SetWire (wire);

	return inf_p;
}

uint32_t
INF::GetSerializedSize (void) const
{
	size_t size =
			4 +                                  /* Packetid */
			2 +                                  /* Length of packet */
			2 +                                  /* Timestamp */
			2 +                                  /* re Lease time */
			NnnSim::SerializedSizeName (m_inf_p->GetOldName ()) + /* Name size */
			NnnSim::SerializedSizeName (m_inf_p->GetNewName ()); /* Name size */

	NS_LOG_INFO ("Serialize size = " << size);
	return size;
}

void
INF::Serialize (Buffer::Iterator start) const
{
	// Get the total size of the serialized packet
	uint32_t totalsize = GetSerializedSize ();

	// Serialize packetid
	start.WriteU32(m_inf_p->GetPacketId());

	// Get the length of the packet
	start.WriteU16(totalsize - 4); // Minus packetid size of 32 bits

	// Check that the lifetime is within the limits
	NS_ASSERT_MSG (0 <= m_inf_p->GetLifetime ().ToInteger (Time::S) &&
			m_inf_p->GetLifetime ().ToInteger (Time::S) < 65535,
			"Incorrect Lifetime (should not be smaller than 0 and larger than 65535");

	// Round lifetime to seconds
	start.WriteU16 (static_cast<uint16_t> (m_inf_p->GetLifetime ().ToInteger (Time::S)));

	NS_ASSERT_MSG (0 <= m_inf_p->GetRemainLease ().ToInteger (Time::S) &&
			m_inf_p->GetRemainLease ().ToInteger (Time::S) < 65535,
			"Incorrect Lease time (should not be smaller than 0 and larger than 65535");

	// Round lease time to seconds
	start.WriteU16 (static_cast<uint16_t> (m_inf_p->GetRemainLease ().ToInteger (Time::S)));

	// Serialize NNN address
	NnnSim::SerializeName(start, m_inf_p->GetOldName());

	// Serialize NNN address
	NnnSim::SerializeName(start, m_inf_p->GetNewName());
}

uint32_t
INF::Deserialize (Buffer::Iterator start)
{
	Buffer::Iterator i = start;

	// Read packet id
	if (i.ReadU32 () != 1)
		throw new INFException ();

	// Read length of packet
	uint16_t packet_len = i.ReadU16 ();

	// Read lifetime of the packet
	m_inf_p->SetLifetime (Seconds (i.ReadU16 ()));

	// Read the PoA Type
	m_inf_p->SetRemainLease (Seconds (i.ReadU16 ()));

	// Deserialize the old name
	m_inf_p->SetOldName(NnnSim::DeserializeName(i));

	// Deserialize the new name
	m_inf_p->SetNewName(NnnSim::DeserializeName(i));

	NS_ASSERT (GetSerializedSize () == (i.GetDistanceFrom (start)));

	return i.GetDistanceFrom (start);
}

void
INF::Print (std::ostream &os) const
{
	m_inf_p->Print (os);
}


}// nnnSIM
} // wire

NNN_NAMESPACE_END
