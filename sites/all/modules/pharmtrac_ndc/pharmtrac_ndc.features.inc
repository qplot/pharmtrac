<?php
/**
 * @file
 * pharmtrac_ndc.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function pharmtrac_ndc_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function pharmtrac_ndc_views_api() {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function pharmtrac_ndc_node_info() {
  $items = array(
    'ndc' => array(
      'name' => t('NDC'),
      'base' => 'node_content',
      'description' => t('Used for NDC checking'),
      'has_title' => '1',
      'title_label' => t('CDM'),
      'help' => '',
    ),
  );
  return $items;
}
