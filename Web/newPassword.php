<?php
   $newPassword = $_POST["newPassword"];
	
	$command = 'python newPassword.py ' . $newPassword;
	$output = passthru($command);
	
?>

<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en"><head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<link rel="stylesheet" type="text/css" href="/style.css">
	<link rel=" icon" type="image/ico" href="favicon.ico">
	
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
       <form  method="post" action="index.html">
  			<input type="submit" value="Powrót na stronę główną systemu">
		</form>
    			</ul>  	
		<p>Uwaga nieprzestrzeganie polityki bezpieczeństwa grozi usunięciem użytkownika z systemu!</p>
		</div>
         <p>			
		</p></div>
	</div>
	</div>
	<div id="footer">
	<div id="copyright"> <a href="https://github.com/PUT-PTM/STMsafeHouse">projekt♥</a> </div>
		<div id="footermenu"><a href="http://www2.st.com/content/st_com/en.html">STM</a>|<a href="http://www.put.poznan.pl/">Politechnika Poznańska</a></div>
	<div class="lewy"></div>
		<div class="prawy"></div>
		
	</div>
</div>
</body>
</html>