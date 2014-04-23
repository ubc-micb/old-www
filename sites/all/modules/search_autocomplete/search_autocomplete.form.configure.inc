<?php

/**
 * @file
 * Search Autocomplete
 * Helper functions to retrive suggestions from database
 *
 * @authors
 * Miroslav Talenberg (Dominique CLAUSE) <http://www.axiomcafe.fr/contact>
 *
 * Sponsored by:
 * www.axiomcafe.fr
 */

/**
 * MENU CALLBACK:
 * Define the form to configure the suggestions.
 * @return  A rendered form
 */
function search_autocomplete_form_configure($form, &$form_state) {
  $base = "admin/config/search/search_autocomplete";  // base URL for this module configurations

  // get data from database
  $fid = arg(4);

  $result = db_select('search_autocomplete_forms', 'f')
    ->fields('f')
    ->condition('fid', $fid)
    ->execute()
    ->fetchAllAssoc('fid');

  foreach ($result as $item) {
    $form['fid'] = array(
      '#type' => 'hidden',
      '#value' => $fid,
    );
    // ------------------------------------------------------------------
    // HOW - How to display Search Autocomplete suggestions
    $form['search_autocomplete_how'] = array(
      '#type'           => 'fieldset',
      '#title'          => t('HOW - How to display Search Autocomplete suggestions?'),
      '#collapsible'    => TRUE,
      '#collapsed'      => TRUE
    );
    // Minimum characters to set autocompletion on
    $form['search_autocomplete_how']['min_char'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Minimum keyword size that uncouple autocomplete search'),
      '#description'    => t('Please enter the minimum number of character a user must input before autocompletion starts.'),
      '#default_value'  => $item->min_char,
      '#maxlength'      => 2,
      '#required'       => TRUE
    );
    // Number of suggestions to display
    $form['search_autocomplete_how']['max_sug'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Limit of the autocomplete search result'),
      '#description'    => t('Please enter the maximim number of suggestion to display.'),
      '#default_value'  => $item->max_sug,
      '#maxlength'      => 2,
      '#required'       => TRUE
    );
    // check if form should be autosubmitted
    $form['search_autocomplete_how']['no_results'] = array(
      '#type'           => 'textfield',
      '#title'          => t('Behaviour when no suggestions are found'),
      '#description'    => t('Enter a message to display when no results are found. Leave empty for no message.'),
      '#default_value'  => $item->no_results,
      '#maxlength'      => 50,
      '#required'       => FALSE
    );
    // check if form should be autosubmitted
    $form['search_autocomplete_how']['auto_submit'] = array(
      '#type'           => 'checkbox',
      '#title'          => t('Auto Submit'),
      '#description'    => t('If enabled, the form will be submitted automatically as soon as your user choose a suggestion in the popup list.'),
      '#default_value'  => $item->auto_submit
    );
    // check if form should be autoredirected
    $form['search_autocomplete_how']['auto_redirect'] = array(
      '#type'           => 'checkbox',
      '#title'          => t('Auto Redirect'),
      '#description'    => t('If enabled, the user will be directly routed to the suggestion he choosed instead of performing form validation process. Only works if "link" attribute is existing and if "Auto Submit" is enabled.'),
      '#default_value'  => $item->auto_redirect
    );

    // ------------------------------------------------------------------
    // WHAT - What to display in Search Autocomplete suggestions
    $form['search_autocomplete_what'] = array(
      '#type'           => 'fieldset',
      '#title'          => t('WHAT - What to display in Search Autocomplete suggestions?'),
      '#description'    => t('Choose which data should be added to autocompletion suggestions.'),
      '#collapsible'    => TRUE,
      '#collapsed'      => FALSE,
      '#theme'          => 'search_autocomplete_form_configuration_fieldset'
    );
    $form['search_autocomplete_what']['suggestions'] = array(
      '#type'           => 'item',
      '#title'          => t('Suggestion source'),
      '#description'    => t('Choose the source of suggestions to display in this form')
    );
    // use a callback
    $form['search_autocomplete_what']['suggestions']['callback'] = array( );
    $form['search_autocomplete_what']['callback']['callback_option'] = array(
      '#type'           => 'radio',
      '#title'          => t('Callback URL:'),
      '#return_value'   => 1,
      '#default_value'  => $item->data_source<2 ? 1 : 2,
      '#prefix'         => '<div class="form-radios">',
      '#parents'        => array('suggestions')
    );
    $descr =  t('Enter the url where to retrieve suggestions. It can be internal (absolute or relative) or external.') . '<br/>' .
              t('To make an easy internal suggestion url, create a view using "Autocompletion JSON" view style ! (<a href="http://projects.axiomcafe.fr/search-autocomplete/advanced-configuration/add-suggestion-type-version-3x">see documentation</a>)') . '<br/>' .
              t('Available views are:') . '<br/>' .
              'autocomplete-nodes :  ' . t('suggest node titles'). '<br/>' .
              'autocomplete-users :  ' . t('suggest user names'). '<br/>' .
              'autocomplete-words :  ' . t('suggest the node titles for nodes containing the user input');
    $form['search_autocomplete_what']['callback']['callback_textfield'] = array(
      '#type'           => 'textfield',
      '#description'    =>  $descr,
      '#default_value'  => $item->data_callback,
      '#size'           => 80,         // The default size is a bit large...
      '#suffix'         => '',       // End of the "form-radios" style.
      '#attributes'     => array('onClick' => '$("input[name=suggestions][value=1]").attr("checked", true);')
    );
    // use a static resource

    $form['search_autocomplete_what']['suggestions']['staticresource'] = array( );
    $form['search_autocomplete_what']['staticresource']['staticresource_option'] = array(
      '#type'           => 'radio',
      '#title'          => t('Static resource :'),
      '#return_value'   => 2,
      '#default_value'  => $item->data_source<2 ? 1 : 2,
      '#parents'        => array('suggestions')
    );
    $form['search_autocomplete_what']['staticresource']['staticresource_textfield'] = array(
      '#type'           => 'textarea',
      '#description'    => t('Please enter one suggestion per line. You can use the syntax: "foo => http://bar" per line if you wish to add a jumping to URL for the suggestion. Please refer to <a href="http://projects.axiomcafe.fr/search-autocomplete">documentation</a>.'),
      '#default_value'  => $item->data_static,
      '#size'           => 20,         // The default size is a bit large...
      '#suffix'         => '</div>',  // End of the "form-radios" style.
      '#attributes'     => array('onClick' => '$("input[name=suggestions][value=2]").attr("checked", true);')
    );

    // template to use
    $themes = array();
    $files = file_scan_directory( drupal_get_path('module', 'search_autocomplete') . '/css', '/.*\.css\z/', array('recurse' => FALSE));
    foreach ($files as $file) {
      if ($file->name != 'jquery.autocomplete') $themes[$file->filename] = $file->name;
    }
    $form['search_autocomplete_what']['theme'] = array(
      '#type'           => 'select',
      '#title'          => t('Select a theme for your suggestions'),
      '#options'        => $themes,
      '#default_value'  => $item->theme,
      '#description'    => t('Choose the theme to use for autocompletion dropdown popup. Read <a href="http://projects.axiomcafe.fr/search-autocomplete">documentation</a> to learn how to make your own.'),
    );


    // ------------------------------------------------------------------
    // ADVANCED - Advanced options
    $form['search_autocomplete_advanced'] = array(
      '#type'             => 'fieldset',
      '#title'            => t('ADVANCED - Advanced options'),
      '#collapsible'      => TRUE,
      '#collapsed'        => TRUE
    );
    $form['search_autocomplete_advanced']['selector'] = array(
      '#type'             => 'textfield',
      '#title'            => t('ID selector for this form'),
      '#description'      => t('Please change only if you know what you do, read <a href="http://projects.axiomcafe.fr/search-autocomplete">documentation</a> first.'),
      '#default_value'    => $item->selector,
      '#maxlength'        => 255,
      '#size'             => 35
    );
    // Add button validation
    $form['submit'] = array(
      '#type'             => 'submit',
      '#value'            => t('Save configuration')
    );
  }
  return $form;
}
// -------------------------------------------------------------------------------------
/**
 * Implements hook_validate().
 * Save the changes in the database
 */
function search_autocomplete_form_configure_validate($form, &$form_state) {

  $values = $form_state['values'];
  if ($values['suggestions'] == 1) {
    $new_url = preg_replace('/\?[^=]*=[^&]*/', '', $values['callback_textfield']);
    if (!drupal_valid_path($new_url)) {
      form_set_error('url', 'callback url is not valid: ' . $new_url);
    }
  }
}

// -------------------------------------------------------------------------------------
/**
 * Implements hook_submit().
 * Save the changes in the database
 */
function search_autocomplete_form_configure_submit($form, &$form_state) {
  global $base_url;

  $ok_query = TRUE;  // so far so good!
  // get form submission values
  $values = $form_state['values'];
  //Update the database with the new values
  $what = '';
  $sids = '';
  $weights = '';

  // analyse incoming callback
  $callback = $values['callback_textfield'];
  $data_type = 2;                                                               // if static resource => type = 2
  if ($callback != '') {
    if (url_is_external($callback)) {                                           // if path is absolute:
      if (strcmp(substr($callback, 0, strlen($base_url)), $base_url) === 0) {       // if path is internal:
        $callback = str_replace($base_url . "/", "", $callback);                    // get it relative
        $data_type = 1;                                                             // type = 1
      }
      else {                                                                  // if external: type = 0
        $data_type = 0;
      }
    }
    else {                                                                    // if path is not absolute:
      $data_type = 1;
    }
  }
  if ($values['suggestions'] == 2) $data_type = 2;

  // ###
  // UPDATE THE FORM
  // -> update form
  db_update('search_autocomplete_forms')
    ->fields(array(
      'min_char'        => $values['min_char'],
      'max_sug'         => $values['max_sug'],
      'auto_submit'     => $values['auto_submit'],
      'auto_redirect'   => $values['auto_redirect'],
      'no_results'      => $values['no_results'],
      'selector'        => $values['selector'],
      'data_source'     => $data_type,
      'data_callback'   => $callback,
      'data_static'     => $values['staticresource_textfield'],
      'theme'           => $values['theme']
    ))
    ->condition('fid', $values['fid'])
    ->execute();
  // ###
  // UPDATE CHILD LIST BUT NOT THE ADVANCED OPTIONS
  $fids = _search_autocomplete_get_all_children($values['fid']);
  // update the settings for this form + every children form
  foreach ($fids as $fid) {
    // -> update form
    db_update('search_autocomplete_forms')
      ->fields(array(
        'min_char'        => $values['min_char'],
        'max_sug'         => $values['max_sug'],
        'no_results'      => $values['no_results'],
        'auto_submit'     => $values['auto_submit'],
        'auto_redirect'   => $values['auto_redirect'],
        'theme'           => $values['theme']
      ))
      ->condition('fid', $fid)
      ->execute();
  }
  // ###
  drupal_clear_css_cache();
  $form_state['redirect'] = 'admin/config/search/search_autocomplete';
  $ok_query ? drupal_set_message(t("Configuration success !")) : drupal_set_message(t("An error has occured while saving the settings. Please, double check your settings!"), 'error');
}

/////////////////////////////////////////////////////////////////////////////////////////
////                             HELPER FUNCTIONS                                    ////

// -------------------------------------------------------------------------------------
/**
 * Helper function: get the array of fids every of his children of the caller but not
 * caller fid.
 */
function _search_autocomplete_get_all_children($fid, &$items = array(), $depth = 0) {
  if ($depth)
    $items[] = $fid;

  //$result = db_query('SELECT * FROM {search_autocomplete_forms} WHERE parent_fid=:parent_fid', array(':parent_fid' => $fid));
  $result = db_select('search_autocomplete_forms', 'f')
      ->fields('f')
      ->condition('parent_fid', $fid)
      ->execute()
      ->fetchAllAssoc('fid');

  foreach ($result as $item) {
    ++$depth;
    _search_autocomplete_get_all_children($item->fid, $items, $depth);
  }
  return $items;
}