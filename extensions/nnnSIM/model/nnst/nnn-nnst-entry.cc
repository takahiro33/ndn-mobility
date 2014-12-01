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
#include <iomanip>

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
#include "nnn-nnst-entry-facemetric.h"

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
      Entry::RemoveFace (const Ptr<Face> &face)
      {
	fmtr_set_by_face& face_index = m_faces.get<i_face> ();

	face_index.erase(face);
      }

      void
      Entry::AddPoA (Ptr<Face> face, Address poa, Time e_lease, uint32_t cost)
      {
	FaceMetric tmp (face, poa, e_lease, cost);

	m_faces.insert(tmp);

	Simulator::Schedule(e_lease, &Entry::cleanExpired, this);
      }

      std::vector<Address>
      Entry::GetPoAs()
      {
	fmtr_set_by_poa& poa_index = m_faces.get<i_poa> ();
	fmtr_set_by_poa::iterator it = poa_index.begin ();

	std::vector<Address> poas;

	while (it != poa_index.end ())
	  {
	    poas.push_back(it->GetAddress ());
	    ++it;
	  }

	return poas;
      }

      uint32_t
      Entry::GetPoAsN()
      {
	return GetPoAs().size();
      }

      void
      Entry::RemovePoA (Address poa)
      {
	fmtr_set_by_poa& poa_index = m_faces.get<i_poa> ();

	poa_index.erase(poa);
      }

      void
      Entry::cleanExpired()
      {
	fmtr_set_by_lease& lease_index = m_faces.get<i_lease> ();
	Time now = Simulator::Now();

	//std::cout << "Deleting expired entries at " << now << std::endl;

	fmtr_set_by_lease::iterator it = lease_index.begin();

	while (! (m_faces.size () == 0))
	  {
	    if (it->GetExpireTime() <= now)
	      {
		m_faces.erase(*it);
		break;
	      }

	    ++it;
	  }
      }

      void
      Entry::printByAddress ()
      {
	fmtr_set_by_poa& poa_index = m_faces.get<i_poa> ();
	fmtr_set_by_poa::iterator it = poa_index.begin ();

	std::cout << std::setw(32) << GetAddress();

	if (it != poa_index.end ())
	  {
	    std::cout << *it;
	    ++it;
	  }

	while (it != poa_index.end())
	  {
	    std::cout << std::setw(32) << *it;
	    ++it;
	  }
      }

      void
      Entry::printByLease ()
      {
	fmtr_set_by_lease& lease_index = m_faces.get<i_lease> ();
	fmtr_set_by_lease::iterator it = lease_index.begin ();

	std::cout << std::setw(32) << GetAddress();

	if (it != lease_index.end ())
	  {
	    std::cout << *it;
	    ++it;
	  }

	while (it != lease_index.end())
	  {
	    std::cout << std::setw(32) << *it;
	    ++it;
	  }
      }

      void
      Entry::printByFace ()
      {
	fmtr_set_by_face& face_index = m_faces.get<i_face> ();
	fmtr_set_by_face::iterator it = face_index.begin ();

	std::cout << std::setw(32) << GetAddress();

	if (it != face_index.end ())
	  {
	    std::cout << *it;
	    ++it;
	  }

	while (it != face_index.end())
	  {
	    std::cout << std::setw(32) << *it;
	    ++it;
	  }
      }

      std::ostream& operator<< (std::ostream& os, const Entry &entry)
      {
	fmtr_set tmp = entry.m_faces;
	fmtr_set_by_face& face_index = tmp.get<nnst::i_face> ();
	fmtr_set_by_face::iterator it = face_index.begin ();

	os  << std::setw(32) << entry.GetAddress();

	if (it != face_index.end ())
	  {
	    os << *it;
	    ++it;
	  }

	while (it != face_index.end())
	  {
	    os << std::setw(32) << *it;
	    ++it;
	  }

	return os;
      }


    } /* namespace nnst */
  } /* namespace nnn */
} /* namespace ns3 */
