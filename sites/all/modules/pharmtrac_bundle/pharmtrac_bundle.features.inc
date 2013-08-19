<?php
/**
 * @file
 * pharmtrac_bundle.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function pharmtrac_bundle_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function pharmtrac_bundle_views_api() {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function pharmtrac_bundle_node_info() {
  $items = array(
    'bundle' => array(
      'name' => t('Bundle'),
      'base' => 'node_content',
      'description' => t('Use bundle to make a bag of dispenses for tracking'),
      'has_title' => '1',
      'title_label' => t('Barcode'),
      'help' => '',
    ),
  );
  return $items;
}
