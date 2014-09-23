/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011 University of California, Los Angeles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#include <ns3-dev/ns3/log.h>
#include <ns3-dev/ns3/unused.h>
#include <ns3-dev/ns3/packet.h>

#include "nnn-nullp.h"

NS_LOG_COMPONENT_DEFINE ("nnn.NULLp");

namespace ns3 {
namespace nnn {

NULLp::NULLp ()
 : m_packetid (0)
 , m_ttl      (Seconds (0))
 , m_length   (0)
 , m_payload  (Create<Packet> ())
 , m_wire     (0)
{

}

NULLp::NULLp (Ptr<Packet> payload)
 : m_packetid (0)
 , m_ttl      (Seconds (1))
 , m_wire     (0)
{
	if (m_payload == 0)
	{
		m_payload = Create<Packet> ();
		m_length = 0;
	} else
	{
		m_payload = payload;
		m_length = m_payload->GetSize();
	}
}

NULLp::NULLp (const NULLp &nullp)
 : m_packetid (0)
 , m_ttl      (nullp.m_ttl)
 , m_payload  (nullp.GetPayload ()->Copy ())
 , m_length   (nullp.m_length)
 , m_wire     (0)
{
	NS_LOG_FUNCTION("NULLp correct copy constructor");
}

void
NULLp::SetLifetime (Time ttl)
{
	m_ttl = ttl;
	m_wire = 0;
}

Time
NULLp::GetLifetime () const
{
	return m_ttl;
}

void
NULLp::SetLength (int32_t len)
{
	m_length = len;
}

uint32_t
NULLp::GetLength () const
{
	return m_length;
}

void
NULLp::SetPayload (Ptr<Packet> payload)
{
	m_payload = payload;
	m_wire = 0;
}

Ptr<const Packet>
NULLp::GetPayload () const
{
	return m_payload;
}

void
NULLp::Print (std::ostream &os) const
{
	os << "<NULLp>\n";
	os << "  <TTL>" << GetLifetime () << "</TTL>\n";
	os << "  <Length>" << GetLength () << "</Length>\n";
	if (m_payload != 0)
	{
		os << "  <Payload>Yes</Payload>\n";
	} else
	{
		os << "  <Payload>No</Payload>\n";
	}
	os << "</NULLp>";
}

} // namespace nnn
} // namespace ns3
