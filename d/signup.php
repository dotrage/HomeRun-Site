<?php 
	$request = $_POST;
	$response = array();
	
	$build->loadClass("DB",array(
		"localhost","tcennis","bigbe4r","hr_demo"	
	));
	
	if (!empty($request['email'])){
		$build->db->query("INSERT INTO reservations (email,timestamp) VALUES ('%s',%d)",array($request['email'],date("U")));
	}
	
	echo json_encode($response);
?>