# Template Matching Example - Normalized Cross Correlation (NCC)
#
# 这个例子展示了如何使用OpenMV凸轮的NCC功能将小部分图像与图像的各个部分
# 进行匹配...期望获得极其可控的环境NCC并不是全部有用的。
#
# 警告：NCC支持需要重做！到目前为止，这个功能需要做大量的工作才能有用。
# 这个脚本将重新表明功能的存在，但在目前的状态是不足的。

import time, sensor, image
from image import SEARCH_EX, SEARCH_DS
#从imgae模块引入SEARCH_EX和SEARCH_DS。使用from import仅仅引入SEARCH_EX,
#SEARCH_DS两个需要的部分，而不把image模块全部引入。

# Reset sensor
sensor.reset()

# Set sensor settings
sensor.set_contrast(1)
sensor.set_gainceiling(16)
# Max resolution for template matching with SEARCH_EX is QQVGA
sensor.set_framesize(sensor.HQQVGA)
# You can set windowing to reduce the search image.
#sensor.set_windowing(((640-80)//2, (480-60)//2, 80, 60))
sensor.set_pixformat(sensor.RGB565)

# Load template.
# Template should be a small (eg. 32x32 pixels) grayscale image.
templates = ["/u.pgm", "/u2.pgm", "/u3.pgm", "/s.pgm", "/s2.pgm", "/s3.pgm", "/s4.pgm", "/h.pgm", "/h2.pgm", "/h3.pgm"] #保存多个模板
#加载模板图片

clock = time.clock()

# Run template matching
while (True):
    clock.tick()
    img = sensor.snapshot()

    img.to_grayscale(copy = False)
    # find_template(template, threshold, [roi, step, search])
    # ROI: The region of interest tuple (x, y, w, h).
    # Step: The loop step used (y+=step, x+=step) use a bigger step to make it faster.
    # Search is either image.SEARCH_EX for exhaustive search or image.SEARCH_DS for diamond search
    #
    # Note1: ROI has to be smaller than the image and bigger than the template.
    # Note2: In diamond search, step and ROI are both ignored.
    for t in templates:
        template = image.Image(t)
        #对每个模板遍历进行模板匹配
        r = img.find_template(template, 0.6, step=4, search=SEARCH_EX, roi=(0, 0, 320, 170))
    #find_template(template, threshold, [roi, step, search]),threshold中
    #的0.7是相似度阈值,roi是进行匹配的区域（左上顶点为（10，0），长80宽60的矩形），
    #注意roi的大小要比模板图片大，比frambuffer小。
    #把匹配到的图像标记出来
        if r:
            img.draw_rectangle(r)
            print(t) #打印模板名字

    #print(clock.fps())
