# Overview

This project was developed 

# GUI Installation

To use python GUI developed for PID tuning you need to install some programs and packages on your PC.

First you'll need to have python3 installed.

## Install pip

on Windows:

```bash
python -m pip install -U pip
``` 

on Linux:

```bash
pip install -U pip
```

## Install PyQt5:

Graphical interface is written in python using qt5, so you need to install also PyQt5:

```bash
pip install PyQt5
```

## Install PyQtGraph

```bash
pip install pyqtgraph
```


## Install PySerial

```bash
pip install pyserial
```

If you are getting error that module serial doesnt exist use pip3 for installation:

```bash
pip3 install pyserial
```

# GSM commands

## Pokreni motor. - Turns motor ON.
## Zaustavi motor. - Turns motor OFF.
## Brzina __. - Sets the velocity of the motor.
