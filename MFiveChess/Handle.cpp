#include "pch.h"
#include "Handle.h"

SharedMemoryListener::SharedMemoryListener()
    : hMapFile(nullptr), mappedPoint(nullptr), hEvent(nullptr), stopListening(false) {}

SharedMemoryListener::~SharedMemoryListener()
{
    stopListeningThread(); // ȷ��ֹͣ�����߳�
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
        AfxMessageBox(L"��ȡ�����ڴ�ʧ��!");
        CloseHandle(hMapFile);
        hMapFile = nullptr;
        return false;
    }

    hEvent = CreateEvent(nullptr, FALSE, FALSE, L"Local\\MFiveChessEvent");
    if (hEvent == nullptr)
    {
        AfxMessageBox(L"�����¼�����ʧ��!");
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
        DWORD dwWaitResult = WaitForSingleObject(hEvent, 1000); // �ȴ��¼��źţ�ÿ��һ��

        if (dwWaitResult == WAIT_OBJECT_0)
        {
            // �յ��¼��źţ����Լ����������ڴ��е�����
            if (receivePoint(currentPoint))
            {
                // ������յ��������
                if (!(currentPoint.x == -2 && currentPoint.y == -4))
                {
                    // ��������ض�����ֹ��������������
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
        mappedPoint->x = -1; // ����Ϊ�Ƿ�ֵ
        mappedPoint->y = -1; // ����Ϊ�Ƿ�ֵ      
    }
}

bool SharedMemoryListener::writePoint(const Point& newPoint)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (mappedPoint)
    {
        mappedPoint->x = newPoint.x;
        mappedPoint->y = newPoint.y;
        SetEvent(hEvent); // �����¼��źţ�֪ͨ�����߳���������
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
