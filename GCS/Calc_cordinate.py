import math
import geopy
import geopy.distance

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

def main():
    #Parsing Data Area
    lat_org = 37.2221
    lon_org = 127.1885
    heading = 269.9997 #편각 보정 함수 적용 예정 -> 위도에 따라 선형하게 편각
    pitch = 30
    Lidar = 79.69 * 2
    
    #calc new data
    distance = calc_Distance(pitch, Lidar)
    lat_TGT, lon_TGT = calc_Cord(lat_org, lon_org, heading, distance)
    
    print(distance)
    print(lat_TGT, lon_TGT)

main()