String.prototype.replaceAll = function(str1, str2, ignore){
	return this.replace(new RegExp(str1.replace(/([\/\,\!\\\^\$\{\}\[\]\(\)\.\*\+\?\|\<\>\-\&])/g,"\\$&"),(ignore?"gi":"g")),(typeof(str2)=="string")?str2.replace(/\$/g,"$$$$"):str2);
}

var build = {
	listen: function(callback,load){
		$(window).bind('hashchange', function(){
			if (callback != undefined && typeof callback == "function"){
				var page = build.getPath();
				var obj = build.anchorPageProperties();
				callback(page,obj);
			}			
		});
		if (load != undefined && load){
			var page = build.getPath();
			var obj = build.anchorPageProperties();
			callback(page,obj);
		}
	},
	tpl: {
		load: function(obj){
	    	//template = name of template file
	    	//variables = variables to fill
	    	//data_source = ajax url to call
	    	//path = custom path directory
	    	//directory = subdir for template
	    	//target = target to send data
	    	//mode = mode to use (html,after,before,append,return)
	    	//complete = callback function when complete
	    	
	    	if (obj.data_source != undefined && typeof(obj.data_source) == "object"){
	    		this.processDataSource(obj);
	    		return;
	    	}
	    	
	    	if (obj.template != undefined && obj.template != null && obj.target != undefined && obj.target != null){
	    		if (obj.method != undefined && obj.method != null){
	    			var method = obj.method;
	    		}
	    		else{
	    			var method = "GET";
	    		}
	    		
	    		var path = "/templates/";
	    		
	    		if (obj.path != undefined && obj.path != null){
	    			path += obj.path+"/";    			
	    		}
	    		else{
	    			path += "partials/";
	    		}
	    		
	    		if (obj.directory != undefined && obj.directory != null){
	    			obj.directory = obj.directory.replace("/","");
	    			path += obj.directory + "/";
	    		}
	    		var ts = Math.round((new Date()).getTime() / 1000);
	    		path += obj.template.replaceAll("/","") + ".htm?ts="+ts;	
	    		
	    		$.ajax({
	    			type: method,
	    			url: path,
	    			success: function(html){    				
	    				if (obj.variables != undefined && obj.variables != null){
	    					for(var k in obj.variables){    						
	    						html = html.replaceAll("{{"+k+"}}",obj.variables[k]);    						
	    					}    					
	    				}
	    				if (obj.mode == undefined || obj.mode == null)
	    					obj.mode = "html";
	    				
	    				switch (obj.mode){
	    					case "html":
	    						obj.target.html(html);
	    						break;
	    					case "append":
	    						obj.target.append(html);
	    						break;
	    					case "after":
	    						obj.target.after(html);
	    						break;
	    					case "before":
	    						obj.target.before(html);
	    						break;
	    					case "return":
	    						return html;
	    						break;
	    				}
	    				
	    				//callback
	    				if (obj.complete != undefined && obj.complete != null && typeof obj.complete == "function"){
	    					var cb = obj.complete;
	    					cb(obj.variables);
	    				}
	    			},
	    			error: function (jqXHR, textStatus, errorThrown){
	    				alert("An error has occurred.  Unable to load page at this time.");
	    				console.log(textStatus);
	    				console.log(errorThrown);
	    			}
	    		})
	    	}
	    },
	    iterate: function(obj){
	    	//template = name of template file
	    	//variables = variables to fill
	    	//path = custom path directory
	    	//directory = subdir for template
	    	//target = target to send data
	    	//mode = mode to use (html,after,before,append)
	    	//callback = callback function	
	    	
	    	if (obj.template != undefined && obj.template != null && obj.target != undefined && obj.target != null){
	    		if (obj.method != undefined && obj.method != null){
	    			var method = obj.method;
	    		}
	    		else{
	    			var method = "GET";
	    		}
	    		
	    		var path = "/templates/";
	    		
	    		if (obj.path != undefined && obj.path != null){
	    			path += obj.path+"/";    			
	    		}
	    		else{
	    			path += "partials/";
	    		}
	    		
	    		if (obj.directory != undefined && obj.directory != null){
	    			obj.directory = obj.directory.replace("/","");
	    			path += obj.directory + "/";
	    		}
	    		var ts = Math.round((new Date()).getTime() / 1000);
	    		path += obj.template.replaceAll("/","") + ".htm?ts="+ts;	
	    		
	    		$.ajax({
	    			type: method,
	    			url: path,
	    			success: function(tmp_html){
	    				html = "";
	    				
	    				if (obj.variables != undefined && obj.variables != null && obj.variables.length > 0){    					
	    					for (i=0;i<obj.variables.length;i++){    						
	    						this_html = tmp_html;
		    					for (var k in obj.variables[i]){    
		    						this_html = this_html.replaceAll("{{"+k+"}}",obj.variables[i][k]);    						
		    					}
		    					html += this_html;
	    					}    					
	    				}
	    				else{
	    					return false;
	    				}
	    				
	    				if (obj.mode == undefined || obj.mode == null)
	    					obj.mode = "html";
	    				
	    				switch (obj.mode){
	    					case "html":
	    						obj.target.html(html);
	    						break;
	    					case "append":
	    						obj.target.append(html);
	    						break;
	    					case "after":
	    						obj.target.after(html);
	    						break;
	    					case "before":
	    						obj.target.before(html);
	    						break;
	    				}
	    				
	    				//callback
	    				if (obj.callback != undefined && obj.callback != null && typeof obj.callback == "function"){
	    					var cb = obj.callback;
	    					cb(obj.variables);
	    				}
	    			},
	    			error: function (jqXHR, textStatus, errorThrown){
	    				alert("An error has occurred.  Unable to load page at this time.");
	    				console.log(textStatus);
	    				console.log(errorThrown);
	    			}
	    		})
	    	}
	    }
	},
    processDataSource: function(obj){
    	if (obj.data_source.script != undefined){
    		var url = "/d/"+obj.data_source.script;
    		var type = "GET";
    		var data = null;
    		var up = this;
    		
    		if (obj.variables != undefined && obj.variables != null){
    			var vars = obj.variables;
    		}
    		else{
    			var vars = {};    			
    		}
    		
    		if (obj.data_source.type != undefined){
    			type = obj.data_source.type;
    		}
    		if (obj.data_source.data != undefined){
    			data = obj.data_source.data;
    		}
    		
    		$.ajax({
    			url: url,
    			type: type,
    			data: data,
    			success: function(o){
    				var out = $.parseJSON(o);
    				
    				obj.variables = build.merge_options(vars,out);
    				obj.data_source = null;
    				up.loadTPL(obj);
    			}
    		});    		
    	}
    },
    data: {
    	get: function(obj){
        	if (obj.data != undefined && obj.source != undefined){
        		var url = "/d/"+obj.source;
        		var output = "json";
        		if (obj.output != undefined){
        			output = obj.output;
        		}
        		
        		$.ajax({
        			type: "GET",
        			url: url,
        			data: obj.data,
        			success: function(out){
        				if (output == "json"){
        					out = $.parseJSON(out);
        				}
        				if (obj.success != undefined && typeof(obj.success) == "function"){
        					obj.success(out);
        				}
        			}
        		});
        	}
        },
        post: function(obj){
        	if (obj.data != undefined && obj.source != undefined){
        		var url = "/d/"+obj.source;
        		var output = "json";
        		if (obj.output != undefined){
        			output = obj.output;
        		}
        		
        		$.ajax({
        			type: "POST",
        			url: url,
        			data: obj.data,
        			success: function(out){
        				if (output == "json"){
        					out = $.parseJSON(out);
        				}
        				if (obj.success != undefined && typeof(obj.success) == "function"){
        					obj.success(out);
        				}
        			}
        		});
        	}
        }
    },
    errorMessage: function(obj){
    	if (obj.message != undefined){    		
    		if (obj.target != undefined && obj.target.length != 0){
    			
    			if (obj.status != undefined){ 
    				var status_class = "alert-"+obj.status;
    				var alert_header = ""; 
    				if (obj.status == "success"){
    					obj.target.children("#message-header").html("Success!");
    				}
    				else if (obj.status == "error"){
    					obj.target.children("#message-header").html("Error");
    				}
    				else if (obj.status == "warning"){
    					obj.target.children("#message-header").html("Warning");
    				}    				
    			}
    			else{
    				var status_class = "alert-info";
    			}    			
    			obj.target.addClass("alert");
    			obj.target.addClass(status_class);    			
    			if (obj.target.children("#message-text").length > 0){
    				obj.target.children("#message-text").html(obj.message);
    			}
    			else{
    				obj.target.children(".message-text").html(obj.message);
    			}
    			obj.target.show();	
    		}
    		else{
    			alert(obj.message);
    		}
    	}
    },
    getPath: function(){
    	var path = window.location.pathname.replace("/!","").split("/");
    	if (path[0] == ""){
    		path.splice(0,1);
    	}
    	if (path[path.length-1] == ""){
    		path.splice(path.length-1,1);
    	}
    	return path;
    },
    getQueryString: function(){
    	var url = $(window.location).attr("href").replace($(window.location).attr("origin"),"").split("?");
    	if (url.length > 1){
    		return this.queryStringParams(url[1]);
    	}
    	return null;
    },
    anchorPageProperties: function(href){
    	var $return = new Object();
    	if (href != undefined){
    		href = href.replace("/#!/","");
    	}
    	else{
    		var href = $(window.location).attr("hash");
    		href = href.replace("#!/","");
    	}
    	var q_split = href.split("?");
    	var href_arr = q_split[0].split("/");
    	var args = new Array();
    	
    	if (q_split.length > 1 && q_split[1] != undefined){
    		args = this.queryStringParams(q_split[1]);
    	}
    	
    	if (href_arr.length > 1){
    		var details = new Array();
    		for (i=1;i<href_arr.length;i++){
    			details[i-1] = href_arr[i];
    		}
    	}
    	else{
    		var details = null;
    	}
    	
    	$return.page = href_arr[0].replace("#!","");
    	$return.details = details;
    	$return.args = args;
    	
    	return $return;
    },
    queryStringParams: function(qs){
    	var out = new Array();
    	
    	if (qs != undefined){
    		var arr = qs.split("&");
    		for(var i = 0; i < arr.length; i++) {
    		    var bits = arr[i].split('=');
    		    out[bits[0]] = bits[1];
    		}
    	}
    	
    	return out;
    },
    merge_options: function(obj1,obj2){
        var obj3 = {};
        for (var attrname in obj1) { obj3[attrname] = obj1[attrname]; }
        for (var attrname in obj2) { obj3[attrname] = obj2[attrname]; }
        return obj3;
    },
    tabs: function(obj,canvas){
    	canvas.html("");
		if (obj != undefined && obj.length > 0 && canvas != undefined && canvas.length > 0){
			obj.children('li').removeClass("active");			
			var anchor = build.anchorPageProperties();
			if (anchor.page == ""){
				anchor.page = obj.children('li:nth-child(1)').attr("data-page");
			}			
			if (obj.attr("data-method") != undefined && obj.attr("data-method") != "" && obj.attr("data-source") != undefined && obj.attr("data-source") != ""){
				var methodFunc = build.data.get;
				if (obj.attr("data-method") == "post"){
					var methodFunc = build.data.post;
				}
				var data = {view:anchor.page};
				if (anchor.details != undefined && anchor.details.length > 0){
					data.args = anchor.details
				}
				methodFunc({
					source: obj.attr("data-source"),
					data: data,
					success: function(o){
						if (o != undefined && o.html != undefined){
							canvas.html(o.html);
						}
					}
				});
				obj.children('li[data-page="'+anchor.page+'"]').addClass("active");
			}
			else{
				obj.children('li[data-page="'+anchor.page+'"]').addClass("active");
			}			
			
			obj.children("li").children("a").off("mousedown");
			obj.children("li").children("a").on("mousedown",function(){				
				var page = $(this).parent("li").attr("data-page");
				if (page != undefined && page != ""){
					location = "/#!/"+page;
				}
			});
			
			$(window).off("hashchange");
			$(window).on("hashchange", function(){
				build.tabs(obj,canvas);
			});		
		}
	},
    toggleTabs: function(target,alias){    	    	
    	if (target.length > 0 && alias != undefined) {    		
    		$("#"+target.attr("id")+" li").each(function(){    			
    			if ($(this).attr("data-alias") == alias){
    				$(this).addClass("active");
    			}
    			else{
    				$(this).removeClass("active");
    			}
    		});
    	}
    },
    saveForm: function(obj){    	
    	var formname = obj.attr("data-form");
		var mode = obj.attr("data-mode");		
		
		if (formname != undefined && formname != ""){
			var form = $("#"+formname);
			if (form.length > 0){
				if (form.attr("data-message") != undefined && form.attr("data-message") != ""){
					errorMessageObject = $("#"+form.attr("data-message"));
				}
				var data = form.serialize();
				if (mode != undefined && mode != ""){
					data += "&mode="+mode;
				}
				var source = form.attr("data-source");
				var method = form.attr("data-method");
				if (method == undefined || method == ""){
					method = "post";
				}
				if (source != undefined && source != ""){
					if (method == "get"){
						build.data.get({
							source: source,
							data: data,
							success: function(o){
								if (o != undefined && o.status != undefined){
									if (o.status == "success"){
										if (o.message == undefined){
											o.message = "Form saved successfully";
										}
										
										if (o.location != undefined){
											alert(o.message);
											if (o.location == "refresh"){
												window.location.reload();
											}
											else{
												location = o.location;
											}
										}										
										else if (errorMessageObject != undefined){
											build.errorMessage({
												target: errorMessageObject,
												message: o.message,
												status: "success"
											});
										}		
										else{
											alert(o.message);
										}
									}
									else if (o.status == "error"){
										if (o.message == undefined){
											o.message = "Form unable to be saved at this time.";
										}
										
										if (o.location != undefined){
											alert(o.message);
											if (o.location == "refresh"){
												window.location.reload();
											}
											else{
												location = o.location;
											}
										}										
										else if (errorMessageObject != undefined){
											build.errorMessage({
												target: errorMessageObject,
												message: o.message,
												status: "error"
											});
										}		
										else{
											alert(o.message);
										}
									}
								}
							}
						});
					}
					else{
						build.data.post({
							source: source,
							data: data,
							success: function(o){								
								if (o != undefined && o.status != undefined){
									if (o.status == "success"){
										if (o.message == undefined){
											o.message = "Form saved successfully";
										}
																				
										if (o.location != undefined){
											alert(o.message);
											if (o.location == "refresh"){
												window.location.reload();
											}
											else{
												location = o.location;
											}
										}										
										else if (errorMessageObject != undefined){
											build.errorMessage({
												target: errorMessageObject,
												message: o.message,
												status: "success"
											});
										}		
										else{
											alert(o.message);
										}
									}
									else if (o.status == "error"){
										if (o.message == undefined){
											o.message = "Form unable to be saved at this time.";
										}
										
										if (o.location != undefined){
											alert(o.message);
											if (o.location == "refresh"){
												window.location.reload();
											}
											else{
												location = o.location;
											}
										}										
										else if (errorMessageObject != undefined){
											build.errorMessage({
												target: errorMessageObject,
												message: o.message,
												status: "error"
											});
										}		
										else{
											alert(o.message);
										}
									}
								}
							}
						});
					}
				}
			}			
		}
    },
	lockButton: function(obj){
		if (obj.length > 0){				
			obj.addClass("btn-disabled");
			obj.attr("id",obj.attr("id")+"-disabled");
		}
	},
	unlockButton: function(obj){
		if ($(obj.selector+"-disabled").length > 0){
			$(obj.selector+"-disabled").attr("id",obj.selector.replace("#",""));
			$(obj.selector).removeClass("btn-disabled");
		}
	},
	storage: {
		get: function(name){
			if (Modernizr.localstorage && name != undefined){
				return localStorage.getItem(name);
			}
			else{
				return false;
			}
		},
		set: function(name,value){
			if (Modernizr.localstorage && name != undefined && value != undefined){
				localStorage.setItem(name,value);
				return true;
			}
			else{
				return false;
			}
		},
		delete: function(name){
			if (Modernizr.localstorage && name != undefined){
				return localStorage.removeItem(name);
			}
			else{
				return false;
			}
		}
	}
}