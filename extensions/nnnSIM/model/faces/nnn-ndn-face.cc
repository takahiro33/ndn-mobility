/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnn-ndn-face.cc is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnn-ndn-face.cc is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnn-ndn-face.cc.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <ns3-dev/ns3/log.h>
#include <ns3-dev/ns3/uinteger.h>

#include "nnn-ndn-face.h"

NS_LOG_COMPONENT_DEFINE ("nnn.ndnFace");

namespace ns3
{
  namespace nnn
  {
    NS_OBJECT_ENSURE_REGISTERED (NDNFace);

    TypeId
    NDNFace::GetTypeId ()
    {
      static TypeId tid = TypeId ("ns3::nnn::ndnFace")
            .SetParent<Face> ()
            .SetGroupName ("Nnn")
            ;
      return tid;
    }

    NDNFace::NDNFace (Ptr<Node> node)
    : Face (node)
    {
    	NS_LOG_FUNCTION (this << node);

    	NS_ASSERT_MSG (node != 0, "node cannot be NULL. Check the code");

    	m_ndn_face = CreateObject<ndn::Face> (node);
    }

    NDNFace::~NDNFace ()
    {
    }

  } /* namespace nnn */
} /* namespace ns3 */
