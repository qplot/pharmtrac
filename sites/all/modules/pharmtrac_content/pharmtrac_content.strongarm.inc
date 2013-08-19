<?php
/**
 * @file
 * pharmtrac_content.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function pharmtrac_content_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_export_existing';
  $strongarm->value = 'skip';
  $export['node_export_existing'] = $strongarm;

  return $export;
}
