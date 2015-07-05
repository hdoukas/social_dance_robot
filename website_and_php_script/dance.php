<?php
require("phpMQTT.php");

session_start();

//check if this user has already connected
if (!isset($_SESSION['dance3'])) {
	echo "new session";
  $_SESSION['dance3'] = time();
  //add this new user to the file list
  $file = 'connected.txt';
	// Open the file to get existing content
	$current = file_get_contents($file);
	// Append a new person to the file
	$current .= "1\n";
	// Write the contents back to the file
	file_put_contents($file, $current);

	echo "<p>file written</p>";


} else {
//same user do nothing

  
}

//check how many users have been connected
//send to mqtt the dance command
//check the connected.txt file:
if(file_exists('connected.txt')) {
	$mqtt = new phpMQTT("iot.eclipse.org", 1883, "wso2con");
	$file = "connected.txt"; 

  	$lines = count(file($file)); 

  	if($lines > 2) {
  		//send long dance command:
  		if ($mqtt->connect()) {
			$mqtt->publish("wso2con",'2',0);
			$mqtt->close();
		}
		
  	}

  	else {
  		//send short dance command:
  		if ($mqtt->connect()) {
			$mqtt->publish("wso2con",'1',0);
			$mqtt->close();
		}

  	}
}


?>