/* -*- Mode:C++; c-file-style:"gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnn-nnst.h is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnn-nnst.h is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnn-nnst.h.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef NNN_NNST_H_
#define NNN_NNST_H_

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>

using namespace ::boost;
using namespace ::boost::multi_index;

#include <ns3-dev/ns3/event-id.h>
#include <ns3-dev/ns3/log.h>
#include <ns3-dev/ns3/node.h>
#include <ns3-dev/ns3/nstime.h>
#include <ns3-dev/ns3/object.h>

#include "nnn-nnst-entry.h"
#include "../nnn-naming.h"
#include "../nnn-face.h"
#include "../../utils/trie/trie.h"
#include "../../utils/trie/counting-policy.h"

namespace ns3 {
namespace nnn {
namespace nnst {

class L3Protocol;

//class NULLp;
//typedef NULLp NULLpHeader;
//class SO;
//typedef SO SOHeader;
//class DO;
//typedef DO DOHeader;
//class EN;
//typedef EN ENHeader;
//class AEN;
//typedef AEN AENHeader;
//class REN;
//typedef REN RENHeader;
//class INF;
//typedef INF INFHeader;

/**
 * @ingroup nnn
 * @defgroup nnn NNST
 */
class NNST : public Object,
				protected nnnSIM::trie_with_policy<NNNAddress,
								nnnSIM::smart_pointer_payload_traits<Entry>,
								nnnSIM::counting_policy_traits>
{
public:

	typedef nnnSIM::trie_with_policy<NNNAddress,
			nnnSIM::smart_pointer_payload_traits<Entry>,
			nnnSIM::counting_policy_traits > super;

	static TypeId GetTypeId ();

	NNST();

	~NNST();

	Ptr<Entry>
	ClosestSector (const NNNAddress &nnnaddr);

	Ptr<Entry>
	SignatureMatch (const Address &poa);

	Ptr<Entry>
	Find (const NNNAddress &prefix);

	Ptr<Entry>
	Add (const NNNAddress &prefix, Ptr<Face> face, int32_t metric);

	Ptr<Entry>
	Add (const Ptr<const NNNAddress> &prefix, Ptr<Face> face, int32_t metric);

	void
	Remove (const Ptr<const NNNAddress> &prefix);

	void
	InvalidateAll ();

	void
	RemoveFace ();

	void
	RemoveFromAll (Ptr<Face> face);

	void
	Print (std::ostream &os) const;

	uint32_t
	GetSize () const;

	virtual Ptr<const Entry>
	Begin () const;

	Ptr<Entry>
	Begin ();

	Ptr<const Entry>
	End () const;

	Ptr<Entry>
	End ();

	Ptr<const Entry>
	Next (Ptr<const Entry> item) const;

	Ptr<Entry>
	Next (Ptr<Entry> item);

	uint32_t
	GetSize () const = 0;

	Ptr<NNST>
	GetNNST (Ptr<Object> node);

protected:
	// inherited from Object class
	virtual void NotifyNewAggregate (); ///< @brief Notify when object is aggregated
	virtual void DoDispose (); ///< @brief Perform cleanup

private:
	/**
	 * @brief Remove reference to a face from the entry. If entry had only this face, the whole
	 * entry will be removed
	 */
	void
	RemoveFace (super::parent_trie &item, Ptr<Face> face);
};

Ptr<NNST>
NNST::GetNNST (Ptr<Object> node)
{
	return node->GetObject<NNST> ();
}

} /* namespace nnst */
} /* namespace nnn */
} /* namespace ns3 */

#endif /* NNN_NNST_H_ */
