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

#include <ns3-dev/ns3/event-id.h>
#include <ns3-dev/ns3/node.h>
#include <ns3-dev/ns3/nstime.h>
#include <ns3-dev/ns3/object.h>

namespace ns3 {
namespace nnn {

/**
 * @ingroup nnn
 * @defgroup nnn NNST
 */



class NNST {
public:
	NNST();
	virtual ~NNST();
};

} /* namespace nnn */
} /* namespace ns3 */

#endif /* NNN_NNST_H_ */
