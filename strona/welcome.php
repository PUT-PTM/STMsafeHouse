<?php
   $login = $_POST["username"];
   $password = $_POST["password"];
	
	$command = 'python baza.py ' . $login . ' ' . $password;
	$output = passthru($command);
	
?>

<html>
   
   <head>
      <title>Welcome </title>
   </head>
   
   <body>
	<h1> Witaj :> </h1>
   </body>
   
</html>
