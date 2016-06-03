$(function(){
	$("#email-btn").off("click");
	$("#email-btn").on("click",function(){
		var email = $("#email").val();
		if (email != ""){
			build.data.post({
				source: "signup",
				data: {email:email},
				success: function(){
					$("#email-container").hide();
					$("#action-bar h2").html("You're On Your Way");
					$("#action-bar h3").html("We'll notify you when your invitation is ready.");
				}
			});
		}
	});
});