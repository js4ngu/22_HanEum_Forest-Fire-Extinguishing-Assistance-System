# Infra-Red Camera display code
# Coded by Jiho.K in 2022/09/10
# 640x360 resolution
# black mini circle at center of display

import sensor, image, time

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.VGA)
x = sensor.width()
y = sensor.height()
sensor.set_windowing(0, int((y-x/16*9)/2), x, int(x/16*9))
sensor.skip_frames(time = 2000)

clock = time.clock()

img = sensor.snapshot()
y = img.height()

while(True):
    clock.tick()
    img = sensor.snapshot()
    img.draw_circle(int(x/2), int(y/2), 5, 0, 1, False)
    print(clock.fps())
