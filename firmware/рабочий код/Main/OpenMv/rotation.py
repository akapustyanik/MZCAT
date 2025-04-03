# Untitled - By: Savma - Вс окт 1 2023

import sensor, image, time, math
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

old = 0
oldNum = 0

clock = time.clock()

letter = (0, 36, -8, 12, -24, 7)

k_exp = 1.2
k_elong = 1.6
k_fill_min = 0.1
k_fill_max = 0.8
k_white_max = 0.5
k_blind = 2.5
n_filter = 15

mas = [[[1, 1, 1], [1, 1, 1], [1, 1, 1]], [[1, 0, 1], [1, 1, 1], [1, 1, 1]], [[1, 0, 1], [1, 0, 1], [1, 1, 1]], [[1, 0, 1], [1, 0, 1], [1, 0, 1]], [[1, 0, 1], [1, 1, 1], [1, 0, 1]]]
color_threshold = [(3, 62, -26, 0, -12, 16), (0, 95, -18, 1, 0, 24), (5, 46, 22, 62, -10, 22)]
white = (24, 100, -2, 15, -42, 3)

names = ("?", "S", "U", "H", "G", "Y", "R")

def sgn(a):
    if a > 0: return 1
    if a < 0: return -1
    return 0

while(True):
    clock.tick()
    img = sensor.snapshot()
    img.lens_corr(3)
    flag = 0
    max_pixels = 0
    best_blob = None
    max_white = None
    for wh in img.find_blobs([white], area_threshold = 4000, pixels_threshold = 3000, merge = False):
        if max_white != None:
            if wh.pixels() > max_white.pixels():
                max_white = wh
        else:
            max_white = wh
    if max_white != None:
        img.draw_rectangle(max_white.rect())
        for i in range(len(color_threshold)):
            for col in img.find_blobs([color_threshold[i]], area_threshold = 2500, pixels_threshold = 2000, merge = False, roi = [max_white.x(), max_white.y(), max_white.w(), max_white.h()]):
                if col.pixels() / col.area() > k_fill_min and col.h() / col.w() < k_elong and col.w() / col.h() < k_elong and col.y() > 1 and col.x() > 1 and col.y() + col.h() < img.height() - 11 and col.x() + col.w() < img.width() - 1 and  col.pixels() < 15000 and col.y() + col.h() > img.height() / k_blind:
                    smFlag = 1
                    for wh2 in img.find_blobs([white], merge = True, margin = img.height() + img.width(), roi = [col.x(), col.y(), col.w(), col.h()]):
                        if wh2.pixels() / wh2.area() > k_white_max:
                            smFlag = 0
                            img.draw_rectangle(wh2.rect(), color = (0, 255, 0))
                    if smFlag:
                        img.draw_rectangle(col.rect())
                        flag = i + 4
        for let in img.find_blobs([letter], merge = False, pixels_threshold = 200, area_threshold = 500, roi = [max_white.x(), max_white.y(), max_white.w(), max_white.h()]):
            if let.pixels() / let.area() < k_fill_max and let.pixels() / let.area() > k_fill_min and let.x() > 1 and let.x() + let.w() < img.width() - 1 and let.y() + let.h() > img.height() / k_blind and flag == 0 and let.h() / let.w() < k_elong and let.w() / let.h() < k_elong and let.y() > 1 and let.y() + let.h() < img.height() - 11:
                if max_pixels < let.pixels() ** 2 / let.area():
                    max_pixels = let.pixels() ** 2 / let.area()
                    best_blob = let
    let = best_blob
    if let != None:
        img.draw_rectangle(let.rect())
        min_corners = list(let.min_corners())
        norm_corners = [0, 0, 0, 0]
        alphas = [0, 0, 0, 0]
        for i in range(4):
            alphas[i] = math.atan2(let.cy() - min_corners[i][1], min_corners[i][0] - let.cx()) * 180 / math.pi
        for i in range(3):
            for j in range(3-i):
                if alphas[j] > alphas[j+1]:
                    alphas[j], alphas[j+1] = alphas[j+1], alphas[j]
                    min_corners[j], min_corners[j+1] = min_corners[j+1], min_corners[j]
        h = math.sqrt((min_corners[3][0] - min_corners[0][0]) ** 2 + (min_corners[3][1] - min_corners[0][1]) ** 2)
        w = math.sqrt((min_corners[1][0] - min_corners[0][0]) ** 2 + (min_corners[1][1] - min_corners[0][1]) ** 2)
        beta = math.atan2(min_corners[3][0] - min_corners[0][0], min_corners[0][1] - min_corners[3][1]) * 180 / math.pi
        if abs(beta) > 45:
            if beta < 0:
                alphas[0], alphas[1], alphas[2], alphas[3] = alphas[3], alphas[0], alphas[1], alphas[2]
                min_corners[0], min_corners[1], min_corners[2], min_corners[3] = min_corners[3], min_corners[0], min_corners[1], min_corners[2]
            else:
                alphas[0], alphas[1], alphas[2], alphas[3] = alphas[1], alphas[2], alphas[3], alphas[0]
                min_corners[0], min_corners[1], min_corners[2], min_corners[3] = min_corners[1], min_corners[2], min_corners[3], min_corners[0]
            h, w = w, h
            beta = 90 * -sgn(beta) + beta
        for i in range(4):
            img.draw_string(min_corners[i][0], min_corners[i][1], str(i))
        a1 = math.cos(beta * math.pi / 180) * h
        a2 = math.sin(beta * math.pi / 180) * h
        b1 = math.cos(beta * math.pi / 180) * w
        b2 = math.sin(beta * math.pi / 180) * w
        my_mas1 = [[0, 0, 0], [0, 0, 0], [0, 0, 0]]
        flag = 0
        #print(a1, a2, b1, b2)
        try:
            for i in range(9):
                xroi = min(max(int(min_corners[0][0] + a2 / 6 + b1 / 3 * (i % 3) + a2 / 3 * (i // 3) - b1 / 3 * (((k_exp - 1) / 45 * abs(beta)) / 2)), 0), img.width())
                yroi = min(max(int(min_corners[0][1] + b2 / 6 + b2 / 3 * (i % 3) - a1 / 3 * (i // 3) - a1 / 3 * (((k_exp - 1) / 45 * abs(beta)) / 2 + 1)), 0), img.height())
                wroi = int(b1 / 3 * (((k_exp - 1) / 45 * abs(beta)) + 1))
                hroi = int(a1 / 3 * (((k_exp - 1) / 45 * abs(beta)) + 1))
                #print(i, xroi, yroi, int(b1 / 3), int(a1 / 3))
                for small in img.find_blobs([letter], roi = (xroi, yroi, wroi, hroi), merge = True, margin = int(w + h)):
                    img.draw_rectangle(xroi, yroi, wroi, hroi)
                    my_mas1[2 - i // 3][i % 3] = 1
        except:
            print("err")
        my_mas2 = [my_mas1[2], my_mas1[1], my_mas1[0]]
        my_mas3 = [[my_mas1[2][0], my_mas1[1][0], my_mas1[0][0]], [my_mas1[2][1], my_mas1[1][1], my_mas1[0][1]], [my_mas1[2][2], my_mas1[1][2], my_mas1[0][2]]]
        my_mas4 = [[my_mas1[0][2], my_mas1[1][2], my_mas1[2][2]], [my_mas1[0][1], my_mas1[1][1], my_mas1[2][1]], [my_mas1[0][0], my_mas1[1][0], my_mas1[2][0]]]
        for j in range(len(mas)):
            if my_mas1 == mas[j] or my_mas2 == mas[j] or my_mas3 == mas[j] or my_mas4 == mas[j]:
                flag = j // 2 + 1
                break
    if flag == old:
        oldNum += 1
    else:
        oldNum = 0
    old = flag
    if oldNum > 0 and oldNum <= n_filter:
        red_led.on()
        green_led.on()
        blue_led.on()
        flag = 0
    elif flag == 0:
        red_led.off()
        green_led.off()
        blue_led.off()
    elif flag == 1:
        red_led.off()
        green_led.on()
        blue_led.on()
    elif flag == 2:
        red_led.on()
        green_led.off()
        blue_led.on()
    elif flag == 3:
        red_led.off()
        green_led.off()
        blue_led.on()
    elif flag == 4:
        red_led.off()
        green_led.on()
        blue_led.off()
    elif flag == 5:
        red_led.on()
        green_led.on()
        blue_led.off()
    elif flag == 6:
        red_led.on()
        green_led.off()
        blue_led.off()
    uart.write(str(flag))
    print(names[flag])
