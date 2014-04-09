(function ($) {
  // Some of the following variables can be changed depending on the specs you use
var speed = 400; // speed of the sliding animation
var dir = "/sites/all/themes/new_ubc_clf/js/megamenus"; // this is the name of the directory that holds your ajax files; if files are in root, just make this blank

$(document).ready(function(){

	var navLinks = $("li.UbcUtilNav>a");
	var navLinksActiveSpan = $("li.UbcUtilNav>a.active span");
	var placeOfMindLink = $("a#UbcMindLink");
	var closeLink = $("a#close-link");
	var dropDown = $("#UbcMegaMenuesWrapper");
	var closeLinkString = "<div class='UbcCloseBtn'><a href='#' id='close-link'>close [x]</a></div>";
	var addCloseLink = function() {
		dropDown.append(closeLinkString);
	};


	var loadRss = function() {
		var month = ['January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December'];
		addCloseLink();
  		var feed = new google.feeds.Feed("http://www.aplaceofmind.ubc.ca/feed/?cat=-1");	
	      	feed.setNumEntries(3);
        	feed.load(function(result)
        	{
        		if (result.error)
        		{
        			$('.UbcMegaMenuesPOM .UbcRSS').html('<p>' + result.error.message + '</p>');
        			return;
        		}

        		for (var i = 0; i < result.feed.entries.length; i++) 
        		{
        			var entry = result.feed.entries[i];
        			var h3 = '<h3><a href="' + entry.link + '" target="_blank">' + entry.title + '</a></h3>';
        			var pubDate = new Date(entry.publishedDate);
        			var date = '<p class="date">Submitted: ' + month[pubDate.getMonth()].toString() + ' ' + pubDate.getDate().toString() + ', ' + pubDate.getFullYear().toString() + '</p>';
        			var desc = '<p>' + entry.content + '</p>';
        			var more = '<a href="' + entry.link + '" target="_blank">More \u00BB</a>';
        			var li = $('<li class="RSS">' + h3 + date + desc + more + '</li>');
        			
        			$('.UbcRSS ul').append(li);
        		}
        		
        		var colHeight = 0;
        		
        		$('.UbcRSS ul').children().each(function(index)
        		{
        			var height = $(this).height();
        			
        			if (height > colHeight)
        				colHeight = height;
        		});
        		
        		//Apply height so all the borders are even
        		$('.UbcRSS ul').children().each(function(index)
        		{
        			var ele = $(this);
        			
        			if (ele.hasClass('UbcPMCallout')) return;
        			
        			ele.css('height', colHeight.toString() + 'px');
        		});
        	});
	};

	// this is the click handler that decides the primary open/close actions
	navLinks.click(function(){
		placeOfMindLink.removeClass("selected");
		var ajaxPage = this.href.split("/");
		ajaxPage = ajaxPage[ajaxPage.length-2];
		var ajaxPageName = ajaxPage.split(".")[0];
		
		// drop-down is already opened
		if (dropDown.hasClass("open")) {
			
			// it's open, and the clicked menu item is the same as the open drop-down
			// so it closes
			if (dropDown.hasClass(ajaxPageName)){
				dropDown.removeClass();
				$(this).removeClass();
				dropDown.slideUp();

			// if it's open, and the clicked item is different, keep it open
			// then load the new content
			} else {
				navLinks.removeClass("active UbcSelected");
				$(this).addClass("active UbcSelected");
				dropDown.removeClass();
				dropDown.addClass("open");
				dropDown.addClass(ajaxPageName);
				dropDown.load(dir+"/"+ajaxPage+".html", function() {
					// callback after ajax call is complete
					addCloseLink();										   
				});
			}

		// drop-down is not already opened; add the right classes, open it, and load the content
		} else {
			dropDown.addClass("open");
			dropDown.addClass(ajaxPageName);
			$(this).addClass("active UbcSelected");
			dropDown.load(dir+"/"+ajaxPage+".html", function() {
					addCloseLink();
				});
			dropDown.delay(50).slideDown();
		}
		
		document.location.hash = ajaxPageName; // change the hash for deep linking
		return false;
	});
	
	placeOfMindLink.click(function(event){
		event.preventDefault();
		navLinks.removeClass("UbcSelected");
		if (dropDown.hasClass("open")) {
			if (dropDown.hasClass("placeOfMind")){
				$(this).removeClass("active selected");
				dropDown.slideUp(speed, function() {
					dropDown.removeClass();
				});
			} else {
				navLinks.removeClass("active");
				$(this).addClass("active selected");
				dropDown.removeClass();
				dropDown.addClass("open");
				dropDown.addClass("placeOfMind");
				$('#UbcMegaMenuesWrapper').addClass('UbcMindLink');	
				dropDown.load(dir+"/aplaceofmind.html", function() {
					addCloseLink();		
					loadRss();							
				});
			}
		} else {
			dropDown.addClass("open");
			dropDown.addClass("placeOfMind");
			$(this).addClass("active selected");		
			$('#UbcMegaMenuesWrapper').addClass('UbcMindLink');
			dropDown.load(dir+"/aplaceofmind.html", function() {
				loadRss();	
			});
			dropDown.delay(50).slideDown();
		}	
	
	});
	
}); //end document ready 


$('#close-link').live('click',function(){

	$('#UbcMegaMenuesWrapper').slideUp(400, function() {
		$('#UbcMegaMenuesWrapper').removeClass();
		$("li.UbcUtilNav>a").removeClass();
		$("a#UbcMindLink").removeClass();
	});
});

})(jQuery);
