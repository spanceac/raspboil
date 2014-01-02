from datetime import datetime

def clear_state(nada):
	print "Clearing /dev/shm/state"
	f = open('/dev/shm/state', 'w')
	f.write("")
	f.close()

f = open('/dev/shm/state','r')
line = f.readline()
f.close()
f2 = open('/dev/shm/now','w')

if (line == 'start now'):
	# start now notified by mail.py
	f2.write('1')
elif (line == 'stop'):
	clear_state(0)
	f2.write('2')
elif not line: #if empty file
	#nothing happened
	print 'nothing'
else:
	continut_new = line.split(" ")
	date = continut_new[1].split("\n")
	date = date[0]
	date = date.split("/")
	day = int(date[0])
	month = int(date[1])
	hours = int(date[2])
	minutes = int(date[3])

	now = datetime.now()

	if (month == now.month) and (day == now.day) and (hours == now.hour) and (minutes == now.minute):
		print "The date is now"
		clear_state(0)
		f2.write('1')
	
	else:
		print "Date is NOT now"
		f2.write('0')
f2.close()
	

