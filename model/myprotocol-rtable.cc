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
#include "myprotocol-rtable.h"
#include "ns3/simulator.h"
#include <iomanip>
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("MyprotocolRoutingTable");

// ADD:修改构造函数中的参数
namespace myprotocol {
RoutingTableEntry::RoutingTableEntry (uint16_t x,
                                      uint16_t y,
                                      uint16_t z,
                                      int16_t vx,
                                      int16_t vy,
                                      int16_t vz,
                                      uint16_t timestamp,
                                      Ipv4Address adress)
  : m_x(x),
    m_y(y),
    m_z(z),
    m_vx(vx),
    m_vy(vy),
    m_vz(vz),
    m_timestamp(timestamp),
    m_adress(adress)
{
}
RoutingTableEntry::~RoutingTableEntry ()
{
}


RoutingTable::RoutingTable ()
{
}

bool
RoutingTable::LookupRoute (Ipv4Address id,
                           RoutingTableEntry & rt)
{
  if (m_ipv4AddressEntry.empty ())
    {
      return false;
    }
  std::map<Ipv4Address, RoutingTableEntry>::const_iterator i = m_ipv4AddressEntry.find (id);
  if (i == m_ipv4AddressEntry.end ())
    {
      return false;
    }
  rt = i->second;
  return true;
}

bool
RoutingTable::DeleteRoute (Ipv4Address dst)
{
  if (m_ipv4AddressEntry.erase (dst) != 0)
    {
      // NS_LOG_DEBUG("Route erased");
      return true;
    }
  return false;
}

uint32_t
RoutingTable::RoutingTableSize ()
{
  return m_ipv4AddressEntry.size ();
}

bool
RoutingTable::AddRoute (RoutingTableEntry & rt)
{
  std::pair<std::map<Ipv4Address, RoutingTableEntry>::iterator, bool> result = m_ipv4AddressEntry.insert (std::make_pair (
                                                                                                            rt.GetAdress (),rt));
  return result.second;
}

bool
RoutingTable::Update (RoutingTableEntry & rt)
{
  std::map<Ipv4Address, RoutingTableEntry>::iterator i = m_ipv4AddressEntry.find (rt.GetAdress ());
  if (i == m_ipv4AddressEntry.end ())
    {
      return false;
    }
  i->second = rt;
  return true;
}

void
RoutingTableEntry::Print (Ptr<OutputStreamWrapper> stream) const
{
  *stream->GetStream () << std::setiosflags (std::ios::fixed) << m_x << "\t\t" << m_y << "\t\t" << m_z << "\t\t"
                        << m_vx << "\t\t" << m_vy << "\t\t" << m_vz << "\t\t" << m_timestamp << "\t\t" << m_adress << "\n";
}

void
RoutingTable::Print (Ptr<OutputStreamWrapper> stream) const
{
  *stream->GetStream () << "\n myprotocol Routing table\n" << "x\t\ty\t\tz\t\tvx\t\tvy\t\tvz\t\ttimestamp\t\tadress\n";
  for (std::map<Ipv4Address, RoutingTableEntry>::const_iterator i = m_ipv4AddressEntry.begin (); i
       != m_ipv4AddressEntry.end (); ++i)
    {
      i->second.Print (stream);
    }
  *stream->GetStream () << "\n";
}

// ADD:位置预测函数
Vector 
RoutingTable::PredictPosition(Ipv4Address id){
  RoutingTableEntry rt;
  if(!LookupRoute(id,rt)){
    std::cout<<"not find a valid routing entry!!!\n";
    return Vector(-1,-1,-1);
  }else{
    // 先获取该节点的速度、位置、时间戳
    uint16_t deltaTime = Simulator::Now ().ToInteger(Time::S) - rt.GetTimestamp();
    uint16_t newX = rt.GetX() + deltaTime * rt.GetVx();
    uint16_t newY = rt.GetY() + deltaTime * rt.GetVy();
    uint16_t newZ = rt.GetZ() + deltaTime * rt.GetVz();
    return Vector(newX, newY, newZ);
  }
}

// ADD：筛选邻居节点
void 
RoutingTable::LookupNeighbor(std::map<Ipv4Address, RoutingTableEntry> & neighborTable, Vector myPos){
  // 传输范围250m
  uint16_t TransmissionRange = 250;
  for (std::map<Ipv4Address, RoutingTableEntry>::iterator i = m_ipv4AddressEntry.begin (); i != m_ipv4AddressEntry.end (); i++){
    Vector predictPos = PredictPosition(i->first);
    if(CalculateDistance (predictPos, Vector(-1,-1,-1)) == 0){
      continue;
    }
    double distance = CalculateDistance(predictPos, myPos);
    // 距离小于传输范围，可以认为是邻居
    if(distance <= TransmissionRange){
      neighborTable.insert(std::make_pair(i->first,i->second));
    }
  }
}

// ADD：实现贪婪寻找最优下一条路径
Ipv4Address 
RoutingTable::BestNeighbor (Ipv4Address dst, Vector myPos)
{
  std::map<Ipv4Address, RoutingTableEntry> neighborTable;
  LookupNeighbor(neighborTable, myPos);
  Vector predictDstPos = PredictPosition(dst);
  if(CalculateDistance (predictDstPos, Vector(-1,-1,-1)) == 0){
    return Ipv4Address::GetZero ();
  }

  double initialDistance = CalculateDistance (predictDstPos, myPos);

  if (neighborTable.empty ())
    {
      std::cout<<"BestNeighbor table is empty!!!\n";
      NS_LOG_DEBUG ("BestNeighbor table is empty");
      return Ipv4Address::GetZero ();
    }     //if table is empty (no neighbours)
  Ipv4Address bestFoundID = neighborTable.begin ()->first;
  double bestFoundDistance = CalculateDistance (PredictPosition(neighborTable.begin ()->first), predictDstPos);

  for (std::map<Ipv4Address, RoutingTableEntry>::iterator i = neighborTable.begin (); i != neighborTable.end (); i++){
    if(bestFoundDistance > CalculateDistance (PredictPosition(i->first), predictDstPos)){
      bestFoundID = i->first;
      bestFoundDistance = CalculateDistance (PredictPosition(i->first), predictDstPos);
    }
  }  
  if(initialDistance > bestFoundDistance){
    return bestFoundID;
  }else{
    return Ipv4Address::GetZero (); //so it enters Recovery-mode
  }
}

// ADD：检查是否符合恢复模式的条件（有目的地地址&有邻居&没有可以使用贪婪的下一跳）
bool 
RoutingTable::MatchRecovery(Ipv4Address dst, Vector myPos){
  if(CalculateDistance (PredictPosition(dst), Vector(-1,-1,-1)) == 0){
    return false;
  }
  std::map<Ipv4Address, RoutingTableEntry> neighborTable;
  LookupNeighbor(neighborTable, myPos);
  if (neighborTable.empty ()){
    return false;
  }
  return true;
}

}
}
