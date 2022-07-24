import geopy
import geopy.distance


def calc_Cord(lat_ORG, lon_ORG ,bearing, mDistance):
    distance = 0.001 * mDistance
    
    origin = geopy.Point(lat_ORG, lon_ORG)
    destination = geopy.distance.distance(kilometers=distance).destination(origin, bearing)

    lat_TGT, lon_TGT = destination.latitude, destination.longitude
    return lat_TGT, lon_TGT

def main():
    lat_TGT, lon_TGT = calc_Cord(37.2225, 127.1873, 180, 40)
    print(lat_TGT)
    print(lon_TGT)
    
main()