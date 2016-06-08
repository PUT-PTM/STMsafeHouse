<?php
   $password = $_POST["password"];
   $login = 'root';
	
	$command = 'python bazaRoot.py ' . $password;
	$output = passthru($command);
	
?>

<html>
   
   <head>
      <title>Welcome Root !</title>
   </head>
   
   <body>
	Witaj Root :>
   </body>
   
</html>
