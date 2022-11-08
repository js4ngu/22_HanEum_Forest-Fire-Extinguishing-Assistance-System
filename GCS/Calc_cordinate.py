import math
import geopy
import geopy.distance
import serial

"""
명진당 : 37.2221 / 127.1885
5공학관 : 37.2221 / 127.1876
명진당 ~ 5공 : 79.69 / 269.9997
"""

'''
PORT = 'COM9'
BaudRate = 9600
xbee= serial.Serial(PORT,BaudRate)

#parsing
def Decode(A):
    A = A.decode('utf-8','ignore')
    A = str(A)
    if A[0]=='S':                      
        if (len(A)==125):                
            lat1 = float(A[1:15])
            lon1 = float(A[16:30])
            lat2 = float(A[31:45])
            lon2 = float(A[46:60])
            lat3 = float(A[61:75])
            lon3 = float(A[76:90])
            pitch = float(A[91:100])
            roll = float(A[101:110])
            yaw = float(A[111:120])
            distance = float(A[121:124])
            result= [lat1,lon1,lat2,lon2,lat3,lon3,pitch,roll,yaw,distance]
            return result
        else : 
            print ("ERR : %d" %len(A))
            return False
    else :
        print ("")
        return False

def xbeeread():
        if xbee.readable():
            LINE = xbee.readline()
            code=Decode(LINE) 
            return code
        else : 
            print("")
'''
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
    data = calc_Cord(lat_org, lon_org, heading, distance)
    
    print(distance)
    print(data)

main()