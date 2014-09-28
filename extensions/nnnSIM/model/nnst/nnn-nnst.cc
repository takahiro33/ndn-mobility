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

#include "nnn-nnst.h"

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

uint32_t
NNST::GetSize () const
{
	return m_nvp.size ();
}

Mac48Address
NNST::GetPoA (NNNAddress &name)
{

}

Face
NNST::GetFace (NNNAddress &name)
{
	std::pair<std::multimap<NNNAddress, Face>::iterator,
		std::multimap<NNNAddress,Face>::iterator> res;

	res = m_nvp.equal_range(name);

	if (res.first != res.second)
	{
		std::multimap<NNNAddress,Face>::iterator it = res.second;

		return (*it).second;
	} else
	{

	}
}

void
NNST::AddNNN (NNNAddress &name, Mac48Address &mac, Face face)
{
	m_nvp.insert(std::pair <NNNAddress, Mac48Address> (name, mac));
	m_nvf.insert(std::pair <NNNAddress, Face> (name, face));
}

void
NNST::RemoveNNN (NNNAddress &name)
{

}

} /* namespace nnst */
} /* namespace nnn */
} /* namespace ns3 */
