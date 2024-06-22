from PyQt5.QtWidgets import QApplication
from window import GomokuWindow
from game import Gomoku
import sys
import mmap
import ctypes

class Point(ctypes.Structure):
    _fields_ = [("x", ctypes.c_int),
                ("y", ctypes.c_int)]

shm = None
shm = mmap.mmap(-1, ctypes.sizeof(Point), "Local\\MFiveChessMemory")

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
    
def main():
    check_and_close_shared_memory()
    
if __name__ == '__main__':
    main()
