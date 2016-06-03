<?php 
	$vars = array();
	
	if (defined("SITE_NAME")){
		$pagetitle = SITE_NAME;
	}
	
	$vars['page_title'] = $pagetitle;	
	$vars['styles'] = $headerstyles;
	$vars['scripts'] = $headerscripts;
	$vars['favicon'] = "";
	$vars['nav'] = "";
	
	if (defined("FAVICON")){
		$vars['favicon'] = "<link rel=\"shortcut icon\" href=\"".FAVICON."\" />";
	}
	
	$build->loadTPL("header",$vars,null,true);
?>