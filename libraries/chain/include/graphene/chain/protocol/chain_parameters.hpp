/*
 * Copyright (c) 2015 Cryptonomex, Inc., and contributors.
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once
#include <graphene/chain/protocol/base.hpp>
#include <graphene/chain/protocol/types.hpp>
#include <fc/smart_ref_fwd.hpp>
#include <iostream>

namespace graphene { namespace chain { struct fee_schedule; } }

namespace graphene { namespace chain {

   struct chain_parameters
   {
     struct ext
      {
         /** container to credit parameters */
         struct credit_options
         {
            uint32_t seconds_per_day = 86400; ///< interval in second for one day for credits
            uint32_t max_credit_expiration_days = 7; ///< max expiration time in days for credits
            uint32_t min_witnesses_for_exchange_rate = 7; ///< minimum number of active witnesses who should vote to set new exchange rate
            uint32_t exchange_rate_set_max_interval = 3600; ///< max interval in seconds waiting for witnesses set rates
            uint32_t exchange_rate_set_min_interval = 75; ///< interval in seconds after which looking for min witnesses set rates
         };

         struct credit_referrer_bonus_options
         {
           float karma_account_bonus = 0.5;
           float creditor_referrer_bonus = 0.1;
           float borrower_referrer_bonus = 0.1;
           float fail_creditor_referrer_bonus = 0;
           float fail_borrower_referrer_bonus = 0;
           std::string special_account_name = "ooo-karma-rus";
         };
      };

      /** using a smart ref breaks the circular dependency created between operations and the fee schedule */
      smart_ref<fee_schedule> current_fees;                       ///< current schedule of fees
      uint8_t                 block_interval                      = GRAPHENE_DEFAULT_BLOCK_INTERVAL; ///< interval in seconds between blocks
      uint32_t                maintenance_interval                = GRAPHENE_DEFAULT_MAINTENANCE_INTERVAL; ///< interval in sections between blockchain maintenance events
      uint8_t                 maintenance_skip_slots              = GRAPHENE_DEFAULT_MAINTENANCE_SKIP_SLOTS; ///< number of block_intervals to skip at maintenance time
      uint32_t                committee_proposal_review_period    = GRAPHENE_DEFAULT_COMMITTEE_PROPOSAL_REVIEW_PERIOD_SEC; ///< minimum time in seconds that a proposed transaction requiring committee authority may not be signed, prior to expiration
      uint32_t                maximum_transaction_size            = GRAPHENE_DEFAULT_MAX_TRANSACTION_SIZE; ///< maximum allowable size in bytes for a transaction
      uint32_t                maximum_block_size                  = GRAPHENE_DEFAULT_MAX_BLOCK_SIZE; ///< maximum allowable size in bytes for a block
      uint32_t                maximum_time_until_expiration       = GRAPHENE_DEFAULT_MAX_TIME_UNTIL_EXPIRATION; ///< maximum lifetime in seconds for transactions to be valid, before expiring
      uint32_t                maximum_proposal_lifetime           = GRAPHENE_DEFAULT_MAX_PROPOSAL_LIFETIME_SEC; ///< maximum lifetime in seconds for proposed transactions to be kept, before expiring
      uint8_t                 maximum_asset_whitelist_authorities = GRAPHENE_DEFAULT_MAX_ASSET_WHITELIST_AUTHORITIES; ///< maximum number of accounts which an asset may list as authorities for its whitelist OR blacklist
      uint8_t                 maximum_asset_feed_publishers       = GRAPHENE_DEFAULT_MAX_ASSET_FEED_PUBLISHERS; ///< the maximum number of feed publishers for a given asset
      uint16_t                maximum_witness_count               = GRAPHENE_DEFAULT_MAX_WITNESSES; ///< maximum number of active witnesses
      uint16_t                maximum_committee_count             = GRAPHENE_DEFAULT_MAX_COMMITTEE; ///< maximum number of active committee_members
      uint16_t                maximum_authority_membership        = GRAPHENE_DEFAULT_MAX_AUTHORITY_MEMBERSHIP; ///< largest number of keys/accounts an authority can have
      uint16_t                reserve_percent_of_fee              = GRAPHENE_DEFAULT_BURN_PERCENT_OF_FEE; ///< the percentage of the network's allocation of a fee that is taken out of circulation
      uint16_t                network_percent_of_fee              = GRAPHENE_DEFAULT_NETWORK_PERCENT_OF_FEE; ///< percent of transaction fees paid to network
      uint16_t                lifetime_referrer_percent_of_fee    = GRAPHENE_DEFAULT_LIFETIME_REFERRER_PERCENT_OF_FEE; ///< percent of transaction fees paid to network
      uint32_t                cashback_vesting_period_seconds     = GRAPHENE_DEFAULT_CASHBACK_VESTING_PERIOD_SEC; ///< time after cashback rewards are accrued before they become liquid
      share_type              cashback_vesting_threshold          = GRAPHENE_DEFAULT_CASHBACK_VESTING_THRESHOLD; ///< the maximum cashback that can be received without vesting
      bool                    count_non_member_votes              = true; ///< set to false to restrict voting privlegages to member accounts
      bool                    allow_non_member_whitelists         = false; ///< true if non-member accounts may set whitelists and blacklists; false otherwise
      share_type              witness_pay_per_block               = GRAPHENE_DEFAULT_WITNESS_PAY_PER_BLOCK; ///< CORE to be allocated to witnesses (per block)
      uint32_t                witness_pay_vesting_seconds         = GRAPHENE_DEFAULT_WITNESS_PAY_VESTING_SECONDS; ///< vesting_seconds parameter for witness VBO's
      share_type              worker_budget_per_day               = GRAPHENE_DEFAULT_WORKER_BUDGET_PER_DAY; ///< CORE to be allocated to workers (per day)
      uint16_t                max_predicate_opcode                = GRAPHENE_DEFAULT_MAX_ASSERT_OPCODE; ///< predicate_opcode must be less than this number
      share_type              fee_liquidation_threshold           = GRAPHENE_DEFAULT_FEE_LIQUIDATION_THRESHOLD; ///< value in CORE at which accumulated fees in blockchain-issued market assets should be liquidated
      uint16_t                accounts_per_fee_scale              = GRAPHENE_DEFAULT_ACCOUNTS_PER_FEE_SCALE; ///< number of accounts between fee scalings
      uint8_t                 account_fee_scale_bitshifts         = GRAPHENE_DEFAULT_ACCOUNT_FEE_SCALE_BITSHIFTS; ///< number of times to left bitshift account registration fee at each scaling
      uint8_t                 max_authority_depth                 = GRAPHENE_MAX_SIG_CHECK_DEPTH;
      
      typedef static_variant<ext::credit_options, ext::credit_referrer_bonus_options>  parameter_extension;
      typedef flat_set<parameter_extension> extensions_type;
      extensions_type         extensions;

      /** defined in fee_schedule.cpp */
      void validate()const;

      const ext::credit_options get_credit_options()const
      {
        if( extensions.size() > 0 )
        {
          for( const parameter_extension& e : extensions )
          {
                  if( e.which() == parameter_extension::tag<ext::credit_options>::value )
                  return e.get<ext::credit_options>();
          }
        }
        return ext::credit_options();
      }

      const ext::credit_referrer_bonus_options get_bonus_options()const
      {
        if( extensions.size() > 0 )
        {
          for( const parameter_extension& e : extensions )
          {
                  if( e.which() == parameter_extension::tag<ext::credit_referrer_bonus_options>::value )
                      return e.get<ext::credit_referrer_bonus_options>();
          }
        }
        return ext::credit_referrer_bonus_options();
      }

      void set_credit_options(ext::credit_options& new_co)
      {
        for( chain_parameters::parameter_extension& e : extensions )
        {
          if( e.which() == chain_parameters::parameter_extension::tag<chain_parameters::ext::credit_options>::value )
          {
            e.get<chain_parameters::ext::credit_options>() = new_co;
            return;
          }
        }

        extensions.insert(new_co);
      }

      void set_bonus_options(ext::credit_referrer_bonus_options& new_bo)
      {
        for( chain_parameters::parameter_extension& e : extensions )
        {
          if( e.which() == chain_parameters::parameter_extension::tag<chain_parameters::ext::credit_referrer_bonus_options>::value )
          {
            e.get<chain_parameters::ext::credit_referrer_bonus_options>() = new_bo;
            return;
          }
        }

        extensions.insert(new_bo);
      }
   };

} }  // graphene::chain

FC_REFLECT( graphene::chain::chain_parameters::ext::credit_options,
            (seconds_per_day)
            (max_credit_expiration_days)
            (min_witnesses_for_exchange_rate)
            (exchange_rate_set_max_interval)
            (exchange_rate_set_min_interval)
          )

FC_REFLECT( graphene::chain::chain_parameters::ext::credit_referrer_bonus_options,
            (karma_account_bonus)
            (creditor_referrer_bonus)
            (borrower_referrer_bonus)
            (fail_creditor_referrer_bonus)
            (fail_borrower_referrer_bonus)
            (special_account_name)
          )          

FC_REFLECT_TYPENAME( graphene::chain::chain_parameters::parameter_extension )
FC_REFLECT_TYPENAME( graphene::chain::chain_parameters::extensions_type )


FC_REFLECT( graphene::chain::chain_parameters,
            (current_fees)
            (block_interval)
            (maintenance_interval)
            (maintenance_skip_slots)
            (committee_proposal_review_period)
            (maximum_transaction_size)
            (maximum_block_size)
            (maximum_time_until_expiration)
            (maximum_proposal_lifetime)
            (maximum_asset_whitelist_authorities)
            (maximum_asset_feed_publishers)
            (maximum_witness_count)
            (maximum_committee_count)
            (maximum_authority_membership)
            (reserve_percent_of_fee)
            (network_percent_of_fee)
            (lifetime_referrer_percent_of_fee)
            (cashback_vesting_period_seconds)
            (cashback_vesting_threshold)
            (count_non_member_votes)
            (allow_non_member_whitelists)
            (witness_pay_per_block)
            (worker_budget_per_day)
            (max_predicate_opcode)
            (fee_liquidation_threshold)
            (accounts_per_fee_scale)
            (account_fee_scale_bitshifts)
            (max_authority_depth)
            (extensions)
          )
