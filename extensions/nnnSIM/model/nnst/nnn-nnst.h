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
#include <ns3-dev/ns3/node.h>
#include <ns3-dev/ns3/nstime.h>
#include <ns3-dev/ns3/object.h>

#include "../nnn-naming.h"
#include "../nnn-face.h"

namespace ns3 {
namespace nnn {
namespace nnst {

class i_face {};
class i_metric {};
class i_nth {};
class i_name{};

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
class NNST {
public:

	static TypeId GetTypeId ();

	NNST();

	~NNST();

	virtual Ptr<Entry>
	ClosestSector (const NNNAddress &interest);

	Ptr<Entry>
	SignatureMatch (const Mac48Address &poa);

	virtual Ptr<Entry>
	Find (const NNNAddress &prefix);

	virtual Ptr<Entry>
	Add (const NNNAddress &prefix, Ptr<Face> face, int32_t metric);

	virtual Ptr<Entry>
	Add (const Ptr<const NNNAddress> &prefix, Ptr<Face> face, int32_t metric);

	virtual void
	Remove (const Ptr<const NNNAddress> &prefix);

	virtual void
	InvalidateAll ();

	virtual void
	RemoveFromAll (Ptr<Face> face);

	virtual void
	Print (std::ostream &os) const;

	virtual uint32_t
	GetSize () const;

	virtual Ptr<const Entry>
	Begin () const;

	virtual Ptr<Entry>
	Begin ();

	virtual Ptr<const Entry>
	End () const;

	virtual Ptr<Entry>
	End ();

	virtual Ptr<const Entry>
	Next (Ptr<const Entry> item) const;

	virtual Ptr<Entry>
	Next (Ptr<Entry> item);

	virtual uint32_t
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
	RemoveFace (Ptr<Face> face);
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
