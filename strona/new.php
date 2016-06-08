<?php
   $login = $_POST["username"];
   $password = $_POST["password"];
	
	$command = 'python newUser.py ' . $login . ' ' . $password;
	$output = passthru($command);
	
?>

<html>
   
   <head>
      <title>New User </title>
   </head>
   
   <body>
   </body>
   
</html>