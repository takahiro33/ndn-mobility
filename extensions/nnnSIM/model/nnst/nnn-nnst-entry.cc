/* -*- Mode:C++; c-file-style:"gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnn-nnst-entry.cc is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnn-nnst-entry.cc is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnn-nnst-entry.cc.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <boost/ref.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
namespace ll = boost::lambda;

#define NNN_RTO_ALPHA 0.125
#define NNN_RTO_BETA 0.25
#define NNN_RTO_K 4

#include <ns3-dev/ns3/log.h>
#include <ns3-dev/ns3/node.h>
#include <ns3-dev/ns3/simulator.h>

#include "nnn-nnst-entry.h"

#include "../nnn-naming.h"

NS_LOG_COMPONENT_DEFINE ("nnn.nnst.Entry");

namespace ns3 {
namespace nnn {
namespace nnst {

///////////////////////////////////////////////////////////////////////////////
Entry::Entry()
{

}

Entry::Entry(Ptr<NNST> nnst, Ptr<const NNNAddress> &name)
  : m_nnst        (nnst)
  , m_address     (name)
  , item_         (0)
{
}

Entry::~Entry()
{

}

void
Entry::SetTrie (trie::iterator item)
{
	item_ = item;
}

void
Entry::UpdateStatus (Ptr<Face> face, FaceMetric::Status status)
{
//	NS_LOG_FUNCTION (this << boost::cref(*face) << status);
//
//	FaceMetricByFace::type::iterator record = m_faces.get<i_face> ().find (face);
//	if (record == m_faces.get<i_face> ().end ())
//	{
//		return;
//	}
//
//	m_faces.modify (record,
//			ll::bind (&FaceMetric::SetStatus, ll::_1, status));
//
//	// reordering random access index same way as by metric index
//	m_faces.get<i_nth> ().rearrange (m_faces.get<i_metric> ().begin ());
}

void
Entry::UpdateStatus (Ptr<Face> face, Address poa, FaceMetric::Status status)
{

}

void
Entry::AddOrUpdateRoutingMetric (Ptr<Face> face, int32_t metric)
{
//	NS_LOG_FUNCTION (this);
//	NS_ASSERT_MSG (face != NULL, "Trying to Add or Update NULL face");
//
//	FaceMetricByFace::type::iterator record = m_faces.get<i_face> ().find (face);
//	if (record == m_faces.get<i_face> ().end ())
//	{
//		// Need to update insert
//		m_faces.insert (FaceMetric (face, metric));
//	}
//	else
//	{
//		// don't update metric to higher value
//		if (record->GetRoutingCost () > metric || record->GetStatus () == FaceMetric::NNN_NNST_RED)
//		{
//			m_faces.modify (record,
//					ll::bind (&FaceMetric::SetRoutingCost, ll::_1, metric));
//
//			m_faces.modify (record,
//					ll::bind (&FaceMetric::SetStatus, ll::_1, FaceMetric::NNN_NNST_YELLOW));
//		}
//	}
//
//	// reordering random access index same way as by metric index
//	m_faces.get<i_nth> ().rearrange (m_faces.get<i_metric> ().begin ());
}

void
Entry::AddOrUpdateRoutingMetric (Ptr<Face> face, Address poa, int32_t metric)
{
}

void
Entry::Invalidate ()
{
//	for (FaceMetricByFace::type::iterator face = m_faces.begin ();
//			face != m_faces.end ();
//			face++)
//	{
//		m_faces.modify (face,
//				ll::bind (&FaceMetric::SetRoutingCost, ll::_1, std::numeric_limits<uint16_t>::max ()));
//
//		m_faces.modify (face,
//				ll::bind (&FaceMetric::SetStatus, ll::_1, FaceMetric::NNN_NNST_RED));
//	}
}

void
Entry::UpdateFaceRtt (Ptr<Face> face, const Time &sample)
{
//	FaceMetricByFace::type::iterator record = m_faces.get<i_face> ().find (face);
//	if (record == m_faces.get<i_face> ().end ())
//	{
//		return;
//	}
//
//	m_faces.modify (record,
//			ll::bind (&FaceMetric::UpdateRtt, ll::_1, sample));
//
//	// reordering random access index same way as by metric index
//	m_faces.get<i_nth> ().rearrange (m_faces.get<i_metric> ().begin ());
}

const FaceMetric &
Entry::FindBestCandidate (uint32_t skip/* = 0*/) const
{
	if (m_faces.size () == 0) throw Entry::NoFaces ();
	skip = skip % m_faces.size();
	return m_faces.get<i_nth> () [skip];
}

void
Entry::AddPoA (Ptr<Face> face, Address poa)
{
}

std::vector<Address>
Entry::GetPoAs()
{

}

uint32_t
Entry::GetPoAsN()
{
	return GetPoAs().size();
}

void
Entry::RemovePoA (Address poa)
{

}

void
Entry::printByAddress ()
{

}

void
Entry::printByLease ()
{

}

void
Entry::printByFace ()
{

}

std::ostream& operator<< (std::ostream& os, const Entry &entry)
{
//	for (FaceMetricContainer::type::index<i_nth>::type::iterator metric =
//			entry.m_faces.get<i_nth> ().begin ();
//			metric != entry.m_faces.get<i_nth> ().end ();
//			metric++)
//	{
//		if (metric != entry.m_faces.get<i_nth> ().begin ())
//			os << ", ";
//
//		os << *metric;
//	}
	return os;
}


} /* namespace nnst */
} /* namespace nnn */
} /* namespace ns3 */
