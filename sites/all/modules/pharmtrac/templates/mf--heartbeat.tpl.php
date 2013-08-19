1234
<div id="heartbeat"></div>

<?php
  // drupal_add_library('system', 'ui.slider');  

  $path = drupal_get_path('module', 'pharmtrac');
  
  drupal_add_js('http://js.pusher.com/2.0/pusher.min.js', 'file');
  drupal_add_js($path . '/js/heartbeat.js', 'file');
  // drupal_add_css($path . '/css/calendar.css');
  
  // drupal_add_js(array('pharmtrac' => array('data' => $items)), 'setting');
?>

