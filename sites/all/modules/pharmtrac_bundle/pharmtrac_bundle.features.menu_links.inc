<?php
/**
 * @file
 * pharmtrac_bundle.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function pharmtrac_bundle_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu:bundles
  $menu_links['main-menu:bundles'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'bundles',
    'router_path' => 'bundles',
    'link_title' => 'Bundle',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-47',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Bundle');


  return $menu_links;
}
