import math
import geopy
import geopy.distance
import csv
import os
import pyautogui 
import time

"""
명진당 : 37.2221 / 127.1885
5공학관 : 37.2221 / 127.1876
명진당 ~ 5공 : 79.69 / 269.9997
"""
def calc_Cord(lat_ORG, lon_ORG ,bearing, mDistance):
    distance = 0.001 * mDistance
    
    origin = geopy.Point(lat_ORG, lon_ORG)
    destination = geopy.distance.distance(kilometers=distance).destination(origin, bearing)

    lat_TGT, lon_TGT = destination.latitude, destination.longitude
    return lat_TGT, lon_TGT

def calc_Distance(pitch, Lidar):
    theta = 90 - pitch
    return Lidar * math.cos(math.pi * (theta / 180))

def make_file():
    ####### 미션 플래너에서 사용가능한 파일 만들기
    f=open("D:\\mission planner\\1.tsv",'w',encoding='utf-8',newline='')
    wr=csv.writer(f,delimiter='\t')
    wr.writerow(["QGC WPL 110"])
    wr.writerow([0,1,0,16,0,0,0,0,fat[0],long[0],190.257100,1])
    wr.writerow([1,0,3,21,0.00000000,0.00000000,0.00000000,0.00000000,fat[1],long[1],0.000000,1])
    wr.writerow([2,0,3,20,0.00000000,0.00000000,0.00000000,0.00000000,fat[2],long[2],height[0],1])
    f.close()
    oldname=os.path.join("D:\\mission planner\\","1.tsv")
    newname=os.path.join("D:\\mission planner\\","1.txt")
    os.rename(oldname,newname)

def GSC_hotkey():
    ###### 미션 플래너 핫키
    # print(pyautogui.position()) #화면위치 측정
    pyautogui.doubleClick(1142,88) # 미션 플래너 실행
    time.sleep(10) # 대기시간(조정가능)
    pyautogui.click(103,54) #계획 클릭
    time.sleep(10)
    pyautogui.click(1708,269) #파일로드 클릭
    time.sleep(10)
    pyautogui.click(390,212) #파일선택
    time.sleep(10)
    pyautogui.click(769,591) #파일열기
    time.sleep(10)
    pyautogui.click(1725,406) #쓰기
    time.sleep(10)

def delete_file():
    ###### 파일 삭제(파일 위치 고정을 위해서)
    file_path = "D:\\mission planner\\1.txt"
    if os.path.exists(file_path):
        os.remove(file_path)

def main():
    ####### 센서에서 추출된 gps정보 가정
    fat=[37.2192198,37.21925680,37.21922550]
    long=[127.1847802,127.18478990,127.18481270]
    height=[8]
    #Parsing Data Area
    lat_org = 37.2221
    lon_org = 127.1885
    heading = 269.9997 #편각 보정 함수 적용 예정 -> 위도에 따라 선형하게 편각
    pitch = 30
    Lidar = 79.69 * 2
    
    #calc new data
    distance = calc_Distance(pitch, Lidar)