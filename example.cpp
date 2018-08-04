
#include <package_timedthreadpool.h>

#include <bdlt_currenttime.h>

#include <bslmt_threadutil.h>

#include <bsl_iostream.h>

using namespace BloombergLP;
using namespace package;

bsls::TimeInterval seconds(double howManySeconds)
{
    return bsls::TimeInterval(howManySeconds);
}

void takeTooDamnedLong()
{
    bslmt::ThreadUtil::sleep(bsls::TimeInterval.addSeconds(5));
}

void complain()
{
    bsl::cout << "The thing is taking too damned long.\n";
}

void complainMore()
{
    bsl::cout << "The other thing is taking too damned long, too.\n";
}

int main()
{
    package::TimedThreadPool pool(
        bslmt::ThreadAttributes(),
        1,                          // minThreads
        4,                          // maxThreads
        seconds(10));               // maxIdleTime

    pool.enqueueWithTimeout(&takeTooDamnedLong,
                            seconds(2),
                            &complain);

    pool.enqueueWithDeadline(
        &takeTooDamnedLong,
        bdlt::CurrentTime::now() + seconds(2),
        &complainMore);

    bslmt::ThreadUtil::sleep(seconds(3));
}