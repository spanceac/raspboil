import imaplib
import email
from datetime import datetime
import time
date = ""
action = ""
date_unsplit = ""
have_mail = 0 #variable used to avoid checking empty time structures in check_date at first run if no new mail available

def check_mail(none):
	global date, action, have_mail, date_unsplit
	result, maildata = mail.search(None, '(Unseen FROM "your_emai_address")')

	ids = maildata[0] # data is a list.

	id_list = ids.split() # ids is a space separated string
	################ TO DO Check if id_list is not empty ;)
	if not id_list: #if is empty
		return
	else:
		have_mail = 1
		latest_email_id = id_list[-1] # get the latest
		result, maildata = mail.fetch(latest_email_id, "(RFC822)") # fetch the email body (RFC822) for the given ID

		msg = email.message_from_string(maildata[0][1])
		for part in msg.walk():
		    # each part is a either non-multipart, or another multipart message
		    # that contains further parts... Message is organized like a tree
		    if part.get_content_type() == 'text/plain':
			continut = part.get_payload() # prints the raw text
			stop_content = continut.strip()
			if (stop_content == 'stop'): #print the stop
            			print "Stop mail received"
                                f = open('/dev/shm/state', 'w')
                                f.write("stop") #notify start
                                f.close()
				return 1
			continut_new = continut.split(" ")
			action = continut_new[0]
			date = continut_new[1].split("\n")
			date = date[0]
			date_unsplit = date
			date = date.split("/")
		return

def check_date(nada):
	global date, action
	day = int(date[0])
	month = int(date[1])
	hours = int(date[2])
	minutes = int(date[3])

	if (day > 0 and day <= 31) and (month > 0 and month <=12) and (hours >=0 and hours <= 23) and (minutes >=0 and minutes <= 59):
		print "date is valid"
		#let's see if the date is in the present or future
		now = datetime.now()

		if (month == now.month) and (day == now.day) and (hours == now.hour) and (minutes == now.minute):
			print "The date is now"
			f = open('/dev/shm/state', 'w')
			if (action == "start"):
				f.write("start now") #notify start
			else:
				print "Action is invalid"
			f.close()
			return
		else: #if the date is not now, let's check if it's in the future or the past
			if (month > now.month):
				print "The date is in the future"
				print_date_action(0)
				return
			elif (month == now.month):
				if (day > now.day):
					print "The date is in the future"
					print_date_action(0)
					return
				elif (day == now.day):
					if (hours > now.hour):
						print "The date is in the future"
						print_date_action(0)
						return
					elif (hours == now.hour):
						if(minutes > now.minute):
							print "The date is in the future"
							print_date_action(0)
							return
						else:
							print "The date is in the past"
							return
					else:
						print "The date is in the past"
						return
				else:
					print "The date is in the past"
					return		
			else:
				print "The date is in the past"
				return					
	else:
		print "date is invalid"
		return

def print_date_action(nada):
	global date_unsplit
	print date_unsplit
	f = open('/dev/shm/state', 'w')
	a = 'start' + ' ' + date_unsplit + '\n'
	f.write(a) #notify start
	f.close()


mail = imaplib.IMAP4_SSL('imap.gmail.com')
mail.login('boilers_gmail_username', 'boilers_gmail_password')
mail.list()
# Out: list of "folders" aka labels in gmail.
mail.select("inbox") # connect to inbox.

stop = check_mail(0)

if have_mail == 1 and stop != 1:
	check_date(0)
