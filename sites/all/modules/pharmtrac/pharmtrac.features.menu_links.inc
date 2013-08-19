<?php
/**
 * @file
 * pharmtrac.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function pharmtrac_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: features:<nolink>
  $menu_links['features:<nolink>'] = array(
    'menu_name' => 'features',
    'link_path' => '<nolink>',
    'router_path' => '<nolink>',
    'link_title' => 'Report',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '1',
    'expanded' => '1',
    'weight' => '-47',
  );
  // Exported menu link: features:calendar
  $menu_links['features:calendar'] = array(
    'menu_name' => 'features',
    'link_path' => 'calendar',
    'router_path' => 'calendar',
    'link_title' => 'Ontime calendar',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-50',
    'parent_path' => '<nolink>',
  );
  // Exported menu link: features:orders
  $menu_links['features:orders'] = array(
    'menu_name' => 'features',
    'link_path' => 'orders',
    'router_path' => 'orders',
    'link_title' => 'Order',
    'options' => array(
      'attributes' => array(
        'title' => 'Look up orders',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-50',
  );
  // Exported menu link: features:report-heatmap
  $menu_links['features:report-heatmap'] = array(
    'menu_name' => 'features',
    'link_path' => 'report-heatmap',
    'router_path' => 'report-heatmap',
    'link_title' => 'Ontime heatmap',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-49',
    'parent_path' => '<nolink>',
  );
  // Exported menu link: features:report_chart_1
  $menu_links['features:report_chart_1'] = array(
    'menu_name' => 'features',
    'link_path' => 'report_chart_1',
    'router_path' => 'report_chart_1',
    'link_title' => 'Ontime delivery',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-48',
    'parent_path' => '<nolink>',
  );
  // Exported menu link: features:wallboards
  $menu_links['features:wallboards'] = array(
    'menu_name' => 'features',
    'link_path' => 'wallboards',
    'router_path' => 'wallboards',
    'link_title' => 'Wallboard',
    'options' => array(
      'attributes' => array(
        'title' => 'List of wallboards',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '1',
    'expanded' => '1',
    'weight' => '-49',
  );
  // Exported menu link: main-menu:<front>
  $menu_links['main-menu:<front>'] = array(
    'menu_name' => 'main-menu',
    'link_path' => '<front>',
    'router_path' => '',
    'link_title' => 'Home',
    'options' => array(
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '1',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-50',
  );
  // Exported menu link: main-menu:<nolink>/contents
  $menu_links['main-menu:<nolink>/contents'] = array(
    'menu_name' => 'main-menu',
    'link_path' => '<nolink>/contents',
    'router_path' => '<nolink>',
    'link_title' => 'Contents',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-48',
  );
  // Exported menu link: main-menu:<nolink>/extras
  $menu_links['main-menu:<nolink>/extras'] = array(
    'menu_name' => 'main-menu',
    'link_path' => '<nolink>/extras',
    'router_path' => '<nolink>',
    'link_title' => 'Extras',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-15',
  );
  // Exported menu link: main-menu:<nolink>/reports
  $menu_links['main-menu:<nolink>/reports'] = array(
    'menu_name' => 'main-menu',
    'link_path' => '<nolink>/reports',
    'router_path' => '<nolink>',
    'link_title' => 'Reports',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-21',
  );
  // Exported menu link: main-menu:<nolink>/types
  $menu_links['main-menu:<nolink>/types'] = array(
    'menu_name' => 'main-menu',
    'link_path' => '<nolink>/types',
    'router_path' => '<nolink>',
    'link_title' => 'Types',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-32',
  );
  // Exported menu link: main-menu:<separator>/contents
  $menu_links['main-menu:<separator>/contents'] = array(
    'menu_name' => 'main-menu',
    'link_path' => '<separator>/contents',
    'router_path' => '<separator>',
    'link_title' => 'Contents separator',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-49',
  );
  // Exported menu link: main-menu:<separator>/extras
  $menu_links['main-menu:<separator>/extras'] = array(
    'menu_name' => 'main-menu',
    'link_path' => '<separator>/extras',
    'router_path' => '<separator>',
    'link_title' => 'Extras separator',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-16',
  );
  // Exported menu link: main-menu:<separator>/reports
  $menu_links['main-menu:<separator>/reports'] = array(
    'menu_name' => 'main-menu',
    'link_path' => '<separator>/reports',
    'router_path' => '<separator>',
    'link_title' => 'Reports separator',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-22',
  );
  // Exported menu link: main-menu:<separator>/types
  $menu_links['main-menu:<separator>/types'] = array(
    'menu_name' => 'main-menu',
    'link_path' => '<separator>/types',
    'router_path' => '<separator>',
    'link_title' => 'Types separator',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-33',
  );
  // Exported menu link: main-menu:calendar
  $menu_links['main-menu:calendar'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'calendar',
    'router_path' => 'calendar',
    'link_title' => 'Ontime calendar',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-20',
  );
  // Exported menu link: main-menu:colors
  $menu_links['main-menu:colors'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'colors',
    'router_path' => 'colors',
    'link_title' => 'Color',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-31',
  );
  // Exported menu link: main-menu:content-types
  $menu_links['main-menu:content-types'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'content-types',
    'router_path' => 'content-types',
    'link_title' => 'Content type',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-14',
  );
  // Exported menu link: main-menu:d3/content
  $menu_links['main-menu:d3/content'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'd3/content',
    'router_path' => 'd3/content',
    'link_title' => 'Content links',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-17',
  );
  // Exported menu link: main-menu:devices
  $menu_links['main-menu:devices'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'devices',
    'router_path' => 'devices',
    'link_title' => 'Device',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-46',
  );
  // Exported menu link: main-menu:dispenses
  $menu_links['main-menu:dispenses'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'dispenses',
    'router_path' => 'dispenses',
    'link_title' => 'Dispense',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-45',
  );
  // Exported menu link: main-menu:import
  $menu_links['main-menu:import'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'import',
    'router_path' => 'import',
    'link_title' => 'Import',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-11',
  );
  // Exported menu link: main-menu:location-properties
  $menu_links['main-menu:location-properties'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'location-properties',
    'router_path' => 'location-properties',
    'link_title' => 'Location property',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-30',
  );
  // Exported menu link: main-menu:locations
  $menu_links['main-menu:locations'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'locations',
    'router_path' => 'locations',
    'link_title' => 'Location',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-44',
  );
  // Exported menu link: main-menu:med-features
  $menu_links['main-menu:med-features'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'med-features',
    'router_path' => 'med-features',
    'link_title' => 'Med feature',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-29',
  );
  // Exported menu link: main-menu:med-quantities
  $menu_links['main-menu:med-quantities'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'med-quantities',
    'router_path' => 'med-quantities',
    'link_title' => 'Med quantity',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-42',
  );
  // Exported menu link: main-menu:meds
  $menu_links['main-menu:meds'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'meds',
    'router_path' => 'meds',
    'link_title' => 'Med',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-43',
  );
  // Exported menu link: main-menu:messages
  $menu_links['main-menu:messages'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'messages',
    'router_path' => 'messages',
    'link_title' => 'Message',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-10',
  );
  // Exported menu link: main-menu:mobile-features
  $menu_links['main-menu:mobile-features'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'mobile-features',
    'router_path' => 'mobile-features',
    'link_title' => 'Mobile feature',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-28',
  );
  // Exported menu link: main-menu:mobile-roles
  $menu_links['main-menu:mobile-roles'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'mobile-roles',
    'router_path' => 'mobile-roles',
    'link_title' => 'Mobile role',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-27',
  );
  // Exported menu link: main-menu:orders
  $menu_links['main-menu:orders'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'orders',
    'router_path' => 'orders',
    'link_title' => 'Order',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-41',
  );
  // Exported menu link: main-menu:patients
  $menu_links['main-menu:patients'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'patients',
    'router_path' => 'patients',
    'link_title' => 'Patient',
    'options' => array(
      'attributes' => array(
        'title' => 'List of paitents',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-40',
  );
  // Exported menu link: main-menu:priority-levels
  $menu_links['main-menu:priority-levels'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'priority-levels',
    'router_path' => 'priority-levels',
    'link_title' => 'Priority level',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-26',
  );
  // Exported menu link: main-menu:reject-types
  $menu_links['main-menu:reject-types'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'reject-types',
    'router_path' => 'reject-types',
    'link_title' => 'Reject type',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-25',
  );
  // Exported menu link: main-menu:report-heatmap
  $menu_links['main-menu:report-heatmap'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'report-heatmap',
    'router_path' => 'report-heatmap',
    'link_title' => 'Heatmap',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-18',
  );
  // Exported menu link: main-menu:report_chart_1
  $menu_links['main-menu:report_chart_1'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'report_chart_1',
    'router_path' => 'report_chart_1',
    'link_title' => 'Ontime delivery',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-19',
  );
  // Exported menu link: main-menu:request-types
  $menu_links['main-menu:request-types'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'request-types',
    'router_path' => 'request-types',
    'link_title' => 'Request type',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-24',
  );
  // Exported menu link: main-menu:requests
  $menu_links['main-menu:requests'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'requests',
    'router_path' => 'requests',
    'link_title' => 'Request',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-39',
  );
  // Exported menu link: main-menu:rooms
  $menu_links['main-menu:rooms'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'rooms',
    'router_path' => 'rooms',
    'link_title' => 'Room',
    'options' => array(
      'attributes' => array(
        'title' => 'List of rooms',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-37',
  );
  // Exported menu link: main-menu:settings
  $menu_links['main-menu:settings'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'settings',
    'router_path' => 'settings',
    'link_title' => 'Settings',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-9',
  );
  // Exported menu link: main-menu:shifts
  $menu_links['main-menu:shifts'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'shifts',
    'router_path' => 'shifts',
    'link_title' => 'Shift',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-36',
  );
  // Exported menu link: main-menu:users
  $menu_links['main-menu:users'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'users',
    'router_path' => 'users',
    'link_title' => 'User',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-35',
  );
  // Exported menu link: main-menu:wallboard-columns
  $menu_links['main-menu:wallboard-columns'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'wallboard-columns',
    'router_path' => 'wallboard-columns',
    'link_title' => 'Wallboard column',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-23',
  );
  // Exported menu link: main-menu:wallboards
  $menu_links['main-menu:wallboards'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'wallboards',
    'router_path' => 'wallboards',
    'link_title' => 'Wallboard',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-34',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Color');
  t('Content links');
  t('Content type');
  t('Contents');
  t('Contents separator');
  t('Device');
  t('Dispense');
  t('Extras');
  t('Extras separator');
  t('Heatmap');
  t('Home');
  t('Import');
  t('Location');
  t('Location property');
  t('Med');
  t('Med feature');
  t('Med quantity');
  t('Message');
  t('Mobile feature');
  t('Mobile role');
  t('Ontime calendar');
  t('Ontime delivery');
  t('Ontime heatmap');
  t('Order');
  t('Patient');
  t('Priority level');
  t('Reject type');
  t('Report');
  t('Reports');
  t('Reports separator');
  t('Request');
  t('Request type');
  t('Room');
  t('Settings');
  t('Shift');
  t('Types');
  t('Types separator');
  t('User');
  t('Wallboard');
  t('Wallboard column');


  return $menu_links;
}
