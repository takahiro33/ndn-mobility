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



} /* namespace nnst */
} /* namespace nnn */
} /* namespace ns3 */
