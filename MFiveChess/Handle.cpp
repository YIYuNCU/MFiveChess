#include "pch.h"
#include "Handle.h"

SharedMemoryListener::SharedMemoryListener()
    : hMapFile(nullptr), mappedPoint(nullptr), hEvent(nullptr), stopListening(false) {}

SharedMemoryListener::~SharedMemoryListener()
{
    stopListeningThread(); // 确保停止监听线程
    cleanup();
}

bool SharedMemoryListener::initialize()
{
    hMapFile = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, L"Local\\MFiveChessMemory");
    if (hMapFile == nullptr)
    {
        return false;
    }

    mappedPoint = (Point*)MapViewOfFile(hMapFile, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, sizeof(Point));
    if (mappedPoint == nullptr)
    {
        AfxMessageBox(L"读取共享内存失败!");
        CloseHandle(hMapFile);
        hMapFile = nullptr;
        return false;
    }

    hEvent = CreateEvent(nullptr, FALSE, FALSE, L"Local\\MFiveChessEvent");
    if (hEvent == nullptr)
    {
        AfxMessageBox(L"创建事件对象失败!");
        CloseHandle(hMapFile);
        hMapFile = nullptr;
        return false;
    }

    return true;
}

void SharedMemoryListener::listenSharedMemoryChanges(Point& currentPoint)
{
    while (!stopListening)
    {
        DWORD dwWaitResult = WaitForSingleObject(hEvent, 1000); // 等待事件信号，每秒一次

        if (dwWaitResult == WAIT_OBJECT_0)
        {
            // 收到事件信号，可以继续处理共享内存中的数据
            if (receivePoint(currentPoint))
            {
                // 处理接收到的坐标点
                if (!(currentPoint.x == -2 && currentPoint.y == -4))
                {
                    // 如果不是特定的终止条件，继续处理
                }
            }
        }
    }
}

bool SharedMemoryListener::startListening(Point& currentPoint)
{
    if (!initialize())
    {
        return false;
    }
    listenerThread = std::thread(&SharedMemoryListener::listenSharedMemoryChanges, this, std::ref(currentPoint));
    return true;
}

void SharedMemoryListener::stopListeningThread()
{
    stopListening = true;
    if (listenerThread.joinable())
    {
        listenerThread.join();
    }
}

bool SharedMemoryListener::receivePoint(Point& point)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (mappedPoint)
    {
        point.x = mappedPoint->x;
        point.y = mappedPoint->y;
        return true;
    }
    return false;
}

void SharedMemoryListener::invalidatePoint()
{
    std::lock_guard<std::mutex> lock(mtx);

    if (mappedPoint)
    {
        mappedPoint->x = -1; // 设置为非法值
        mappedPoint->y = -1; // 设置为非法值      
    }
}

bool SharedMemoryListener::writePoint(const Point& newPoint)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (mappedPoint)
    {
        mappedPoint->x = newPoint.x;
        mappedPoint->y = newPoint.y;
        SetEvent(hEvent); // 设置事件信号，通知监听线程有新数据
        return true;
    }
    return false;
}

void SharedMemoryListener::cleanup()
{
    if (hEvent != nullptr)
    {
        CloseHandle(hEvent);
        hEvent = nullptr;
    }

    if (mappedPoint != nullptr)
    {
        UnmapViewOfFile(mappedPoint);
        mappedPoint = nullptr;
    }

    if (hMapFile != nullptr)
    {
        CloseHandle(hMapFile);
        hMapFile = nullptr;
    }
}
