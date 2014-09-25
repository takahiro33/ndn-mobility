/* -*- Mode: C++; c-file-style: "gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnn-do.cc is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnn-do.cc is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnn-en.cc.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <ns3-dev/ns3/log.h>
#include <ns3-dev/ns3/unused.h>
#include <ns3-dev/ns3/packet.h>

#include "nnn-inf.h"

NS_LOG_COMPONENT_DEFINE ("nnn.INF");

namespace ns3 {
namespace nnn {

INF::INF ()
 : m_packetid (5)
 , m_ttl      (Seconds (0))
 , m_wire     (0)
{

}

INF::INF (Ptr<NNNAddress> name)
 : m_packetid (5)
 , m_ttl      (Seconds (1))
 , m_name     (name)
 , m_wire     (0)

INF::INF (const NNNAddress &name, Ptr<Packet> payload)
 : m_packetid (5)
 , m_ttl      (Seconds (1))
 , m_name     (Create<NNNAddress> (name))
 , m_wire     (0)


INF::INF (const INF &inf_p)
 : m_packetid (5)
 , m_ttl      (inf_p.m_ttl)
 , m_length   (inf_p.m_length)
 , m_name     (Create<NNNAddress> (inf_p.GetName()))
 , m_wire     (0)
{
	NS_LOG_FUNCTION("INF correct copy constructor");
}

void
INF::SetName (Ptr<NNNAddress> name)
{
	m_name = name;
	m_wire = 0;
}

void
INF::SetName (const NNNAddress &name)
{
	m_name = Create<NNNAddress> (name);
	m_wire = 0;
}


const NNNAddress&
INF::GetName () const
{
	if (m_name == 0) throw INFException ();
	return *m_name;
}


Ptr<const NNNAddress>
INF::GetNamePtr () const
{
	return m_name;
}

void
INF::SetLifetime (Time ttl)
{
	m_ttl = ttl;
	m_wire = 0;
}

Time
INF::GetLifetime () const
{
	return m_ttl;
}

void
INF::Print (std::ostream &os) const
{
	os << "<INF>\n";
	os << "  <TTL>" << GetLifetime () << "</TTL>\n";
	os << "  <Name>" << GetName () << "</Name>\n";
	os << "</INF>";
}

} // namespace nnn
} // namespace ns3
