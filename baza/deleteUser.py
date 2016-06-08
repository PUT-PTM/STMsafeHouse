#coding=utf-8
import sys
import os
import cgi

login = str(sys.argv[1])

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
			return True
	else:
		return False

def deleteUser(base):
	del base[login]
	return base

def writeToBase(base, baseFile):
	with open(baseFile, 'w') as f:
			for i in base:
				f.write(i+" "+base[i]+'\n')
	return base	

def main():
	if isInBase():
		deleteUser(base)
		writeToBase(base, 'baza.txt')
		os.system('php -f trueRoot.php')
	else:
		os.system('php -f false.php')

if __name__ == "__main__":
	main()
