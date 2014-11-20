/* -*- Mode:C++; c-file-style:"gnu" -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnn-names-container.h is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnn-names-container.h is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnn-names-container.h.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NNN_NAMES_CONTAINER_H_
#define NNN_NAMES_CONTAINER_H_

#include <ns3-dev/ns3/ptr.h>
#include <ns3-dev/ns3/node.h>
#include <ns3-dev/ns3/object.h>
#include <ns3-dev/ns3/simple-ref-count.h>

#include "nnn-names-container-entry.h"
#include "../model/nnn-naming.h"

namespace ns3 {
namespace nnn {

struct address {};
struct lease {};
struct renew {};



class NamesContainer : public SimpleRefCount<NamesContainer>
{

public:
	/**
	 * \brief Interface ID
	 *
	 * \return interface ID
	 */
	static TypeId GetTypeId ();

	NamesContainer();

	virtual ~NamesContainer();

};

} /* namespace nnn */
} /* namespace ns3 */

#endif /* NNN_NAMES_CONTAINER_H_ */
