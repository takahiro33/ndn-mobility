/* -*- Mode: C++; c-file-style: "gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *	             Zhu Li <philipszhuli1990@ruri.waseda.jp>
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

#ifndef _NNN_DEN_HEADER_H_
#define _NNN_DEN_HEADER_H_

#include <vector>

#include <ns3-dev/ns3/mac48-address.h>
#include <ns3-dev/ns3/nstime.h>
#include <ns3-dev/ns3/packet.h>
#include <ns3-dev/ns3/ptr.h>
#include <ns3-dev/ns3/simple-ref-count.h>

#include "../../naming/nnn-address.h"

namespace ns3 {

class Packet;

namespace nnn {

/**
 * @ingroup nnn
 * @brief NNN DEN packet (wire formats are defined in wire)
 **/
class DEN : public SimpleRefCount<DEN>
{
public:
	/**
	 * \brief Constructor
	 *
	 * Creates a REN packet
	 **/
	DEN ();

	/**
	 * \brief Constructor
	 *
	 *
	 * @param name NNN Address Ptr
	 **/
	DEN(Ptr<NNNAddress> name);

	/**
	 * \brief Constructor
	 *
	 * Creates a DEN packet with payload
	 *
	 * @param name NNN Address
	 * @param payload Packet Ptr
	 **/
	DEN(const NNNAddress &name);

	/**
	 * @brief Copy constructor
	 */
	DEN (const DEN &den_p);

	/**
	 * \brief Return Id of the packet
	 *
	 **/
	uint32_t
	GetPacketId ();

	/**
	 * \brief Set interest name
	 *
	 * @param name smart pointer to Name
	 *
	 **/
	void
	SetName (Ptr<NNNAddress> name);

	/**
	 * \brief Another variant to set interest name
	 *
	 * @param name const reference to Name object
	 *
	 **/
	void
	SetName (const NNNAddress &name);

	/**
	 * \brief Get interest name
	 *
	 * Gets name of the interest.
	 **/
	const NNNAddress&
	GetName () const;

	/**
	 * @brief Get smart pointer to the interest name (to avoid extra memory usage)
	 */
	Ptr<const NNNAddress>
	GetNamePtr () const;

	/**
	 * \brief Set time out for DEN packet
	 * Indicates the (approximate) time remaining before the packet times out.
	 * The timeout is relative to the arrival time of the interest at the current node.
	 * Based heavily on the NDN implementation for Interest Life time
	 * \see http://www.ndn.org/releases/latest/doc/technical/InterestMessage.html for more information.
	 * @param[in] time interest lifetime
	 */
	void
	SetLifetime (Time ttl);

	/**
	 * \brief Get time out value for DEN packet
	 * Indicates the (approximate) time remaining before the packet times out.
	 * The timeout is relative to the arrival time of the interest at the current node.
	 * Based heavily on the NDN implementation for Interest Life time
	 * \see http://www.ndn.org/releases/latest/doc/technical/InterestMessage.html for more information.
	 */
	Time
	GetLifetime () const;

	/**
	 * @brief Get wire formatted packet
	 *
	 * If wire formatted packet has not been set before, 0 will be returned
	 */
	inline Ptr<const Packet>
	GetWire () const;

	/**
	 * @brief Set (cache) wire formatted packet
	 */
	inline void
	SetWire (Ptr<const Packet> packet) const;

	/**
	 * @brief Print DEN in plain-text to the specified output stream
	 */
	void
	Print (std::ostream &os) const;

private:
	// NO_ASSIGN
	DEN &
	operator = (const DEN &other) { return *this; }

private:
	uint32_t m_packetid;      ///< @brief Packet Identifier (5 for REN)
	Time m_ttl;               ///< @brief Packet life time (TTL)
	Ptr<NNNAddress> m_name;   ///< @brief Destination NNN Address used in the packet


	mutable Ptr<const Packet> m_wire;
};

inline std::ostream &
operator << (std::ostream &os, const DEN &i)
{
	i.Print (os);
	return os;
}

inline Ptr<const Packet>
DEN::GetWire () const
{
	return m_wire;
}

inline void
DEN::SetWire (Ptr<const Packet> packet) const
{
	m_wire = packet;
}

/**
 * @brief Class for Interest parsing exception
 */
class DENException {};
} //namespace nnn
} //namespace ns3

#endif // _NNN_DEN_HEADER_H_
