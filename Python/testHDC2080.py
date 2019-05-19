#!/usr/bin/python
import sys
import Pi_HDC2080

hdc2000 = Pi_HDC2080.Pi_HDC2080()

print "{ \"temperature\" : %3.1f, \"humidity\" : %3.1f }" %(hdc2000.readTemperature(), hdc2000.readHumidity())

