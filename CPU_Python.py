#!/usr/bin/env python
#__author__ = 'skunda'
# This program logs a Raspberry Pi's CPU temperature to a Thingspeak Channel
# To use, get a Thingspeak.com account, set up a channel, and capture the Channel Key at https://thingspeak.com/docs/tutorials/ 
# Then paste your channel ID in the code for the value of "key" below.
# Then run as sudo python pitemp.py (access to the CPU temp requires sudo access)
# You can see my channel at https://thingspeak.com/channels/41518

import httplib, urllib
import time
import serial
import time
import urllib2
import sys
ser = serial.Serial('/dev/ttyUSB0',9600)
logfile = open('test2.csv', 'a')
sleep = 10 # how many seconds to sleep between posts to the channel
key = '977CK25HVXX7XF55'  # Thingspeak channel to update

def getData():
    line = ser.readline()
    ID,moist,hum,temp = line.split(',')
    return(ID,moist,hum,temp)

# def csv():
#     ID,moist,hum,temp = getData()
#     now = time.strftime("%d/%m/%Y %H:%M:%S", time.localtime())
#     a = "%s,%s,%s,%s,%s" %(now,ID,moist,hum,temp)
#     print a
#     logfile.write(a)
#     logfile.flush()



#Report Raspberry Pi internal temperature to Thingspeak Channel
def thingspeak():
    
    while True:
        ID,moist,hum,temp = getData()
        #Calculate CPU temperature of Raspberry Pi in Degrees C
        params = urllib.urlencode({'field1': temp, 'field2' :hum, 'field3': moist, 'key':key }) 
        headers = {"Content-typZZe": "application/x-www-form-urlencoded","Accept": "text/plain"}
        conn = httplib.HTTPConnection("api.thingspeak.com:80")
        try:
            conn.request("POST", "/update", params, headers)
            response = conn.getresponse()
            print temp
            print hum
            print moist
            print response.status, response.reason
            data = response.read()
            conn.close()
        except:
            print "connection failed.Storing Data locally"
            now = time.strftime("%d/%m/%Y %H:%M:%S", time.localtime())
            a = "%s,%s,%s,%s,%s" %(now,ID,moist,hum,temp)
            print a
            logfile.write(a)
            logfile.flush()
        
#sleep for desired amount of time
if __name__ == "__main__":
        while True:
                thingspeak()
                time.sleep(sleep)

