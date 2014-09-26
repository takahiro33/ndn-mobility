/* -*- Mode:C++; c-file-style:"gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Zhu Li <phillipszhuli1990@gmail.com>
 *           Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnnsim.h is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnnsim.h is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnnsim.h.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NNN_WIRE_NNNSIM_H
#define NNN_WIRE_NNNSIM_H

#include "nnn-common.h"
#include "nnn-nullp.h"
#include "nnn-do.h"
#include "nnn-so.h"
#include "nnn-en.h"
#include "nnn-aen.h"
#include "nnn-ren.h"
#include "nnn-inf.h"

NNN_NAMESPACE_BEGIN

/**
 * @brief Namespace encapsulating wire operations
 */
namespace wire {

/**
 * @brief Namespace for nnnSIM wire format operations
 */
namespace nnnSIM {

/**
 * @brief Routines to serialize/deserialize NULL packets in nnnSIM format
 */
class NULLp : public Header
{
public:
	NULLp ();
	NULLp (Ptr<nnn::NULLp> null_p);

	Ptr<nnn::NULLp>
	GetNULLp ();

	static Ptr<Packet>
	ToWire (Ptr<const nnn::NULLp> null_p);

	static Ptr<nnn::NULLp>
	FromWire (Ptr<Packet> packet);

	// from Header
	static TypeId GetTypeId (void);
	virtual TypeId GetInstanceTypeId (void) const;
	virtual void Print (std::ostream &os) const;
	virtual uint32_t GetSerializedSize (void) const;
	virtual void Serialize (Buffer::Iterator start) const;
	virtual uint32_t Deserialize (Buffer::Iterator start);

private:
	Ptr<nnn::NULLp> m_null_p;
};

/**
 * @brief Routines to serialize/deserialize SO packets in nnnSIM format
 */
class SO : public Header
{
public:
	SO ();
	SO (Ptr<nnn::SO> so);

	Ptr<nnn::SO>
	GetSO ();

	static Ptr<Packet>
	ToWire (Ptr<const nnn::SO> so);

	static Ptr<nnn::SO>
	FromWire (Ptr<Packet> packet);

	// from Header
	static TypeId GetTypeId (void);
	virtual TypeId GetInstanceTypeId (void) const;
	virtual void Print (std::ostream &os) const;
	virtual uint32_t GetSerializedSize (void) const;
	virtual void Serialize (Buffer::Iterator start) const;
	virtual uint32_t Deserialize (Buffer::Iterator start);

private:
	Ptr<nnn::SO> m_so_p;
};

/**
 * @brief Routines to serialize/deserialize DO packets in nnnSIM format
 */
class DO : public Header
{
public:
	DO ();
	DO (Ptr<nnn::DO> do_p);

	Ptr<nnn::DO>
	GetDO ();

	static Ptr<Packet>
	ToWire (Ptr<const nnn::DO> do_p);

	static Ptr<nnn::DO>
	FromWire (Ptr<Packet> packet);

	// from Header
	static TypeId GetTypeId (void);
	virtual TypeId GetInstanceTypeId (void) const;
	virtual void Print (std::ostream &os) const;
	virtual uint32_t GetSerializedSize (void) const;
	virtual void Serialize (Buffer::Iterator start) const;
	virtual uint32_t Deserialize (Buffer::Iterator start);

private:
	Ptr<nnn::DO> m_do_p;
};

class EN : public Header
{
public:
	EN ();
	EN (Ptr<nnn::EN> en_p);

	Ptr<nnn::EN>
	GetEN ();

	static Ptr<Packet>
	ToWire (Ptr<const nnn::EN> en_p);

	static Ptr<nnn::EN>
	FromWire (Ptr<Packet> packet);

	// from Header
	static TypeId GetTypeId (void);
	virtual TypeId GetInstanceTypeId (void) const;
	virtual void Print (std::ostream &os) const;
	virtual uint32_t GetSerializedSize (void) const;
	virtual void Serialize (Buffer::Iterator start) const;
	virtual uint32_t Deserialize (Buffer::Iterator start);

private:
	Ptr<nnn::EN> m_en_p;
};

class AEN : public Header
{
public:
	AEN ();
	AEN (Ptr<nnn::AEN> aen_p);

	Ptr<nnn::AEN>
	GetAEN ();

	static Ptr<Packet>
	ToWire (Ptr<const nnn::AEN> aen_p);

	static Ptr<nnn::AEN>
	FromWire (Ptr<Packet> packet);

	// from Header
	static TypeId GetTypeId (void);
	virtual TypeId GetInstanceTypeId (void) const;
	virtual void Print (std::ostream &os) const;
	virtual uint32_t GetSerializedSize (void) const;
	virtual void Serialize (Buffer::Iterator start) const;
	virtual uint32_t Deserialize (Buffer::Iterator start);

private:
	Ptr<nnn::AEN> m_aen_p;
};

class REN : public Header
{
public:
	REN ();
	REN (Ptr<nnn::REN> ren_p);

	Ptr<nnn::REN>
	GetREN ();

	static Ptr<Packet>
	ToWire (Ptr<const nnn::REN> ren_p);

	static Ptr<nnn::REN>
	FromWire (Ptr<Packet> packet);

	// from Header
	static TypeId GetTypeId (void);
	virtual TypeId GetInstanceTypeId (void) const;
	virtual void Print (std::ostream &os) const;
	virtual uint32_t GetSerializedSize (void) const;
	virtual void Serialize (Buffer::Iterator start) const;
	virtual uint32_t Deserialize (Buffer::Iterator start);

private:
	Ptr<nnn::REN> m_ren_p;
};

class INF : public Header
{
public:
	INF ();
	INF (Ptr<nnn::INF> inf_p);

	Ptr<nnn::INF>
	GetINF ();

	static Ptr<Packet>
	ToWire (Ptr<const nnn::INF> inf_p);

	static Ptr<nnn::INF>
	FromWire (Ptr<Packet> packet);

	// from Header
	static TypeId GetTypeId (void);
	virtual TypeId GetInstanceTypeId (void) const;
	virtual void Print (std::ostream &os) const;
	virtual uint32_t GetSerializedSize (void) const;
	virtual void Serialize (Buffer::Iterator start) const;
	virtual uint32_t Deserialize (Buffer::Iterator start);

private:
	Ptr<nnn::INF> m_inf_p;
};

} // nnnSIM
} // wire

NNN_NAMESPACE_END

#endif // NNN_WIRE_NNNSIM_H
