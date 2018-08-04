#include <package_timedthreadpool.h>
#include <package_threadpoolutil.h>

#include <bsls_timeinterval.h>

namespace BloombergLP {
namespace package {

                         // ---------------------
                         // class TimedThreadPool
                         // ---------------------

// CREATORS
TimedThreadPool::TimedThreadPool(
    const bslmt::ThreadAttributes&  threadAttributes,
    int                             minThreads,
    int                             maxThreads,
    const bsls::TimeInterval&       maxIdleTime,
    bslma::Allocator               *allocator)
: bdlmt::ThreadPool(threadAttributes,
                    minThreads,
                    maxThreads,
                    maxIdleTime.totalMilliseconds(),
                    allocator)
, d_eventScheduler(allocator)
{
    start();
    d_eventScheduler.start();
}

TimedThreadPool::~TimedThreadPool()
{
    stop();
    d_eventScheduler.stop();
}

// MANIPULATORS
int TimedThreadPool::enqueueWithTimeout(
    const bsl::function<void()>& job,
    const bsls::TimeInterval&    timeout,
    const bsl::function<void()>& onTimeout)
{
    return ThreadPoolUtil::enqueueWithTimeout(
        this, &d_eventScheduler, job, timeout, onTimeout);
}

int TimedThreadPool::enqueueWithDeadline(
    const bsl::function<void()>& job,
    const bsls::TimeInterval&    deadline,
    const bsl::function<void()>& onTimeout)
{
    return ThreadPoolUtil::enqueueWithDeadline(
        this, &d_eventScheduler, job, deadline, onTimeout);
}

}  // close package namespace
}  // close BloombergLP namespace
