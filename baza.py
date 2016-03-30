#coding=utf-8

import sys
import os

login = str(sys.argv[1])
password = str(sys.argv[2])


def readBaseFromFile( baseFile):
	base = {}
	with open(baseFile) as f:
		for line in f:
			(key, val) = line.split()
			base[key] = val
	return base
		
base=readBaseFromFile("baza.txt")

def isInBase(base = base):
	if base[login] == password:
		return True
	else:
		return False
		

def main():
	if isInBase():
		os.system("php -f true.php")
	else:
		os.system("php -f false.php")
		

if __name__ == "__main__":
	main()
