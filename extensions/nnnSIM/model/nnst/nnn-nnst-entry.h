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
#include <boost/multi_index/tag.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <ns3-dev/ns3/ptr.h>
#include <ns3-dev/ns3/mac48-address.h>
#include <ns3-dev/ns3/nstime.h>
#include <ns3-dev/ns3/traced-value.h>

#include "../nnn-naming.h"
#include "../nnn-face.h"
#include "../../helper/nnn-face-container.h"

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
	 * @brief Invalidate face
	 *
	 * Set routing metric on all faces to max and status to RED
	 */
	void
	Invalidate ();

	/**
	 * @brief Update RTT averages for the face
	 */
	void
	UpdateFaceRtt (Ptr<Face> face, const Time &sample);

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
  typedef boost::multi_index::multi_index_container<
    FaceMetric,
    boost::multi_index::indexed_by<
      // For fast access to elements using Face
      boost::multi_index::ordered_unique<
        boost::multi_index::tag<i_face>,
        boost::multi_index::const_mem_fun<FaceMetric,Ptr<Face>,&FaceMetric::GetFace>
      >,

      // List of available faces ordered by (status, m_routingCost)
      boost::multi_index::ordered_non_unique<
        boost::multi_index::tag<i_metric>,
        boost::multi_index::composite_key<
          FaceMetric,
          boost::multi_index::const_mem_fun<FaceMetric,FaceMetric::Status,&FaceMetric::GetStatus>,
          boost::multi_index::const_mem_fun<FaceMetric,int32_t,&FaceMetric::GetRoutingCost>
        >
      >,

      // To optimize nth candidate selection (sacrifice a little bit space to gain speed)
      boost::multi_index::random_access<
        boost::multi_index::tag<i_nth>
      >
    >
   > type;
  /// @endcond
};

class Entry : public SimpleRefCount<Entry>
{
public:
	Entry();

	Entry(Ptr<NNST> nnst, const Ptr<const NNNAddress> &name);

	virtual ~Entry();

	uint32_t
	GetSize () const;

	void
	Invalidate ();

	Ptr<Entry>
	SignatureMatch (const Mac48Address &poa);

	Ptr<Entry>
	NNMatch (const NNNAddress &name);

	const NNNAddress&
	GetSector () const { return m_name; }

	/**
	 * @brief Update RTT averages for the face
	 */
	void
	UpdateFaceRtt (Ptr<Face> face, const Time &sample);

	void
	RemoveFace (const Ptr<Face> &face)
	{
		m_faces.erase (face);
	}

	Ptr<NNST>
	GetNNST ();

public:

	Ptr<NNST> m_nnst;             ///< \brief NNST to which entry is added
	Ptr<const NNNAddress> m_name; ///< \brief Name recorded in NNST
	std::list<Ptr<const Mac48Address> > m_poas; ///< \brief List of PoA Addresses linked to a Name
	FaceMetricContainer::type m_faces;

};

} /* namespace nnst */
} /* namespace nnn */
} /* namespace ns3 */

#endif /* NNN_NNST_ENTRY_H_ */
