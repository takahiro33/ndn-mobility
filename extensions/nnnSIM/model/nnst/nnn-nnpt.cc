/* -*- Mode:C++; c-file-style:"gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnn-nnpt.cc is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnn-nnpt.cc is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnn-nnpt.cc.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "nnn-nnpt.h"

namespace ns3 {
namespace nnn {

TypeId
NNPT::GetTypeId (void)
{
	static TypeId tid = TypeId ("ns3::nnn::NNPT") // cheating ns3 object system
    		.SetParent<Object> ()
    		.SetGroupName ("nnn")
    		;
	return tid;
}

NNPT::NNPT() {
	// TODO Auto-generated constructor stub

}

NNPT::~NNPT() {
	// TODO Auto-generated destructor stub
}

std::ostream&
operator<< (std::ostream& os, const NNPT &nnpt)
{
  return os;
}

} /* namespace nnn */
} /* namespace ns3 */
