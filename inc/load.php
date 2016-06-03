<?php 
	/**
	 * SHAMROCK LOADER
	 * @package shamrock
	 * 
	 * This script initiates configurations and the Shamrock Build API
	 * Alter this at your own risk.
	 * 
	 */

	require_once($_SERVER['DOCUMENT_ROOT']."/config.php");
	require_once($_SERVER['DOCUMENT_ROOT']."/inc/config.php");
	require_once($_SERVER['DOCUMENT_ROOT']."/inc/native/build.inc");	
	
	$build = new Build;		
	$build->get_vars(get_defined_vars());	
	$build->init();
	
	//$build->loadClass("API",array(API_KEY,API_SECRET));

	$build->loadPage();
?>