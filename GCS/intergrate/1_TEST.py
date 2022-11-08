import math
import csv
import os
import geopy
import geopy.distance
import pyautogui
import time

"""
명진당 : 37.2221 / 127.1885
5공학관 : 37.2221 / 127.1876
명진당 ~ 5공 : 79.69 / 269.9997
킨텍스 : 37.6645 / 126.7417
"""

BASE_LAT = 37.6645
BASE_LON = 126.7417
HEIGHT1  = 193.54
OPEN_FILE_PATH = "C:\\Users\\SEP16\\Desktop\\22_HanEum_Forest-Fire-Extinguishing-Assistance-System\\MP\\1.tsv"
FILE_PATH = "C:\\Users\\SEP16\\Desktop\\22_HanEum_Forest-Fire-Extinguishing-Assistance-System\\MP\\"

xbee = serial.Serial(        # serial 객체 생성
    port='COM25',            # 시리얼통신에 사용할 포트
    baudrate=9600            # 통신속도 지정
)

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

def hotkeyHP():
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

def main():
    #Parsing Data Area
    #raw_data = xbee.readline()
    #data = parsing(raw_data)
    lat_org = BASE_LAT
    lon_org = BASE_LON+0.0005
    heading = 269.9997 #편각 보정 함수 적용 예정 -> 위도에 따라 선형하게 편각
    pitch = 30
    Lidar = 79.69 * 2 * 10
    
    removeWP()
    distance = calc_Distance(pitch, Lidar)
    data = calc_Cord(lat_org, lon_org, heading, distance)
    print(distance)
    print(data)
    writeWP(data)

main()