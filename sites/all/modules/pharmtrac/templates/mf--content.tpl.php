<div id="slider-distance"></div>
<div id="slider-charge"></div>
<div id="graph"></div>

<?php
  drupal_add_library('system', 'ui.slider');  

  $path = drupal_get_path('module', 'pharmtrac');
  // drupal_add_js($path . '/js/an_location.js');
  
  drupal_add_js('http://d3js.org/d3.v3.min.js', 'external');
  
  drupal_add_css($path . '/css/contents.css');
  drupal_add_js($path . '/js/contents.js');
?>

