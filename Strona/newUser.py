#coding=utf-8
import sys
import os
import cgi

login = str(sys.argv[1])
password = str(sys.argv[2])
def readBaseFromFile( baseFile):
	base = {}
	with open(baseFile, 'r') as f:
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

def addUser(base):
	base[login] = password
	return base

def writeToBase(base, baseFile):
	with open(baseFile, 'w') as f:
			for i in base:
				f.write(i+" "+base[i]+'\n')
	return base	

def main():
	if not isInBase():
		addUser(base)
		writeToBase(base, 'baza.txt')
		print "<html>Poprawnie dodano użytkownika</html>"
	else:
		print "<html>Użytkownik już istnieje w bazie !</html>"

if __name__ == "__main__":
	main()
