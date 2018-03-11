// Copyright (c) 2017-2018 YxomTech
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <boost/program_options/variables_map.hpp>

#include "VarNote_core/VarNote_basic_impl.h"
#include "VarNote_core/verification_context.h"
#include <unordered_map>

namespace tests
{
  struct block_index {
      size_t height;
      crypto::hash id;
      crypto::hash longhash;
      VarNote::block blk;
      VarNote::blobdata blob;
      std::list<VarNote::transaction> txes;

      block_index() : height(0), id(VarNote::null_hash), longhash(VarNote::null_hash) { }
      block_index(size_t _height, const crypto::hash &_id, const crypto::hash &_longhash, const VarNote::block &_blk, const VarNote::blobdata &_blob, const std::list<VarNote::transaction> &_txes)
          : height(_height), id(_id), longhash(_longhash), blk(_blk), blob(_blob), txes(_txes) { }
  };

  class proxy_core
  {
      VarNote::block m_genesis;
      std::list<crypto::hash> m_known_block_list;
      std::unordered_map<crypto::hash, block_index> m_hash2blkidx;

      crypto::hash m_lastblk;
      std::list<VarNote::transaction> txes;

      crypto::cn_context m_cn_context;

      bool add_block(const crypto::hash &_id, const crypto::hash &_longhash, const VarNote::block &_blk, const VarNote::blobdata &_blob);
      void build_short_history(std::list<crypto::hash> &m_history, const crypto::hash &m_start);


  public:
    void on_synchronized(){}
    uint64_t get_current_blockchain_height(){return 1;}
    bool init(const boost::program_options::variables_map& vm);
    bool deinit(){return true;}
    bool get_short_chain_history(std::list<crypto::hash>& ids);
    bool get_stat_info(VarNote::core_stat_info& st_inf){return true;}
    bool have_block(const crypto::hash& id);
    bool get_blockchain_top(uint64_t& height, crypto::hash& top_id);
    bool handle_incoming_tx(const VarNote::blobdata& tx_blob, VarNote::tx_verification_context& tvc, bool keeped_by_block);
    bool handle_incoming_block(const VarNote::blobdata& block_blob, VarNote::block_verification_context& bvc, bool update_miner_blocktemplate = true);
    void pause_mine(){}
    void resume_mine(){}
    bool on_idle(){return true;}
    bool find_blockchain_supplement(const std::list<crypto::hash>& qblock_ids, VarNote::NOTIFY_RESPONSE_CHAIN_ENTRY::request& resp){return true;}
    bool handle_get_objects(VarNote::NOTIFY_REQUEST_GET_OBJECTS::request& arg, VarNote::NOTIFY_RESPONSE_GET_OBJECTS::request& rsp, VarNote::VarNote_connection_context& context){return true;}
  };
}
