# Untitled - By: Savma - Вс окт 1 2023

import sensor, image, time
from pyb import LED
from pyb import UART

red_led   = LED(1)
green_led = LED(2)
blue_led  = LED(3)
ir_led    = LED(4)

uart = UART(3, 115200)

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_vflip(True)
sensor.set_hmirror(True)

sensor.set_brightness(3)
sensor.set_contrast(3)

clock = time.clock()

old = 0
oldNum = 0


names = ("?", "S", "U", "H")
colors = ("green", "yellow", "red")

leds = [0, 1, 2, 3]

symbol_threshold = [(0, 10, -14, 5, -14, 5), 0, [0, 0, 0], 0]

color_threshold = [(0, 65, -33, -8, 0, 35), (37, 70, -11, 6, 15, 36), (0, 32, 11, 54, 9, 40), 0, 0]

white = (45, 100, -19, 10, -13, 13)

kMax = 1.3
kLim = 9 / 40

while(True):
    #red_led.on()
    #green_led.on()
    #blue_led.on()
    clock.tick()
    img = sensor.snapshot()
    #print(clock.fps())
    for i in range(3):
        for bl in img.find_blobs([color_threshold[i]], area_threshold = 2500, pixels_threshold = 2000, merge = False):
            if bl.h() / bl.w() < 3 and bl.w() / bl.h() < 3:
                if color_threshold[3] != 0:
                    if bl.area() > color_threshold[3].area():
                        color_threshold[3] = bl
                        color_threshold[4] = i + 4
                else:
                    color_threshold[3] = bl
                    color_threshold[4] = i + 4
    if color_threshold[3] != 0:
        c = color_threshold[4]
        if c != old:
            oldNum = 0
        oldNum += 1
        old = c
        if oldNum < 3:
            c = 0
        img.draw_rectangle(color_threshold[3].rect(), color = (255 * (c != 4), 255 * (c != 5), 255 * (c != 6)))
        if c:
            if c != 4:
                red_led.on()
            else:
                red_led.off()
            if c != 5:
                green_led.on()
            else:
                green_led.off()
            if c != 6:
                blue_led.on()
            else:
                blue_led.off()
            print(colors[c - 4])
        else:
            print(names[0])
            red_led.on()
            green_led.on()
            blue_led.on()
        uart.write(str(c))
        color_threshold[3] = 0
        color_threshold[4] = 0
    else:
        for sb in img.find_blobs([symbol_threshold[0]], area_threshold = 2500, pixels_threshold = 1000, merge = True, margin = 150):
            whiteFlag = 0
            for wh in img.find_blobs([white], merge = True, margin = 100, roi = sb.rect()):
                if sb.pixels() / wh.pixels() < 1.25:
                    whiteFlag = 1
                print(sb.pixels() / wh.pixels())
            if sb.x() != 0 and sb.x() + sb.w() != img.width() and sb.area() > sb.pixels() * 2 and sb.h() / sb.w() < kMax * 1.5 and sb.h() / sb.w() > kMax / 2 and whiteFlag:
                if symbol_threshold[1] != 0:
                    if symbol_threshold[1].area() < sb.area():
                        symbol_threshold[1] = sb
                else:
                    symbol_threshold[1] = sb
        if symbol_threshold[1] != 0:
            img.draw_rectangle(symbol_threshold[1].rect(), color = (0, 255, 0))
            s = symbol_threshold[1]
            k = s.h() / s.w()
            edge = 0
            if s.y() == 0:
                edge = 1
            else:
                if s.y() + s.h() > img.height() - 10:
                    edge = 2
            centre = [0, 0, 0]
            if edge != 0 and k < kMax / 1.2:
                if kLim < 0.5:
                    if edge == 1:
                        for i in range(2):
                            for bl in img.find_blobs([symbol_threshold[0]], pixels_threshold = 70, merge = True, margin = 10, roi =
                            (s.x(), int(s.y() + s.h() - s.h() * ((i + 1) * kLim + (1 - kLim * 3) * (i == 1)) * kMax / k), s.w(),
                            max(int(s.h() * kMax / k * (kLim + (1 - kLim * 3) * (i == 1))), 0))):
                                place = [0, 0, 0]
                                place[0] = bl.cx() > s.cx()
                                place[1] = bl.x() > s.cx()
                                place[2] = bl.x() + bl.w() > s.cx()
                                if place[0] != place[1] or place[0] != place[2]:
                                    centre[2 - i] = 1
                                symbol_threshold[2][2 - i] += 1
                                img.draw_rectangle(bl.rect())
                            if centre[2 - i] == 1:
                                symbol_threshold[2][2 - i] = 1
                    else:
                        for i in range(2):
                            for bl in img.find_blobs([symbol_threshold[0]], pixels_threshold = 70, merge = True, margin = 10, roi =
                            (s.x(), int(s.y() + s.h() * kLim * kMax / k * i), s.w(),
                            min(int(s.h() * kMax / k * (kLim + (1 - kLim * 3) * (i == 1))), img.height()))):
                                place = [0, 0, 0]
                                place[0] = bl.cx() > s.cx()
                                place[1] = bl.x() > s.cx()
                                place[2] = bl.x() + bl.w() > s.cx()
                                if place[0] != place[1] or place[0] != place[2]:
                                    centre[i] = 1
                                symbol_threshold[2][i] += 1
                                img.draw_rectangle(bl.rect())
                            if centre[i] == 1:
                                symbol_threshold[2][i] = 1
            else:
                for i in range(3):
                    for bl in img.find_blobs([symbol_threshold[0]], pixels_threshold = 70, merge = True, margin = 10, roi =
                    (s.x(), int(s.y() + s.h() * (i * kLim + (1 - kLim * 3) * (i == 2))), s.w(),
                    int(s.h() * (kLim + (1 - kLim * 3) * (i == 1))))):
                        place = [0, 0, 0]
                        place[0] = bl.cx() > s.cx()
                        place[1] = bl.x() > s.cx()
                        place[2] = bl.x() + bl.w() > s.cx()
                        if place[0] != place[1] or place[0] != place[2]:
                            centre[i] = 1
                        symbol_threshold[2][i] += 1
                        img.draw_rectangle(bl.rect())
                    if centre[i] == 1:
                        symbol_threshold[2][i] = 1
            s = symbol_threshold[2]
            if (s[0] == 1 or edge == 1) and s[1] == 1 and (s[2] == 1 or edge == 2):
                symbol_threshold[3] = 1
            else:
                if (s[0] == 2 or edge == 1) and s[1] == 2 and (s[2] == 1 or edge == 2):
                    symbol_threshold[3] = 2
                else:
                    if (s[0] == 2 or edge == 1) and s[1] == 1 and (s[2] == 2 or edge == 2):
                        symbol_threshold[3] = 3
        if symbol_threshold[3] != old:
            oldNum = 0
        oldNum += 1
        old = symbol_threshold[3]
        if oldNum < 5:
            symbol_threshold[3] = 0
        print(names[symbol_threshold[3]])
        uart.write(str(symbol_threshold[3]))
        if symbol_threshold[3] == 1:
            red_led.on()
        else:
            red_led.off()
        if symbol_threshold[3] == 2:
            green_led.on()
        else:
            green_led.off()
        if symbol_threshold[3] == 3:
            blue_led.on()
        else:
            blue_led.off()
        symbol_threshold[1] = 0
        symbol_threshold[2] = [0, 0, 0]
        symbol_threshold[3] = 0
