#pragma once

#include <thread>
#include <mutex>

struct Point
{
    int x;
    int y;
};

class SharedMemoryListener
{
public:
    SharedMemoryListener();
    ~SharedMemoryListener();
    /// <summary>
    /// 用于创建装载有判断共享内存是否变化的函数的线程
    /// </summary>
    /// <param name="currentPoint">将变化后的信息写入该变量</param>
    /// <returns>是否开启成功</returns>
    bool startListening(Point& currentPoint);
    /// <summary>
    /// 用于关闭共享内存监听线程
    /// </summary>
    void stopListeningThread();
    /// <summary>
    /// 用于将共享内存中的点初始化（写入非法值）
    /// </summary>
    void invalidatePoint();
    /// <summary>
    /// 用于向共享内存中写入点
    /// </summary>
    /// <param name="newPoint">要写入的点的信息</param>
    /// <returns>是否写入成功</returns>
    bool writePoint(const Point& newPoint);
    /// <summary>
    /// 用于关闭共享内存，关闭线程
    /// </summary>
    void cleanup();
    bool receivePoint(Point& point);
private:
    /// <summary>
    /// 用于存放共享内存的句柄
    /// </summary>
    HANDLE hMapFile;
    /// <summary>
    /// 用于存放共享内存的地址
    /// </summary>
    Point* mappedPoint;
    /// <summary>
    /// 用于存放事件句柄
    /// </summary>
    HANDLE hEvent;
    /// <summary>
    /// 用于创建监听者线程
    /// </summary>
    std::thread listenerThread;
    /// <summary>
    /// 用于创建互斥锁
    /// </summary>
    std::mutex mtx;
    /// <summary>
    /// 用于记录是否停止监听，方便线程判断
    /// </summary>
    bool stopListening;
    /// <summary>
    /// 用于初始化内存空间
    /// </summary>
    /// <returns></returns>
    bool initialize();
    /// <summary>
    /// 用于判断共享内存是否发生变化
    /// </summary>
    /// <param name="currentPoint">如果变化则写入该点中</param>
    void listenSharedMemoryChanges(Point& currentPoint);
    
};
