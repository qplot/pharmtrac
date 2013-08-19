<?php
/**
 * @file
 * pharmtrac.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function pharmtrac_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: features.
  $menus['features'] = array(
    'menu_name' => 'features',
    'title' => 'Feature menu',
    'description' => 'Menu items for any enabled features.',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Feature menu');
  t('Menu items for any enabled features.');


  return $menus;
}