<div id="nodejs"></div>

<?php
  $path = drupal_get_path('module', 'pharmtrac');
  
  drupal_add_js($path . '/js/socket.io.min.js', 'file');
  drupal_add_js($path . '/js/nodejs.js', 'file');
  // drupal_add_js(array('pharmtrac' => array('data' => $items)), 'setting');
?>



