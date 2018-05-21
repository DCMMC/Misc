/**
* OS Lab3
* Process Scheduling
* ! 现代 Linux 中必须要 root 权限才能使用自定义调度算法
*/
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifndef PTHREAD_DEMO
// #define PTHREAD_DEMO
#endif

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <string.h> // strerror()
#include <utility> 
#include <string>
#include <vector>
#include <queue>

using namespace std;

/**
 * 进程控制块(Process Control Block)
 */
typedef struct PCB
{
    // process id
    string id;
    // states: ready, running, blocked
    // 进程的初始状态都是 ready
    enum stateEnum
    {
        ready,
        running,
        blocked
    };
    int state;
    // scheduling type
    enum scheEnum
    {
        PRIO,
        RR
    };
    int sche;
    // priority
    int priority;

    // 进程到达时间(模拟)
    int comeTime;
    // 已经服务时间(模拟)
    int serverTime;
    // 完成时间(模拟)
    int finishTime;
    // 需要服务的时间(模拟)
    int needTime;

    // 下一个 PCB 的指针, 将队指放在链表里面
    struct PCB *next;
} PCB;

// 互斥对象  
pthread_mutex_t mutex_demo = PTHREAD_MUTEX_INITIALIZER; 

void *thread_function_demo(void *data)
{
    pid_t pid = getpid();
    pthread_t tid = pthread_self();
    // sched policy
    int policy;
    int status;
    pthread_attr_t attr;
    if( (status = pthread_getattr_np(tid, &attr)) ) {
        cerr << "pthread_getattr_np failed: " << strerror(status) << endl;
    }
    if ( (status = pthread_attr_getschedpolicy(&attr, &policy)) ) {
        cerr << "pthread_attr_getschedpolicy failed: " << strerror(status);
    }
    string policy_str;
    switch (policy) {
        case SCHED_RR: policy_str = "SCHED_RR"; break;
        case SCHED_BATCH: policy_str = "SCHED_BATCH"; break;
        case SCHED_FIFO: policy_str = "SCHED_FIFO"; break;
        case SCHED_OTHER: policy_str = "SCHED_OTHER"; break;
        default: policy_str = "SCHED_not_sense";
    }

    // 加锁  
    if ( (status = pthread_mutex_lock(&mutex_demo)) ) {
        cerr << "pthread_mutex_lock failed: " << strerror(status) << endl;
        exit(status);
    }

    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    cout << "process " << pid << " thread" << tid << "(using policy: " << policy_str <<
        ") start running @"
        << buf << "\n" << endl;
    // sleep 2s
    sleep(2);
    now = time(0);
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    cout << "process " << pid << " thread " << tid << "(using policy: " << policy_str
        << " finish running @"
        << buf << "\n" << endl;

    // 释放互斥锁  
    if ( (status = pthread_mutex_unlock(&mutex_demo)) ) {
        cerr << "pthread_mutex_unlock failed: " << strerror(status) << endl;
    }
    
    pthread_exit((void *)0);
}

/**
 * test entry
 */
int main(void)
{
#ifdef PTHREAD_DEMO
    /**
     * pthread demo
     * 五个线程, 使用实时调度算法 RR, 设置优先级为 max / 2, 并且设置线程都是 joinable
     */
    pthread_t tid[5];
    pthread_attr_t attr;
    // get the default attributes
    pthread_attr_init(&attr);
    int status;
    if ( (status = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE)) ) {
        cerr << "pthread_attr_setdetachstate failed: " << strerror(status) << endl;
    }
    // 初始化互斥对象, 这样有一个线程在使用 CPU 并且没有结束的时候其他本程序 fork 出来的线程都被堵塞
    if ( (status = pthread_mutex_init(&mutex_demo, NULL)) ) {
        cerr << "pthread_mutex_init failed: " << strerror(status) << endl;
    }
    // RR 调度算法下的最大优先级
    auto max_prio = sched_get_priority_max(SCHED_RR);
    struct sched_param params;
    if ( (status = pthread_attr_getschedparam (  
         &attr, &params)) ) {
             cerr << "pthread_attr_getschedparam failed: " << strerror(status) << endl;
    }
    params.sched_priority = max_prio / 2;
    
    // set the scheduling algorithm to PROCESS or SYSTEM
    //  按照 POSIX 标准规定, SYSTEM 表明该 thread 将会跟相同 scheduling allocation
    // domain 的所有 thread (包括不属于当前 process 的其他 threads) 竞争所有资源
    // 而 PROCESS 表明 thread 只跟相同 process 的 threads 竞争.
    // pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    // ! 不继承父线程的调度策略, 如果是继承父线程的调度策略, 则 attr 和 param 中的内容都会被
    // ! 忽略, 并且设置优先级和自定义调度策略在 现代 Linux 中对普通用户都是默认禁止的, 必须使用 root 权限
    // ! 或者更改 /etc/security/limits.conf
    // ! ref: https://stackoverflow.com/questions/10704983/opera
    // ! tion-not-permitted-while-setting-new-priority-for-thread
    if ( (status = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED)) ) {
        cerr << "pthread_attr_setinheritsched failed: " << strerror(status) << endl;
    }
    /* 设置子线程为RR调度策略, 还有 SCHED_FIFO and SCHED_OTHER */
    // By default, user tasks in Linux have the scheduling policy SCHED_OTHER(Time Sharing)
    // 而且 RR 和 FIFO 一般是用来做 real-time 调度
    if ( (status = pthread_attr_setschedpolicy(&attr, SCHED_RR)) ) {
        cerr << "pthread_attr_setschedpolicy failed: " << strerror(status) << endl;
    }
    /* 设置优先级为 max */
    if ( (status = pthread_attr_setschedparam(&attr, &params)) ) {
        cerr << "pthread_attr_setschedparam failed: " << strerror(status) << endl;
    }

    // create the threads
    for (int i = 0; i < 5; i++) {
        status = pthread_create(&tid[i], &attr, thread_function_demo, NULL);
        if (status) {
            cerr << "Cannot create thread " << i << ": " << strerror(status)
            << ", maybe you should use 'root' to continue..." << endl;
        }
    }

    if (status) {
        pthread_mutex_destroy(&mutex_demo);  
        pthread_attr_destroy(&attr);
        exit(-1);
    }

    // now join(wait) on each thread
    for (int i = 0; i < 5; i++) {
        int status = pthread_join(tid[i], NULL);
        if (status != 0) {
            cerr << "Error join" << endl;
        }
    }

    pthread_mutex_destroy(&mutex_demo);
    pthread_attr_destroy(&attr);
#endif

    /**
     * OS_Lab 3
     * 使用非抢占式
     * 因为实在不好在一个已经受 OS 调度的环境中再去仿真真实的调度(如果要的话, 必须添加 linux module, 或者
     * 用汇编关闭时钟中断, pthread 使用实时最高级优先级, 自己接管调度, 汇编来进行上下文切换)
     */
    cout << "Type the algorithm: (priority/roundrobin)\n";
    string algoName;
    cin >> algoName;
    cout << "input name and need time(<= 50) :(\":q to indicates the end\")\n";
    string name;

    vector<pair<string, size_t>> tasks;
    while ((cin >> name) && name != ":q") {
        size_t needTime;
        cin >> needTime;

        tasks.push_back(make_pair(name, needTime));
    }

    if (algoName == "priority") {
        auto comp = [](const PCB * a, const PCB * b) {
            return a->priority < b->priority;
        };

        priority_queue<PCB *, vector<PCB *>, decltype(comp)> pq(comp);
        for (auto i : tasks) {
            auto p = new PCB();
            p->id = i.first;
            p->comeTime = 0;
            p->sche = PCB::PRIO;
            p->needTime = i.second;
            p->serverTime = 0;
            p->state = PCB::ready;
            p->priority = 50 - i.second;
            pq.push(p);
        }

        cout << "output of " << algoName << endl;
        cout << "NAME CPUTIME NEEDTIME PRIORITY STATE\n";
        // CPU 用时
        int time_count = 0;
        while (!pq.empty()) {
            auto p = pq.top();
            p->priority -= 3;
            p->serverTime++;
            time_count++;
            p->needTime--;
            if (p->needTime > 0) {
                p->state = PCB::blocked;
                pq.pop();
                pq.push(p);
                cout << p->id << "\t" << time_count << "\t" << p->needTime <<
                    "\t" << p->priority << "\tblocked\n";
            } else {
                // 完成
                p->finishTime = time_count;
                cout << p->id << "\t" << time_count << "\t" << p->needTime << "\t"
                    << p->priority << "\tfinished\n";
                pq.pop();
            }
        }
    } else if (algoName == "roundrobin") {
        // 就绪队列
        queue<PCB *> readyQ;
        // 到达时间都一样为 0, 如果到达时间不一样, 还需要像给 tasks 按照到达时间排序
        for (auto i : tasks) {
            auto p = new PCB();
            p->id = i.first;
            p->sche = PCB::RR;
            p->state = PCB::ready;
            p->comeTime = 0;
            p->needTime = i.second;
            p->serverTime = 0;
            // priority 都为 0
            p->priority = 0;
            readyQ.push(p);
        }

        cout << "ouput of " << algoName << endl;
        cout << "NAME CPUTIME NEEDTIME COUNT ROUND STATE\n";
        // CPU 运行时间
        int time_count = 0;
        while (!readyQ.empty()) {
            auto p = readyQ.front();
            time_count += (p->needTime == 1 ? 1 : 2);
            p->serverTime += (p->needTime == 1 ? 1 : 2);
            p->needTime -= (p->needTime == 1 ? 1 : 2);
            if (p->needTime == 0) {
                // finished
                p->finishTime = time_count;
                cout << p->id << "\t" << time_count << "\t" << p->needTime << "\t"
                    << readyQ.size() << "\tfinished\n";
                readyQ.pop();
            } else {
                p->state = PCB::blocked;
                cout << p->id << "\t" << time_count << "\t" << p->needTime << "\t"
                    << readyQ.size() << "\tblocked\n";
                readyQ.pop();
                readyQ.push(p);
            }
        }

    } else {
        cerr << "Wrong scheduling name: " << algoName << endl;
    }

    return 0;
} ///~