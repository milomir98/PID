from PyQt5 import QtWidgets,QtCore
from pyqtgraph import PlotWidget, plot
import pyqtgraph as pg
import sys  # We need sys so that we can pass argv to QApplication
import os
from ui import Ui_MainWindow
from random import randint
from serialth import ReadThread


class MainWindow(QtWidgets.QMainWindow):

    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)

        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        self.x = list(range(500))
        self.y = [0]*500
        self.y1 = [0]*500

        pen = pg.mkPen(color=(255, 0, 0))
        self.data_line =  self.ui.widget.plot(self.x, self.y, pen=pen)
        pen = pg.mkPen(color=(0, 255, 0))
        self.data_line2 =  self.ui.widget.plot(self.x, self.y, pen=pen)

        #self.ui.widget.setYRange(0,200)

        self.ui.kd_v.sliderReleased.connect(self.pasteValue_kd)
        self.ui.kp_v.sliderReleased.connect(self.pasteValue_kp)
        self.ui.ki_v.sliderReleased.connect(self.pasteValue_ki)
        self.ui.speed_v.sliderReleased.connect(self.pasteValue_speed)

        self.ui.kd_v.valueChanged.connect(self.sendValue_kd)
        self.ui.kp_v.valueChanged.connect(self.sendValue_kp)
        self.ui.ki_v.valueChanged.connect(self.sendValue_ki)
        self.ui.speed_v.valueChanged.connect(self.sendValue_speed)

        self.worker = ReadThread()
        self.worker.data_recv.connect(self.update_plot_data)
        self.worker.state.connect(self.update_terminal)
        self.worker.start()

        self.ui.lineEdit.returnPressed.connect(self.write_data)
    
    def pasteValue_kd(self):
        val = self.ui.kd_v.value()
        self.worker.write(chr(int(100)))
        if(val < 256):
            self.worker.write(chr(0))
        else:
            self.worker.write(chr(val>>8))

        self.worker.write(chr(val & 0xFF))
        

    def pasteValue_ki(self):
        val = self.ui.ki_v.value()
        self.worker.write(chr(int(105)))
        if(val < 256):
            self.worker.write(chr(0))
        else:
            self.worker.write(chr(val>>8))

        self.worker.write(chr(val & 0xFF))

    def pasteValue_kp(self):
        val = self.ui.kp_v.value()
        self.worker.write(chr(int(112)))
        if(val < 256):
            self.worker.write(chr(0))
        else:
            self.worker.write(chr(val>>8))

        self.worker.write(chr(val & 0xFF))

    def pasteValue_speed(self):
        val = self.ui.speed_v.value()
        self.worker.write(chr(int(115)))
        if(val < 256):
            self.worker.write(chr(0))
        else:
            self.worker.write(chr(val>>8))

        self.worker.write(chr(val & 0xFF))
    
    def sendValue_kd(self):
        self.ui.kd_le.setText(str(self.ui.kd_v.value()))

    def sendValue_ki(self):
        self.ui.ki_le.setText(str(self.ui.ki_v.value()))

    def sendValue_kp(self):
        self.ui.kp_le.setText(str(self.ui.kp_v.value()))

    def sendValue_speed(self):
        self.ui.speed_le.setText(str(self.ui.speed_v.value()))
    
    def write_data(self):
        a = (self.ui.lineEdit.text())
        
        if(self.ui.checkBox.isChecked()):
            self.worker.write(chr(int(a)))
        else:
            self.worker.write(a)
            #self.worker.write(a)
        self.ui.lineEdit.clear()

    def update_terminal(self, data):
        self.ui.plainTextEdit.appendPlainText(data)

    def update_plot_data(self, recv):
        
        self.x = self.x[1:]  # Remove the first y element.
        self.x.append(self.x[-1] + 1)  # Add a new value 1 higher than the last.

        self.y = self.y[1:]  # Remove the first 
        self.y.append( float(recv[0]))  # Add a new random value.

        self.data_line.setData(self.x, self.y)  # Update the data
        
        self.y1 = self.y1[1:]  # Remove the first 
        if(float(recv[1])>510):
            recv[1]='0'
        self.y1.append(float(recv[1]))  # Add a new random value.

        
        self.data_line2.setData(self.x, self.y1)

def main():
    app = QtWidgets.QApplication(sys.argv)
    main = MainWindow()
    main.show()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()