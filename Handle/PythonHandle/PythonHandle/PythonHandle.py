import ctypes
import mmap
import time
import random

class Point(ctypes.Structure):
    _fields_ = [("x", ctypes.c_int),
                ("y", ctypes.c_int)]

def send_continuous_points_to_shared_memory():
    try:
        # ���������ڴ����
        shm = mmap.mmap(-1, ctypes.sizeof(Point), "Local\\MFiveChessMemory")
        point = Point.from_buffer(shm)
        
        while True:
            # �����µĵ���Ϣ��ʾ����������ɣ�
            point.x = random.randint(1, 100)
            point.y = random.randint(1, 100)
            print("({}, {})".format(point.x, point.y))
            # ���Ʒ�������
            time.sleep(5)  # ÿ5�뷢��һ��
    
    except Exception as e:
        print(f"ERROR: {e}")
    
    finally:
        if shm:
            shm.close()

# ʾ�������������������Ϣ�������ڴ�
send_continuous_points_to_shared_memory()
