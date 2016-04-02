<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en"><head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
	<link rel="stylesheet" type="text/css" href="/style.css">
	<link rel=" icon" type="image/ico" href="favicon.ico">

	<title>Witaj Root !</title>	
</head>
<body>
       <center>
            <img src="/tlo.jpg"><br>
        </center>
<div id="wrapper">
	<div id="top"><h1>STM SAFE HOUSE</h1></div>
	<div id="content">		
	<h5>Witaj w systemie <?php echo $_POST['username']; ?> !<h5/>	
		<div id="c1">
			
			<div id="tekst">
		<div>	
		<h5>Witaj Root wybierz akcję którą chcesz wykonać:</h5>	
		<form  method="post" action="index.html">
  			<input type="submit" value="Powrót na stronę główną systemu">
		</form>		
		<form  method="post" action="hasloUser.php">
  			<input type="submit" value="Zmiana hasła">
		</form>
		<form  method="post" action="newUser.php">
  			<input type="submit" value="Dodanie Nowego użytkownika systemu">
		</form>		
		<form  method="post" action="deleteUser.php">
  			<input type="submit" value="Usuniecie Użytkownika z systemu">
		</form>	
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