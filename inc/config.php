<?php 
	/**
	 * 	MASTER CONFIGURATION FILE
	 *	This file is the master configuration file.  Please do not edit this file.  If you wish to set configurations for
	 *	your project, please use /config.php.  Altering this page may result in bad things man.
	 */	

	if (!defined("SITE_TITLE")){
		define("SITE_TITLE","Shamrock Framework v2");
	}
	if (!defined("TIMEZONE")){
		define("TIMEZONE","UTC");
	}
	
	if (!defined("PASSWORD_SALT")){
		define("PASSWORD_SALT","abc1234");
	}
?>