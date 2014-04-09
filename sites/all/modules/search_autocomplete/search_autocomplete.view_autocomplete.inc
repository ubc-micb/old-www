<?php

/**
 * @file
 * Search Autocomplete
 * Enables autocomplete functionality on search fields.
 *
 * @authors
 * Miroslav Talenberg (Dominique CLAUSE) <http://www.axiomcafe.fr/contact>
 *
 * Sponsored by:
 * www.axiomcafe.fr
 */
 
/**
 * Implements hook_views_api().
 */
function search_autocomplete_views_api() {
  return array(
    'api' => '3.0',
    'path' => drupal_get_path('module', 'search_autocomplete') . '/views',
  );
}

/**
 * Implements hook_views_pre_render().
 */
function search_autocomplete_views_pre_render(&$view) {
  if (isset($view->plugin_name) && $view->plugin_name == 'search_autocomplete') {
    // Support for Video field.
    if (!empty($view->result)) {
      // Process each View result.
      foreach ($view->result as $row => $result) {
        // Only process the entity fields defined by the View.
        foreach ($view->field as $field_name => $field) {
          if ($field instanceof views_handler_field_field) {
            if ($field->field_info['type'] == 'video') {
              
              // Get the Video URL.
              $video  = $field->get_value($view->result[$row]);
              $url    = file_create_url($video[0]['uri']);
              $render_array = array(
                '#type'   => 'markup',
                '#markup' => filter_xss($url),
              );
              // Substitute embed code with URL. @todo Add support for escaped embed codes.
              $view->result[$row]->{'field_' . $field_name}[0]['rendered'] = $render_array;
            }
          }
        }
      }
    }
  }
}

/**
 * We almost duplicate the content_handler_field_multiple::render function
 * to get the multiple rendered field values in an array
 * @param $field
 * @param $values
 * @return unknown_type
 */
function _search_autocomplete_render_multiple_field($field, $values) {
  $options = $field->options;
  // If this is not a grouped field, use content_handler_field::render().
  if (!$field->defer_query) {
    return $field->render($values);
  }
  // We're down to a single node here, so we can retrieve the actual field
  // definition for the node type being considered.
  $content_field = content_fields($field->content_field['field_name'], $values->{$field->aliases['type']});
  $vid = $values->{$field->field_alias};
  if (isset($field->field_values[$vid])) {
    // Gather items, respecting the 'Display n values starting from m' settings.
    $count_skipped = 0;
    $items = array();
    foreach ($field->field_values[$vid] as $item) {
      if (empty($options['multiple']['multiple_from']) || ($count_skipped >= $options['multiple']['multiple_from'])) {
        if (empty($options['multiple']['multiple_number']) || (count($items) < $options['multiple']['multiple_number'])) {
          // Grab the nid - needed for render_link().
          $nid = $item['_nid'];
          unset($item['_nid']);
          $items[] = $item;
        }
        else {
          break;
        }
      }
      $count_skipped++;
    }

    // Build a pseudo-node from the retrieved values.
    $node = drupal_clone($values);
    // content_format and formatters will need a 'type'.
    $node->type = $values->{$field->aliases['type']};
    $node->nid = $values->{$field->aliases['nid']};
    $node->vid = $values->{$field->aliases['vid']};

    // Some formatters need to behave differently depending on the build_mode
    // (for instance: preview), so we provide one.
    $node->build_mode = NODE_BUILD_NORMAL;

    // Render items.
    $formatter_name = $options['format'];
    if ($items && ($formatter = _content_get_formatter($formatter_name, $content_field['type']))) {
      $rendered = array();
      if (content_handle('formatter', 'multiple values', $formatter) == CONTENT_HANDLE_CORE) {
        // Single-value formatter.
        $n = 0;
        foreach ($items as $item) {
          $output = content_format($content_field, $item, $formatter_name, $node);
          if (!empty($output)) {
            $rendered[++$n] = $field->render_link($output, (object) array('nid' => $nid));
          }
        }
      }
      else {
        // Multiple values formatter.
        $output = content_format($content_field, $items, $formatter_name, $values);
        if (!empty($output)) {
          $rendered[++$n] = $field->render_link($output, (object) array('nid' => $nid));
        }
      }
      if (count($rendered) > 1) {
        // TODO: could we use generic field display ?
        //return theme('content_view_multiple_field', $rendered, $content_field, $values);
        return $rendered;
      }
      elseif ($rendered) {
        return $rendered[1];
      }
    }
  }

  return '';
}

/**
 * Takes each field from a row object and renders the field as determined by the field's theme
 *
 * @param $view
 *   View the row belongs to
 * @param $row
 *   Row object
 * @return array
 *   Object containing all the raw and rendered fields
 */
function _search_autocomplete_render_fields($view, $row) {
  $field_ids = array_keys($view->field);
  $rendered_fields = array();
  foreach ($field_ids as $id) {
    $field = $view->field[$id];
    $field_is_multiple = FALSE;
    $field_raw = array();
    if ((isset($field->options['multiple']['group']))&& isset($field->field_values)) {
      $field_output = _search_autocomplete_render_multiple_field($field, $row);
      $n = 0;
      if (is_array($field_output)) {
        foreach ($field->field_values[$row->{$field->field_alias}] as $item) {
          $field_raw[++$n] = $item["value"];
        }
        $field_is_multiple = TRUE;
      }
      else $field_raw = $view->field[$field->options['id']]->advanced_render($row);
    }
    else {
      $field_output = $view->field[$field->options['id']]->advanced_render($row);
      $field_raw = (isset($view->field[$id]->field_alias) && isset($row->{$view->field[$id]->field_alias})) ? $row->{$view->field[$id]->field_alias} : NULL;
    }

    $img_match = array();
    $src_match = array();
    if (is_array($field_output)) {
      foreach ($field_output as $i => $f) {
        if (preg_match("/<img[^>]+>/i", $f, $img_match)) {
          if (preg_match('/(src)="([^"]*)"/i', $img_match[0], $src_match))
          $field_output[$i] = ($src_match[2]);
        }
      }
    }
    else {
      if (preg_match("/<img[^>]+>/i", $field_output, $img_match)) {
        if (preg_match('/(src)="([^"]*)"/i', $img_match[0], $src_match))
          $field_output = ($src_match[2]);
      }
    }

    if (empty($field->options['exclude'])) {
      if (empty($field->options['exclude']) && !($field->options['hide_empty'] && (empty($field_output)))) {
        $object = new stdClass();
        $object->id = $id;
        // Respect the 'empty' value if empty and "No results text" is given.
        if (empty($field_output) && $field->options['empty']) {
          $object->content = $field->options['empty'];
        } else {
          $object->content = $field_output;
        }
        $object->raw = $field_raw;
        $object->class = drupal_clean_css_identifier(strtolower($id));//views_css_safe($id);
        
        $object->label = check_plain($view->field[$id]->label());
                
        if ($object->label) {
          if ($view->field[$id]->options['element_label_colon']) {
            $object->label .= ': ';
          } else {
            $object->label .= ' ';
          }
        }
        
        $object->is_multiple = $field_is_multiple;
        $rendered_fields[$id] = $object;

      }
    }
  }
  return $rendered_fields;
}

/**
 * Gets JSON data from a View rendered in the JSON data document style.
 *
 * This is useful for when working with a JSON view in code.
 *
 * @param $name
 *   The name of the view.
 * @param $display_id
 *   The display of the view to use.
 * @param $args
 *   The arguments to pass to the view.
 * @param $raw
 *   If TRUE, the JSON data is returned as a string.  Otherwise, an object
 *   representation is returned.
 * @return
 *   The JSON data in the form of an object or a string or NULL otherwise.
 */
function search_autocomplete_get($name, $display_id = 'default', $args = array(), $raw = FALSE) {
  $view = views_get_view($name);
  if (!is_object($view)) return NULL;

  $preview    = $view->preview($display_id, $args);
  $start_pos  = strpos($preview, '{');
  $finish_pos = strrpos($preview, '}');
  $length     = $finish_pos - $start_pos + 1;
  $json       = trim(substr($preview, $start_pos, $length));

  if ($raw) {
    return $json;
  }

  return json_decode($json);
}

/**
 * Render a view's output as JSON.
 *
 * The function will directly output a JSON string instead of returning it.
 *
 * @param $items
 *   The collection of items to encode into JSON.
 * @param $options
 *   Render options.
 */

/**
 * Encodes JSON in a pretty-printed fashion.
 *
 * @deprecated The $option parameter in PHP 5.4.0 json_encode() deprecates this function.
 *
 * @see _search_autocomplete_json_encode
 */
function _search_autocomplete_encode_formatted($v, $depth = 0) {
  $base_indent  = '&nbsp;&nbsp;';
  $eol          = '<br />';
  $indent       = str_repeat($base_indent, $depth);

  // This is based on the drupal_to_js() function.
  switch (gettype($v)) {
    case 'boolean':
      // Lowercase is necessary!
      return $v ? 'true' : 'false';

    case 'integer':
    case 'double':
      return $v;

    case 'resource':
    case 'string':
      $search   = array('"', chr(92), chr(8), chr(12), chr(13) . chr(10), chr(10), chr(13), chr(9));
      $replace  = array('\"', '\\', '\b', '\f', '\n', '\n', '\r', '\t');
      $output   = str_replace($search, $replace, $v);
/* *
      $output = str_replace(array("\r", "\n", "<", ">", "&"),
                           array('\r', '\n', '\x3c', '\x3e', '\x26'),
                           addslashes($output));
/* */
      return '"' . check_plain($output) . '"';

    case 'array':
      // Arrays in JSON can't be associative.  If the array is empty or if it
      // has sequential whole number keys starting with 0, it's not associative
      // so we can go ahead and convert it as an array.
      if (empty($v) || array_keys($v) === range(0, sizeof($v) - 1)) {
        $output = array();
        foreach ($v as $val) {
          $output[] = $indent . $base_indent . _search_autocomplete_encode_formatted($val, $depth + 1);
        }
        return '[' . (!empty($output) ? $eol . implode(',' . $eol, $output) . $eol . $indent : '') . ']';
      }
      // Otherwise, fall through to convert the array as an object.

    case 'object':
      $output = array();
      foreach ($v as $key => $val) {
        $output[] = $indent . $base_indent . _search_autocomplete_encode_formatted(strval($key)) . ' : ' . _search_autocomplete_encode_formatted($val, $depth + 1);
      }
      return '{' . (!empty($output) ? $eol . implode(',' . $eol, $output) . $eol . $indent : '') . '}';

    default:
      return 'null';
  }
}

function _search_autocomplete_debug_stop($var, $location) {
  print("Location:$location\n");
  var_dump($var);
  module_Invoke_all('exit');
  exit;
}

/**
 * Backwards-compatible JSON encoder.
 *
 * Provides backwars-compatible support for more JSON encoding formats.
 * Uses PHP's native JSON encoding when PHP 5.3.0 or greater is detected.
 * Fallbacks to manual encoding/escaping when PHP 5.2.x and below is detected.
 *
 * @param array $rows
 *   Results from template_preprocess_views_search_autocomplete_style_simple().
 * @param int $bitmask
 *   Integer to use as the $bitmask parameter for json_encode().
 */
function _search_autocomplete_json_encode($rows, $bitmask = NULL) {
  
  if (PHP_MAJOR_VERSION >= 5 && PHP_MINOR_VERSION >= 3) {
    $json = json_encode($rows, $bitmask);
    // Encoding features not supported before 5.4.x.
    if (PHP_MINOR_VERSION <= 4) {
       $json = str_replace(array('\/'), array('/'), $json);
    }
  } else {
    $json = json_encode($rows); 
    $json = str_replace(array('\/'), array('/'), $json);
  }
  return $json;
}



/**
 * Implements hook_views_default_views().
 * 
 * Creates a defaults view as a user helper
 */
function search_autocomplete_views_default_views() {
  return search_autocomplete_view_autocomplete();
}
/**
 * Begin view
 */
function search_autocomplete_view_autocomplete() {

  // AUTOCOMPLETE NODE VIEW
  $view_nodes = new view;
  $view_nodes->name = 'autocomplete_nodes';
  $view_nodes->description = '';
  $view_nodes->tag = 'default';
  $view_nodes->base_table = 'node';
  $view_nodes->human_name = 'autocomplete_nodes';
  $view_nodes->core = 7;
  $view_nodes->api_version = '3.0';
  $view_nodes->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view_nodes->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Node title suggestions for autocompletion';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Reset';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'search_autocomplete';
  /* Champ: Contenu: Titre */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['title']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Sort criterion: Contenu: Titre */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  /* Filter criterion: Contenu: Publié */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: Page */
  $handler = $view_nodes->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'autocomplete-nodes';
  $translatables['autocomplete_nodes'] = array(
    t('Master'),
    t('Node title suggestions for autocompletion'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('title'),
    t('Page'),
  );
  $views[$view_nodes->name] = $view_nodes;
  
  // AUTOCOMPLETE USER VIEW
  $view_users = new view;
  $view_users->name = 'autocomplete_users';
  $view_users->description = '';
  $view_users->tag = 'default';
  $view_users->base_table = 'users';
  $view_users->human_name = 'autocomplete_users';
  $view_users->core = 7;
  $view_users->api_version = '3.0';
  $view_users->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view_users->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'autocomplete_users';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access user profiles';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Reset';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'search_autocomplete';
  /* Champ: Utilisateur: Nom */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Sort criterion: Utilisateur: Nom */
  $handler->display->display_options['sorts']['name']['id'] = 'name';
  $handler->display->display_options['sorts']['name']['table'] = 'users';
  $handler->display->display_options['sorts']['name']['field'] = 'name';
  /* Filter criterion: Utilisateur: Actif */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: Page */
  $handler = $view_users->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'autocomplete-users';
  $translatables['autocomplete_users'] = array(
    t('Master'),
    t('autocomplete_users'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Page'),
  );
  $views[$view_users->name] = $view_users;
  
  return $views;
}