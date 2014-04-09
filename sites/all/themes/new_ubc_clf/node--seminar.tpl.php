<p style="text-align:right; margin: 0;"><?php print print_insert_link(); ?></p>
<div class="seminar-event">

<h2>THE UNIVERSITY OF BRITISH COLUMBIA</h2>
<h4>Department of Microbiology and Immunology<br />presents a seminar by:</h4>
<h1><?php print render($content['field_presenter_name']) ?></h1>
<h4><?php print render($content['field_presenter_title']) ?></h4>
<h4>entitled:</h4>
<h2><?php print render($content['field_seminar_title']) ?></h2>
<?php if(isset($content['body'])): ?><div class="body"><?php print render($content['body']) ?></div><?php endif; ?>
<h3><?php print render($content['field_date']) ?><br />
Location:<br />
<?php print render($content['field_location1']) ?>
<?php print render($content['field_location_2']) ?></h3>
<?php if(isset($content['field_sponsored'])): ?><h3>Hosted by <?php print render($content['field_sponsored']) ?></h3><?php endif; ?>
<img alt="" src="/sites/default/files/roles/drupal_office/ubclogo.png" />
</div>
