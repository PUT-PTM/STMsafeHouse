<?php
	$separator = " ";
    $username = $_POST["username"];
	$username .= ' ';    
    $password =  $_POST["password"];
    //$data = array( $username, $password);
    
    
    // This is the data you want to pass to Python
$data = array('as', 'df', 'gh');

// Execute the python script with the JSON data
//$result = shell_exec('python /path/to/myScript.py ' . escapeshellarg(json_encode($data)));
    
 $result = shell_exec('python user.py ' . escapeshellarg(json_encode($data)) . ' 2>&1');

// Decode the result
$resultData = json_decode($result, true);

// This will contain: array('status' => 'Yes!')
var_dump($resultData);
   	
?>
<html>
   
   <head>
      <title>Welcome </title>
   </head>
   
   <body>
      <h1>Welcome <?php echo $_POST["username"] ?></h1> 
      <h1>Welcome <?php echo $_POST["password"] ?></h1>
      <h2><a href = "logout.php">Sign Out</a></h2>
   </body>
   
</html>