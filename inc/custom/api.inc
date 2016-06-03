<?php 
	class API {
		public $useragent = "HomeRun v1.0 PHP API Client";
		
		protected $api_key;
		protected $api_secret;
		
		public function __construct($key,$secret){
			$this->api_key = $key;
			$this->api_secret = $secret;
			$this->api_url = API_SERVER;			
		}
		
		public function get($endpoint,$data=array()){
			$return = $this->request("get",$endpoint,$data);
			return $return;
		}
		
		public function post($endpoint,$data=array()){
			if (empty($data)){
				$data['post'] = 1;
			}
			$return = $this->request("post",$endpoint,$data);
			return $return;
		}
		
		public function put($endpoint,$data=array()){
			if (empty($data)){
				$data['put'] = 1;
			}
			$return = $this->request("put",$endpoint,$data);
			return $return;
		}
		
		public function delete($endpoint,$data=array()){
			$return = $this->request("delete",$endpoint,$data);
			return $return;
		}
		
		private function request($method,$endpoint,$data){
			if (!empty($method) && !empty($endpoint)){
				$ch = curl_init();
				
				$url = $this->api_url.$endpoint;
				
				$sig = hash_hmac("sha256",$this->api_key."/".$endpoint,$this->api_secret);
				
				$headers = array(
					"api-key: ".$this->api_key,
					"api-sig: ".$sig
				);
				
				curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
				curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
				curl_setopt($ch, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
				
				if ($method == "get"){
					if (!empty($data)){
						$url .= "?".http_build_query($data);
					}
				}
				else if ($method == "post"){
					curl_setopt($ch, CURLOPT_POST, 1);
					if (!empty($data)){
						curl_setopt($ch, CURLOPT_POSTFIELDS, http_build_query($data));
					}
				}
				else if ($method == "put"){
					curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "PUT");
					if (!empty($data)){
						curl_setopt($ch, CURLOPT_POSTFIELDS, http_build_query($data));
					}
				}
				else if ($method == "delete"){
					curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "DELETE");
					if (!empty($data)){
						curl_setopt($ch, CURLOPT_POSTFIELDS, http_build_query($data));
					}
				}
				else{
					error_log("METHOD: ".$method);
					die();
				}

				curl_setopt($ch, CURLOPT_URL, $url);
				
				$result = curl_exec($ch);

				$http_response_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
				
				curl_close($ch);
								
				return json_decode($result);
			}
			else{
				return "Invalid use of API Client";	
			}
		}
	}
?>