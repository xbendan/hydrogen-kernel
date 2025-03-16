#pragma once

#include <Meta/Array.h>
#include <System/Threading/Process.h>

namespace System::Threading {

    Process* createProcess(strview name);
    Process* createIdleProcess();
    Process* createKernelProcess();
    Process* createProcessEx(strview name);
    Process* destoryProcess(Process* process);
    Process* destoryProcess(u16 pid);
    Thread*  createThread(Process* process);
    Thread*  createThreadEx(Process* process, u8 priority, strview name);

    /*
     * Thread priority levels are defined as follows:
     * 31: Real-time
     * 30-25: Highest priority
     * 24-19: High priority
     * 18-13: Normal priority
     * 12-7: Low priority
     * 6-2: Lowest priority
     * 0: Idle thread
     *
     * The real-time priority level is reserved for the highest-priority
     * threads that require immediate attention. It will not be preempted by
     * any other thread or system scheduler. In another word, unless the thread
     * yields or blocks, it will run until it completes.
     *
     * The idle thread is a special thread that runs when no other threads are
     * ready to run. It is the lowest priority thread and will run only when no
     * other threads are ready to run.
     *
     * The priority levels are used to determine the order in which threads are
     * scheduled to run. The higher the priority level, the sooner the thread
     * will run. Theroetically, a thread with higher priority will usually run
     * before a thread with lower priority, but it is not guaranteed, because we
     * need to reserve some time for lower priority threads to run to avoid the
     * starvation problem.
     *
     * Process send signal to the scheduler to wake up the thread or call up an
     * immediate running to insert the thread to the front the ready queue. In
     * addition, if one processor is running a thread with lower priority, the
     * scheduler will preempt the thread immediately. The condition is:
     *      priority(thread) > priority(running thread) + 10
     */

    struct PriorityQueue
    {};

    /**
     * @brief Yield the current thread to the scheduler
     *
     * When a process or thread calls yield, it notifies the operating system
     * that it has finished its current work or does not need to continue
     * execution for the time being and is willing to relinquish the CPU.
     *
     * The scheduler of the operating system changes the state of the process or
     * thread from "Running" to "Ready".
     *
     * Yielding is a way to voluntarily give up the CPU, allowing other
     * processes or threads to run.
     * @return Res<>
     */
    void scheduleYield();

    /**
     * @brief Preempt current CPU
     *
     * This is called when current thread runs out of time slice. The task
     * scheduler preempts periodically.
     */
    void schedulePreempt();

    u8 setThreadPriority(NonnullRef<Thread> thread, u8 newPriority);
    u8 getThreadPriority(NonnullRef<Thread> thread);
    Array<NonnullRef<Thread>> getRunningThreads();
    Array<NonnullRef<Thread>> getReadyThreads();

    constexpr int getPriorityTimeSlice(u8 priority)
    {
        int slice = 100;
        slice *= 1 << (priority / 6);
        slice /= 8;
        return slice;
    }
}