
#include <package_threadpoolutil.h>

#include <bdlt_currenttime.h>

namespace BloombergLP {
namespace package {

                     // -------------------------------
                     // class ThreadPoolUtil_WithCancel
                     // -------------------------------

// CREATORS
ThreadPoolUtil_WithCancel::ThreadPoolUtil_WithCancel(
        const bsl::function<void()>&               job,
        const bdlmt::EventScheduler::EventHandle&  event,
        bdlmt::EventScheduler                     *scheduler)
: d_job(job)
, d_event(event)
, d_scheduler_p(scheduler)
{
    BSLS_ASSERT_OPT(d_scheduler_p);
    BSLS_ASSERT_OPT(job);
}

// ACCESSORS
void ThreadPoolUtil_WithCancel::operator()() const
{
    d_job();
    
    BSLS_ASSERT(d_scheduler_p);
    d_scheduler_p->cancelEvent(d_event);
}


                     // --------------------------------
                     // class ThreadPoolUtil_WithTimeout
                     // --------------------------------

// CREATORS
ThreadPoolUtil_WithTimeout::ThreadPoolUtil_WithTimeout(
        const bsl::function<void()>&  job,
        const bsls::TimeInterval&     timeout,
        const bsl::function<void()>&  onTimeout,
        bdlmt::EventScheduler        *scheduler)
: d_job(job)
, d_timeout(timeout)
, d_onTimeout(onTimeout)
, d_scheduler_p(scheduler)
{
    BSLS_ASSERT_OPT(d_scheduler_p);
    BSLS_ASSERT_OPT(job);
}

// ACCESSORS
void ThreadPoolUtil_WithTimeout::operator()() const
{
    const bsls::TimeInterval deadline = bdlt::CurrentTime::now() + d_timeout;

    bdlmt::EventScheduler::EventHandle event;
    BSLS_ASSERT(d_scheduler_p);
    d_scheduler_p->scheduleEvent(&event, deadline, d_onTimeout);

    ThreadPoolUtil_WithCancel(d_job, event, d_scheduler_p)();
}

}  // close package namespace
}  // close enterprise namespace
