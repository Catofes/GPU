//
// Created by herbertqiao on 2/28/18.
//

#ifndef ROOTSCRIPT_REDUCE_H
#define ROOTSCRIPT_REDUCE_H

#include <thrust/detail/config.h>
#include <thrust/reduce.h>
#include <thrust/detail/seq.h>
#include <thrust/detail/temporary_array.h>
#include <thrust/system/cuda/detail/bulk.h>
#include <thrust/system/cuda/detail/decomposition.h>
#include <thrust/system/cuda/detail/execution_policy.h>
//#include <thrust/system/cuda/detail/execute_on_stream.h>
#include <thrust/detail/type_traits.h>

namespace thrust
{
    namespace system
    {
        namespace cuda
        {
            namespace detail
            {
                namespace reduce_detail
                {


                    template<typename DerivedPolicy,
                            typename InputIterator,
                            typename OutputType,
                            typename BinaryFunction>
                    __host__ __device__

                    OutputType my_tuned_reduce(execution_policy<DerivedPolicy> &exec,
                                               InputIterator first,
                                               InputIterator last,
                                               OutputType init,
                                               BinaryFunction binary_op)
                    {
                        typedef typename thrust::iterator_difference<InputIterator>::type size_type;

                        const size_type n = last - first;

                        if (n <= 0) return init;

                        cudaStream_t s = stream(thrust::detail::derived_cast(exec));

                        const size_type groupsize = 8;
                        const size_type grainsize = 7;
                        const size_type tile_size = groupsize * grainsize;
                        const size_type num_tiles = (n + tile_size - 1) / tile_size;
                        const size_type subscription = 10;
                        bulk_::concurrent_group<
                                bulk_::agent<grainsize>,
                                groupsize
                        > g;

                        const size_type num_groups =
                                thrust::min < size_type > (subscription * g.hardware_concurrency(), num_tiles);

                        aligned_decomposition<size_type> decomp(n, num_groups, tile_size);

                        thrust::detail::temporary_array<OutputType, DerivedPolicy> partial_sums(exec, decomp.size());

                        // reduce into partial sums
                        bulk_::async(bulk_::par(s, g, decomp.size()), reduce_detail::reduce_partitions(),
                                     bulk_::root.this_exec, first, decomp, partial_sums.begin(), init,
                                     binary_op).wait();

                        if (partial_sums.size() > 1) {
                            // reduce the partial sums
                            bulk_::async(bulk_::par(s, g, 1), reduce_detail::reduce_partitions(), bulk_::root.this_exec,
                                         partial_sums.begin(), partial_sums.end(), partial_sums.begin(), binary_op);
                        } // end while

                        return get_value(exec, &partial_sums[0]);
                    } // end tuned_reduce()
                }
            }
        }
    }
}
#endif //ROOTSCRIPT_REDUCE_H
