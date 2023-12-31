#ifndef MYPROTOCOL4_RTABLE_H
#define MYPROTOCOL4_RTABLE_H

#include <cassert>
#include <map>
#include <sys/types.h>
#include "ns3/ipv4.h"
#include "ns3/ipv4-route.h"
#include "ns3/timer.h"
#include "ns3/net-device.h"
#include "ns3/output-stream-wrapper.h"
// ADD：添加Vector
#include "ns3/vector.h"
// 添加移动模型
#include "ns3/mobility-model.h"

namespace ns3 {
namespace myprotocol4 {

class RoutingTableEntry
{
public:
  RoutingTableEntry (uint16_t x = 0,uint16_t y = 0,uint16_t z = 0, 
                    int16_t vx = 0,int16_t vy = 0,int16_t vz = 0,
                    uint16_t timestamp = 0, Ipv4Address adress = Ipv4Address ());

  ~RoutingTableEntry ();

  void
  Print (Ptr<OutputStreamWrapper> stream) const;

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
  void SetVx(int16_t vx){
    m_vx = vx;
  }
  int16_t GetVx() const{
    return m_vx;
  }
  void SetVy(int16_t vy){
    m_vy = vy;
  }
  int16_t GetVy() const{
    return m_vy;
  }
  void SetVz(int16_t vz){
    m_vz = vz;
  }
  int16_t GetVz() const{
    return m_vz;
  }
  void SetTimestamp(uint16_t timestamp){
    m_timestamp = timestamp;
  }
  uint16_t GetTimestamp() const{
    return m_timestamp;
  }
  void SetAdress (Ipv4Address adress)
  {
    m_adress = adress;
  }
  Ipv4Address GetAdress () const
  {
    return m_adress;
  }

private:
  //ADD: 当前位置、速度、时间戳
  uint16_t m_x;
  uint16_t m_y;
  uint16_t m_z;
  int16_t m_vx;
  int16_t m_vy;
  int16_t m_vz;
  uint16_t m_timestamp;
  Ipv4Address m_adress;
};

class RoutingTable
{
public:
  /// c-tor
  RoutingTable ();
  /**
   * Add routing table entry if it doesn't yet exist in routing table
   * \param r routing table entry
   * \return true in success
   */
  bool
  AddRoute (RoutingTableEntry & r);
  /**
   * Delete routing table entry with destination address dst, if it exists.
   * \param dst destination address
   * \return true on success
   */
  bool
  DeleteRoute (Ipv4Address dst);
  /**
   * Lookup routing table entry with destination address dst
   * \param dst destination address
   * \param rt entry with destination address dst, if exists
   * \return true on success
   */
  bool
  LookupRoute (Ipv4Address dst, RoutingTableEntry & rt);
  /**
   * Updating the routing Table with routing table entry rt
   * \param rt routing table entry
   * \return true on success
   */
  bool
  Update (RoutingTableEntry & rt);
  /// Delete all entries from routing table
  void
  Clear ()
  {
    m_positionTable.clear ();
  }
  /**
   * Print routing table
   * \param stream the output stream
   */
  void
  Print (Ptr<OutputStreamWrapper> stream) const;

  // ADD: 位置预测函数
  Vector PredictPosition(Ipv4Address id); 

  // ADD：筛选邻居节点
  void LookupNeighbor(std::map<Ipv4Address, RoutingTableEntry> & neighborTable, Vector myPos);

  Ipv4Address BestNeighbor (std::map<Ipv4Address, RoutingTableEntry> neighborTable, Vector dstPos, Vector myPos);    //dstPos需要时经过预测后的目的地位置

  void Purge();

private:
  // 表项过期时间
  uint16_t m_entryLifeTime;
  /// an entry in the routing table.
  std::map<Ipv4Address, RoutingTableEntry> m_positionTable;
  /// neighbor table
  std::map<Ipv4Address, RoutingTableEntry> m_neiborTable;
};
}
}
#endif
