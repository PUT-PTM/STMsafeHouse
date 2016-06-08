#!/usr/bin/python
#coding=utf-8
print "Content-type:text/html\r\n\r\n"
print '<html>'
print '<head>'
print '<title>Socket kurcze !</title>'
print '</head>'
print '<body>'
print '<h2>Hello Word! This is my first CGI program</h2>'
print '</body>'
print '</html>'

from BaseHTTPServer import BaseHTTPRequestHandler,HTTPServer
from os import curdir, sep
import os
import subprocess
import socket

import socket
import threading
bind_ip = "0.0.0.0"
bind_port = 9999
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((bind_ip,bind_port))
server.listen(5)
print "[*] Nasłuchiwanie na porcie %s:%d" % (bind_ip,bind_port)
# wątek do obsługi klienta
def handle_client(client_socket):
# drukuje informacje przesłane przez klienta
request = client_socket.recv(1024)
print "[*] Odebrano: %s" % request
# wysyła pakiet z powrotem
client_socket.send("ACK!")
client_socket.close()
while True:
	client,addr = server.accept()
	print "[*] Przyjęto połączenie od: %s:%d" % (addr[0],addr[1])
# utworzenie wątku klienta do obsługi przychodzących danych

client_handler = threading.Thread(target=handle_client,args=(client,))
client_handler.start()
