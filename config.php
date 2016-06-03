<?php
	/**	
	 * 	SITE CONFIGURATION FILE
	 * 	Customize your site by setting common configuration settings for Shamrock, plus add any additional required configurations
	 *  for custom classes or functions here.  You may also override Shamrock's default configurations here as well.
	 *  
	 *  Shamrock also accepts configuration settings passed via environment variables set in your web server's host configuration.
	 *  To pass configurations from your host configuration file, simple add these with the prefix 'CONFIG_'.  The value will be
	 *  set in the application will truncate the CONFIG_ prefix, leaving your configuration name set as a constant.
	 *  
	 */
	
	define("SITE_TITLE","HomeRun");
	//define("FAVICON","/favicon.png");
	define("TIMEZONE","America/Denver");

	// Define javascript files that should always be included in each page
	$scripts = array(
		"jquery-1.10.2.min",
		"build"
	);
	define("SCRIPTS",json_encode($scripts));
	
	// Define CSS files that should always be included in each page
	$styles = array(
		array("file"=>"style.css")
	);
	define("STYLES",json_encode($styles));
	
	define("SCRIPTS_PATH","/js");
	
	//DO NOT CHANGE THIS ONCE IN USE.  EXISTING PASSWORD AUTHENTICATION WILL FAIL.
	define("PASSWORD_SALT","notusedatthistime");
	
	//MASTER EMAIL FOR THIS SITE
	define("MASTER_EMAIL","chris@functionized.com");
?>