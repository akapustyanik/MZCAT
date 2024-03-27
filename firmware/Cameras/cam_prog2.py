import time, sensor, image, math, pyb
from image import SEARCH_EX, SEARCH_DS
from pyb import Pin
from pyb import LED

red_led   = LED(1)
green_led = LED(2)
blue_led  = LED(3)

threshold_index = 0 # 0 for red, 1 for green, 2 for blue

# Color Tracking Thresholds (L Min, L Max, A Min, A Max, B Min, B Max)
# The below thresholds track in general red/green/blue things. You may wish to tune them...
thresholds = [(0, 100, 21, 39, -2, 19), # generic_red_thresholds
              (0, 100, -26, 0, -4, 18), # generic_green_thresholds
              (0, 100, 0, 14, 5, 29)] # generic_yellow_thresholds
# Reset sensor
sensor.reset()

#sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking

# Set sensor settings
sensor.set_contrast(1)
sensor.set_gainceiling(16)
# Max resolution for template matching with SEARCH_EX is QQVGA
sensor.set_framesize(sensor.QQQVGA)
# You can set windowing to reduce the search image.
#sensor.set_windowing(((640-80)//2, (480-60)//2, 80, 60))
sensor.set_pixformat(sensor.RGB565)

#sensor.set_windowing(160, 80)

EXPOSURE_TIME_SCALE = 0.8
sensor.set_auto_exposure(False)
current_exposure_time_in_microseconds=  sensor.get_exposure_us()
sensor.set_auto_exposure(False, \
    exposure_us = int(current_exposure_time_in_microseconds* EXPOSURE_TIME_SCALE))

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
    img.draw_line(0, 121, 160, 121, thickness = 1)
    pin5.value(0)
    pin4.value(0)
    #if len(img.find_blobs([thresholds[0]], pixels_threshold=1200, area_threshold=200, merge=True, margin = 10, roi = (0, 0, 120, 65))): #red
        #pin5.value(1)
        #pin4.value(0)
        #print("red")
        #red_led.on()
        #time.sleep_ms(t)
        #red_led.off()
    #elif len(img.find_blobs([thresholds[1]], pixels_threshold=1000, area_threshold=200, merge=True, margin = 10, roi = (0, 0, 120, 65))): #green
        #pin5.value(1)
        #pin4.value(1)
        #print("green")
        #green_led.on()
        #time.sleep_ms(70)
        #green_led.off()
    #elif len(img.find_blobs([thresholds[2]], pixels_threshold=900, area_threshold=200, merge=True, margin = 10, roi = (0, 0, 120, 65))): #yellow
        #pin5.value(1)
        #pin4.value(0)
        #print("yellow")
        #red_led.on()
        #green_led.on()
        #time.sleep_ms(t)
        #red_led.off()
        #green_led.off()
    #else:
        #pin5.value(0)
        #pin4.value(0)
        #print("none")

    #to graystale
    img.to_grayscale(copy = False)
    #image.graycale_to_binary(copy = False)

    for t in templates_u:
        template_u = image.Image(t)
        if img.find_template(template_u, 0.65, step=4, search=SEARCH_EX, roi = (0, 0, 160, 120)) != None:
            pin5.value(1)
            pin4.value(1)
            print("u")
            red_led.on()
            blue_led.on()
            time.sleep_ms(70)
            red_led.off()
            blue_led.off()
    for t in templates_s:
        template_s = image.Image(t)
        if img.find_template(template_s, 0.6, step=4, search=SEARCH_EX, roi = (0, 0, 160, 120)) != None:
            pin5.value(0)
            pin4.value(1)
            blue_led.on()
            print("s")
            time.sleep_ms(70)
            blue_led.off()
    for t in templates_h:
        template_h = image.Image(t)
        if img.find_template(template_h, 0.68, step=4, search=SEARCH_EX, roi = (0, 0, 160, 120)) != None:
            pin5.value(1)
            pin4.value(0)
            green_led.on()
            blue_led.on()
            print("h")
            time.sleep_ms(70)
            green_led.off()
            blue_led.off()
