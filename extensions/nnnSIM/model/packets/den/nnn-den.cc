/* -*- Mode: C++; c-file-style: "gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *           Zhu Li <philipszhuli1990@ruri.waseda.jp>
 *
 *  nnn-den.h is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnn-den.h is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnn-den.h.  If not, see <http://www.gnu.org/licenses/>.
 */

#include<ns3-dev/ns3/log.h>
#include <ns3-dev/ns3/unused.h>
#include <ns3-dev/ns3/packet.h>

#include "nnn-den.h"

NS_LOG_COMPONENT_DEFINE ("nnn.DEN");

namespace ns3 {
namespace nnn{

DEN::DEN ()
: m_packetid (6)
, m_ttl		(Seconds (0))
, m_wire		(0)
{

}

DEN::DEN (Ptr<NNNAddress> name)
 : m_packetid (6)
 , m_ttl      (Seconds (1))
 , m_name     (name)
 , m_wire     (0)
{

}

DEN::DEN (const NNNAddress &name)
 : m_packetid (6)
 , m_ttl      (Seconds (1))
 , m_name     (Create<NNNAddress> (name))
 , m_wire     (0)
{

}

DEN::DEN (const DEN &den_p)
 : m_packetid (6)
 , m_ttl      (den_p.m_ttl)
 , m_name     (Create<NNNAddress> (den_p.GetName()))
 , m_wire     (0)
{
	NS_LOG_FUNCTION("DEN correct copy constructor");
}

uint32_t
DEN::GetPacketId()
{
	return m_packetid;
}

void
DEN::SetName(Ptr<NNNAddress> name)
{
	m_name = name;
	m_wire = 0;
}

void
DEN::SetName (const NNNAddress &name)
{
	m_name = Create<NNNAddress> (name);
	m_wire = 0;
}

const NNNAddress&
DEN::GetName () const
{
	if (m_name == 0) throw DENException ();
	return *m_name;
}

Ptr<const NNNAddress>
DEN::GetNamePtr () const
{
	return m_name;
}

void
DEN::SetLifetime (Time ttl)
{
	m_ttl = ttl;
	m_wire = 0;
}

Time
DEN::GetLifetime () const
{
	return m_ttl;
}


void
DEN::Print (std::ostream &os) const
{
	os << "<DEN>\n";
	os << " <TTL>" << GetLifetime () << "</TTL>\n";
	os << " <Name>" << GetName () << "</Name>\n";
}

} // namespace nnn
} // namespace ns3
