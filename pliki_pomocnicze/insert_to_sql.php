<?php
	session_start();
	
	
	if(isset($_POST['temp1']) && isset($_POST['temp2']) && isset($_POST['humidity']) && isset($_POST['pressure']))
	{
		
	require_once "connect1.php";
	
	
		$temp1 = $_POST['temp1'];
		$temp2 = $_POST['temp2'];
		$humidity = $_POST['humidity'];
		$pressure = $_POST['pressure'];
		$currentDateTime = date('Y-m-d H:i:s'); 
		echo("test");
		
		
		$polaczenie = @new mysqli($host,$db_user,$db_password,$db_name,$db_port);
	
		if($polaczenie->connect_errno!=0)
		{
			echo "Blad polaczenia z baza SQL".$polaczenie->connect_errno;
		}
		else
		{	
					
			$polaczenie->query(sprintf("INSERT INTO `measurement`(`date`, `temp1`, `temp2`, `humidity`, `pressure`) VALUES ('%s', '%s', '%s', '%s', '%s')", 
				$currentDateTime, mysqli_real_escape_string($polaczenie,$temp1), mysqli_real_escape_string($polaczenie,$temp2), mysqli_real_escape_string($polaczenie,$humidity), mysqli_real_escape_string($polaczenie,$pressure)));

			$polaczenie->close();		
		}	
	}
	else
	{
		echo "Brak wszystkich wymaganych danych!";
	}
	
	
?>