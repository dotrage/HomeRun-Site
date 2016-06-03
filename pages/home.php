<?php 	
	$build->no_cache();	
	$build->enqueueScript("pages/home.js");
	//$build->enqueueStyle("style.css");
	//$build->enqueueStyle("home.css");
	
	$in = array();
	$build->loadHeader();
	$build->loadTPL("home",$in,null,true);
	$build->loadFooter();
?>