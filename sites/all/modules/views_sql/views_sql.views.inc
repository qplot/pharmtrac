<?php
/**
 * @file
 * pharmtrac.views.inc
 */


/**
 * Implements hook_views_data().
 */
function views_sql_views_data() {
  $data['views_sql']['table']['group'] = t('Global');
  $data['views_sql']['table']['join'] = array(
    // #global is a special flag which let's a table appear all the time.
    '#global' => array(),
  );
  $data['views_sql']['views_sql'] = array(
    'title' => t('SQL field'),
    'help' => t('Apply sql function to field'),
    'field' => array(
      'handler' => 'views_handler_field_sql',
    ),
  );
  return $data;
}
