#!/usr/bin/python
import sys          
import time
import datetime
import Pi_HDC2080



print "Program Started at:"+ time.strftime("%Y-%m-%d %H:%M:%S")
print ""

hdc2000 = Pi_HDC2080.Pi_HDC2080()

print "------------"
print "Manfacturer ID=0x%X"% hdc2000.readManufacturerID()  
print "Device ID=0x%X"% hdc2000.readDeviceID()  
print "configure register = 0x%X" % hdc2000.readConfigRegister()

while True:
        
        print "-----------------"
        print "Temperature = %3.1f C" % hdc2000.readTemperature()
        print "Humidity = %3.1f %%" % hdc2000.readHumidity()
        print "-----------------"

        time.sleep(3.0)
