<div style="float: left;"><em><?php if ($submitted) {
echo date( "F j, Y",$node->created);
} ?></em></div>
<div style="float: right;"><a href="<?php echo '/print/' . ($node->nid) ?>"><img title="Print" alt="Print" style="vertical-align: middle; padding-right: 4px" src="/sites/all/modules/print/icons/print_icon.gif">Print</a></div>
<div style="clear: both"></div>
<?php print render($content) ?>
<p class="back"><a href="/news-events/news">< All News</a></p>
