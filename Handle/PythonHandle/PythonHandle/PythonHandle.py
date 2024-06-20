import ctypes
import mmap
import time
import random

class Point(ctypes.Structure):
    _fields_ = [("x", ctypes.c_int),
                ("y", ctypes.c_int)]

def send_continuous_points_to_shared_memory():
    try:
        # 创建共享内存对象
        shm = mmap.mmap(-1, ctypes.sizeof(Point), "Local\\MFiveChessMemory")
        point = Point.from_buffer(shm)
        
        while True:
            # 生成新的点信息（示例：随机生成）
            point.x = random.randint(1, 100)
            point.y = random.randint(1, 100)
            print("({}, {})".format(point.x, point.y))
            # 控制发送速率
            time.sleep(5)  # 每5秒发送一次
    
    except Exception as e:
        print(f"ERROR: {e}")
    
    finally:
        if shm:
            shm.close()

# 示例：持续发送随机点信息到共享内存
send_continuous_points_to_shared_memory()
