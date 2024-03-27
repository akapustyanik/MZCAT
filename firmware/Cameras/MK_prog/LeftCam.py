import sensor, time, image
from pyb import LED
from pyb import Pin
THRESHOLD = (0, 80)
sensor.reset()
print("op")
sensor.set_contrast(1)
#sensor.set_vflip(True)
sensor.set_hmirror(True)
sensor.set_gainceiling(16)
sensor.set_framesize(sensor.QCIF)
sensor.set_pixformat(sensor.GRAYSCALE)
clock = time.clock()
print("op")
templateH = image.Image("/h.pgm")
templateU = image.Image("/u.pgm")
templateS = image.Image("/s.pgm")
templateH2 = image.Image("/h2.pgm")
templateU2 = image.Image("/u2.pgm")
templateS2 = image.Image("/s2.pgm")
templateHS = image.Image("/hS.pgm")
templateUS = image.Image("/uS.pgm")
templateSS = image.Image("/sS.pgm")
pin5 = Pin('P4', Pin.OUT_PP, Pin.PULL_NONE)
pin6 = Pin('P5', Pin.OUT_PP, Pin.PULL_NONE)
pin0 = Pin('P3', Pin.OUT_PP, Pin.PULL_UP)
pin0.value(1)
def inter(Iimg, endX, endY, xo, yo, xt, yt):
    if (xo == yo | yo == yt): pass
    xIT = 0
    yIT = 0
    temp = Iimg
    for xIT in range(endX):
        for yIT in range(endY):
            Ix = xIT * ((xt - xo) / endX) + xo
            Iy = yIT * ((yt - yo) / endY) + yo
            Ix2 = (xIT + 1) * ((xt - xo) / endX) + xo
            Iy2 = (yIT + 1) * ((yt - yo) / endY) + yo
            nOfPix = 0
            pixSumm = 0
            for xIT2 in range(int(Ix), int(Ix2)):
                for yIT2 in range(int(Iy), int(Iy2)):
                    if xIT2 >= Iimg.width() or yIT2 >= Iimg.height():
                        continue
                    nOfPix += 1
                    pixSumm += Iimg.get_pixel(int(xIT2), int(yIT2))
            if(nOfPix == 0):continue
            temp.set_pixel(yIT, xIT, int(pixSumm / nOfPix))
    return (temp)
while(True):
    clock.tick()
    img = sensor.snapshot().binary([THRESHOLD])
    for x in range(150, 176):
        for y in range(0, 144):
            img.set_pixel(x,y,0)
    pixMax = 0
    for blob in img.find_blobs([(0, 120), (120,255)], pixels_threshold=10, area_threshold=10):
        if(blob.pixels() > pixMax) and (blob.pixels() < 10000):
            pixMax = blob.pixels()
            maxBlob = blob
    if(maxBlob):
        img.draw_rectangle(maxBlob.rect())
        img = inter(img, 22, 22, maxBlob.x(), maxBlob.y(), maxBlob.x() + maxBlob.w(), maxBlob.y() + maxBlob.h()).binary([THRESHOLD])
        h = img.find_template(templateH, 0.6,roi = (0,0,22,22), steps = 2, serach = image.SEARCH_EX)
        s = img.find_template(templateS, 0.4,roi = (0,0,22,22), steps = 2, serach = image.SEARCH_EX)
        u = img.find_template(templateU, 0.5,roi = (0,0,22,22), steps = 2, serach = image.SEARCH_EX)
        h2 = img.find_template(templateH2, 0.6,roi = (0,0,22,22), steps = 2, serach = image.SEARCH_EX)
        s2 = img.find_template(templateS2, 0.4,roi = (0,0,22,22), steps = 2, serach = image.SEARCH_EX)
        u2 = img.find_template(templateU2, 0.5,roi = (0,0,22,22), steps = 2, serach = image.SEARCH_EX)
        hs = img.find_template(templateHS, 0.6,roi = (0,0,22,22), steps = 2, serach = image.SEARCH_EX)
        ss = img.find_template(templateSS, 0.4,roi = (0,0,22,22), steps = 2, serach = image.SEARCH_EX)
        us = img.find_template(templateUS, 0.5,roi = (0,0,22,22), steps = 2, serach = image.SEARCH_EX)
        if(h or h2 or hs):
            print("h")
            pin5.value(1)
            pin6.value(1)
            LED(2).on()
            #pin0.value(0)
        else:
            if(u or u2 or us):
                print("u")
                pin5.value(1)
                pin6.value(0)
                LED(2).on()
                #pin0.value(0)
            else:
                if(s or s2 or ss):
                    print("s")
                    pin5.value(0)
                    pin6.value(1)
                    LED(2).on()
                    #pin0.value(0)
                else:
                    pin5.value(0)
                    pin6.value(0)
                    LED(2).off()
                    #pin0.value(1)
