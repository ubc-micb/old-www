<?php

/**
 * @file
 * Search Autocomplete
 * Sets the admin part of the module: permissions, hooks, callbacks, etc...
 *
 * @authors
 * Miroslav Talenberg (Dominique CLAUSE) <http://www.axiomcafe.fr/contact>
 *
 * Sponsored by:
 * www.axiomcafe.fr
 */


/**
 * Implements hook_permission().
 * Valid permissions for this module
 * @return
 *   An array of valid permissions for the autocomplete module
 */
function search_autocomplete_permission() {
  return array(
    'administer Search Autocomplete' => array(
      'title' => t('Administer Search Autocomplete'),
      'description' => t('Access administration panel for autocompletion settings.'),
    ),
    'use Search Autocomplete' => array(
      'title' => t('Use Search Autocomplete'),
      'description' => t('Allow usage of autocompletion on forms.'),
    ),
  );
} // function search_autocomplete_permissions()

/**
 * Implements hook_theme().
 * Define the function to render our forms
 */
function search_autocomplete_theme($existing, $type, $theme, $path) {
  return array(
    'search_autocomplete_treelist_form' => array(                    // register theme function for draggable treelist search forms
      'render element' => 'form',
    )
  );
} // function search_autocomplete_theme($existing, $type, $theme, $path)

/**
 * Implements hook_menu().
 * Create an administration page to access admin form
 */
function search_autocomplete_menu() {

  // create the admin setting page: list of forms
  $items['admin/config/search/search_autocomplete'] = array(
    'title'             => 'Search Autocomplete settings',
    'description'       => 'Choose settings and suggestion items for autocompletion in input forms.',
    'page callback'     => 'drupal_get_form',
    'page arguments'    => array('search_autocomplete_treelist_form'),
    'access arguments'  => array('administer Search Autocomplete'),
    'file'              => 'search_autocomplete.form.treelist.inc',
    'type'              => MENU_NORMAL_ITEM,
    'weight'            => 0
  );
  // create a tab for settings
  $items['admin/config/search/search_autocomplete/settings'] = array(
    'title'             => 'Search Autocomplete settings',
    'type'              => MENU_DEFAULT_LOCAL_TASK,
    'weight'            => 1
  );
  // create an admin setting page: add a new form
  $items['admin/config/search/search_autocomplete/add'] = array(
    'title'             => 'Add a form',
    'page callback'     => 'drupal_get_form',
    'page arguments'    => array('search_autocomplete_form_add'),
    'access arguments'  => array('administer Search Autocomplete'),
    'file'              => 'search_autocomplete.form.add.inc',
    'type'              => MENU_LOCAL_TASK,
    'weight'            => 2
  );
  // create an admin setting page: configure a form
  $items['admin/config/search/search_autocomplete/%/configure'] = array(
    'title'             => 'Search Autocomplete - Configuration',
    'page callback'     => 'drupal_get_form',
    'page arguments'    => array('search_autocomplete_form_configure'),
    'access arguments'  => array('administer Search Autocomplete'),
    'file'              => 'search_autocomplete.form.configure.inc',
  );
  // create an admin setting page: delete a form
  $items['admin/config/search/search_autocomplete/%/delete/%'] = array(
    'title'             => 'Search Autocomplete - Delete',
    'page callback'     => 'drupal_get_form',
    'page arguments'    => array('search_autocomplete_form_delete', 4, 6),
    'access arguments'  => array('administer Search Autocomplete'),
    'file'              => 'search_autocomplete.form.delete.inc',
  );
  return $items;
} // function search_autocomplete_menu()

/**
 * Implements hook_help().
 */
function search_autocomplete_help($path, $arg) {
  $help = '';
  switch ($path) {
    case 'admin/modules#description':
      $help = t('Allows the users with the right permission to use advanced autocompletion features on forms.');
      break;
    case 'admin/config/search/search_autocomplete':
      $help  = '<div>' . t('Search Autocomplete helps you to enhance your search forms with autocompleted suggestions.') . '</div>';
      $help .= '<div>' . t('Use this form to activate the forms you want to autocomplete.') . '</div>';
      $help .= '<div>' . t('You may want to add new possible form to autocomplete. To do so, please use the tab <a href="search_autocomplete/add">Add a form</a>.') . '</div>';
      break;
    case 'admin/config/search/search_autocomplete/add':
      $help  = '<div>' . t('This page allows you to add a new form to be autocompleted with the Search Autocomplete module.') . '</div>';
      $help .= '<div>' . t('This is an advanced feature configuration. Use it only if you know what you are doing and after reading <a href="http://projects.axiomcafe.fr/search-autocomplete">documentation</a>. If you wish help, please ask for a new default form to be added in the next release of Search Autocomplete module.') . '</div>';
      break;
    case 'admin/config/search/search_autocomplete/%/configure':
      $help  = '<div>' . t('You are currently configuring the options to autocomplete a form.') . '</div>';
      $help .= '<div>' . t('Every children forms will be modified as well.') . '</div>';
      break;
  }
  return $help;
}
