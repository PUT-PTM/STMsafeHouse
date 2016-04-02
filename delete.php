<?php
   $login = $_POST["username"];
	
	$command = 'python deleteUser.py ' . $login;
	$output = passthru($command);
	
?>

<html>
   
   <head>
      <title>delete user </title>
   </head>
   
   <body>
   </body>
   
</html>