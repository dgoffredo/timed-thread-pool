![timed-thread-pool](timed-thread-pool.png)

Timed Thread Pool
=================
Invoke a function when a thread pool job is taking too long.

Why
---
Somebody was complaining that BDE's [bdlmt::ThreadPool][thread-pool] doesn't do
this.

What
----
`timed-thread-pool` is a template for two BDE-style components that
implement a thread pool, based on [bdlmt::ThreadPool][thread-pool], that
additionally allows each enqueued job to be associated with a timeout
(relative to the beginning of the job execution) or with a deadline (absolute),
after which if the job has not yet finished, a user-supplied callback will
be invoked.

How
---
### Installation
To "install" this repository's code into your BDE-style package, run
```console
$ ./rename-package <package-name> <path/to/package>
```
The `package_*` files will be transformed into files approriate for use in the
specified package. Then the transformed files will be copied into the specified
directory.

### Usage
See [example.cpp](example.cpp) for a facetious usage example.

More
----
### About the Implementation
`timed-thread-pool` is two components. The main component provides
[TimedThreadPool](package_timedthreadpool.h), which is the class used in the
[example](example.cpp).

There is a second component as well. The second component defines a utility
namespace [struct ThreadPoolUtil](package_threadpoolutil.h), which is used
in the implementation of `TimedThreadPool`. However, the utility functions
are public and parameterized on the type of the thread pool. So, the logic
of scheduling timeouts along with enqueued jobs can be reused for other
types of thread pools, such as [bdlmt::FixedThreadPool][fixed].

[thread-pool]: https://bloomberg.github.io/bde/group__bdlmt__threadpool.html
[fixed]: https://bloomberg.github.io/bde/group__bdlmt__fixedthreadpool.html