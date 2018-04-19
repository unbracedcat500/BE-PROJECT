import serial
import time
import urllib2
import sys
ser = serial.Serial('/dev/ttyUSB0',9600)
logfile = open('test1.csv', 'a')
def getData():
	line = ser.readline()
	ID,moist,hum,temp = line.split(',')
	return(ID,moist,hum,temp)

def csv():
	ID,moist,hum,temp = getData()
	now = time.strftime("%d/%m/%Y %H:%M:%S", time.localtime())
 	a = "%s,%s,%s,%s,%s" %(now,ID,moist,hum,temp)
 	print a
 	logfile.write(a)
    	logfile.flush()

def thingSpeak():
	baseURL = 'https://api.thingspeak.com/update?api_key=977CK25HVXX7XF55'
	try:
		ID,moist,hum,temp = getData()
		f = urllib2.urlopen(baseURL +
                                "&field1=%s&field2=%s&field3=%s" % (temp, hum, moist))
		print f.read()
		f.close()
	except:
		print 'exiting'
		break

def check_internet():
    try :
        stri = "https://www.google.co.in"
        data = urllib.urlopen(stri)
        return True
    except:
        return False
def main():
	
	while True:
		internet = check_internet()
		if internet == True:
			thingspeak() 
		else:
			csv()

if __name__ == '__main__':
	main()


