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

	Simulator::Schedule(nameEntry.m_lease_expire, &NamesContainer::cleanExpired, this);
}

void
NamesContainer::addEntry (NNNAddress name, Time lease_expire)
{
	container.insert(NamesContainerEntry(name, lease_expire));

	Simulator::Schedule(lease_expire, &NamesContainer::cleanExpired, this);
}

void
NamesContainer::addEntry (NNNAddress name, Time lease_expire, Time renew)
{
	container.insert(NamesContainerEntry(name, lease_expire, renew));

	Simulator::Schedule(lease_expire, &NamesContainer::cleanExpired, this);
}

void
NamesContainer::deleteEntry (NamesContainerEntry nameEntry)
{
	container.erase(nameEntry);
}

void
NamesContainer::deleteEntry (NNNAddress name)
{
	NamesContainerEntry tmp = findEntry (name);

	container.erase(tmp);
}

bool
NamesContainer::foundName (NNNAddress name)
{
	names_set_by_name::iterator it = container.get<address> ().find(name);

	if (it == container.get<address> ().end())
		return false;
	else
		return true;
}

NamesContainerEntry
NamesContainer::findEntry (NNNAddress name)
{
	names_set_by_name::iterator it = container.get<address> ().find(name);

	if (it != container.get<address> ().end())
	{
		return *it;
	}
	else
	{
		return NamesContainerEntry ();
	}
}

NNNAddress
NamesContainer::findNewestName ()
{
	names_set_by_name::iterator it = container.get<address> ().end();

	it--;

	return it->m_name;
}

Time
NamesContainer::findNameExpireTime (NNNAddress name)
{
	NamesContainerEntry tmp = findEntry(name);

	return tmp.m_lease_expire;
}

uint32_t
NamesContainer::size()
{
	return container.size();
}

bool
NamesContainer::isEmpty()
{
	return (container.size() == 0);
}

void
NamesContainer::cleanExpired()
{
	Time now = Simulator::Now();

	//std::cout << "Deleting expired entries at " << now << std::endl;

	names_set_by_lease::iterator it = container.get<lease> ().begin();

	while (! isEmpty())
	{
		if (it->m_lease_expire <= now)
		{
			deleteEntry(*it);
			break;
		}

		++it;
	}
}

void
NamesContainer::printByAddress ()
{
	names_set_by_name::iterator it = container.get<address> ().begin();

	std::cout << "NNN Address\t| Lease Expire\t| Renew" << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;

	while (it != container.get<address> ().end())
	{
		std::cout << *it;
		++it;
	}
}

void
NamesContainer::printByLease ()
{
	names_set_by_lease::iterator it = container.get<lease> ().begin();

	std::cout << "NNN Address\t| Lease Expire\t| Renew" << std::endl;
	std::cout << "--------------------------------------------------" << std::endl;

	while (it != container.get<lease> ().end ())
	{
		std::cout << *it;
		++it;
	}
}

} /* namespace nnn */
} /* namespace ns3 */
