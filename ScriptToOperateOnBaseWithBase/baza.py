#coding=utf-8
import sys
import os
import cgi
import cgitb
import sqlite
cgitb.enable()


login = str(sys.argv[1])
password = str(sys.argv[2])



os.system('chmod 777 *')
with open('login.txt', 'w') as f2:
	f2.write(login +" "+password)
#os.system('echo lubie kotki444444444444444444444 > "/"tmp"/"login.txt')


def readBaseFromFile( baseFile):
	base = {}
	with open(baseFile) as f:
		for line in f:
			(key, val) = line.split()
			base[key] = val
	return base
		
base=readBaseFromFile("baza.txt")

def isInBase(base = base):
	if login in base:
		if base[login] == password:
			return True
	else:
		return False
		

def main():
	if isInBase():
		os.system('php -f true.php')

	else:
		os.system('php -f false.php')

if __name__ == "__main__":
	main()

