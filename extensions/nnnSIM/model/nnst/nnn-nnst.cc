/* -*- Mode:C++; c-file-style:"gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnn-nnst.cc is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnn-nnst.cc is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnn-nnst.cc.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/core.hpp>
#include <boost/ref.hpp>

namespace ll = boost::lambda;

#include "nnn-nnst.h"
#include "nnn-nnst-entry.h"

NS_LOG_COMPONENT_DEFINE ("nnn.nnst.impl");

namespace ns3 {
  namespace nnn {

    TypeId
    NNST::GetTypeId (void)
    {
      static TypeId tid = TypeId ("ns3::nnn::NNST") // cheating ns3 object system
    		    .SetParent<Object> ()
    		    .SetGroupName ("nnn")
    		    .AddConstructor<NNST> ();
      return tid;
    }

    NNST::NNST() {
    }

    NNST::~NNST() {
    }

    Ptr<nnst::Entry>
    NNST::ClosestSector (const NNNAddress &nnnaddr)
    {
      super::iterator item = super::longest_prefix_match (nnnaddr);
      // @todo use predicate to search with exclude filters

      if (item == super::end ())
	return 0;
      else
	return item->payload ();
    }

    Ptr<nnst::Entry>
    NNST::Find (const NNNAddress &name)
    {
      super::iterator item = super::find_exact (name);

      if (item == super::end ())
	return 0;
      else
	return item->payload ();
    }

    // To be deleted
    Ptr<nnst::Entry>
    NNST::Add (const NNNAddress &prefix, Ptr<Face> face, int32_t metric)
    {
      return Add (Create<NNNAddress> (prefix), face, metric);
    }

    // This one to be deleted as well
    Ptr<nnst::Entry>
    NNST::Add (const Ptr<const NNNAddress> &name, Ptr<Face> face, int32_t metric)
    {
    }

    Ptr<nnst::Entry>
    NNST::Add (const NNNAddress &name, Ptr<Face> face, Address poa, Time lease_expire, int32_t metric)
    {
      return Add (Create<NNNAddress> (name), face, poa, lease_expire, metric);
    }

    Ptr<nnst::Entry>
    NNST::Add (const Ptr<const NNNAddress> &prefix, std::vector<Ptr<Face> > faces, Address poa, Time lease_expire, int32_t metric)
    {
      Ptr<nnst::Entry> tmp;
      for (std::vector<Ptr<Face> >::iterator i = faces.begin(); i != faces.end (); ++i)
	{
	  tmp = Add(prefix, *i, poa, lease_expire, metric);
	}

      return tmp;
    }

    Ptr<nnst::Entry>
    NNST::Add (const Ptr<const NNNAddress> &prefix, Ptr<Face> face, std::vector<Address> poas, Time lease_expire, int32_t metric)
    {
      Ptr<nnst::Entry> tmp;
      for (std::vector<Address>::iterator i = poas.begin(); i != poas.end (); ++i)
	{
	  tmp = Add(prefix, face, *i, lease_expire, metric);
	}

      return tmp;
    }

    Ptr<nnst::Entry>
    NNST::Add (const Ptr<const NNNAddress> &name, Ptr<Face> face, Address poa, Time lease_expire, int32_t metric)
    {
      NS_LOG_FUNCTION (this->GetObject<Node> ()->GetId () << boost::cref(*name) << boost::cref(*face) << metric);

      // will add entry if doesn't exists, or just return an iterator to the existing entry
      bool p_entry = false;
      std::pair< super::iterator, bool > result = super::insert (*name, 0);
      if (result.first != super::end ())
	{
	  if (result.second)
	    {
	      Ptr<nnst::Entry> newEntry = Create<nnst::Entry> (this, name);

	      newEntry->AddPoA(face, poa, lease_expire, metric);
	      newEntry->SetTrie (result.first);
	      result.first->set_payload (newEntry);
	      p_entry = true;
	    }

	  super::modify (result.first,
	                 ll::bind (&nnst::Entry::AddOrUpdateRoutingMetric, ll::_1, face, metric));

	  if (result.second)
	    {
	      // notify forwarding strategy about new NNST entry
	      NS_ASSERT (this->GetObject<ForwardingStrategy> () != 0);
	      this->GetObject<ForwardingStrategy> ()->DidAddNNSTEntry (result.first->payload ());
	    }

	  // If this is a new entry, then the PoA has not been added
	  if (!p_entry)
	    {
	      result.first->payload()->AddPoA(face, poa, lease_expire, metric);
	    }

	  return result.first->payload ();
	}
      else
	return 0;
    }

    void
    NNST::InvalidateAll ()
    {
      NS_LOG_FUNCTION (this->GetObject<Node> ()->GetId ());

      super::parent_trie::recursive_iterator item (super::getTrie ());
      super::parent_trie::recursive_iterator end (0);
      for (; item != end; item++)
	{
	  if (item->payload () == 0) continue;

	  super::modify (&(*item),
	                 ll::bind (&nnst::Entry::Invalidate, ll::_1));
	}
    }

    void
    NNST::Remove (const Ptr<const NNNAddress> &name)
    {
      NS_LOG_FUNCTION (this->GetObject<Node> ()->GetId () << boost::cref(*name));

      super::iterator nnstEntry = super::find_exact (*name);
      if (nnstEntry != super::end ())
	{
	  // notify forwarding strategy about soon be removed FIB entry
	  NS_ASSERT (this->GetObject<ForwardingStrategy> () != 0);
	  this->GetObject<ForwardingStrategy> ()->WillRemoveNNSTEntry (nnstEntry->payload ());

	  super::erase (nnstEntry);
	}
    }

    void
    NNST::RemoveFromAll (Ptr<Face> face)
    {
      NS_LOG_FUNCTION (this);

      Ptr<nnst::Entry> entry = Begin ();
      while (entry != End ())
	{
	  entry->RemoveFace (face);
	  if (entry->m_faces.size () == 0)
	    {
	      Ptr<nnst::Entry> nextEntry = Next (entry);

	      // notify forwarding strategy about soon be removed FIB entry
	      NS_ASSERT (this->GetObject<ForwardingStrategy> () != 0);
	      this->GetObject<ForwardingStrategy> ()->WillRemoveNNSTEntry (entry);

	      super::erase (StaticCast<nnst::Entry> (entry)->to_iterator ());
	      entry = nextEntry;
	    }
	  else
	    {
	      entry = Next (entry);
	    }
	}
    }

    void
    NNST::RemoveFromAll (Address poa)
    {
      NS_LOG_FUNCTION (this);

      Ptr<nnst::Entry> entry = Begin ();
      while (entry != End ())
	{
	  entry->RemovePoA (poa);
	  if (entry->m_faces.size () == 0)
	    {
	      Ptr<nnst::Entry> nextEntry = Next (entry);

	      // notify forwarding strategy about soon be removed NNST entry
	      NS_ASSERT (this->GetObject<ForwardingStrategy> () != 0);
	      this->GetObject<ForwardingStrategy> ()->WillRemoveNNSTEntry (entry);

	      super::erase (entry->to_iterator ());
	      entry = nextEntry;
	    }
	  else
	    {
	      entry = Next (entry);
	    }
	}
    }

    void
    NNST::Print (std::ostream &os) const
    {
      // !!! unordered_set imposes "random" order of item in the same level !!!
      super::parent_trie::const_recursive_iterator item (super::getTrie ());
      super::parent_trie::const_recursive_iterator end (0);
      for (; item != end; item++)
	{
	  if (item->payload () == 0) continue;

	  os << item->payload ();
	}
    }

    void
    NNST::PrintByMetric () const
    {
      super::parent_trie::const_recursive_iterator item (super::getTrie ());
      super::parent_trie::const_recursive_iterator end (0);
      for (; item != end; item++)
	{
	  if (item->payload () == 0) continue;

	  item->payload ()->printByMetric ();
	}
    }

    void
    NNST::PrintByAddress () const
    {
      super::parent_trie::const_recursive_iterator item (super::getTrie ());
      super::parent_trie::const_recursive_iterator end (0);
      for (; item != end; item++)
	{
	  if (item->payload () == 0) continue;

	  item->payload ()->printByAddress ();
	}
    }

    void
    NNST::PrintByLease () const
    {
      super::parent_trie::const_recursive_iterator item (super::getTrie ());
      super::parent_trie::const_recursive_iterator end (0);
      for (; item != end; item++)
	{
	  if (item->payload () == 0) continue;

	  item->payload ()->printByLease();
	}
    }

    void
    NNST::PrintByFace () const
    {
      super::parent_trie::const_recursive_iterator item (super::getTrie ());
      super::parent_trie::const_recursive_iterator end (0);
      for (; item != end; item++)
	{
	  if (item->payload () == 0) continue;

	  item->payload ()->printByFace();
	}
    }

    uint32_t
    NNST::GetSize ()
    {
      return super::getPolicy ().size ();
    }

    Ptr<const nnst::Entry>
    NNST::Begin () const
    {
      super::parent_trie::const_recursive_iterator item (super::getTrie ());
      super::parent_trie::const_recursive_iterator end (0);
      for (; item != end; item++)
	{
	  if (item->payload () == 0) continue;
	  break;
	}

      if (item == end)
	return End ();
      else
	return item->payload ();
    }

    Ptr<nnst::Entry>
    NNST::Begin ()
    {
      super::parent_trie::recursive_iterator item (super::getTrie ());
      super::parent_trie::recursive_iterator end (0);
      for (; item != end; item++)
	{
	  if (item->payload () == 0) continue;
	  break;
	}

      if (item == end)
	return End ();
      else
	return item->payload ();
    }

    Ptr<const nnst::Entry>
    NNST::End () const
    {
      return 0;
    }

    Ptr<nnst::Entry>
    NNST::End ()
    {
      return 0;
    }

    Ptr<const nnst::Entry>
    NNST::Next (Ptr<const nnst::Entry> from) const
    {
      if (from == 0) return 0;

      super::parent_trie::const_recursive_iterator item (*StaticCast<const nnst::Entry> (from)->to_iterator ());
      super::parent_trie::const_recursive_iterator end (0);
      for (item++; item != end; item++)
	{
	  if (item->payload () == 0) continue;
	  break;
	}

      if (item == end)
	return End ();
      else
	return item->payload ();
    }

    Ptr<nnst::Entry>
    NNST::Next (Ptr<nnst::Entry> from)
    {
      if (from == 0) return 0;

      super::parent_trie::recursive_iterator item (*StaticCast<nnst::Entry> (from)->to_iterator ());
      super::parent_trie::recursive_iterator end (0);
      for (item++; item != end; item++)
	{
	  if (item->payload () == 0) continue;
	  break;
	}

      if (item == end)
	return End ();
      else
	return item->payload ();
    }

    Ptr<NNST>
    NNST::GetNNST (Ptr<Object> node)
    {
      return node->GetObject<NNST> ();
    }

    void
    NNST::NotifyNewAggregate ()
    {
      Object::NotifyNewAggregate ();
    }

    void
    NNST::DoDispose (void)
    {
      clear ();
      Object::DoDispose ();
    }

    void
    NNST::RemoveFace (super::parent_trie &item, Ptr<Face> face)
    {
      if (item.payload () == 0) return;
      NS_LOG_FUNCTION (this);

      super::modify (&item,
                     ll::bind (&nnst::Entry::RemoveFace, ll::_1, face));
    }

    void
    NNST::RemovePoA (super::parent_trie &item, Address poa)
    {
      if (item.payload () == 0) return;
      NS_LOG_FUNCTION (this);

      super::modify (&item,
                     ll::bind (&nnst::Entry::RemovePoA, ll::_1, poa));
    }

    std::ostream&
    operator<< (std::ostream& os, const NNST &nnst)
    {
      os << "Node " << Names::FindName (nnst.GetObject<Node>()) << std::endl;
      os << std::setw(30) << "3N Address"
	  << std::setw(29) << "PoA Address"
	  << std::setw(8) << "Lease"
	  << std::setw(8) << "Face"
	  << std::setw(4) << "Status"
	  << std::setw(5) << "Cost"
	  << std::setw(6) << "Metric";

      nnst.Print (os);
      return os;
    }

  } /* namespace nnn */
} /* namespace ns3 */
