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

    bool startListening(Point& currentPoint);
    void stopListeningThread();
    void invalidatePoint();
    bool writePoint(const Point& newPoint);
    void cleanup();
    bool receivePoint(Point& point);
private:
    HANDLE hMapFile;
    Point* mappedPoint;
    HANDLE hEvent;
    std::thread listenerThread;
    std::mutex mtx;
    bool stopListening;

    bool initialize();
    void listenSharedMemoryChanges(Point& currentPoint);
    
};
