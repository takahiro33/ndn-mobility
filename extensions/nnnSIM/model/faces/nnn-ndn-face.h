/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright 2014 Waseda University, Sato Laboratory
 *   Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 *
 *  nnn-ndn-face.h is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  nnn-ndn-face.h is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero Public License for more details.
 *
 *  You should have received a copy of the GNU Affero Public License
 *  along with nnn-ndn-face.h.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NNN_NDN_FACE_H_
#define NNN_NDN_FACE_H_

#include <ns3-dev/ns3/callback.h>
#include <ns3-dev/ns3/node.h>
#include <ns3-dev/ns3/object.h>
#include <ns3-dev/ns3/ptr.h>
#include <ns3-dev/ns3/traced-callback.h>
#include <ns3-dev/ns3/type-id.h>

#include <ns3-dev/ns3/ndn-face.h>
#include <ns3-dev/ns3/ndn-interest.h>
#include <ns3-dev/ns3/ndn-data.h>

#include "../nnn-face.h"

namespace ns3
{
  namespace nnn
  {

    class NDNFace : public nnn::Face, public ndn::Face
    {
    public:
      static TypeId
      GetTypeId ();

      NDNFace (Ptr<Node> node);

      virtual
      ~NDNFace ();

      bool
      ReceiveNULLp (Ptr<NULLp> n_i);

      bool
      ReceiveSO (Ptr<SO> so_i);

      bool
      ReceiveDO (Ptr<DO>  do_i);

      bool
      ReceiveEN (Ptr<EN> en_i);

      bool
      ReceiveAEN (Ptr<AEN> aen_i);

      bool
      ReceiveREN (Ptr<REN> ren_i);

      bool
      ReceiveDEN (Ptr<DEN> den_i);

      bool
      ReceiveData (Ptr<ndn::Data> data);

      bool
      ReceiveInterest (Ptr<ndn::Interest> interest);

      bool
      SendNULLp (Ptr<const NULLp> n_o);

      bool
      SendSO (Ptr<const SO> so_o);

      bool
      SendDO (Ptr<const DO> do_o);

      bool
      SendEN (Ptr<const EN> en_o);

      bool
      SendAEN (Ptr<const AEN> aen_o);

      bool
      SendREN (Ptr<const REN> ren_o);

      bool
      SendDEN (Ptr<const DEN> den_o);

      bool
      SendINF (Ptr<const INF> inf_o);

      bool
      ReceiveINF (Ptr<INF> inf_i);

      bool
      SendData (Ptr<const ndn::Data> data);

      bool
      SendInterest (Ptr<const ndn::Interest> interest);

    protected:
      bool
      Receive (Ptr<Packet> packet);

      bool
      Send (Ptr<Packet> packet);

    };

  } /* namespace nnn */
} /* namespace ns3 */

#endif /* NNN_NDN_FACE_H_ */
