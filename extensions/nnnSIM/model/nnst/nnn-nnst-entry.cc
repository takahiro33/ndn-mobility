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

#define NDN_RTO_ALPHA 0.125
#define NDN_RTO_BETA 0.25
#define NDN_RTO_K 4

#include <ns3-dev/ns3/log.h>
#include <ns3-dev/ns3/node.h>
#include <ns3-dev/ns3/simulator.h>

#include "nnn-nnst-entry.h"
#include "nnn-nnst.h"

#include "../nnn-naming.h"

NS_LOG_COMPONENT_DEFINE ("nnn.nnst.Entry");

namespace ns3 {
namespace nnn {
namespace nnst {

struct FaceMetricByFace
{
	typedef FaceMetricContainer::type::index<i_face>::type
			type;
};


void
FaceMetric::UpdateRtt (const Time &rttSample)
{
	// const Time & this->m_rttSample

	//update srtt and rttvar (RFC 2988)
	if (m_sRtt.IsZero ())
	{
		//first RTT measurement
		NS_ASSERT_MSG (m_rttVar.IsZero (), "SRTT is zero, but variation is not");

		m_sRtt = rttSample;
		m_rttVar = Time (m_sRtt / 2.0);
	}
	else
	{
		m_rttVar = Time ((1 - NDN_RTO_BETA) * m_rttVar + 1.0 * NDN_RTO_BETA * Abs(m_sRtt - rttSample));
		m_sRtt = Time ((1 - NDN_RTO_ALPHA) * m_sRtt + 1.0 * NDN_RTO_ALPHA * rttSample);
	}
}

void
Entry::UpdateFaceRtt (Ptr<Face> face, const Time &sample)
{
	FaceMetricByFace::type::iterator record = m_faces.get<i_face> ().find (face);
	if (record == m_faces.get<i_face> ().end ())
	{
		return;
	}

	m_faces.modify (record,
			ll::bind (&FaceMetric::UpdateRtt, ll::_1, sample));

	// reordering random access index same way as by metric index
	m_faces.get<i_nth> ().rearrange (m_faces.get<i_metric> ().begin ());
}

void
Entry::Invalidate ()
{
  for (FaceMetricByFace::type::iterator face = m_faces.begin ();
       face != m_faces.end ();
       face++)
    {
      m_faces.modify (face,
                      ll::bind (&FaceMetric::SetRoutingCost, ll::_1, std::numeric_limits<uint16_t>::max ()));

      m_faces.modify (face,
                      ll::bind (&FaceMetric::SetStatus, ll::_1, FaceMetric::NNN_NNST_RED));
    }
}

Entry::Entry(Ptr<NNST> nnst, const Ptr<const NNNAddress> &name)
	: m_nnst        (nnst)
	, m_name        (name)
	, m_poas        (std::list<Ptr<const Mac48Address> > ())
{

}

Entry::~Entry() {
	// TODO Auto-generated destructor stub
}

Ptr<NNST>
Entry::GetNNST ()
{
	return m_nnst;
}



} /* namespace nnst */
} /* namespace nnn */
} /* namespace ns3 */
