#!/usr/bin/python
#coding=utf-8
import cgi, cgitb

f=cgi.FieldStorage()
l1=f.getfirst('liczba1');l2=f.getfirst('liczba2');dz=f.getfirst('dzialanie')
try:
	l1=int(l1);l2=int(l2)
except:
	l1=0;l2=0
	if dz=='+':
		wynik=l1+l2
	elif dz=='-':
		wynik=l1-l2
	elif dz=='*':
		wynik=l1*l2
	else :
		if l2!=0:
			wynik=l1/l2
		else:
			wynik='inf'
print "Content-type: text/html\n\n"
#print f.getfirst('dzialanie')
print """
	<html>
	<head>
	<meta charset="utf-8">
	</head>
	<body>
	<form method="get"><h2>Kalkulator</h2>
	<input type="text" name="liczba1" value="%s">
	<select name="dzialanie">
	< ;option>+</option>
	<option>-</option>
	<option>*</option>
	<option>/</option>
	</select>
	<input type="text" name="liczba2" value="%s">
	=
	<input type="text" disabled='disabled' name="wynik" value="%s">
	<input type="submit" value="OK">
	</form>
	</body>
	</html>""" % (str(l1), str(l2), str(wynik))
