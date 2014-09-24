/* -*- Mode:C++; c-file-style:"gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Zhu Li <phillipszhuli1990@gmail.com>
 *           Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnn-wire.cc is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnn-wire.cc is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnn-wire.cc.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <ns3-dev/ns3/integer.h>
#include <ns3-dev/ns3/global-value.h>

#include "nnn-common.h"
#include "nnn-do.h"
#include "nnn-header-helper.h"
#include "nnn-so.h"
#include "nnn-wire.h"

#include "nnnsim.h"
#include "wire-nnnsim.h"

NNN_NAMESPACE_BEGIN

static
GlobalValue g_wireFormat ("nnn::WireFormat",
		"Default wire format for nnnSIM.  nnnSIM will be accepting packets "
		"in any supported packet formats, but if encoding requested, it will "
		"use default wire format to encode (0 for nnnSIM (default))",
		IntegerValue (Wire::WIRE_FORMAT_NNNSIM),
		MakeIntegerChecker<int32_t> ());

static inline uint32_t
GetWireFormat ()
{
	static int32_t format = -1;
	if (format >= 0)
		return format;

	IntegerValue value;
	g_wireFormat.GetValue (value);
	format = value.Get ();

	return format;
}

Ptr<Packet>
Wire::FromNULLp (Ptr<const NULLp> n_o, int8_t wireFormat)
{
	if (wireFormat == WIRE_FORMAT_DEFAULT)
		wireFormat = GetWireFormat ();

	if (wireFormat == WIRE_FORMAT_NNNSIM)
		return wire::nnnSIM::NULLp::ToWire (n_o);
	else
	{
		NS_FATAL_ERROR ("Unsupported format requested");
		return 0;
	}
}

Ptr<NULLp>
Wire::ToNULLp (Ptr<Packet> packet, int8_t wireFormat)
{
	if (wireFormat == WIRE_FORMAT_AUTODETECT)
	{
		try
		{
			HeaderHelper::Type type = HeaderHelper::GetNnnHeaderType (packet);
			switch (type)
			{
			case HeaderHelper::NULL_NNN:
			{
				return wire::nnnSIM::NULLp::FromWire (packet);
			}
			default:
				NS_FATAL_ERROR ("Unsupported format");
				return 0;
			}

			// exception will be thrown if packet is not recognized
		}
		catch (UnknownHeaderException)
		{
			NS_FATAL_ERROR ("Unknown NNN header");
			return 0;
		}
	}
	else
	{
		if (wireFormat == WIRE_FORMAT_NNNSIM)
			return wire::nnnSIM::NULLp::FromWire (packet);
		else
		{
			NS_FATAL_ERROR ("Unsupported format requested");
			return 0;
		}
	}
}

Ptr<Packet>
Wire::FromSO (Ptr<const SO> so_p, int8_t wireFormat/* = WIRE_FORMAT_DEFAULT*/)
{
	if (wireFormat == WIRE_FORMAT_DEFAULT)
		wireFormat = GetWireFormat ();

	if (wireFormat == WIRE_FORMAT_NNNSIM)
		return wire::nnnSIM::SO::ToWire (so_p);
	else
	{
		NS_FATAL_ERROR ("Unsupported format requested");
		return 0;
	}
}

Ptr<SO>
Wire::ToSO (Ptr<Packet> packet, int8_t wireFormat/* = WIRE_FORMAT_AUTODETECT*/)
{
	if (wireFormat == WIRE_FORMAT_AUTODETECT)
	{
		try
		{
			HeaderHelper::Type type = HeaderHelper::GetNnnHeaderType (packet);
			switch (type)
			{
			case HeaderHelper::SO_NNN:
			{
				return wire::nnnSIM::SO::FromWire (packet);
			}
			default:
				NS_FATAL_ERROR ("Unsupported format");
				return 0;
			}

			// exception will be thrown if packet is not recognized
		}
		catch (UnknownHeaderException)
		{
			NS_FATAL_ERROR ("Unknown NNN header");
			return 0;
		}
	}
	else
	{
		if (wireFormat == WIRE_FORMAT_NNNSIM)
			return wire::nnnSIM::SO::FromWire (packet);
		else
		{
			NS_FATAL_ERROR ("Unsupported format requested");
			return 0;
		}
	}
}

Ptr<Packet>
Wire::FromDO (Ptr<const DO> do_p, int8_t wireFormat/* = WIRE_FORMAT_DEFAULT*/)
{
	if (wireFormat == WIRE_FORMAT_DEFAULT)
		wireFormat = GetWireFormat ();

	if (wireFormat == WIRE_FORMAT_NNNSIM)
		return wire::nnnSIM::DO::ToWire (do_p);
	else
	{
		NS_FATAL_ERROR ("Unsupported format requested");
		return 0;
	}
}

Ptr<DO>
Wire::ToDO (Ptr<Packet> packet, int8_t wireFormat/* = WIRE_FORMAT_AUTODETECT*/)
{
	if (wireFormat == WIRE_FORMAT_AUTODETECT)
	{
		try
		{
			HeaderHelper::Type type = HeaderHelper::GetNnnHeaderType (packet);
			switch (type)
			{
			case HeaderHelper::DO_NNN:
			{
				return wire::nnnSIM::DO::FromWire (packet);
			}
			default:
				NS_FATAL_ERROR ("Unsupported format");
				return 0;
			}

			// exception will be thrown if packet is not recognized
		}
		catch (UnknownHeaderException)
		{
			NS_FATAL_ERROR ("Unknown NNN header");
			return 0;
		}
	}
	else
	{
		if (wireFormat == WIRE_FORMAT_NNNSIM)
			return wire::nnnSIM::DO::FromWire (packet);
		else
		{
			NS_FATAL_ERROR ("Unsupported format requested");
			return 0;
		}
	}
}

std::string
Wire::FromSOStr (Ptr<const SO> so_p, int8_t wireFormat/* = WIRE_FORMAT_DEFAULT*/)
{
	Ptr<Packet> pkt = FromSO (so_p, wireFormat);
	std::string wire;
	wire.resize (pkt->GetSize ());
	pkt->CopyData (reinterpret_cast<uint8_t*> (&wire[0]), wire.size ());

	return wire;
}

Ptr<SO>
Wire::ToSOStr (const std::string &wire, int8_t type/* = WIRE_FORMAT_AUTODETECT*/)
{
	Ptr<Packet> pkt = Create<Packet> (reinterpret_cast<const uint8_t*> (&wire[0]), wire.size ());
	return ToSO (pkt, type);
}

std::string
Wire::FromDOStr (Ptr<const DO> do_p, int8_t wireFormat/* = WIRE_FORMAT_DEFAULT*/)
{
	Ptr<Packet> pkt = FromDO (do_p, wireFormat);
	std::string wire;
	wire.resize (pkt->GetSize ());
	pkt->CopyData (reinterpret_cast<uint8_t*> (&wire[0]), wire.size ());

	return wire;
}

Ptr<DO>
Wire::ToDOStr (const std::string &wire, int8_t type/* = WIRE_FORMAT_AUTODETECT*/)
{
	Ptr<Packet> pkt = Create<Packet> (reinterpret_cast<const uint8_t*> (&wire[0]), wire.size ());
	return ToDO (pkt, type);
}

std::string
Wire::FromName (Ptr<const NNNAddress> name, int8_t wireFormat/* = WIRE_FORMAT_DEFAULT*/)
{

	if (wireFormat == WIRE_FORMAT_DEFAULT)
		wireFormat = GetWireFormat ();

	if (wireFormat == WIRE_FORMAT_NNNSIM)
	{
		Buffer buf;
		buf.AddAtStart (wire::NnnSim::SerializedSizeName (*name));
		Buffer::Iterator i = buf.Begin ();

		wire::NnnSim::SerializeName (i, *name);

		std::string wire;
		wire.resize (buf.GetSize ());
		buf.CopyData (reinterpret_cast<uint8_t *>(&wire[0]), wire.size ());

		return wire;
	}
	else
	{
		NS_FATAL_ERROR ("Unsupported format requested");
	}
}

Ptr<NNNAddress>
Wire::ToName (const std::string &name, int8_t wireFormat/* = WIRE_FORMAT_DEFAULT*/)
{
	Buffer buf;
	buf.AddAtStart (name.size ());
	Buffer::Iterator i = buf.Begin ();
	i.Write (reinterpret_cast<const uint8_t *> (&name[0]), name.size ());

	i = buf.Begin ();

	if (wireFormat == WIRE_FORMAT_DEFAULT)
		wireFormat = GetWireFormat ();

	if (wireFormat == WIRE_FORMAT_NNNSIM)
	{
		return wire::NnnSim::DeserializeName (i);
	}
	else
	{
		NS_FATAL_ERROR ("Unsupported format requested");
	}
}

NNN_NAMESPACE_END
