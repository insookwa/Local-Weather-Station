<!DOCTYPE html>
<html>
<?php

$servername = "localhost";

// REPLACE with your Database name
$dbname = "weather_station";
// REPLACE with Database user
$username = "root";
// REPLACE with Database user password
$password = "";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT * FROM weather_data order by reading_time desc limit 40";

$result = $conn->query($sql);
?>


<meta name="viewport" content="width=device-width, initial-scale=1">
<script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
<style>
table, th, td {
  border: 1px solid black;
  border-collapse: collapse;
}
th, td {
  padding: 5px;
  text-align: left;
}
</style>
<title>WEATHER STATION DATA</title>
  <body>
   <center><h2>NDEJJE UNIVERSITY WEATHER STATION BY WATER RESEARCH LABARATORY LOGS</h2></center> 
	<table style="width:100%"> 
  <caption><b>LADY IRENE STATION </b></caption>
	<thead>
		<tr>
			<th>Id</th>
			<th>Temperature</th>
			<th>Humidity</th>
			<th colspan="2">Time Stamp</th>
		</tr>
	</thead>
	
    <?php while ($row = mysqli_fetch_array($result)) { ?>
		<tr>
			<td><?php echo $row['id']; ?></td>
			<td><?php echo $row['temperature']; ?></td>
			<td><?php echo $row['humidity']; ?></td>
			<td><?php echo $row['reading_time']; ?></td>
			
		</tr>
	<?php } ?>
</table>
</body>
</html>

