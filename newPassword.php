<?php
   $newPassword = $_POST["newPassword"];
	
	$command = 'python newPassword.py ' . $newPassword;
	$output = passthru($command);
	
?>

<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en"><head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<link rel="stylesheet" type="text/css" href="/style.css">
	<link rel=" icon" type="image/ico" href="/gfx/favicon.ico">
	
	<title>Zmiana hasła</title>	
</head>
<body>
       <center>
            <img src="/tlo.jpg"><br>
        </center>
<div id="wrapper">
	<div id="top"><h1>STM SAFE HOUSE</h1></div>
	<div id="top"><h2>Panel Użyszkodnika systemu</h2></div>
	<div id="content">		
	
	
		<div id="c1">
			
			<div id="tekst">
		<div>
		<form name="newPassword" action="index.html" method="post" accept-charset="utf-8">  
    		<h1> Poprawnie zmieniono hasło</h1>
        		<input type="submit" value="Powrót na główną"></li>
        </form>
    			</ul>  	
		<p>Uwaga nieprzestrzeganie polityki bezpieczeństwa grozi usunięciem użytkownika z systemu!</p>
		</div>
         <p>			
		</p></div>
	</div>
	</div>
	<div id="footer">
		<div id="copyright"> <a href="http://www.fizyka.umk.pl/%7Egniewko/python/bandpass/filtr_pasmowy.py">skrypt♥</a> </div>
		<div id="footermenu"><a href="http://fizyka.umk.pl/%7Egniewko/python/bandpass/index.html">Home</a>|<a href="http://www.fizyka.umk.pl/%7Egniewko/python.html">Python WFAiIS</a>|<a href="https://www.youtube.com/watch?v=zKKGA30bHG0">FFT in 6 min</a></div>
		<div class="lewy"></div>
		<div class="prawy"></div>
		
	</div>
</div>

<script id="script-httpbaluptongithubcomjquerysyntaxhighlighterprettifyprettifyminjs" src="Bandpass%20Python_pliki/prettify.js" type="text/javascript"></script><link id="stylesheet-httpbaluptongithubcomjquerysyntaxhighlighterprettifyprettifymincss" href="Bandpass%20Python_pliki/prettify.css" media="screen" rel="stylesheet" type="text/css"><link id="stylesheet-httpbaluptongithubcomjquerysyntaxhighlighterstylesstylemincss" href="Bandpass%20Python_pliki/style_002.css" media="screen" rel="stylesheet" type="text/css"><link id="stylesheet-httpbaluptongithubcomjquerysyntaxhighlighterstylesthemebaluptonmincss" href="Bandpass%20Python_pliki/theme-balupton.css" media="screen" rel="stylesheet" type="text/css"></body></html>