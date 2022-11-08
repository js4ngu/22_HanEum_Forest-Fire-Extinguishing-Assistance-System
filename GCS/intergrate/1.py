import math
import csv
import os
import geopy
import geopy.distance
import pyautogui
import time
import serial  # pyserial 모듈 불러오기

BASE_LAT = 37.6645
BASE_LON = 126.7417
HEIGHT1  = 193.54
OPEN_FILE_PATH = "C:\\Users\\SEP16\\Desktop\\22_HanEum_Forest-Fire-Extinguishing-Assistance-System\\MP\\1.tsv"
FILE_PATH = "C:\\Users\\SEP16\\Desktop\\22_HanEum_Forest-Fire-Extinguishing-Assistance-System\\MP\\"


def calc_Cord(lat_ORG, lon_ORG ,bearing, mDistance):
    distance = 0.001 * mDistance
    origin = geopy.Point(lat_ORG, lon_ORG)
    destination = geopy.distance.distance(kilometers=distance).destination(origin, bearing)

    lat_TGT, lon_TGT = destination.latitude, destination.longitude
    return lat_TGT, lon_TGT

def calc_Distance(pitch, Lidar):
    theta = 90 - pitch
    return Lidar * math.cos(math.pi * (theta / 180))

def writeWP(data):
    ####### 미션 플래너에서 사용가능한 파일 만들기
    f=open(OPEN_FILE_PATH,'w',encoding='utf-8',newline='')
    wr=csv.writer(f,delimiter='\t')
    wr.writerow(["QGC WPL 110"])
    wr.writerow([0,1,0,16,0,0,0,0,BASE_LAT,BASE_LON,HEIGHT1,1])
    wr.writerow([1,0,3,22,0.00000000,0.00000000,0.00000000,0.00000000,0,0,0.000000,1])
    wr.writerow([2,0,3,16,0.00000000,0.00000000,0.00000000,0.00000000,BASE_LAT,BASE_LON,3.000000,1])
    wr.writerow([3,0,3,183,10,2000,0,0,0,0,3,1])
    wr.writerow([4,0,3,21,0.00000000,0.00000000,0.00000000,0.00000000,data[0],data[1],3,1])
    f.close()
    oldname=os.path.join(FILE_PATH,"1.tsv")
    newname=os.path.join(FILE_PATH,"1.txt")
    os.rename(oldname,newname)

def removeWP():
    file_path = FILE_PATH + "1.txt"
    if os.path.exists(file_path):
        os.remove(file_path)

def hotkeyMP():
    pyautogui.click(134,102) #계획 클릭
    time.sleep(2)
    pyautogui.click(2771,413) #파일로드 클릭
    time.sleep(2)
    pyautogui.click(435,353) #파일선택
    time.sleep(2)
    pyautogui.click(1060,947) #파일열기
    time.sleep(2)
    pyautogui.click(2703,644) #쓰기
    time.sleep(2)

xbee = serial.Serial(                    # serial 객체 생성
    port='COM7',            # 시리얼통신에 사용할 포트
    baudrate=9600,                # 통신속도 지정
    parity=serial.PARITY_NONE,      # 패리티 비트 설정방식
    stopbits=serial.STOPBITS_ONE,   # 스톱비트 지정
    bytesize=serial.EIGHTBITS,      # 데이터 비트수 지정
    timeout=1                       # 타임아웃 설정
)

def readXbee(): 
    if xbee.readable():
        LINE = xbee.readline().decode('utf-8','ignore')
        LINE = str(LINE).split()
        PARSE = LINE
        return PARSE

    else :
        return False

def calc_Cord(lat_ORG, lon_ORG ,bearing, mDistance):
    distance = 0.001 * mDistance
    origin = geopy.Point(lat_ORG, lon_ORG)
    destination = geopy.distance.distance(kilometers=distance).destination(origin, bearing)

    lat_TGT, lon_TGT = destination.latitude, destination.longitude
    return lat_TGT, lon_TGT

def calc_Distance(pitch, Lidar):
    theta = 90 - pitch
    return Lidar * math.cos(math.pi * (theta / 180))

def main():
    while True:
        raw_DATA = readXbee()
        if len(raw_DATA) == 10 :
            print("RAW DATA : ",raw_DATA)
            lat1 = float(raw_DATA[0])
            lon1 = float(raw_DATA[1])
            lat2 = float(raw_DATA[2])
            lon2 = float(raw_DATA[3])
            lat3 = float(raw_DATA[4])
            lon3 = float(raw_DATA[5])
            pitch = float(raw_DATA[6])
            heading = float(raw_DATA[7])
            roll = float(raw_DATA[8])
            Lidar = int(raw_DATA[9]) * 10

            removeWP()
            distance = calc_Distance(pitch, Lidar)
            data = calc_Cord(lat1, lon1, heading, distance)
            print("Distance : ", distance)
            print("TGT LAT LON : ", data)
            writeWP(data)

main()