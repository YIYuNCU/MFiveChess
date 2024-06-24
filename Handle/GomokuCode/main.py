from game import Gomoku
from link import open_shared_memory , SharedMemoryListener ,close_shared_memory
import sys
from m_UI import create_ui

def main():
    root, add_message = create_ui()
    if not(open_shared_memory()):
        sys.exit(1)
    # 创建并启动共享内存监听线程
    listener_thread = SharedMemoryListener()
    listener_thread.start()
    root.mainloop()
    sys.exit(0)
    '''send_point_to_shared_memory()'''

if __name__ == '__main__':
    main()
