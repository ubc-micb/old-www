<!-- BEGIN UBC CLF GLOBAL UTILITY HEADER -->
<div id="UbcToolBarWrapper">
	<div id="UbcToolBar">
      <ul id="UbcToolNav">
        <li class="UbcUtilNav"><a id="UbcCampusLinks" href="http://www.ubc.ca/campuses/">Campuses</a></li>
        <li class="UbcUtilNav"><a id="UbcDirectLinks" href="http://www.ubc.ca/directories/">UBC Directories</a></li>
        <li class="UbcUtilNav"><a id="UbcQuickLinks" href="http://www.ubc.ca/quicklinks/">UBC Quick Links</a></li>
        <li class="UbcForm">
          <form id="UbcSearchForm" action="http://www.ubc.ca/search/" method="get">
            <input type="text" name="q" value="" class="UbcSearchField" />
            <input type="image" src="<? print base_path() . path_to_theme(); ?>/images/header/spacer.gif" class="UbcSearchBtn" value="Search" alt="Search" />
          </form>
        </li>
      </ul>
	</div>

</div><!-- End UbcToolBarWrapper -->

<div id="UbcMegaMenuesWrapper" class="loading">&nbsp;</div>

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
<div id="UbcNavWrapper">
    <?php if ( $page['menu']): ?>
	<?php print render($page['menu']); ?>
    <?php endif; ?>
</div>
<!-- END UBC CLF PRIMARY NAVIGATION -->

<!-- BEGIN UBC CLF CONTENT SPACE -->
<div id="UbcContentWrapper">
	<?php print render($page['content']); ?>
		<?php if ( $page['welcome']): ?>
			<div id="UbcWelcomeMessage">
				<?php print render($page['welcome']); ?>
				<div class="arrow-border"></div>
				<div class="arrow"></div>
			</div>
		<?php endif; ?>	
		<?php if ( $page['slideshow']): ?>
			<div id="UbcFrontSlideshow">
				<?php print render($page['slideshow']); ?>
			</div>
		<?php endif; ?>	
		<?php if ( $page['sidebar_right']): ?>
		   	<div id="UbcRightSidebarWrapper">
				<div id="RightSidebar">
	   	 			<?php print render($page['sidebar_right']); ?>
		 		</div>
    		</div>
    	<?php endif; ?>
		<div id="UbcFrontPageLeft">
			<?php if ( $page['left']): ?>
				 <?php print render($page['left']); ?>
			<?php endif; ?>
		</div>
		<div id="UbcFrontPageRight">
			<?php if ( $page['right']): ?>
		 		 <?php print render($page['right']); ?>
			<?php endif; ?>
		</div>

</div><!-- End UbcContentWrapper -->
<!-- END UBC CLF CONTENT SPACE -->