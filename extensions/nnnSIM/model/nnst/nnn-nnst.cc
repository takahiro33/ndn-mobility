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

NS_LOG_COMPONENT_DEFINE ("nnn.nnst.impl");

namespace ns3 {
namespace nnn {
namespace nnst {

TypeId
NNST::GetTypeId (void)
{
	static TypeId tid = TypeId ("ns3::nnn::nnst") // cheating ns3 object system
    		.SetParent<Object> ()
    		.SetGroupName ("nnn")
    		;
	return tid;
}

NNST::NNST() {
	// TODO Auto-generated constructor stub

}

NNST::~NNST() {
	// TODO Auto-generated destructor stub
}

void
NNST::NotifyNewAggregate ()
{
	Object::NotifyNewAggregate ();
}

void
NNST::DoDispose (void)
{
	Object::DoDispose ();
}

Ptr<Entry>
NNST::ClosestSector (const NNNAddress &nnnaddr)
{
	super::iterator item = super::longest_prefix_match (nnnaddr);
	// @todo use predicate to search with exclude filters

	if (item == super::end ())
		return 0;
	else
		return item->payload ();
}

Ptr<Entry>
NNST::Find (const NNNAddress &name)
{
	super::iterator item = super::find_exact (name);

	if (item == super::end ())
		return 0;
	else
		return item->payload ();
}


Ptr<Entry>
NNST::Add (const NNNAddress &name, Ptr<Face> face, int32_t metric)
{
	return Add (Create<NNNAddress> (name), face, metric);
}

Ptr<Entry>
NNST::Add (const Ptr<const NNNAddress> &name, Ptr<Face> face, int32_t metric)
{
	NS_LOG_FUNCTION (this->GetObject<Node> ()->GetId () << boost::cref(*name) << boost::cref(*face) << metric);

	// will add entry if doesn't exists, or just return an iterator to the existing entry
	std::pair< super::iterator, bool > result = super::insert (*name, 0);
	if (result.first != super::end ())
	{
		if (result.second)
		{
			Ptr<Entry> newEntry = Create<Entry> (this, name);
			newEntry->SetTrie (result.first);
			result.first->set_payload (newEntry);
		}

		super::modify (result.first,
				ll::bind (&Entry::AddOrUpdateRoutingMetric, ll::_1, face, metric));

		if (result.second)
		{
			// notify forwarding strategy about new NNST entry
			NS_ASSERT (this->GetObject<ForwardingStrategy> () != 0);
			this->GetObject<ForwardingStrategy> ()->DidAddNNSTEntry (result.first->payload ());
		}

		return result.first->payload ();
	}
	else
		return 0;
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
NNST::InvalidateAll ()
{
	NS_LOG_FUNCTION (this->GetObject<Node> ()->GetId ());

	super::parent_trie::recursive_iterator item (super::getTrie ());
	super::parent_trie::recursive_iterator end (0);
	for (; item != end; item++)
	{
		if (item->payload () == 0) continue;

		super::modify (&(*item),
				ll::bind (&Entry::Invalidate, ll::_1));
	}
}

void
NNST::RemoveFace (super::parent_trie &item, Ptr<Face> face)
{
	if (item.payload () == 0) return;
	NS_LOG_FUNCTION (this);

	super::modify (&item,
			ll::bind (&Entry::RemoveFace, ll::_1, face));
}

void
NNST::RemoveFromAll (Ptr<Face> face)
{
	NS_LOG_FUNCTION (this);

	Ptr<Entry> entry = Begin ();
	while (entry != End ())
	{
		entry->RemoveFace (face);
		if (entry->m_faces.size () == 0)
		{
			Ptr<Entry> nextEntry = Next (entry);

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

		os << item->payload ()->GetAddress() << "\t" << *item->payload () << "\n";
	}
}

uint32_t
NNST::GetSize () const
{
	return super::getPolicy ().size ();
}

Ptr<const Entry>
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

Ptr<const Entry>
NNST::End () const
{
	return 0;
}

Ptr<const Entry>
NNST::Next (Ptr<const Entry> from) const
{
	if (from == 0) return 0;

	super::parent_trie::const_recursive_iterator item (*StaticCast<const Entry> (from)->to_iterator ());
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

Ptr<Entry>
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

Ptr<Entry>
NNST::End ()
{
	return 0;
}

Ptr<Entry>
NNST::Next (Ptr<Entry> from)
{
	if (from == 0) return 0;

	super::parent_trie::recursive_iterator item (*StaticCast<Entry> (from)->to_iterator ());
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

} /* namespace nnst */
} /* namespace nnn */
} /* namespace ns3 */
