from PyQt5.QtWidgets import QApplication
#from window import GomokuWindow
from game import Gomoku
from link import open_shared_memory , SharedMemoryListener ,close_shared_memory
import sys


def main():
    # g = Gomoku()
    # g.play()
    print(f"开启内存空间")
    if(open_shared_memory()):
        print(f"成功开启")
    print(f"尝试开启AI")
    # 创建并启动共享内存监听线程
    listener_thread = SharedMemoryListener()
    listener_thread.start()
    print(f"开启成功")
    app = QApplication(sys.argv)
    #ex = GomokuWindow()
    sys.exit(app.exec_())
    '''send_point_to_shared_memory()'''

if __name__ == '__main__':
    main()
