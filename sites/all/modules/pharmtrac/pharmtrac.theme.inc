<?php

/**
 * @file
 * Code for the Drupal Theme Functions.
 */
 
/**
 * Pharmtrac api functions
 */
include_once 'pharmtrac.api.inc';

/**
 * Implements hook_theme()
 */
function pharmtrac_theme() {
  $themes = array(); 
  
  $path = drupal_get_path('module', 'pharmtrac');
  
  $themes['view_title'] = array(
    'variables' => array('title' => ''),
  );
  $themes['mf__content'] = array(
    'arguments' => array(),
    'template' => 'mf--content',
    'path' => $path . '/templates'
  );
  $themes['mf__request_picture'] = array(
    'arguments' => array(),
    'template' => 'mf--request-picture',
    'path' => $path . '/templates'
  );
  $themes['mf__ontime_calendar'] = array(
    'arguments' => array(),
    'template' => 'mf--ontime-calendar',
    'path' => $path . '/templates'
  );
  $themes['mf__heartbeat'] = array(
    'arguments' => array(),
    'template' => 'mf--heartbeat',
    'path' => $path . '/templates'
  );
  $themes['mf__nodejs'] = array(
    'arguments' => array(),
    'template' => 'mf--nodejs',
    'path' => $path . '/templates'
  );
  
  return $themes;
}

/**
 * Theme function for view title
 * @param $variables['type']
 *  type of the content type of the view
 * @param $variables['caption']
 *  [OPTIONAL] link caption, by default 'new $type'
 * @param $variables['link']
 *  [OPTIONAL] link, by default 'node/add/$type'
 * @param $variables['destination']
 *  [OPTIONAL] page return link, by default '$type(s)'
 * @param $variables['title']
 *  [OPTIONAL] page title, by default '$type(s)'
 */
function theme_view_title($variables) {
  $caption = !empty($variables['caption']) ? $variables['caption'] : 'new ' . $variables['type'];
  $link = !empty($variables['link']) ? $variables['link'] : 'node/add/' . $variables['type'];
  $destination = !empty($variables['destination']) ? $variables['destination'] : $_GET['q'];
  $title = !empty($variables['title']) ? $variables['title'] : ucwords($variables['type'] . 's');
  $query = array('destination' => $destination);
  $class = !empty($variables['class']) ? $variables['class'] : 'add-link pull-right';
  if (!empty($variables['query']))
    $query = array_merge($variables['query'], $query);
  $output = '<div class="' . $class . '">';
  $output .= l('<i class="icon-pencil icon-white"></i> ' . $caption, $link, array('html'=> TRUE, 'attributes'=> array('class'=> 'btn btn-primary'), 'query' => $query));
  $output .= '</div>';
  if ($title)
    $output .= '<h1 class="page-header">' . $title . '</h1>';
  return $output;
}

/**
 * Statistic Block
 */
function pharmtrac_block_stats_view() {
  $types = array('order','patient','med','dispense','request','device');
  $counts = array();
  foreach ($types as $value) {
    $count = getEntitiesCount(array('type'=>$value));
    $counts[] = 'Total ' . $value . ': ' . $count;
  }
  $items = array(
    'items' => $counts,
  );
  return theme('item_list', $items);
}

/**
 * Wallboard Footer Block
 */
function pharmtrac_block_wallboard_stats_view() {
  $settings = variable_get('plusdelta_color_settings');
  $location_id = is_numeric(arg(1))?arg(1):NULL;
  $counts = plusdelta_api_getWallboardCounts($settings, $location_id);
  // dsm($counts); 
  
  return theme('table', array(
    'header' => getMappedArray('label', $counts), 
    'rows' => array(getMappedArray('count', $counts))
  ));
  // $items = array();
  // foreach ($counts as $key => $value) {
  //   $item = '<h3>' . $value['label'] . '</h3>';
  //   $item .= '<span>' . $value['count'] . '</span>';
  //   $items[] = $item;
  // }
  // 
  // return theme('item_list', array('items' => $items, 'attributes' => array('class' => 'well')));
}

/**
 * Generic view title display
 */
function pharmtrac_view_title_view($title, $type, $link = '', $destination = '') {
}
