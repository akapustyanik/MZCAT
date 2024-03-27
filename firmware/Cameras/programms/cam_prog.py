import time, sensor, image, math, pyb
from image import SEARCH_EX, SEARCH_DS
from pyb import Pin

threshold_index = 0 # 0 for red, 1 for green, 2 for blue

# Color Tracking Thresholds (L Min, L Max, A Min, A Max, B Min, B Max)
# The below thresholds track in general red/green/blue things. You may wish to tune them...
thresholds = [(0, 100, 6, 127, 4, 127), # generic_red_thresholds
              (2, 96, -21, -12, 0, 8), # generic_green_thresholds
              (0, 100, -15, 6, 16, 39)] # generic_blue_thresholds
# Reset sensor
sensor.reset()

#sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking

# Set sensor settings
sensor.set_contrast(1)
sensor.set_gainceiling(16)
# Max resolution for template matching with SEARCH_EX is QQVGA
sensor.set_framesize(sensor.QQVGA)
# You can set windowing to reduce the search image.
#sensor.set_windowing(((640-80)//2, (480-60)//2, 80, 60))
sensor.set_pixformat(sensor.RGB565)

# Load template.
# Template should be a small (eg. 32x32 pixels) grayscale image.
templates_u = ["/u.pgm", "/u2.pgm", "/u3.pgm"]
templates_s = ["/s.pgm", "/s2.pgm", "/s3.pgm", "/s4.pgm"]
templates_h = ["/h.pgm", "/h2.pgm", "/h3.pgm"]

pin5 = Pin('P5', Pin.OUT_PP, Pin.PULL_NONE)
pin4 = Pin('P4', Pin.OUT_PP, Pin.PULL_NONE)

clock = time.clock()

# Run template matching
while (True):
    clock.tick()
    img = sensor.snapshot()
    if len(img.find_blobs([thresholds[0]], pixels_threshold=200, area_threshold=200, merge=True, margin = 10)): #red
        pin5.value(1)
        pin4.value(1)
        print("red")
    elif len(img.find_blobs([thresholds[1]], pixels_threshold=200, area_threshold=200, merge=True, margin = 10)): #green
        pin5.value(1)
        pin4.value(0)
        print("green")
    elif len(img.find_blobs([thresholds[2]], pixels_threshold=200, area_threshold=200, merge=True, margin = 10)): #yellow
        pin5.value(0)
        pin4.value(1)
        print("yellow")
    else:
        pin5.value(0)
        pin4.value(0)
        print("none")

    #to graystale
    img.to_grayscale(copy = False)

    for t in templates_u:
        template_u = image.Image(t)
        if img.find_template(template_u, 0.70, step=4, search=SEARCH_EX) != None:
            pin5.value(1)
            pin4.value(1)
            print("u")
    for t in templates_s:
        template_s = image.Image(t)
        if img.find_template(template_s, 0.70, step=4, search=SEARCH_EX) != None:
            pin5.value(1)
            pin4.value(0)
            print("s")
    for t in templates_h:
        template_h = image.Image(t)
        if img.find_template(template_h, 0.70, step=4, search=SEARCH_EX) != None:
            pin5.value(0)
            pin4.value(1)
            print("h")
