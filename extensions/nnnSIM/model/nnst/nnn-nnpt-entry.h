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

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/tag.hpp>

#include <ns3-dev/ns3/ptr.h>
#include <ns3-dev/ns3/simple-ref-count.h>
#include <ns3-dev/ns3/nstime.h>
#include <ns3-dev/ns3/traced-value.h>

#include "nnn-nnpt.h"
#include "../nnn-naming.h"
#include "../nnn-face.h"
#include "../../utils/trie/trie.h"
#include "../../utils/trie/counting-policy.h"
#include "../../utils/trie/trie-with-policy.h"
#include "../../helper/nnn-face-container.h"

namespace ns3 {
namespace nnn {

//class NNPT;


//namespace nnpt {

/**
 * @ingroup ndn-pit
 * @brief structure for PIT entry
 *
 * All set-methods are virtual, in case index rearrangement is necessary in the derived classes
 */
class NNPTEntry :  public SimpleRefCount<NNPTEntry>
{
public:

//  typedef std::set< IncomingFace > in_container; ///< @brief incoming faces container type
//  typedef in_container::iterator in_iterator;                ///< @brief iterator to incoming faces

  typedef std::set< uint32_t > nonce_container;  ///< @brief nonce container type

  /**
   * \brief PIT entry constructor
   * \param prefix Prefix of the PIT entry
   * \param offsetTime Relative time to the current moment, representing PIT entry lifetime
   * \param fibEntry A FIB entry associated with the PIT entry
   */
  NNPTEntry ();

  NNPTEntry (NNNAddress oldName, NNNAddress newName, Time lease_expire);

  NNPTEntry (NNNAddress oldName, NNNAddress newName, Time lease_expire, Time renew);

  /**
   * @brief Virtual destructor
   */
  virtual ~NNPTEntry ();

  /**
   * @brief Update lifetime of PIT entry
   *
   * @param lifetime desired lifetime of the pit entry (relative to the Simulator::Now ())
   *
   * This function will update PIT entry lifetime to the maximum of the current lifetime and
   * the lifetime Simulator::Now () + lifetime
   */
  virtual void
  UpdateLifetime (const Time &lifetime);

  /**
   * @brief Offset the currently set PIT lifetime (allowed both negative and positive offsets)
   *
   * @param offsetTime positive or negative offset for the PIT lifetime.
   *
   * If PIT expire time becomes less than Simulator::Now, then it is adjusted to Simulator::Now.
   */
  virtual void
  OffsetLifetime (const Time &offsetTime);

  /**
   * @brief Get current expiration time of the record
   *
   * @returns current expiration time of the record
   */
  const Time &
  GetExpireTime () const;

  /**
   * @brief Check if nonce `nonce` for the same prefix has already been seen
   *
   * @param nonce Nonce to check
   */
  bool
  IsNonceSeen (uint32_t nonce) const;

  /**
   * @brief Add `nonce` to the list of seen nonces
   *
   * @param nonce nonce to add to the list of seen nonces
   *
   * All nonces are stored for the lifetime of the PIT entry
   */
  virtual void
  AddSeenNonce (uint32_t nonce);

  /**
   * @brief Add `face` to the list of incoming faces
   *
   * @param face Face to add to the list of incoming faces
   * @returns iterator to the added entry
   */
//  virtual in_iterator
//  AddIncoming (Ptr<Face> face);

  /**
   * @brief Remove incoming entry for face `face`
   */
  virtual void
  RemoveIncoming (Ptr<Face> face);

  /**
   * @brief Clear all incoming faces either after all of them were satisfied or NACKed
   */
  virtual void
  ClearIncoming ();

  /**
   * @brief Remove all references to face.
   *
   * This method should be called before face is completely removed from the stack.
   * Face is removed from the lists of incoming and outgoing faces
   */
  virtual void
  RemoveAllReferencesToFace (Ptr<Face> face);

  /**
   * @brief Flag outgoing face as hopeless
   */
  // virtual void
  // SetWaitingInVain (out_iterator face);
  virtual void
  SetWaitingInVain (Ptr<Face> face);

  /**
   * @brief Get associated FIB entry
   */
//  Ptr<fib::Entry>
//  GetFibEntry ();

  /**
   * @brief Get associated list (const reference) of incoming faces
   */
//  const in_container &
//  GetIncoming () const;


  /**
   * @brief Get number of outgoing faces (needed for python bindings)
   */
  uint32_t GetOutgoingCount () const;

  bool operator< (const NamesContainerEntry e) const { return m_lease_expire < e.m_lease_expire; }

  NNNAddress m_name;
  Time m_lease_expire;
  Time m_renew;

  /**
   * @brief Get Interest (if several interests are received, then nonce is from the first Interest)
   */
//  Ptr<const Interest>
//  GetInterest () const;

private:
  friend std::ostream& operator<< (std::ostream& os, const Entry &entry);
/*
protected:
  Pit &m_container; ///< @brief Reference to the container (to rearrange indexes, if necessary)

  Ptr<const Interest> m_interest; ///< \brief Interest of the PIT entry (if several interests are received, then nonce is from the first Interest)
  Ptr<fib::Entry> m_fibEntry;     ///< \brief FIB entry related to this prefix

  nonce_container m_seenNonces;  ///< \brief map of nonces that were seen for this prefix
  in_container  m_incoming;      ///< \brief container for incoming interests
  out_container m_outgoing;      ///< \brief container for outgoing interests

  Time m_expireTime;         ///< \brief Time when PIT entry will be removed

  Time m_lastRetransmission; ///< @brief Last time when number of retransmissions were increased
  uint32_t m_maxRetxCount;   ///< @brief Maximum allowed number of retransmissions via outgoing faces

  std::list< boost::shared_ptr<fw::Tag> > m_fwTags; ///< @brief Forwarding strategy tags
*/

};

std::ostream& operator<< (std::ostream& os, const NNPTEntry &entry);

//} /* namespace nnpt */
} /* namespace nnn */
} /* namespace ns3 */

#endif /* NNN_NNPT_ENTRY_H_ */
