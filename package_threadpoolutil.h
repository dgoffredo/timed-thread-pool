#ifndef INCLUDED_PACKAGE_THREADPOOLUTIL
#define INCLUDED_PACKAGE_THREADPOOLUTIL

#include <bdlmt_eventscheduler.h>

#include <bsls_assert.h>
#include <bsls_timeinterval.h>

#include <bsl_functional.h>

namespace BloombergLP {
namespace package {

                           // =====================
                           // struct ThreadPoolUtil
                           // =====================

struct ThreadPoolUtil {
    // This utility 'struct' provides a namespace for a suite of functions
    // operating on generic thread pools.

    template <typename THREAD_POOL>
    static int enqueueWithTimeout(THREAD_POOL                  *threadPool,
                                  bdlmt::EventScheduler        *scheduler,
                                  const bsl::function<void()>&  job,
                                  const bsls::TimeInterval&     timeout,
                                  const bsl::function<void()>&  onTimeout);
        // Enqueue the specified 'job' to be executed by the next available
        // thread managed by the specified 'threadPool'. Use the specified
        // 'scheduler' to ensure that from the time the 'job' begins execution,
        // if execution has not completed within the specified 'timeout'
        // (expressed as a time interval relative to the start of execution),
        // the specified 'onTimeout' is invoked. Return zero if the 'job' was
        // enqueued successfullyy, or return a nonzero value if an error
        // occurred.

    template <typename THREAD_POOL>
    static int enqueueWithDeadline(THREAD_POOL                  *threadPool,
                                   bdlmt::EventScheduler        *scheduler,
                                   const bsl::function<void()>&  job,
                                   const bsls::TimeInterval&     deadline,
                                   const bsl::function<void()>&  onTimeout);
        // Enqueue the specified 'job' to the executed by the next available
        // thread managed by the specified 'threadPool'. Use the specified
        // 'scheduler' to ensure that if the 'job' has not completed by the
        // specified 'deadline' (expressed as the absolute time from the
        // epoch), the specified 'onTimeout' is invoked. Return zero if the
        // 'job' was enqueued successfully and the timeout event was scheduled
        // successfully, or return a nonzero value if an error occurred.
};

                     // ===============================
                     // class ThreadPoolUtil_WithCancel
                     // ===============================

class ThreadPoolUtil_WithCancel {
    // This component-private function-like object is used to cancel a timeout
    // event after executing the associated job.

    // DATA
    const bsl::function<void()>                     d_job;
    const bdlmt::EventScheduler::EventHandle        d_event;
    bdlmt::EventScheduler                    *const d_scheduler_p;

  public:
    // CREATORS
    explicit ThreadPoolUtil_WithCancel(
        const bsl::function<void()>&               job,
        const bdlmt::EventScheduler::EventHandle&  event,
        bdlmt::EventScheduler                     *scheduler);
        // Create a new 'ThreadPoolUtil_WithCancel' object that, when invoked,
        // will invoke the specified 'job' and then cancel the specified
        // 'event' on the specified 'scheduler'.

    // ACCESSORS
    void operator()() const;
        // Invoke the job contained within the object. Afterward, cancel the
        // event contained within this object on the scheduler contained within
        // this object.
};

                     // ================================
                     // class ThreadPoolUtil_WithTimeout
                     // ================================

class ThreadPoolUtil_WithTimeout {
    // This component-private function-like object is used to schedule a
    // timeout for a job to be invoked, and to cancel the timeout after
    // invocation.

    // DATA
    const bsl::function<void()>        d_job;
    const bsls::TimeInterval           d_timeout;
    const bsl::function<void()>        d_onTimeout;
    bdlmt::EventScheduler       *const d_scheduler_p;

  public:
    // CREATORS
    explicit ThreadPoolUtil_WithTimeout(
        const bsl::function<void()>&  job,
        const bsls::TimeInterval&     timeout,
        const bsl::function<void()>&  onTimeout,
        bdlmt::EventScheduler        *scheduler);
        // Create a new 'ThreadPoolUtil_WithTimeout' object that, when invoked,
        // will invoke the specified 'job' after scheduling the specified
        // 'onTimeout' to execute after the specified 'timeout' using the
        // specified 'scheduler', and then cancel the timeout once the 'job'
        // has completed.

    // ACCESSORS
    void operator()() const;
        // Schedule the timeout contained within this object using the
        // scheduler contained within this object. Then invoke the job
        // contained within this object before finally canceling the timeout.
};

// ============================================================================
//                      INLINE FUNCTION DEFINITIONS
// ============================================================================

                        // ---------------------
                        // struct ThreadPoolUtil
                        // ---------------------

template <typename THREAD_POOL>
int ThreadPoolUtil::enqueueWithTimeout(
    THREAD_POOL                  *threadPool,
    bdlmt::EventScheduler        *scheduler,
    const bsl::function<void()>&  job,
    const bsls::TimeInterval&     timeout,
    const bsl::function<void()>&  onTimeout)
{
    BSLS_ASSERT_OPT(threadPool);
    BSLS_ASSERT_OPT(scheduler);

    return threadPool->enqueueJob(
        ThreadPoolUtil_WithTimeout(job, timeout, onTimeout, scheduler));
}

template <typename THREAD_POOL>
int ThreadPoolUtil::enqueueWithDeadline(
    THREAD_POOL                  *threadPool,
    bdlmt::EventScheduler        *scheduler,
    const bsl::function<void()>&  job,
    const bsls::TimeInterval&     deadline,
    const bsl::function<void()>&  onTimeout)
{
    BSLS_ASSERT_OPT(threadPool);
    BSLS_ASSERT_OPT(scheduler);

    bdlmt::EventScheduler::EventHandle event;
    scheduler->scheduleEvent(&event, deadline, onTimeout);

    if (event == bdlmt::EventScheduler::EventHandle())
        return 1;

    return threadPool->enqueueJob(
        ThreadPoolUtil_WithCancel(job, event, scheduler));
}

}  // close package namespace
}  // close enterprise namespace

#endif
