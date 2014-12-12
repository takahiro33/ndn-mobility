/* -*- Mode:C++; c-file-style:"gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Takahiro Miyamoto <mt3.mos@gmail.com>
 *           Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
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
    }

    NNPT::~NNPT() {
    }

    void
    NNPT::addEntry (NNNAddress oldName, NNNAddress newName, Time lease_expire)
    {
      container.insert(NNPTEntry(oldName, newName, lease_expire));

      Simulator::Schedule(lease_expire, &NNPT::cleanExpired, this);
    }

    void
    NNPT::addEntry (NNNAddress oldName, NNNAddress newName, Time lease_expire, Time renew)
    {
      container.insert(NNPTEntry(oldName, newName, lease_expire, renew));

      Simulator::Schedule(lease_expire, &NNPT::cleanExpired, this);
    }

    void
    NNPT::addEntry (NNPTEntry nnptEntry)
    {
      container.insert(nnptEntry);

      Simulator::Schedule(nnptEntry.m_lease_expire, &NNPT::cleanExpired, this);
    }

    void
    NNPT::deleteEntry (NNNAddress oldName)
    {
      NNPTEntry tmp = findEntry (oldName);
      container.erase(tmp);
    }

    void
    NNPT::deleteEntry (NNPTEntry nnptEntry)
    {
      container.erase(nnptEntry);
    }

    bool
    NNPT::foundName (NNNAddress name)
    {
      pair_set_by_name& names_index = container.get<pair> ();
      pair_set_by_name::iterator it = names_index.find(name);

      if (it == names_index.end())
	return false;
      else
	return true;
    }

    NNNAddress
    NNPT::findPairedName (NNNAddress oldName)
    {
      pair_set_by_name& pair_index = container.get<pair> ();
      pair_set_by_name::iterator it = pair_index.find(oldName);

      if (it != pair_index.end())
	{
	  NNPTEntry tmp = *it;
	  NNNAddress newName = tmp.m_newName;
	  return newName;
	}
      else
	{
	  return oldName;
	}
    }

    NNPTEntry
    NNPT::findEntry (NNNAddress name)
    {
      pair_set_by_name& pair_index = container.get<pair> ();
      pair_set_by_name::iterator it = pair_index.find(name);

      if (it != pair_index.end())
	{
	  return *it;
	}
      else
	{
	  return NNPTEntry ();

	}
    }

    NNNAddress
    NNPT::findNewestName ()
    {
      pair_set_by_name& pair_index = container.get<pair> ();
      pair_set_by_name::iterator it = pair_index.end();

      it--;

      return it->m_oldName;
    }

    void
    NNPT::updateLeaseTime (NNNAddress oldName, Time lease_expire)
    {
      pair_set_by_name& pair_index = container.get<pair> ();
      pair_set_by_name::iterator it = pair_index.find(oldName);

      if (it != pair_index.end())
	{
	  NNPTEntry tmp = *it;

	  tmp.m_lease_expire = lease_expire;
	  tmp.m_renew = lease_expire - Seconds (1);

	  if (pair_index.replace(it, tmp))
	    {
	      Simulator::Schedule(lease_expire, &NNPT::cleanExpired, this);
	    }
	}
    }

    void
    NNPT::updateLeaseTime (NNNAddress oldName, Time lease_expire, Time renew)
    {
      pair_set_by_name& pair_index = container.get<pair> ();
      pair_set_by_name::iterator it = pair_index.find(oldName);

      if (it != pair_index.end())
	{
	  NNPTEntry tmp = *it;

	  tmp.m_lease_expire = lease_expire;
	  tmp.m_renew = renew;

	  if (pair_index.replace(it, tmp))
	    {
	      Simulator::Schedule(lease_expire, &NNPT::cleanExpired, this);
	    }
	}
    }

    uint32_t
    NNPT::size ()
    {
      return container.size();
    }

    bool
    NNPT::isEmpty ()
    {
      return (container.size() == 0);
    }

    Time
    NNPT::findNameExpireTime (NNNAddress name)
    {
      NNPTEntry tmp = findEntry(name);

      return tmp.m_lease_expire;
    }

    Time
    NNPT::findNameExpireTime (NNPTEntry nnptEntry)
    {
      return nnptEntry.m_lease_expire;
    }

    void
    NNPT::cleanExpired ()
    {
      pair_set_by_lease& lease_index = container.get<st_lease> ();
      Time now = Simulator::Now();

      //std::cout << "Deleting expired entries at " << now << std::endl;

      pair_set_by_lease::iterator it = lease_index.begin();

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
    NNPT::printByAddress ()
    {
      pair_set_by_name& pair_index = container.get<pair> ();
      pair_set_by_name::iterator it = pair_index.begin();

      std::cout << "Old Address\t| New Address\t| Lease Expire\t| Renew" << std::endl;
      std::cout << "--------------------------------------------------------" << std::endl;

      while (it != pair_index.end())
	{
	  std::cout << *it;
	  ++it;
	}
    }

    void
    NNPT::printByLease ()
    {
      pair_set_by_lease& lease_index = container.get<st_lease> ();
      pair_set_by_lease::iterator it = lease_index.begin();

      std::cout << "NNN Address\t| New Address\t| Lease Expire\t| Renew" << std::endl;
      std::cout << "--------------------------------------------------------" << std::endl;

      while (it != lease_index.end ())
	{
	  std::cout << *it;
	  ++it;
	}
    }

    std::ostream&
    operator<< (std::ostream& os, const NNPT &nnpt)
    {
      return os;
    }

  } /* namespace nnn */
} /* namespace ns3 */
