<div id="nodejs">123</div>

<?php
  $path = drupal_get_path('module', 'pharmtrac_notification');
  
  drupal_add_js($path . '/js/socket.io.min.js', 'file');
  drupal_add_js($path . '/js/notification.js', 'file');
  // drupal_add_js(array('pharmtrac' => array('data' => $items)), 'setting');
?>



