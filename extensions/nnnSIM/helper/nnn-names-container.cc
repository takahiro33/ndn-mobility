/* -*- Mode:C++; c-file-style:"gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnn-names-container.cc is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnn-names-container.cc is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnn-names-container.cc.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "nnn-names-container.h"

namespace ns3 {
namespace nnn {

NamesContainer::NamesContainer ()
{
}

NamesContainer::~NamesContainer ()
{
}

void
NamesContainer::addEntry (NamesContainerEntry nameEntry)
{
	container.insert(nameEntry);
}

void
NamesContainer::addEntry (NNNAddress name, Time lease_expire)
{
	container.insert(NamesContainerEntry(name, lease_expire));
}

void
NamesContainer::addEntry (NNNAddress name, Time lease_expire, Time renew)
{
	container.insert(NamesContainerEntry(name, lease_expire, renew));
}

void
NamesContainer::deleteEntry (NamesContainerEntry nameEntry)
{

}

void
NamesContainer::deleteEntry (NNNAddress name)
{

}

Ptr<NamesContainerEntry>
NamesContainer::findEntry (NNNAddress name)
{

}

bool
NamesContainer::foundName (NNNAddress name)
{

}

void
NamesContainer::printByAddress ()
{

}

void
NamesContainer::printByLease ()
{

}

} /* namespace nnn */
} /* namespace ns3 */
