 <?php
 

$servername = "mysql1.ugu.pl";
$username = "db698510";
$password = "e23we23w";
$dbname = "db698510";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}

$sql = "SELECT DISTINCT * FROM measurement order by ID DESC limit 1";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
  // output data of each row
  while($row = $result->fetch_assoc()) {
    echo $row["date"] . " - Temp1: " . $row["temp1"]. " Temp2: " . $row["temp2"]. " Wilgotnosc: " . $row["humidity"]. " Cisnienie: " . $row["pressure"]. "<br>";
  }
} else {
  echo "0 results";
}
$conn->close();
?> 