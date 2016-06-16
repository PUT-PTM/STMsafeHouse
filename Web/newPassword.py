#coding=utf-8
import sys
import os
import cgi

noweHaslo = str(sys.argv[1])
print noweHaslo

def readBaseFromFile( baseFile):
	base = {}
	with open(baseFile) as f:
		for line in f:
			(key, val) = line.split()
			base[key] = val
	return base

def writeBaseToFile( baza):
	with open('baza.txt', 'w') as f:
		for i in baza:
			f.write(i+" "+baza[i]+"\n")


def readLogin():
	with open('/tmp/login.txt', 'r') as f:
		login = f.read().split()[0]
	return login
	
login = readLogin()
baza = readBaseFromFile('baza.txt')
baza[login] = noweHaslo
print baza
writeBaseToFile(baza)


print """<title>Zmieniono hasło</title> <br/><h1>Poprawnie zmieniono hasło<h1> """
