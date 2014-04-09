<!-- BEGIN UBC CLF GLOBAL UTILITY HEADER -->
<div id="UbcToolBarWrapper">
	<div id="UbcToolBar">
		<div id="UbcToolNavWrapper">
		  <ul id="UbcToolNav">
			<li class="UbcUtilNav"><a id="UbcCampusLinks" href="http://www.ubc.ca/campuses/">Campuses</a></li>
			<li class="UbcUtilNav"><a id="UbcDirectLinks" href="http://www.ubc.ca/directories/">UBC Directories</a></li>
			<li class="UbcUtilNav"><a id="UbcQuickLinks" href="http://www.ubc.ca/quicklinks/">UBC Quick Links</a></li>
		  </ul>
		  <?php if ( $page['search']): ?>
			<div id="UbcSearchForm">
				<?php print render($page['search']); ?>
			</div>
		  <?php endif; ?>
      	</div>
	</div>
</div><!-- End UbcToolBarWrapper -->
<div class="wrapper front">
<div id="UbcMegaMenuesWrapper">&nbsp;</div>

<!-- END UBC CLF GLOBAL UTILITY HEADER -->

<!-- BEGIN UBC CLF VISUAL IDENTITY HEADER -->
<div id="UbcHeaderWrapper">
    <ul id="UbcHeader">
        <li><a id="UbcLogo" href="http://www.ubc.ca">The University of British Columbia</a></li>
        <li><a id="UbcMindLink" href="http://www.aplaceofmind.ubc.ca/">a place of mind</a></li>
        <li><a id="UbcHeaderLine" href="http://www.science.ubc.ca/">Faculty of Science</a></li>
        <li><a id="UbcSubUnitLine" href="/">Department of Microbiology and Immunology</a></li>
    </ul>
</div>
<!-- END UBC CLF VISUAL IDENTITY HEADER -->

<!-- BEGIN UBC CLF PRIMARY NAVIGATION -->
<div id="UbcNavWrapper" class="UbcFrontNavWrapper">
    <?php if ( $page['menu']): ?>
		<?php print render($page['menu']); ?>
    <?php endif; ?>
</div>
<!-- END UBC CLF PRIMARY NAVIGATION -->

<!-- BEGIN UBC CLF CONTENT SPACE -->
<div id="UbcFrontPageWrapper">
	<div id="UbcFrontContentWrapper" class="UbcFrontContent">
			<?php if ( $page['slideshow']): ?>
					<?php print render($page['slideshow']); ?>
			<?php endif; ?>
			<?php if ( $page['calendar']): ?>
				<div id="UbcFrontCalendar">
					<?php print render($page['calendar']); ?>
				</div>
			<?php endif; ?>
			<?php if ( $page['welcome']): ?>
				<div id="UbcWelcomeMessage" class="clear">
					<?php print render($page['welcome']); ?>
				</div>
			<?php endif; ?>	
	</div><!-- End UbcContentWrapper -->
	
	<div id="UbcFrontPageLeft" class="UbcFrontContent">
		<?php if ( $page['left']): ?>
			 <?php print render($page['left']); ?>
		<?php endif; ?>
	</div>
	
	<div id="UbcFrontPageRight" class="UbcFrontContent">
		<?php if ( $page['right']): ?>
			 <?php print render($page['right']); ?>
		<?php endif; ?>
	</div>
	
</div>
<!-- END UBC CLF CONTENT SPACE -->
<div class="push clear"></div>
</div>


<div class="footer clear">

<div id="UbcLogin" class="clear">
	<?php if ( $page['footer']): ?>
		<?php print render($page['footer']); ?>
	<?php endif; ?>
</div>

<!-- BEGIN UBC CLF VISUAL IDENTITY FOOTER -->
<div id="UbcBottomInfoWrapper">
    <div id="UbcBottomInfo">
        <div id="UbcBottomInfoLeft"><a href="http://www.ubc.ca"><img src="<?php print base_path() . path_to_theme(); ?>/images/footer/logo.gif" alt="a place of mind, The University of British Columbia" /></a></div>
        <div id="UbcBottomInfoRight"><p>The Department of Microbiology & Immunology<br />
    	    The University of British Columbia<br />
    	    2350 Health Sciences Mall Vancouver, BC Canada V6T 1Z3 <br/></p>
        </div>

    </div>
</div><!-- End UbcBottomInfo -->
<!-- END UBC CLF VISUAL IDENTITY FOOTER -->

<!-- BEGIN UBC CLF GLOBAL UTILITY FOOTER -->
<div id="UbcUtilNavWrapper">
    <p><a href="http://www.emergency.ubc.ca">Emergency Procedures</a>&nbsp;|&nbsp;<a href="http://www.ubc.ca/accessibility/">Accessibility</a>&nbsp;|&nbsp;<a href="http://www.ubc.ca/about/contact/">Contact UBC</a>&nbsp;|&nbsp;<a href="http://www.ubc.ca/copyright/">&copy; Copyright  The University of British Columbia</a></p>
</div><!-- End UbcUtilNavWrapper -->
<!-- END UBC CLF GLOBAL UTILITY FOOTER -->
</div>
