/* -*- Mode:C++; c-file-style:"gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnn-nnst-entry.h is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnn-nnst-entry.h is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnn-nnst-entry.h.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef NNN_NNST_ENTRY_H_
#define NNN_NNST_ENTRY_H_

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/tag.hpp>

#include <ns3-dev/ns3/ptr.h>
#include <ns3-dev/ns3/mac48-address.h>
#include <ns3-dev/ns3/nstime.h>
#include <ns3-dev/ns3/traced-value.h>

#include "../nnn-naming.h"
#include "../nnn-face.h"
#include "../../helper/nnn-face-container.h"

using namespace ::boost;
using namespace ::boost::multi_index;

template<class KeyExtractor1,class KeyExtractor2>
struct key_from_key
{
public:
	typedef typename KeyExtractor1::result_type result_type;

	key_from_key(
			const KeyExtractor1& key1_=KeyExtractor1(),
			const KeyExtractor2& key2_=KeyExtractor2()):
				key1(key1_),key2(key2_)
	{}

	template<typename Arg>
	result_type operator()(Arg& arg)const
	{
		return key1(key2(arg));
	}

private:
	KeyExtractor1 key1;
	KeyExtractor2 key2;
};

namespace ns3 {
namespace nnn {

class NNNAddress;
class NNST;

namespace nnst {

class FaceMetric
{
public:

	enum Status { NNN_NNST_GREEN = 1,
		NNN_NNST_YELLOW = 2,
		NNN_NNST_RED = 3
	};

	FaceMetric (Ptr<Face> face)
	: m_face   (face)
	, m_status (NNN_NNST_GREEN)
	, m_sRtt   (Seconds (0))
	, m_rttVar (Seconds (0))
	, m_routingCost (0)
	{

	}

	/**
	 * \brief Comparison operator used by boost::multi_index::identity<>
	 */
	bool
	operator< (const FaceMetric &fm) const { return *m_face < *fm.m_face; } // return identity of the face

	/**
	 * @brief Comparison between FaceMetric and Face
	 */
	bool
	operator< (const Ptr<Face> &face) const { return *m_face < *face; }

	/**
	 * @brief Return Face associated with FaceMetric
	 */
	Ptr<Face>
	GetFace () const { return m_face; }

	void
	SetStatus (Status status)
	{
		m_status.Set (status);
	}

	Status
	GetStatus () const
	{
		return m_status;
	}

	/**
	 * @brief Get current routing cost
	 */
	int32_t
	GetRoutingCost () const
	{
		return m_routingCost;
	}

	/**
	 * @brief Set routing cost
	 */
	void
	SetRoutingCost (int32_t routingCost)
	{
		m_routingCost = routingCost;
	}

	/**
	 * @brief Get current estimate for smoothed RTT value
	 */
	Time
	GetSRtt () const
	{
		return m_sRtt;
	}

	/**
	 * @brief Get current estimate for the RTT variation
	 */
	Time
	GetRttVar () const
	{
		return m_rttVar;
	}

	/**
	 * \brief Recalculate smoothed RTT and RTT variation
	 * \param rttSample RTT sample
	 */
	void
	UpdateRtt (const Time &rttSample);

private:
	Ptr<Face> m_face; ///< Face
	TracedValue<Status> m_status; ///< \brief Status of next hop
	Time m_sRtt;                   ///< \brief Round-trip time variation
	Time m_rttVar;       ///< \brief round-trip time variation
	int32_t m_routingCost; ///< \brief routing protocol cost (interpretation of the value depends on the underlying routing protocol)
};

/// @cond include_hidden
class i_face {};
class i_metric {};
class i_nth {};
/// @endcond

/**
 * @ingroup ndn-fib
 * @brief Typedef for indexed face container of Entry
 *
 * Currently, there are 2 indexes:
 * - by face (used to find record and update metric)
 * - by sector order
 */
struct FaceMetricContainer
{
	/// @cond include_hidden
	typedef multi_index_container<
			FaceMetric,
			indexed_by<
				// For fast access to elements using Face
				ordered_unique<
				tag<i_face>,
				const_mem_fun<FaceMetric,Ptr<Face>,&FaceMetric::GetFace>
			>,

			// List of available faces ordered by (status, m_routingCost)
			ordered_non_unique<
				tag<i_metric>,
				composite_key<
					FaceMetric,
					const_mem_fun<FaceMetric,FaceMetric::Status,&FaceMetric::GetStatus>,
					const_mem_fun<FaceMetric,int32_t,&FaceMetric::GetRoutingCost>
				>
			>,

			// To optimize nth candidate selection (sacrifice a little bit space to gain speed)
			random_access<
				tag<i_nth>
			>
		>
	> type;
	/// @endcond
};


class Entry : public SimpleRefCount<Entry>
{
public:
	Entry();

	Entry(Ptr<NNST> nnst, const Ptr<const Address> &name)
	  : m_nnst        (nnst)
	  , m_address     (name)
	{
	}

	virtual ~Entry();

	Ptr<NNST>
	GetNNST ()
	{
		return m_nnst;
	}

	Ptr<Address>
	GetAddress ()
	{
		return m_address;
	}

private:
	Ptr<NNST> m_nnst;             ///< \brief NNST to which entry is added
	Ptr<Address> m_address;       ///< \brief Address used for the NNST Entry
};

class PoAEntry : public Entry
{
};

class NNNEntry : public Entry
{
	class NoFaces {};

	void
	UpdateStatus (Ptr<Face> face, FaceMetric::Status status);

	void
	Invalidate ();

	void
	UpdateFaceRtt (Ptr<Face> face, const Time &sample);

	const FaceMetric &
	FindBestCandidate (uint32_t skip = 0) const;

	void
	RemoveFace (const Ptr<Face> &face)
	{
		m_faces.erase (face);
	}

	void
	AddPoa (Address address);

	std::vector<Ptr<PoAEntry> >
	GetPoas ()
	{
		return m_poa_addrs;
	}

	size_t
	GetPoasN ()
	{
		return m_poa_addrs.size ();
	}

private:
	std::vector<Ptr<PoAEntry> > m_poa_addrs;
	FaceMetricContainer::type m_faces;
};



} /* namespace nnst */
} /* namespace nnn */
} /* namespace ns3 */

#endif /* NNN_NNST_ENTRY_H_ */
