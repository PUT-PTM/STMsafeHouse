import smtplib

gmail_user = 'stm.plytka@gmail.com'
gmail_pwd = 'KochamManka'
FROM = gmail_user # sendmail needs the from
TO = 'nika.pawlaczyk@gmail.com'
message = 'Ktos Ci sie wlamuje na chate Kotku <3\n\n\nKochajacy, Zawsze Twoj\nSTM'

# SMTP_SSL
server_ssl = smtplib.SMTP_SSL("smtp.gmail.com", 465)
print "fsdfsdf"
server_ssl.ehlo() # optional, called by login()
print "fsdfsdf"
server_ssl.login(gmail_user, gmail_pwd)
server_ssl.sendmail(FROM, TO, message)
server_ssl.close()
print 'successfully sent the mail'
