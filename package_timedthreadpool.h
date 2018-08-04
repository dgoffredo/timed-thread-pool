#ifndef INCLUDED_PACKAGE_TIMEDTHREADPOOL
#define INCLUDED_PACKAGE_TIMEDTHREADPOOL

#include <bdlmt_eventscheduler.h>
#include <bdlmt_threadpool.h>

#include <bslalg_typetraits.h>

#include <bsl_functional.h>

namespace BloombergLP {
namespace bsls { class TimeInterval; }
namespace package {

                         // =====================
                         // class TimedThreadPool
                         // =====================

class TimedThreadPool : private bdlmt::ThreadPool {
    // This class implements a thread pool used for concurrently executing
    // multiple user-defined functions ("jobs"), each with an optionally
    // specified timeout or deadline, after which if the job has not completed,
    // an additional user-defined function ("onTimeout") is executed.

    // DATA
    bdlmt::EventScheduler d_eventScheduler;

    public:
    // TRAITS
    BSLALG_DECLARE_NESTED_TRAITS(TimedThreadPool,
                                 bslalg::TypeTraitUsesBslmaAllocator);

    // CREATORS
    explicit TimedThreadPool(const bslmt::ThreadAttributes&  threadAttributes,
                             int                             minThreads,
                             int                             maxThreads,
                             const bsls::TimeInterval&       maxIdleTime,
                             bslma::Allocator               *allocator = 0);
        // Construct a timed thread pool with the specified 'threadAttributes',
        // 'minThread' and 'maxThreads' minimum and maximum number of threads,
        // respectively, the specified 'maxIdleTime' maximum idle time, and
        // using the optionally specified 'allocator' to supply memory. If
        // 'allocator' is zero, the currently installed default allocator will
        // be used instead.

    ~TimedThreadPool();
        // Disable queuing on this thread pool and wait until all pending jobs
        // complete, then shut down all processing threads, and finally destroy
        // this object.

    // MANIPULATORS
    int enqueueWithTimeout(const bsl::function<void()>&   job,
                           const bsls::TimeInterval&      timeout,
                           const bsl::function<void())>&  onTimeout);
        // Enqueue the specified 'job' to be executed by the next available
        // thread. Ensure that from the time the 'job' begins execution, if
        // execution has not completed within the specified 'timeout'
        // (expressed as a time interval relative to the start of execution),
        // the specified 'onTimeout' is invoked. Return zero if the 'job' was
        // enqueued successfully and the timeout event was scheduled
        // successfully, or return a nonzero value if an error occurred.

    int enqueueWithDeadline(const bsl::function<void()>&  job,
                            const bsls::TimeInterval&     deadline,
                            const bsl::function<void()>&  onTimeout);
        // Enqueue the specified 'job' to the executed by the next available
        // thread. Ensure that if the 'job' has not completed by the specified
        // 'deadline' (expressed as the absolute time from the epoch), the
        // specified 'onTimeout' is invoked. Return zero if the 'job' was
        // enqueued successfully and the timeout event was scheduled
        // successfully, or return a nonzero value if an error occurred.

    using bdlmt::ThreadPool::drain;
    using bdlmt::ThreadPool::enqueueJob;
    using bdlmt::ThreadPool::resetPercentBusy;

    // ACCESSORS
    using bdlmt::ThreadPool::maxThreads;
    using bdlmt::ThreadPool::maxIdleTime;
    using bdlmt::ThreadPool::minThreads;
    using bdlmt::ThreadPool::numActiveThreads;
    using bdlmt::ThreadPool::numPendingJobs;
    using bdlmt::ThreadPool::numWaitingThreads;
    using bdlmt::ThreadPool::percentBusy;
    using bdlmt::ThreadPool::threadFailures;
};

}  // close package namespace
}  // close enterprise namespace
#endif
