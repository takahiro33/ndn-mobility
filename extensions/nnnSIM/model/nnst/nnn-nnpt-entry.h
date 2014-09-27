/* -*- Mode:C++; c-file-style:"gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnn-nnpt-entry.h is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnn-nnpt-entry.h is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnn-nnst-entpy.h.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef NNN_NNPT_ENTRY_H_
#define NNN_NNPT_ENTRY_H_

namespace ns3 {
namespace nnn {

class NNPT;

namespace nnpt {

class Entry {
public:
	Entry();
	virtual ~Entry();
};

} /* namespace nnpt */
} /* namespace nnn */
} /* namespace ns3 */

#endif /* NNN_NNPT_ENTRY_H_ */
