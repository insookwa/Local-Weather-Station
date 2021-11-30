<?php


$servername = "localhost";

// REPLACE with your Database name
$dbname = "weather_station";
// REPLACE with Database user
$username = "root";
// REPLACE with Database user password
$password = "";



$temperature = $humidity = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    
    
        $temperature = test_input($_POST["temperature"]);
        $humidity = $_POST["humidity"];
		$logfile = "logs.txt";;
		$logdata = "Temperature = "+$temperature+" humidity= "+$humidity+" Time: "+ date("Y:m:D:h:i:s");
		$fp = fopen($logfile, 'w');
		fwrite($fp,$logdata);
		fclose($fp);
	   
        
        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 
        
        $sql = "INSERT INTO weather_data (temperature, humidity)
        VALUES ('" . $temperature . "', '" . $humidity . "')";
        
        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } 
        else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
    
        $conn->close();
    


}
else {
    echo "No data posted with HTTP POST.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
