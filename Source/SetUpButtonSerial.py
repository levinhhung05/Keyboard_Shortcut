import pyautogui
import subprocess
import webbrowser
import serial
import os

#Connect to COM port
ser = serial.Serial('COM4', 9600)

def open_path(x):
    if x.startswith("http"):
        webbrowser.open(x)
    else:
        os.startfile(x)

path = r"D:/Dai_hoc/TKHTN"
url = "https://mybk.hcmut.edu.vn/my/index.action"

pyautogui.PAUSE = 0
actions = {
    "0": lambda: pyautogui.hotkey('ctrl', 'c'),
    "1": lambda: pyautogui.hotkey('ctrl', 'v'),
    "2": lambda: pyautogui.hotkey('ctrl', 'a'),
    "3": lambda: pyautogui.hotkey('win', 'printscreen'),
    
    "4": lambda: pyautogui.hotkey('ctrl','b'),
    "5": lambda: pyautogui.hotkey('ctrl','i'),
    "6": lambda: pyautogui.hotkey('ctrl','e'),
    "7": lambda: pyautogui.hotkey('ctrl','n'),
    
    "8": lambda: webbrowser.open(url),
    "9": lambda: open_path(path),
    ":": lambda: pyautogui.hotkey('alt','f4'),
    ";": lambda: pyautogui.hotkey('alt','z'),
    
    "<": lambda: pyautogui.hotkey('volumemute'),
    "=": lambda: open_path(r"C:/Windows/System32/calc.exe"),
    ">": lambda: open_path(r"C:/Windows/explorer.exe"),
    "?": lambda: webbrowser.open("https://www.youtube.com"),
    
    # Encoder
    "L": lambda: pyautogui.hotkey('volumeup'),
    "X": lambda: pyautogui.hotkey('volumedown'),
    "P": lambda: pyautogui.hotkey('ctrl', '+'),
    "T": lambda: pyautogui.hotkey('ctrl', '-'),
}

print("Ready...")

while True:
    if ser.in_waiting:
        v = ser.readline().decode().strip()
        print("Received:", v)

        if v in actions:
            actions[v]()
