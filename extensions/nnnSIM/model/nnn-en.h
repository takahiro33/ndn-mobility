/* -*- Mode: C++; c-file-style: "gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnn-do.h is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnn-do.h is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnn-do.h.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _NNN_EN_HEADER_H_
#define _NNN_EN_HEADER_H_

#include <ns3-dev/ns3/simple-ref-count.h>
#include <ns3-dev/ns3/nstime.h>
#include <ns3-dev/ns3/packet.h>
#include  <ns3-dev/ns3/ptr.h>
#include <ns3-dev/ns3/mac48-address.h>

#include "nnn-address.h"

namespace ns3 {

class Packet;

namespace nnn {

/**
 * @ingroup nnn
 * @brief NNN EN packet (wire formats are defined in wire)
 **/
class EN : public SimpleRefCount<en>
{
public:
	/**
	 * \brief Constructor
	 *
	 * Creates a EN packet with no payload
	 **/
	EN ();

	/**
	 * \brief Constructor
	 *
	 *
	 * @param name MAC vector
	 **/
	EN(std::vector<MACAddress48> signature);

	/**
	 * @brief Copy constructor
	 */
	EN (const EN &en_p);

	/**
	 * \brief Get packet Identifier
	 *
	 **/
	void
	GetId();

	/**
	 * \brief Add Signature(MAC)
	 *
	 * @param signature MAC vectors
	 *
	 **/
	void
	AddSig (std::vector<MACAddress48> signature);

	/**
	 * \brief Get number of MN's Signatures
	 *
	 * @param  const reference to Name object
	 *
	 **/
    void
    GetSize () const;

	/**
	 * \brief Get Signatures of MN
	 *
	 **/
    const signature
	GetSig () const;

	/**
	 * \brief Set time out for EN packet
	 * Indicates the (approximate) time remaining before the packet times out.
	 * The timeout is relative to the arrival time of the interest at the current node.
	 * Based heavily on the NDN implementation for Interest Life time
	 * \see http://www.ndn.org/releases/latest/doc/technical/InterestMessage.html for more information.
	 * @param[in] time interest lifetime
	 */
	void
	SetLifetime (Time ttl);

	/**
	 * \brief Get time out value for EN packet
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
	 * @brief Print EN in plain-text to the specified output stream
	 */
	void
	Print (std::ostream &os) const;

private:
	// NO_ASSIGN
	EN &
	operator = (const EN &other) { return *this; }

private:
	uint32_t m_packetid;      ///< @brief Packet Identifier (3 for EN)
	Time m_ttl;               ///< @brief Packet life time (TTL)
	uint48_t m_sign;   ///< @brief Packet Identifier (MAC)
	Ptr<NNNAddress> m_name;   ///< @brief Destination NNN Address used in the packet

	mutable Ptr<const Packet> m_wire;
};

inline std::ostream &
operator << (std::ostream &os, const EN &i)
{
	i.Print (os);
	return os;
}

inline Ptr<const Packet>
EN::GetWire () const
{
	return m_wire;
}

inline void
EN::SetWire (Ptr<const Packet> packet) const
{
	m_wire = packet;
}

/**
 * @brief Class for Interest parsing exception
 */
class ENException {};

} // namespace nnn
} // namespace ns3

#endif // _NNN_EN_HEADER_H_
