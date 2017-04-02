//  Copyright (c) 2015-2017 John Biddiscombe
//  Copyright (c) 2017      Thomas Heller
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HPX_PARCELSET_POLICIES_LIBFABRIC_SENDER_HPP
#define HPX_PARCELSET_POLICIES_LIBFABRIC_SENDER_HPP

#include <plugins/parcelport/libfabric/libfabric_memory_region.hpp>
#include <plugins/parcelport/libfabric/rdma_memory_pool.hpp>
#include <plugins/parcelport/libfabric/pinned_memory_vector.hpp>
#include <plugins/parcelport/libfabric/header.hpp>
#include <plugins/parcelport/libfabric/performance_counter.hpp>

#include <hpx/runtime/parcelset/locality.hpp>

#include <hpx/util/atomic_count.hpp>
#include <hpx/util/unique_function.hpp>

#include <boost/container/small_vector.hpp>

#include <memory>

// include for iovec
#include <sys/uio.h>

namespace hpx {
namespace parcelset {
namespace policies {
namespace libfabric
{
    struct parcelport;

    // --------------------------------------------------------------------
    // each sender owns a header_region that is reused for each new message
    // other buffers must be allocated and deallocated
    // --------------------------------------------------------------------
    struct sender
    {
        typedef header<HPX_PARCELPORT_LIBFABRIC_MESSAGE_HEADER_SIZE> header_type;
        static constexpr unsigned int header_size = header_type::header_block_size;

        typedef rdma_memory_pool                                 memory_pool_type;
        typedef pinned_memory_vector<char, header_size>          snd_data_type;
        typedef parcel_buffer<snd_data_type,serialization::serialization_chunk>
            snd_buffer_type;

        typedef boost::container::small_vector<libfabric_memory_region*,4> zero_copy_vector;

        // --------------------------------------------------------------------
        sender(parcelport* pp, fid_ep* endpoint, fid_domain* domain,
            rdma_memory_pool* memory_pool)
          : parcelport_(pp)
          , endpoint_(endpoint)
          , domain_(domain)
          , memory_pool_(memory_pool)
          , buffer_(snd_data_type(memory_pool_), memory_pool_)
          , header_region_(nullptr)
          , message_region_(nullptr)
          , completion_count_(0)
          , sends_posted_(0)
          , sends_deleted_(0)
          , acks_received_(0)
        {
            // the header region is reused multiple times
            header_region_ =
                memory_pool_->allocate_region(memory_pool_->small_.chunk_size());
            LOG_DEVEL_MSG("Create sender: " << hexpointer(this));
        }

        // --------------------------------------------------------------------
        ~sender()
        {
            memory_pool_->deallocate(header_region_);
        }

        // --------------------------------------------------------------------
        snd_buffer_type get_new_buffer()
        {
            LOG_DEBUG_MSG("Returning a new buffer object from sender " << hexpointer(this));
            return snd_buffer_type(snd_data_type(memory_pool_), memory_pool_);
        }

        // --------------------------------------------------------------------
        template <typename Handler, typename ParcelPostprocess>
        void async_write(Handler && handler, ParcelPostprocess && parcel_postprocess)
        {
            HPX_ASSERT(false);
        }

        // --------------------------------------------------------------------
        void verify(parcelset::locality const & parcel_locality_id) const {}

        void async_write_impl();

        void handle_send_completion();

        void handle_message_completion_ack();

        void cleanup();

        // --------------------------------------------------------------------
        parcelport                *parcelport_;
        fid_ep                    *endpoint_;
        fid_domain                *domain_;
        rdma_memory_pool          *memory_pool_;
        fi_addr_t                  dst_addr_;
        snd_buffer_type            buffer_;
        libfabric_memory_region   *header_region_;
        libfabric_memory_region   *message_region_;
        header_type               *header_;
        zero_copy_vector           rma_regions_;
        hpx::util::atomic_count    completion_count_;
        // for debugging
        performance_counter<unsigned int> sends_posted_;
        performance_counter<unsigned int> sends_deleted_;
        performance_counter<unsigned int> acks_received_;
        //
        util::unique_function_nonser<void(error_code const&)> handler_;
        util::function_nonser<void(sender*)>                  postprocess_handler_;
        //
        struct iovec               region_list_[2];
        void                      *desc_[2];
    };
}}}}

#endif