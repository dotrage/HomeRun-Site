<?php 
	define("DB_QUERY_REGEXP", "/(%d|%s|%%|%f|%b)/");
	
	class DB {
	
		public function __construct($host=null,$user=null,$pass=null,$name=null){			
			if (file_exists($_SERVER['DOCUMENT_ROOT']."/config/database.php")){
				require($_SERVER['DOCUMENT_ROOT']."/config/database.php");
				
				if (!empty($connections[ENV])){
					$meta = $connections[ENV];
				}
			
				if (!empty($meta['connection'])){
					$this->host = $meta['connection']['host'];
					$this->user = $meta['connection']['username'];
					$this->pass = $meta['connection']['password'];
					$this->name = $meta['connection']['database'];
					if (!empty($meta['connection']['port'])){
						$this->port = $meta['connection']['port'];
					}
				}
				else if (!empty($meta['connections'])){
					$this->host = $meta['connections'][0]['host'];
					$this->user = $meta['connections'][0]['username'];
					$this->pass = $meta['connections'][0]['password'];
					$this->name = $meta['connections'][0]['database'];
					if (!empty($meta['connections'][0]['port'])){
						$this->port = $meta['connections'][0]['port'];
					}
					$this->connections = $meta['connections'];
				}
			}
			else{
				$this->host = $host;
				$this->user = $user;
				$this->pass = $pass;
				$this->name = $name;
			}
		}
	
		public function setup(){
			if (empty($this->host) || empty($this->user) || empty($this->pass) || empty($this->name)){
				return false;
			}
			else{
				return true;
			}
		}
		
		//Confirm the server is accessible
		public function check(){
			if (!empty($this->host) && !empty($this->name) && !empty($this->user) && !empty($this->pass)){
				$dbc = @mysql_connect($this->host, $this->user, $this->pass);
				if (!$dbc){
					return false;
				}
				else if (!mysql_select_db($this->name,$dbc)){
					return false;
				}
			}
			else{
				return false;
			}
			return true;
		}
	
		// Opens a connection to the database and sets up mysql_query object
		public function query($sql,$args=array()){
			if (!empty($sql) && !is_array($sql)){
				$sql = array("query" => $sql);
			}
	
			if (!empty($sql['query'])){
				//$query = sprintf($sql['query'],$arguments);
			}
	
			if (!empty($sql['show_error'])){
				$show_error = $sql['show_error'];
			}
			else{
				$show_error = TRUE;
			}
	
	
			if (!$this->setup()){
				printf("Data Source is Invalid.");
				exit;
			}
	
			if (!($conn=mysql_connect($this->host, $this->user, $this->pass))){
				printf("Error connecting to database.");
				exit;
			}
	
			$this->string_sanitize($args, TRUE);
			$query = preg_replace_callback(DB_QUERY_REGEXP, array(&$this, 'string_sanitize'), $sql['query']);
			if ($show_error){
				$db = mysql_select_db($this->name,$conn) or die("Unable to connect to database.");
				$result = mysql_query($query,$conn) or (error_log(mysql_error()) and error_log($query) and die(mysql_error()));
			}
			else{
				$db = mysql_select_db($this->name,$conn);
				$result = mysql_query($query,$conn);
			}
	
			if (!$result){
				if ($show_error){
					echo "Error querying database.";
				}
			}
			else{
				return $result;
			}
	
		}
	
		// Executes queue and return a single variable
		public function get_var($sql){
	
			if (!empty($sql['output'])){
				if ($sql['output'] == "array" || $sql['output'] == "object"){
					$type = $sql['output'];
				}
				else{
					$type = "array";
				}
			}
			else{
				$type = "array";
			}
	
			$args = func_get_args();
			array_shift($args);
	
			if (isset($args[0]) and is_array($args[0])){
				$args = $args[0];
			}
	
			$result = $this->query($sql,$args);
	
			if (count($result)==1){
				if (count($result[0]) == 1){
					return $result[0][0];
				}
			}
	
		}
	
		// Executes query and filters results into a single array or object
		public function get_row($sql){
	
			if (!empty($sql['output'])){
				if ($sql['output'] == "array" || $sql['output'] == "object"){
					$type = $sql['output'];
				}
				else{
					$type = "array";
				}
			}
			else{
				$type = "array";
			}
	
			$args = func_get_args();
			array_shift($args);
	
			if (isset($args[0]) and is_array($args[0])){
				$args = $args[0];
			}
	
			$result = $this->query($sql,$args);
	
			if ($result){
				$row = mysql_fetch_assoc($result);
				if ($type == "array"){
					return $row;
				}
				if ($type == "object"){
					return (object) $row;
				}
			}
	
		}
	
		// Executes query and filters results into an array or object
		public function get_results($sql){
	
			if (!empty($sql['output'])){
				if ($sql['output'] == "array" || $sql['output'] == "object"){
					$type = $sql['output'];
				}
				else{
					$type = "array";
				}
			}
			else{
				$type = "array";
			}
	
			$args = func_get_args();
			array_shift($args);
	
			if (isset($args[0]) and is_array($args[0])){
				$args = $args[0];
			}
	
			$result = $this->query($sql,$args);
	
			$return = array();
	
			while ($row = mysql_fetch_assoc($result)){
				if ($type == "array"){
					$return[] = $row;
				}
				if ($type == "object"){
					$return[] = (object) $row;
				}
			}
	
			if ($result){
				if ($type == "array"){
					return $return;
				}
				if ($type == "object"){
					return (object) $return;
				}
			}
		}
	
		// Executes query and returns row count for result set
		public function num_rows($sql){
	
			$args = func_get_args();
			array_shift($args);
	
			if (isset($args[0]) and is_array($args[0])){
				$args = $args[0];
			}
	
			$result = $this->query($sql,$args);
	
			if ($result){
				$row = mysql_num_rows($result);
				return $row;
			}
	
		}
	
		private function string_sanitize($match,$init=FALSE){
			static $args = NULL;
			if ($init) {
				$args = $match;
				return;
			}
	
			switch ($match[1]) {
				case '%d':
					return (int) array_shift($args); // We don't need db_escape_string as numbers are db-safe
				case '%s':
					return mysql_real_escape_string(array_shift($args));
				case '%%':
					return '%';
				case '%f':
					return (float) array_shift($args);
				case '%b': // binary data
					return mysql_real_escape_string(array_shift($args));
			}
		}
	
	}