<?php

/**
 * @file
 * Views theme to render view fields as JSON.
 *
 * - $view: The view in use.
 * - $rows: Array of row objects as rendered by _search_autocomplete_render_fields
 * - $attachment: Not used currently
 * - $options: The options for the style passed in from the UI.
 *
 * @ingroup views_templates
 * @see search_autocomplete.views.inc
 */
function template_preprocess_views_search_autocomplete_style(&$vars) {
  global $base_root;

  $view = $vars["view"];
  $rows = $vars["rows"];

  $arg = '';
  if (array_key_exists(0, $view->args))
    $arg = $view->args[0] ? $view->args[0] : '';

  $base = $view->base_table;
  $objects = array();

  $vars['bitmask'] = NULL;

  $should_translite = search_autocomplete_get_translite();

  foreach ($rows as $row) {

    $object = array();

    /* Convert the $rows into a hierachial key=>value array */
    foreach ($row as  $field) {
      $matches = array();

      $regexp = "<a\s[^>]*href=(\"??)([^\" >]*?)\\1[^>]*>(.*)<\/a>";
      if (preg_match("/$regexp/siU", htmlspecialchars_decode($field->content, ENT_QUOTES), $matches)) {

        $link   = $base_root . $matches[2];
        $value  = $matches[3];

        //hack to romanize accents, this needs module transliteration enabled
        if (function_exists('transliteration_get') && $should_translite) {
          $romanised = transliteration_get($value, '?', language_default('language'));
        }
        else  $romanised = $value;

        //compare both $value and $romanised
        if ($arg == '' || mb_stripos($value, $arg, 0, 'UTF-8')  !== FALSE || mb_stripos( $romanised, $arg, 0, 'UTF-8')  !== FALSE ) {

          $label = "";
          if ($field->label)
            $label = strip_tags($field->label);

          $object['link']  = $link;
          $object['label'] = $label . $value;
          $object['value'] = $romanised;

          $objects[] = $object;

        }
      }
    }
  }

  // check if user wants nested arrays
  $vars["rows"] = $objects;
}
