from pickle import TRUE
import pyautogui
import time

while TRUE:
    a = input()
    if a == 'a':
        print(pyautogui.position())