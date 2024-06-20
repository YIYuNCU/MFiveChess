#pragma once

struct Point {
    int x;
    int y;
};

class SharedMemoryListener {
private:
    HANDLE hMapFile;
    Point* mappedPoint;
    std::mutex mtx;
    std::thread listenerThread;
    bool stopListening;

    void listenSharedMemoryChanges();

public:
    SharedMemoryListener();
    ~SharedMemoryListener();

    bool initialize();
    bool startListening();
    void stopListeningThread();
    bool receivePoint(Point& point);
    void SharedMemoryListener::invalidatePoint();
};

