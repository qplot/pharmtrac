<?php
/**
 * @file
 * pharmtrac_content.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function pharmtrac_content_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_features_date_format_default().
 */
function pharmtrac_content_features_date_format_default() {
  $fdf_config = array();

  $fdf_config[] = array(
    'dfid' => '37',
    'format' => 'y.m',
    'type' => 'custom',
    'locked' => '0',
  );
  $fdf_config[] = array(
    'dfid' => '38',
    'format' => 'y.m.d',
    'type' => 'custom',
    'locked' => '0',
  );
  $fdf_config[] = array(
    'dfid' => '39',
    'format' => 'y.W',
    'type' => 'custom',
    'locked' => '0',
  );
  $fdf_config[] = array(
    'dfid' => '43',
    'format' => 'Y-m-d',
    'type' => 'custom',
    'locked' => '0',
  );
  return $fdf_config;
}

/**
 * Implements hook_features_date_type_default().
 */
function pharmtrac_content_features_date_type_default() {
  $fdt_config = array();

  $fdt_config[] = array(
    'is_new' => FALSE,
    'module' => '',
    'type' => 'calendar_day',
    'title' => 'Calendar Day',
    'locked' => '0',
  );
  $fdt_config[] = array(
    'is_new' => FALSE,
    'module' => '',
    'type' => 'wallboard_day',
    'title' => 'Wallboard Day',
    'locked' => '0',
  );
  $fdt_config[] = array(
    'is_new' => FALSE,
    'module' => '',
    'type' => 'wallboard_hour',
    'title' => 'Wallboard Hour',
    'locked' => '0',
  );
  $fdt_config[] = array(
    'is_new' => FALSE,
    'module' => '',
    'type' => 'wallboard_month',
    'title' => 'Wallboard Month',
    'locked' => '0',
  );
  $fdt_config[] = array(
    'is_new' => FALSE,
    'module' => '',
    'type' => 'wallboard_week',
    'title' => 'Wallboard Week',
    'locked' => '0',
  );
  $fdt_config[] = array(
    'is_new' => FALSE,
    'module' => '',
    'type' => 'wallboard_year',
    'title' => 'Wallboard Year',
    'locked' => '0',
  );
  return $fdt_config;
}
