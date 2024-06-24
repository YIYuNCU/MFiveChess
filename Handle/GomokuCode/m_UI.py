import tkinter as tk
from tkinter import Scrollbar

def create_ui():
    root = tk.Tk()
    root.title("Resizable UI with Scrollbar Example")

    # 设置窗口的最小尺寸为 300x200
    root.minsize(300, 200)
    root.maxsize(300, 250)

    # 创建主框架
    main_frame = tk.Frame(root)
    main_frame.pack(fill=tk.BOTH, expand=True)

    # 创建滚动条
    scrollbar = Scrollbar(main_frame)
    scrollbar.pack(side=tk.RIGHT, fill=tk.Y)

    # 创建文本框用于显示累计信息，并绑定滚动条
    text = tk.Text(main_frame, wrap=tk.WORD, yscrollcommand=scrollbar.set)
    text.pack(fill=tk.BOTH, expand=True)

    # 将滚动条与文本框关联
    scrollbar.config(command=text.yview)

    # 定义消息添加函数
    def add_message(message):
        text.insert(tk.END, message + "\n")

    return root, add_message

if __name__ == "__main__":
    root, add_message = create_ui()
    root.mainloop()
