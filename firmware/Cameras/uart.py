# Arduino as an I2C master device, OpenMV as an I2C slave device.
#
# Please connect OpenMV and Arduino as follows:
#
# OpenMV Cam Master I2C Data  (P5) - Arduino Uno Data  (A4)
# OpenMV Cam Master I2C Clock (P4) - Arduino Uno Clock (A5)
# OpenMV Cam Ground                - Arduino Ground

import pyb, ustruct

text = "Hello World!\n"
data = ustruct.pack("<%ds" % len(text), text)
# Use "USTRUCT" to generate packets that need to be sent
# "<" Put the data in a small end in Struct
# "% DS" put the string into the data stream, such as "13S" corresponding to "Hello World! \ N" (13 Chars).
# See https://docs.python.org/3/library/struct.html

# READ ME!!!
#
# Please understand, when your OpenMV camera is not an I2C master device, no matter what interrupt callback is used.
#                                            When this happens,
# Arduino will get NAK and have to read data again from OpenMV. Please note,
# OpenMV and Arduino are not good at solving the I2C error. In OpenMV and Arduino,
# You can restore the function by renewing the I2C peripherals, reinitialize the outset.

# PMV's hardware I2C bus is 2
bus = pyb.I2C(2, pyb.I2C.SLAVE, addr=0x12)
bus.deinit() #
bus = pyb.I2C(2, pyb.I2C.SLAVE, addr=0x12)
print("Waiting for Arduino...")



# Note that in order to work normally, the OpenMV CAM must run this script before Arduino poll data.
# Otherwise, I2C byte frames become messy. So, keep Arduino in the RESET status,
# Until OpenMV displays "Waiting for Arduino ...".


while(True):
    try:
        bus.send(ustruct.pack("<h", len(data)), timeout=10000) # First send the length (16-bits).
        try:
            bus.send(data, timeout=10000) # And then send data
            print("Sent Data!") #No, when you encounter an error, it will be displayed.
        except OSError as err:
            pass # Don't worry, you will jump
            # Please note that there are 3 possible errors. Timeout error (Timeout Error),
            #Energy error (General Purpose Error) or busy error
            # (Busy Error). "Err.Arg [0]" error code separately
            # Is 116, 5, 16.
    except OSError as err:
        pass # Don't worry, you will jump
        # Please note that there are 3 possible errors. Timeout error (Timeout Error),
        #Energy error (General Purpose Error) or busy error
        # (Busy Error). "Err.Arg [0]" error code separately
        # Is 116, 5, 16.

