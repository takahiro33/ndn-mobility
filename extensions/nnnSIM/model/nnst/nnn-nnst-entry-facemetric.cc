/* -*- Mode:C++; c-file-style:"gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnn-nnst-entry-facemetric.cc is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnn-nnst-entry-facemetric.cc is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnn-nnst-entry-facemetric.cc.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "nnn-nnst-entry-facemetric.h"

namespace ns3 {
namespace nnn {
namespace nnst {

FaceMetric::FaceMetric (Ptr<Face> face, Address addr, Time lease_expire, int32_t cost)
: m_face         (face)
, m_dst_addr     (addr)
, m_lease_expire (lease_expire)
, m_status       (NNN_NNST_GREEN)
, m_routingCost  (cost)
, m_sRtt         (Seconds (0))
, m_rttVar       (Seconds (0))
{
}

FaceMetric::~FaceMetric() {
	// TODO Auto-generated destructor stub
}

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
		m_rttVar = Time ((1 - NNN_RTO_BETA) * m_rttVar + 1.0 * NNN_RTO_BETA * Abs(m_sRtt - rttSample));
		m_sRtt = Time ((1 - NNN_RTO_ALPHA) * m_sRtt + 1.0 * NNN_RTO_ALPHA * rttSample);
	}
}

std::ostream& operator<< (std::ostream& os, const FaceMetric &metric)
{
//	static const std::string statusString[] = {"","g","y","r"};
//
//	os << *metric.m_face << "(" << metric.m_routingCost << ","<< statusString [metric.m_status] << "," << metric.m_face->GetMetric () << ")";
	return os;
}

} /* namespace nnst */
} /* namespace nnn */
} /* namespace ns3 */
