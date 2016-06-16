#coding=utf-8
import sys
import os
import cgi

#password = str(sys.argv[1])
password = 'toor'
login = 'root'

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
		if base['root'] == password:
			return True
	else:
		return False
		

def main():
	if isInBase():
		os.system('php -f trueRoot.php')
	else:
		os.system('php -f false.php')

if __name__ == "__main__":
	main()
