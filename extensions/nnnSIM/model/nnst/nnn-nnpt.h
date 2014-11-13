/* -*- Mode:C++; c-file-style:"gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnn-nnpt.h is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnn-nnpt.h is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnn-nnpt.h.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef NNN_NNPT_H_
#define NNN_NNPT_H_

#include <ns3-dev/ns3/simple-ref-count.h>
#include <ns3-dev/ns3/node.h>

#include <ostream>

namespace ns3 {
namespace nnn {

class NNPT {
public:
	static TypeId GetTypeId ();

	NNPT();

	virtual ~NNPT();

	virtual void
	AddPair();

	virtual void
	DeletePair();

	virtual void
	SearchOldName();

	virtual void
	SearchNewName();

private:

protected:

};

std::ostream& operator<< (std::ostream& os, const NNPT &nnpt);

} /* namespace nnn */
} /* namespace ns3 */

#endif /* NNN_NNPT_H_ */
