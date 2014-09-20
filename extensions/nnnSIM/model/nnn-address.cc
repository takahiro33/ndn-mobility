/* -*- Mode:C++; c-file-style:"gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnn-address.cc is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnn-address.cc is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnn-address.cc.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

#include <ctype.h>

#include "nnn-address.h"
#include "error.h"

using namespace std;

NNN_NAMESPACE_BEGIN

ATTRIBUTE_HELPER_CPP (NNNAddress);

///////////////////////////////////////////////////////////////////////////////
//                              CONSTRUCTORS                                 //
///////////////////////////////////////////////////////////////////////////////

NNNAddress::NNNAddress ()
{
}

// Create a valid NNN address
// No more than 16 hexadecimal characters with a maximum of 15 "."
NNNAddress::NNNAddress (const string &name)
{
	string::const_iterator i = name.begin ();
	string::const_iterator end = name.end ();

	// Check that we have only hexadecimal characters and dots
	boost::regex e("[^.0-9a-fA-F]");
	boost::match_results<std::string::const_iterator> what;
	boost::match_flag_type flags = boost::match_default;

	if (boost::regex_search(i, end, what, e, flags))
	{
		BOOST_THROW_EXCEPTION(error::NNNAddress () << error::msg("NNN address should be composed of only hexadecimal characters and dots!"));
	}

	// Check that string has less than 15 dots.
	int dotcount = count(i, end, '.');

	if (dotcount > 15)
	{
		BOOST_THROW_EXCEPTION(error::NNNAddress () << error::msg("NNN address should not have more than 15 '.'"));
	}

	int namesize = name.size () - dotcount;

	// Check that the total size of the string is lower than 31
	if (namesize > 16)
	{
		BOOST_THROW_EXCEPTION(error::NNNAddress () << error::msg("NNN address is of maximum 16 hexadecimal characters!"));
	}
}

NNNAddress::NNNAddress (const NNNAddress &other)
{
	m_address_comp = other.m_address_comp;
}

NNNAddress &
NNNAddress::operator= (const NNNAddress &other)
{
	m_address_comp = other.m_address_comp;
	return *this;
}

std::string
NNNAddress::toString () const
{
  ostringstream os;
  toString (os);
  return os.str ();
}

NNNAddress
NNNAddress::operator+ (const NNNAddress &name) const
{
  NNNAddress newName;
  return newName;
}

void
NNNAddress::toString (std::ostream &os) const
{
//	for (NNNAddress::const_iterator comp = begin (); comp != end (); comp++)
//	{
//		os << comp->toUri (os);
//		os << ".";
//	}
}

int
NNNAddress::compare (const NNNAddress &NNNAddress) const
{

}

inline size_t
NNNAddress::size () const
{
  return m_address_comp.size ();
}

NNN_NAMESPACE_END
