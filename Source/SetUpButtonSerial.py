import serial
import pyautogui
import subprocess
import webbrowser
# Mở cổng COM
ser = serial.Serial('COM5', 9600)
# Đường dẫn đến ứng dụng
app_path = "đường dẫn đến ứng dụng của bạn"

# Đường dẫn đến trang web
web_path = "đường dẫn đến trang web của bạn"
while True:
    if ser.in_waiting > 0:
        value = ser.readline().decode().strip() # Đọc giá trị từ cổng COM
        if value == '0':
            pyautogui.hotkey('ctrl', 'c')  # Copy
        elif value == '1':
            pyautogui.hotkey('ctrl', 'v')  # Paste
        elif value == '2':
           subprocess.Popen(app_path)
        elif value == '3':
            webbrowser.open(web_path)
        elif value == '4':
            pyautogui.hotkey('volumemute') # Mute
        elif value == '5':
            pyautogui.hotkey('volumemute') # Unmute (cùng hotkey với Mute)
        elif value == 'X':
            pyautogui.hotkey('volumedown') # Unmute (cùng hotkey với Mute)
        elif value == 'L':
            pyautogui.hotkey('volumeup') # Unmute (cùng hotkey với Mute)
        elif value == 'T':
            pyautogui.hotkey('ctrl', '-') # Unmute (cùng hotkey với Mute)
        elif value == 'P':
            pyautogui.hotkey('ctrl', '+') # Unmute (cùng hotkey với Mute)