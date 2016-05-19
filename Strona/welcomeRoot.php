<?php
   $password = $_POST["password"];
	
	$command = 'python bazaRoot.py ' . $password;
	$output = passthru($command);
	
?>

<html>
   
   <head>
      <title>Welcome Root !</title>
   </head>
   
   <body>
   </body>
   
</html>