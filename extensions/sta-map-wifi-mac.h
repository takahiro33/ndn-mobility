/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2006, 2009 INRIA
 * Copyright (c) 2009 MIRKO BANCHI
 * Copyright (c) 2014 JAIRO EDUARDO LOPEZ
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 * Author: Mirko Banchi <mk.banchi@gmail.com>
 *
 * Modified NS-3 sta-wifi-mac to be able to choose AP according to other data.
 * Author: Jairo Eduardo Lopez <jairo@ruri.waseda.jp>
 */
#ifndef STA_MAP_WIFI_MAC_H
#define STA_MAP_WIFI_MAC_H

#include <map>
#include <string>

#include <ns3-dev/ns3/regular-wifi-mac.h>

#include <ns3-dev/ns3/event-id.h>
#include <ns3-dev/ns3/packet.h>
#include <ns3-dev/ns3/traced-callback.h>

#include <ns3-dev/ns3/supported-rates.h>
#include <ns3-dev/ns3/amsdu-subframe-header.h>

#include <ns3-dev/ns3/mobility-module.h>

namespace ns3  {

class MgtAddBaRequestHeader;

/**
 * \ingroup wifi
 *
 * The Wifi MAC high model for a non-AP STA in a BSS.
 */
class StaMApWifiMac : public RegularWifiMac
{
public:
  static TypeId GetTypeId (void);

  StaMApWifiMac ();
  virtual ~StaMApWifiMac ();

  /**
   * \param packet the packet to send.
   * \param to the address to which the packet should be sent.
   *
   * The packet should be enqueued in a tx queue, and should be
   * dequeued as soon as the channel access function determines that
   * access is granted to this MAC.
   */
  virtual void Enqueue (Ptr<const Packet> packet, Mac48Address to);

  /**
   * \param missed the number of beacons which must be missed
   * before a new association sequence is started.
   */
  void SetMaxMissedBeacons (uint32_t missed);
  /**
   * \param timeout
   *
   * If no probe response is received within the specified
   * timeout, the station sends a new probe request.
   */
  void SetProbeRequestTimeout (Time timeout);
  /**
   * \param timeout
   *
   * If no association response is received within the specified
   * timeout, the station sends a new association request.
   */
  void SetAssocRequestTimeout (Time timeout);

  /**
   * Start an active association sequence immediately.
   */
  void StartActiveAssociation (void);

  /**
   * Obtain a list of all the SSIDs seen by the MAC interface
   */
  std::map<std::string, Ssid> GetSSIDList (void);

  /**
   * Choose SSID depending on how close the node is to an AP
   */
  void SetSSIDviaDistance(Ptr<MobilityModel> node, std::map<std::string, Ptr<MobilityModel> > aps);

private:
  enum MacState
  {
    ASSOCIATED,
    WAIT_PROBE_RESP,
    WAIT_ASSOC_RESP,
    BEACON_MISSED,
    REFUSED
  };

  void SetActiveProbing (bool enable);
  bool GetActiveProbing (void) const;
  virtual void Receive (Ptr<Packet> packet, const WifiMacHeader *hdr);
  void SendProbeRequest (void);
  void SendAssociationRequest (void);
  void TryToEnsureAssociated (void);
  void AssocRequestTimeout (void);
  void ProbeRequestTimeout (void);
  bool IsAssociated (void) const;
  bool IsWaitAssocResp (void) const;
  void MissedBeacons (void);
  void RestartBeaconWatchdog (Time delay);
  SupportedRates GetSupportedRates (void) const;
  void SetState (enum MacState value);

  HtCapabilities GetHtCapabilities (void) const;


  enum MacState m_state;
  Time m_probeRequestTimeout;
  Time m_assocRequestTimeout;
  EventId m_probeRequestEvent;
  EventId m_assocRequestEvent;
  EventId m_beaconWatchdog;
  Time m_beaconWatchdogEnd;
  uint32_t m_maxMissedBeacons;

  // Vectors to hold information about SSIDs obtained
  std::map<std::string, Ssid> m_seenSSID;

  TracedCallback<Mac48Address> m_assocLogger;
  TracedCallback<Mac48Address> m_deAssocLogger;
};

} // namespace ns3

#endif /* STA_MAP_WIFI_MAC_H */