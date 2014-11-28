/* -*- Mode:C++; c-file-style:"gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnn-nnpt.cc is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnn-nnpt.cc is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnn-nnpt.cc.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "nnn-nnpt.h"

namespace ns3 {
namespace nnn {

TypeId
NNPT::GetTypeId (void)
{
	static TypeId tid = TypeId ("ns3::nnn::NNPT") // cheating ns3 object system
    		.SetParent<Object> ()
    		.SetGroupName ("nnn")
    		;
	return tid;
}

NNPT::NNPT() {
	// TODO Auto-generated constructor stub

}

NNPT::~NNPT() {
	// TODO Auto-generated destructor stub
}


void
NNPT::addEntry (NNNAddress oldName, NNNAddress newName)
{
	container.insert(oldName, newName);

//	Simulator::Schedule(oldName.m_lease_expire, &NamesContainer::cleanExpired, this);
}

void
NNPT::addEntry (NNNAddress oldName, NNNAddress newName, Time lease_expire)
{

}

void
NNPT::addEntry (NNPT nnpt)
{

}

void
NNPT::deleteEntry (NNNAddress oldName)
{

}

void
NNPT::deleteEntry (NNPTEntry nnptEntry)
{

}

void
NNPT::deleteEntry (NNNAddress oldName, NNNAddress newName)
{

}

bool
NNPT::foundName (NNNAddress name)
{

}

NNPTEntry
NNPT::findEntry (NNNAddress name)
{

}

NNNAddress
NNPT::findNewestName ()
{

}

void
NNPT::updateLeaseTime (NNNAddress oldName, Time lease_expire)
{

}

void
NNPT::updateLeaseTime (NNNAddress oldName, Time lease_expire, Time renew)
{

}

uint32_t
NNPT::size ()
{

}

bool
NNPT::isEmpty ()
{

}

Time
NNPT::findNameExpireTime (NNNAddress name)
{

}

Time
NNPT::findNameExpireTime (NNPTEntry nnptEntry)
{

}

void
NNPT::cleanExpired ()
{

}

void
NNPT::printByAddress ()
{

}

void
NNPT::printByLease ()
{

}

void
NNPT::informEntry (NNNAddress oldName, NNNAddress newName, Time lease_expire) // where to go?
{

}


std::ostream&
operator<< (std::ostream& os, const NNPT &nnpt)
{
  return os;
}

} /* namespace nnn */
} /* namespace ns3 */
