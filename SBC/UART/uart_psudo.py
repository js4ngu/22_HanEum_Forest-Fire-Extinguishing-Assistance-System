import time
from turtle import distance          # 타임모듈 불러오기
import serial  # pyserial 모듈 불러오기
import math
import geopy
import geopy.distance

"""
명진당 : 37.2221 / 127.1885
5공학관 : 37.2221 / 127.1876
명진당 ~ 5공 : 79.69 / 269.9997
"""
MCU = serial.Serial(                # serial 객체 생성
    port='/dev/ttyAMA1',            # 시리얼통신에 사용할 포트
    baudrate=115200,                # 통신속도 지정
    parity=serial.PARITY_NONE,      # 패리티 비트 설정방식
    stopbits=serial.STOPBITS_ONE,   # 스톱비트 지정
    bytesize=serial.EIGHTBITS,      # 데이터 비트수 지정
    timeout=1                       # 타임아웃 설정
)


def calc_Cord(lat_ORG, lon_ORG, bearing, mDistance):
    distance = 0.001 * mDistance

    origin = geopy.Point(lat_ORG, lon_ORG)
    destination = geopy.distance.distance(
        kilometers=distance).destination(origin, bearing)

    lat_TGT, lon_TGT = destination.latitude, destination.longitude
    return lat_TGT, lon_TGT


def calc_Distance(pitch, Lidar):
    theta = 90 - pitch
    return Lidar * math.cos(math.pi * (theta / 180))


def main():
    while True:
        data = MCU.readine()
        GPS = [[int(data[0:14]), int(data[16:30])],
               [int(data[31:45]), int(data[47:56])],
               [int(data[58:72]), int(data[74:89])]]

        IMU = [int(data[91:100]), int(data[101:110]), int(data[111:120])]
        Lidar = int(data[121:125])

        # Parsing Data Area
        lat_org = GPS[0][1]
        lon_org = GPS[0][2]
        heading = IMU[2]  # 편각 보정 함수 적용 예정 -> 위도에 따라 선형하게 편각
        pitch = IMU[0]

        # calc new data
        distance = calc_Distance(pitch, Lidar)
        lat_TGT, lon_TGT = calc_Cord(lat_org, lon_org, heading, distance)

        print(distance)
        print(lat_TGT, lon_TGT)


main()
