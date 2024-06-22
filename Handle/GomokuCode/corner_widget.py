from PyQt5.QtGui import QPainter, QPen
from PyQt5.QtWidgets import QWidget
from PyQt5.QtCore import Qt


class CornerWidget(QWidget):

    def __init__(self, parent):
        super().__init__(parent=parent)
        self.setFixedSize(38, 38)

    def paintEvent(self, e):
        qp = QPainter()
        qp.begin(self)
        pen = QPen(Qt.red, 3, Qt.SolidLine)
        qp.setPen(pen)
        qp.drawLine(0, 16, 0, 0)
        qp.drawLine(0, 0, 16, 0)
        qp.drawLine(30, 0, 36, 0)
        qp.drawLine(36, 0, 36, 16)
        qp.drawLine(36, 30, 36, 36)
        qp.drawLine(36, 36, 28, 36)
        qp.drawLine(16, 36, 0, 36)
        qp.drawLine(0, 36, 0, 30)
