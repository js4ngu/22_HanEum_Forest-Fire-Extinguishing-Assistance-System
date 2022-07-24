import geopy
import geopy.distance

#def m2km(m): 
#    return m*0.001

# given: lat_oscar, lon_oscar, b = bearing in degrees, d = distance in kilometers

lat_oscar = 37.2221
lon_oscar = 127.1876
bearing = 230
distance = 83 * 0.001

origin = geopy.Point(lat_oscar, lon_oscar)
destination = geopy.distance.distance(kilometers=distance).destination(origin, bearing)

lat_Tango, lon_Tango = destination.latitude, destination.longitude

print(lat_Tango)
print(lon_Tango)