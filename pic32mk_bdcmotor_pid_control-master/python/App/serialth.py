import sys
import time
from PyQt5 import QtWidgets, QtCore
from random import randint
import serial

class ReadThread(QtCore.QThread):
    data_recv = QtCore.pyqtSignal(list)
    state = QtCore.pyqtSignal(str)
    def __init__(self):
        super().__init__()
        self.val = ''
        self.x = ['','','']
        
        self.ser = serial.Serial('COM6', 115200)
        self.ser.close()
        self.ser.open()

    @QtCore.pyqtSlot()
    def run(self):
        while True:
            
            data = self.ser.read()
            #print(data)
            if(data == b's'):
                self.state.emit("start motor with PID [0 - 500] rpm")
            elif(data == b'P'):
                self.state.emit("Set Kp")
            elif (data == b'I'):
                self.state.emit("Set Ki")
            elif (data == b'D'):
                self.state.emit("Set Kd")
            elif (data == b'S'):
                self.state.emit("Stop")
            elif (data == b'M'):
                self.state.emit("Set speed manual R[0-100] L[128-228]")
            elif (data == b'G'):
                self.val = ''
                self.val = self.ser.readline()
                self.x = self.val.split()
                self.data_recv.emit(self.x)
            else:
                print('Ovde')
                print(data)

    def write(self, data):
        self.ser.write(data.encode())

    def terminate(self):
        self.ser.close()
