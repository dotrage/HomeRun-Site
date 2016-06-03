<?php
	/**
	 * Shamrock Build API	
	 * @package shamrock
	 * 
	 * Do not remove or edit this file!
	 * 
	 */

	class Build {
		function __construct(){
			$this->slug = $this->slug();
			$this->hooks = array();
			$this->scripts = array();		
			$this->request_type = $_SERVER['REQUEST_METHOD'];	
		}
		
		public function init(){
			session_start();
			$this->setup_config();
			$this->load_classes();
		}
		
		public function loadHeader($custom=null){
			global $build;
			
			$this->processHooks("before","build","header");

			$pagetitle = "";
			if (!empty($this->page_title)){
				$pagetitle = $this->page_title;
			}
			else if (defined("SITE_TITLE")){
				$pagetitle = SITE_TITLE;
			}
			
			if (!empty($this->page_sub_title)){
				$pagetitle .= " | " . $this->page_sub_title;
			}
			
			$headerstyles = $this->load_header_styles();
			$headerscripts = $this->load_header_scripts();			
			
			if (!empty($custom) && file_exists($_SERVER['DOCUMENT_ROOT']."/".$custom.".php")){
				require_once($_SERVER['DOCUMENT_ROOT']."/".$custom.".php");
			}
			else{
				require_once($_SERVER['DOCUMENT_ROOT']."/header.php");
			}
			
			$this->processHooks("after","build","header");
		}
		
		public function loadPage($page=null){
			if (!empty($this->page_vars)){
				foreach($this->page_vars as $k=>$v){
					$$k = $v;
				}
			}
			
			if (empty($page)){
				$page = (!empty($this->slug['page']))?$this->slug['page']:"home";
			}
			
			if ($page == "d"){
				$this->loadDataPage();
			}			
			else if (file_exists($_SERVER['DOCUMENT_ROOT']."/pages/".$page.".php")){
				require_once($_SERVER['DOCUMENT_ROOT']."/pages/".$page.".php");
			}
			else{
				require_once($_SERVER['DOCUMENT_ROOT']."/pages/home.php");
			}
		}
		
		public function loadDataPage(){
			$full_slug = implode("/",$this->slug['args']);			
			$args = $this->slug['args'];
			$this->slug['page'] = $args[0];
			array_shift($args);
			$this->slug['args'] = $args;
			
			$build = $this;
			if (file_exists($_SERVER['DOCUMENT_ROOT']."/d/".$full_slug.".php")){
				require_once($_SERVER['DOCUMENT_ROOT']."/d/".$full_slug.".php");
			}			
			else if (file_exists($_SERVER['DOCUMENT_ROOT']."/d/".$this->slug['page'].".php")){
				require_once($_SERVER['DOCUMENT_ROOT']."/d/".$this->slug['page'].".php");
			}
		}
		
		public function loadFooter($custom=null){
			global $build;
			
			$this->processHooks("before","build","footer");
			
			if (!empty($custom) && file_exists($_SERVER['DOCUMENT_ROOT']."/".$custom.".php")){
				require_once($_SERVER['DOCUMENT_ROOT']."/".$custom.".php");
			}
			else{
				require_once($_SERVER['DOCUMENT_ROOT']."/footer.php");
			}
			
			$this->processHooks("after","build","footer");
		}
		
		public function loadTPL($template,$vars=array(),$subdir=null,$echo=false){
			if (!empty($template)){
				if (!empty($subdir)){
					if (file_exists($_SERVER['DOCUMENT_ROOT']."/templates/".$subdir."/".$template.".htm")){
						$file = $_SERVER['DOCUMENT_ROOT']."/templates/".$subdir."/".$template.".htm";
					}
				}
				else{
					if (file_exists($_SERVER['DOCUMENT_ROOT']."/templates/native/".$template.".htm")){
						$file = $_SERVER['DOCUMENT_ROOT']."/templates/native/".$template.".htm";
					}
					else if (file_exists($_SERVER['DOCUMENT_ROOT']."/templates/pages/".$template.".htm")){
						$file = $_SERVER['DOCUMENT_ROOT']."/templates/pages/".$template.".htm";						
					}
					else if (file_exists($_SERVER['DOCUMENT_ROOT']."/templates/partials/".$template.".htm")){
						$file = $_SERVER['DOCUMENT_ROOT']."/templates/partials/".$template.".htm";
					}
					else if (file_exists($_SERVER['DOCUMENT_ROOT']."/templates/".$template.".htm")){
						$file = $_SERVER['DOCUMENT_ROOT']."/templates/".$template.".htm";
					}
				}
				
				if (!empty($file)){
					$html = file_get_contents($file);

					if (!empty($html)){
						if (!empty($vars) && (is_array($vars) || is_object($vars))){
							foreach ($vars as $k=>$v){
								if (!is_array($v) && !is_object($v)){
									$html = str_replace("{{".$k."}}",$v,$html);
								}
							}
						}
						
						if ($echo){
							echo $html;	
						}
						else{
							return $html;
						}
					}
				}
			}
		}
		
		public function get_vars($vars=array()){
			if (!empty($vars)){
				$this->page_vars = $vars;
			}
		}
		
		public function setPageTitle($title){
			if (!empty($title)){
				$this->page_title = $title;
			}
		}
		
		public function setPageSubTitle($title){
			if (!empty($title)){
				$this->page_sub_title = $title;
			}
		}		
		
		public function hook($mode=null,$target=null,$function=null,$function_args=null){
			if (!empty($mode) && !empty($target) && !empty($function)){
				$class = null;
				if (is_array($function) && count($function) == 2){					
					if (class_exists($function[0])){
						$class = $function[0];
						$function_ref = $function[1];						
					}
				}
				else{
					if (!empty($function['function']))
					if (function_exists($function['function'])){
						$function_ref = $function['function'];
					}
				}
				if (!empty($function_ref)){
					$this->construct_hook($mode,$target,$function_ref,$function_args,$class);
				}
			}
		} 
		
		public function processHooks($mode,$class,$action){
			if (!empty($mode)){
				if (!empty($class)){
					if (!empty($this->hooks[$class][$mode][$action]) && is_array($this->hooks[$class][$mode][$action])){
						$hooks = $this->hooks[$class][$mode][$action];
					}
				}
				else{
					if (!empty($this->hooks['build'][$mode][$action]) && is_array($this->hooks['build'][$mode][$action])){
						$hooks = $this->hooks['build'][$mode][$action];
					}
				}
				
				if (!empty($hooks)){
					foreach ($hooks as $hook){
						$this->apply_hook($hook);
					}
				}
			}
		}
		
		public function loadClass($obj=null,$args=array(),$name=null){
			if (!empty($obj) && class_exists($obj)){
				if (empty($name)){
					$name = strtolower($obj);
				}				
				
				if (empty($this->$name)){
					if (!empty($args)){
						$class = new ReflectionClass($name);					
						$this->$name = $class->newInstanceArgs($args);
					}
					else{
						$this->$name = new $obj;
					}
				}
			}
		}
		
		public function enqueueStyle($style){
			$this->styles[] = $style;
		}
		
		public function enqueueScript($script){
			$this->scripts[] = $script;
		}
		
		public function redirect($url){
			header("Location: ".$url);
			exit();
		}
		
		public function no_cache(){
			header('Cache-Control: private, no-store, no-cache, must-revalidate');
			header('Cache-Control: post-check=0, pre-check=0', false);
			header('Pragma: no-cache');
			header('Expires: Mon, 26 July 1997 05:00:00 GMT');
			header('Last-Modified: '.gmdate("D, d M Y H:i:s")." GMT");
		}
		
		public function trace($error,$mode="error_log"){
			$return = "";
			if (is_array($error) || is_object($error)){
				$return = print_r($error,1);
			}
			else {
				$return = $error;
			}
			
			if ($mode == "echo"){
				echo $return;
			}
			else{
				error_log($return);
			}
		}
		
		
		
		
		
		
		
		private function slug(){
			if (!empty($_GET['uri'])){
				if (strpos($_GET['uri'],"?")){
					$tmp = explode("?",trim($_GET['uri'],"/"));
					$tmp = $tmp[0];
					$tmp = explode("/",$tmp);
					$slug['page'] = $tmp[0];
					array_shift($tmp);
					$slug['args'] = $tmp;
										
					return $slug;
				}
				else{
					$tmp = explode("/",trim($_GET['uri'],"/"));
					$slug['page'] = $tmp[0];
					array_shift($tmp);
					$slug['args'] = $tmp;
					return $slug;
				}
			}
			else{
				return array("page"=>"home");
			}
		}
		
		private function load_header_styles(){
			$path = (defined("STYLES_PATH"))?rtrim(STYLES_PATH,"/"):"/css";
			$html = "";
			if (!empty($this->styles)){
				foreach ($this->styles as $style){
					if (is_array($style)){
						$media = "";
						if (!empty($style['media'])){
							$media = " media=\"".$style['media']."\"";
						}						
						if (!empty($style['file'])){
							if (!strpos("__".$style['file'],"http://") && !strpos("__".$style['file'],"https://")){
								$style['file'] = $path."/".$style['file'];
							}
							
							$html .= "<link href=\"".$style['file']."\"".$media." rel=\"stylesheet\" type=\"text/css\">\n";
						}						
					}
					else{
						if (!strpos("__".$style,"http://") && !strpos("__".$style,"https://")){
							$style = "/css/".$style;
						}
						
						$html .= "<link href=\"".$style."\" rel=\"stylesheet\" type=\"text/css\">\n";
					}
				}
			}
			
			return $html;
		}
		
		private function load_header_scripts(){
			$path = (defined("SCRIPTS_PATH"))?rtrim(SCRIPTS_PATH,"/"):"/scripts";
			$html = "";
			if (!empty($this->scripts)){								
				foreach ($this->scripts as $script){
					if (is_array($script)){
						if (!empty($script['require'])){
							$html .= "<script type=\"text/javascript\">var req_token = \"".str_replace(".","",microtime(1))."\"; var require = { urlArgs:\"token=\" + req_token };</script>\n";
							$html .= "<script data-main=\"".$path."/".$script['require']."\" src=\"/scripts/require-jquery.js\"></script>\n";
						}						
					}
					else{						
						if (strpos("_".$script,"https://") || strpos("_".$script,"http://")){
							$html .= "<script src=\"".$script."?".str_replace(".","",microtime(1))."\"></script>\n";
						}
						else{
							if (substr($script,strlen($script)-3,3) == ".js"){
								$html .= "<script src=\"".$path."/".$script."?".str_replace(".","",microtime(1))."\"></script>\n";
							}
							else{
								$html .= "<script src=\"".$path."/".$script.".js?".str_replace(".","",microtime(1))."\"></script>\n";
							}
						}
					}
				}				
			}
			
			return $html;
		}
		
		private function loaded_files(){
			$files = get_included_files();
			$payload = array();
			foreach ($files as $file){
				$payload[$file] = 1;
			}
			return $payload;
		}
		
		private function setup_config(){
			if (defined("TIMEZONE")){
				date_default_timezone_set(TIMEZONE);
			}
			if (!empty($_SERVER)){
				foreach ($_SERVER as $k=>$v){
					if (strtoupper(substr($k,0,7)) == "CONFIG_"){
						$name = str_replace("CONFIG_","",strtoupper($k));
						if (!defined($name)){
							define($name,$v);
						}
					}
				}
			}
			if (defined("SCRIPTS")){
				$scripts = json_decode(SCRIPTS,1);
				foreach ($scripts as $script){
					$this->scripts[] = $script;
				}
			}
			if (defined("STYLES")){
				$styles = json_decode(STYLES,1);
				foreach ($styles as $style){
					$this->styles[] = $style;
				}
			}
		}
		
		private function load_classes(){
			global $build;
			
			$loaded_files = $this->loaded_files();
			
			if ($handle = opendir($_SERVER['DOCUMENT_ROOT']."/inc/native")) {
				while (false !== ($entry = readdir($handle))) {
					if ($entry != "." && $entry != ".." && $entry != "build.inc") {
						$file = file_get_contents($_SERVER['DOCUMENT_ROOT']."/inc/native/".$entry);
						$tokens = token_get_all($file);
						$class_token = true;
						$classes = array();
						foreach ($tokens as $token){
							if (is_array($token)){
								if ($token[0] == T_CLASS){
									$class_token = true;
								} 
								else if ($class_token && $token[0] == T_STRING){
									$class_name = strtolower($token[1]);
									if (empty($this->$class_name)){
										$classes[$class_name] = $token[1];
									}									
									$class_token = false;
								}
							}
						}
						if (!empty($classes) && empty($loaded_files[$_SERVER['DOCUMENT_ROOT']."/inc/native/".$entry])){
							require_once($_SERVER['DOCUMENT_ROOT']."/inc/native/".$entry);
							foreach($classes as $k=>$v){
								$this->$k = new $v;
							}
						}
					}
				}
			}
			
			if ($handle2 = opendir($_SERVER['DOCUMENT_ROOT']."/inc/custom")) {
				while (false !== ($entry = readdir($handle2))) {
					if ($entry != "." && $entry != "..") {						
						$file = file_get_contents($_SERVER['DOCUMENT_ROOT']."/inc/custom/".$entry);
						
						$tokens = token_get_all($file);
						$class_token = true;
						$classes = array();
						
						foreach ($tokens as $token){
							if (is_array($token)){
								if ($token[0] == T_CLASS){
									$class_token = true;
								}
								else if ($class_token && $token[0] == T_STRING){
									$class_name = strtolower($token[1]);
									if (empty($this->$class_name)){
										$classes[$class_name] = $token[1];
									}
									$class_token = false;
								}
							}
						}						
						if (!empty($classes) && empty($loaded_files[$_SERVER['DOCUMENT_ROOT']."/inc/custom/".$entry])){
							require_once($_SERVER['DOCUMENT_ROOT']."/inc/custom/".$entry);
						}
					}
				}
			}
		}
		
		private function construct_hook($mode,$target,$function,$function_args,$class=null){
			if (!empty($class)){
				if (is_array($target)){
					if (!empty($function_args)){
						$this->hooks[$target[0]][$mode][$target[1]][] = array("class"=>$class,"function"=>$function,"args"=>$function_args);
					}
					else{
						$this->hooks[$target[0]][$mode][$target[1]][] = array("class"=>$class,"function"=>$function);
					}
				}
				else{
					if (!empty($function_args)){
						$this->hooks['build'][$mode][$target][] = array("class"=>$class,"function"=>$function,"args"=>$function_args);
					}
					else{
						$this->hooks['build'][$mode][$target][] = array("class"=>$class,"function"=>$function);
					}
				}
			}
			else{
				if (is_array($target)){
					if (!empty($function_args)){
						$this->hooks[$target[0]][$mode][$target[1]][] = array("function"=>$function,"args"=>$function_args);
					}
					else{
						$this->hooks[$target[0]][$mode][$target[1]][] = array("function"=>$function);
					}
				}
				else{
					if (!empty($function_args)){
						$this->hooks['build'][$mode][$target][] = array("function"=>$function,"args"=>$function_args);
					}
					else{
						$this->hooks['build'][$mode][$target][] = array("function"=>$function);
					}
				}
			}
		}
		
		private function apply_hook($hook){
			if (!empty($hook['function'])){
				if (!empty($hook['class'])){
					if (class_exists($hook['class'])){
						if (!empty($hook['args'])){
							$hook['class']->$hook['function']($hook['args']);
						}
						else{
							$hook['class']->$hook['function']();
						}
					}
				}	
				else{
					if (function_exists($hook['function'])){
						if (!empty($hook['args'])){
							$hook['function']($hook['args']);
						}
						else{
							$hook['function']();
						}
					}		
				}
			}			
		}		
	}
?>