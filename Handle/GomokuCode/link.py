import ctypes
import mmap
import time
import threading
from game import Gomoku
import sys

ISMAI = False

if ISMAI:
    from m_ai import FiveChessAI
    mai = FiveChessAI()
class Point(ctypes.Structure):
    _fields_ = [("x", ctypes.c_int),
                ("y", ctypes.c_int)]

# 全局变量，用于存储共享内存对象和控制读写的标志
shm = None
enable_send = False
enable_get = True
enable_repentance = False
obj = Gomoku()
# 定义全局锁对象
write_lock = threading.Lock()

# 判断是否允许读取或写入
def judging_read(x , y):
    global enable_get
    if x == -1 and y == -1 and not enable_get:
        enable_get = True
        write_point_to_shared_memory(-2, -2)
def judging_win(x , y):
    if x == -1 and y == 0:
        print(f"游戏结束")
        check_and_close_shared_memory()
        return True
    else:
        return False
def judging_repentance(x , y):
    global enable_repentance, enable_get, enable_send
    x, y = read_point_from_shared_memory()
    
    if x == -1 and y == 1 and not enable_repentance:
        enable_repentance = True
        enable_get = False
        enable_send = False
        write_point_to_shared_memory(-2, 1)
        print(f"等待悔棋点")
        return False
    elif x == -1 and y == 2 and not enable_repentance:
        enable_repentance = True
        enable_get = False
        enable_send = False
        write_point_to_shared_memory(-2, 3)
        print(f"等待悔棋点")
        return True

    return False  # 如果以上条件都不满足，则返回 False
def judging_write(x , y):
    global enable_send
    x, y = read_point_from_shared_memory()
    if x == -1 and y == -2 and not enable_send:
        enable_send = True
def judging_err(x , y):
    x, y = read_point_from_shared_memory()
    if x == -3 and y == -3 :
        #close_shared_memory()
        return True
    return False

# 打开共享内存并初始化
def open_shared_memory():
    global shm
    try:
        # 创建共享内存对象
        shm = mmap.mmap(-1, ctypes.sizeof(Point), "Local\\MFiveChessMemory")
        write_point_to_shared_memory(-2, -4)
        return True
    except Exception as e:
        print(f"Error opening shared memory: {e}")
        return False

# 写入共享内存
def write_point_to_shared_memory(x, y):
    if shm is None:
        print("Shared memory not opened.")
        return False
    
    try:
        with write_lock:  # 使用互斥锁保护写入操作
            point = Point.from_buffer(shm)
            point.x = ctypes.c_int(x)  # 将Python整数转换为ctypes.c_int
            point.y = ctypes.c_int(y)  # 将Python整数转换为ctypes.c_int
            shm.flush()  # 确保数据被刷新到共享内存
            return True
    
    except Exception as e:
        print(f"Error writing to shared memory: {e}")
        return False

# 读取共享内存内容
def read_point_from_shared_memory():
    if shm is None:
        print("Shared memory not opened.")
        return None, None
    
    try:
        point = Point.from_buffer(shm)
        return point.x, point.y
    
    except Exception as e:
        print(f"Error reading from shared memory: {e}")
        return None, None

# 关闭共享内存
def close_shared_memory():
    global shm
    if shm:
        shm.close()
        shm = None
        print("Shared memory closed.")

# 定义一个线程类，用于执行共享内存监听任务
class SharedMemoryListener(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.daemon = True  # 设置线程为守护线程，使得主程序退出时，线程也会退出
    def run(self):
        global enable_get, enable_send , enable_repentance
        x0 = -2
        y0 = -4
        rep = False
        while True:
            x, y = read_point_from_shared_memory()  # 调用共享内存读取函数
            if judging_err(x , y) == True:
                print("出现错误")
                close_shared_memory()
                sys.exit(1)
            if x == -2:
                time.sleep(0.11)
                continue
            judging_read(x ,y)
            judging_write(x ,y)
            rep = judging_repentance(x , y)
            if judging_win(x , y) == True:
                sys.exit(1)
            if x == x0 or y == y0:
                time.sleep(0.11)
                continue
            x0 = x
            y0 = y
            if enable_repentance and x0 > 0 and y0 > 0:
                obj.delete_1step(x0 , y0)
                print(f"已删除({x0},{y0})")
                if rep == False and enable_repentance:
                    write_point_to_shared_memory(-2, 2)
                    enable_repentance = False
                    time.sleep(0.11)
                    continue
                elif enable_repentance:
                    write_point_to_shared_memory(-2, 4)
                    enable_repentance = False
                    enable_get = True
                    write_point_to_shared_memory(-2, 4)
                    time.sleep(0.11)
                    continue
            if enable_get and x0 >= 0 and y0 >= 0:
                if ISMAI:
                    mai.player_move( x0 , y0 )
                else:
                    obj.move_1step(True, x, y)
                write_point_to_shared_memory(-2, -1)
                enable_get = False
                time.sleep(0.5)
                if ISMAI:
                    x , y = mai.ai_move()
                else:
                    x, y = obj.ai_play_1step_py_python()
                write_point_to_shared_memory(x, y)
                count = 5
                a = -9
                b = -9
                while a != -1 and b != -1:
                    a, b = read_point_from_shared_memory()
                    count -= 1
                    if count == 0:
                        write_point_to_shared_memory(x, y)
                        count = 5
                    time.sleep(0.11)
            time.sleep(0.11)  # 每0.1秒钟检查一次共享内存

def check_and_close_shared_memory():
    global shm
    if shm:
        try:
            shm.close()
            shm = None
            print("Shared memory closed.")
            return True
        except Exception as e:
            print(f"Error closing shared memory: {e}")
            return False
    else:
        print("Shared memory not opened.")
        return False

