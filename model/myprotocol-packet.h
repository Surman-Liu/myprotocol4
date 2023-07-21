/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2010 Hemanth Narra
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
 * Author: Hemanth Narra <hemanth@ittc.ku.com>
 *
 * James P.G. Sterbenz <jpgs@ittc.ku.edu>, director
 * ResiliNets Research Group  http://wiki.ittc.ku.edu/resilinets
 * Information and Telecommunication Technology Center (ITTC)
 * and Department of Electrical Engineering and Computer Science
 * The University of Kansas Lawrence, KS USA.
 *
 * Work supported in part by NSF FIND (Future Internet Design) Program
 * under grant CNS-0626918 (Postmodern Internet Architecture),
 * NSF grant CNS-1050226 (Multilayer Network Resilience Analysis and Experimentation on GENI),
 * US Department of Defense (DoD), and ITTC at The University of Kansas.
 */

#ifndef MYPROTOCOL_PACKET_H
#define MYPROTOCOL_PACKET_H

#include <iostream>
#include "ns3/header.h"
#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"

namespace ns3 {
namespace myprotocol {
/**
 * \ingroup myprotocol
 * \brief myprotocol Update Packet Format
 * \verbatim
 |      0        |      1        |      2        |       3       |
  0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                      Destination Address                      |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                            HopCount                           |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 |                       Sequence Number                         |
 +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * \endverbatim
 */

class MyprotocolHeader : public Header
{
public:
  /**
   * Constructor
   *
   * \param dst destination IP address
   * \param hopcount hop count
   * \param dstSeqNo destination sequence number
   */
  MyprotocolHeader (uint16_t x = 0, uint16_t y = 0, uint16_t z = 0, uint16_t vx = 0, uint16_t vy = 0, uint16_t vz = 0, uint16_t sign = 0, 
                    uint16_t timestamp = 0, Ipv4Address myadress = Ipv4Address ());
  virtual ~MyprotocolHeader ();
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;
  virtual uint32_t GetSerializedSize () const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;

  //ADD
  void SetX(uint16_t x){
    m_x = x;
  }
  uint16_t GetX() const{
    return m_x;
  }
  void SetY(uint16_t y){
    m_y = y;
  }
  uint16_t GetY() const{
    return m_y;
  }
  void SetZ(uint16_t z){
    m_z = z;
  }
  uint16_t GetZ() const{
    return m_z;
  }
  void SetVx(uint16_t vx){
    m_vx = vx;
  }
  uint16_t GetVx() const{
    return m_vx;
  }
  void SetVy(uint16_t vy){
    m_vy = vy;
  }
  uint16_t GetVy() const{
    return m_vy;
  }
  void SetVz(uint16_t vz){
    m_vz = vz;
  }
  uint16_t GetVz() const{
    return m_vz;
  }
  void SetSign(uint16_t sign){
    m_sign = sign;
  }
  uint16_t GetSign() const{
    return m_sign;
  }
  void SetTimestamp(uint16_t timestamp){
    m_timestamp = timestamp;
  }
  uint16_t GetTimestamp() const{
    return m_timestamp;
  }
  void
  SetMyadress (Ipv4Address myadress)
  {
    m_myadress = myadress;
  }
  Ipv4Address
  GetMyadress () const
  {
    return m_myadress;
  }
private:
  //ADD:添加位置信息、速度信息、时间戳
  uint16_t m_x;
  uint16_t m_y;
  uint16_t m_z;
  uint16_t m_vx;
  uint16_t m_vy;
  uint16_t m_vz;
  uint16_t m_sign;      //记录速度是否为负数，0:都不是负数，1:X轴速度为负，2:Y轴速度为负，3:Z轴速度为负,4：xy为负数，5：xz为负数，6：yz为负数，7：全部都是负数
  //以秒为单位的整数，每次重新运行代码都是从0s开始
  uint16_t m_timestamp;
  Ipv4Address m_myadress;
};

static inline std::ostream & operator<< (std::ostream& os, const MyprotocolHeader & packet)
{
  packet.Print (os);
  return os;
}

class DataHeader : public Header
{
public:
  /// c-tor
  DataHeader (uint16_t dstPosx = 0, uint16_t dstPosy = 0, uint16_t dstPosz = 0, 
              uint16_t dstVelx = 0, uint16_t dstVely = 0, uint16_t dstVelz = 0, 
              uint16_t sign = 0, uint16_t timestamp = 0, 
              uint16_t recPosx = 0, uint16_t recPosy = 0, uint16_t recPosz = 0, uint16_t inRec  = 0);

  ///\name Header serialization/deserialization
  static TypeId GetTypeId ();
  TypeId GetInstanceTypeId () const;
  uint32_t GetSerializedSize () const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
  void Print (std::ostream &os) const;

  /// get/set function
  void SetDstPosx (uint16_t posx)
  {
    m_dstPosx = posx;
  }
  uint16_t GetDstPosx () const
  {
    return m_dstPosx;
  }
  void SetDstPosy (uint16_t posy)
  {
    m_dstPosy = posy;
  }
  uint16_t GetDstPosy () const
  {
    return m_dstPosy;
  }
  void SetDstPosz (uint16_t posz)
  {
    m_dstPosz = posz;
  }
  uint16_t GetDstPosz () const
  {
    return m_dstPosz;
  }
  void SetDstVelx (uint16_t velx)
  {
    m_dstVelx = velx;
  }
  uint16_t GetDstVelx () const
  {
    return m_dstVelx;
  }
  void SetDstVely (uint16_t vely)
  {
    m_dstVely = vely;
  }
  uint16_t GetDstVely () const
  {
    return m_dstVely;
  }
  void SetDstVelz (uint16_t velz)
  {
    m_dstVelz = velz;
  }
  uint16_t GetDstVelz () const
  {
    return m_dstVelz;
  }
  void SetSign (uint16_t sign)
  {
    m_dstSign = sign;
  }
  uint16_t GetSign () const
  {
    return m_dstSign;
  }
  void SetTimestamp (uint16_t timestamp)
  {
    m_timestamp = timestamp;
  }
  uint16_t GetTimestamp () const
  {
    return m_timestamp;
  }
  void SetRecPosx (uint16_t posx)
  {
    m_recPosx = posx;
  }
  uint16_t GetRecPosx () const
  {
    return m_recPosx;
  }
  void SetRecPosy (uint16_t posy)
  {
    m_recPosy = posy;
  }
  uint16_t GetRecPosy () const
  {
    return m_recPosy;
  }
  void SetRecPosz (uint16_t posz)
  {
    m_recPosz = posz;
  }
  uint16_t GetRecPosz () const
  {
    return m_recPosz;
  }
  void SetInRec (uint16_t rec)
  {
    m_inRec = rec;
  }
  uint16_t GetInRec () const
  {
    return m_inRec;
  }

  bool operator== (DataHeader const & o) const;

private:
  uint16_t m_dstPosx;          ///< Destination Position x
  uint16_t m_dstPosy;          ///< Destination Position x
  uint16_t m_dstPosz;
  uint16_t m_dstVelx;
  uint16_t m_dstVely;
  uint16_t m_dstVelz;
  uint16_t m_dstSign;
  uint16_t m_timestamp;          ///< 目的地时间的timestamp
  uint16_t m_recPosx;          ///< x of position that entered Recovery-mode
  uint16_t m_recPosy;          ///< y of position that entered Recovery-mode
  uint16_t m_recPosz; 
  uint16_t m_inRec;             ///< 1 if in Recovery-mode, 0 otherwise
};

std::ostream & operator<< (std::ostream & os, DataHeader const & h);
}
}

#endif /* MYPROTOCOL_PACKET_H */
