<div>color scale: 
  <select>
    <optgroup label="Colors by Cynthia Brewer.">
      <option value="BrBG">BrBG</option>
      <option value="PiYG">PiYG</option>
      <option value="PRGn">PRGn</option>
      <option value="PuOr">PuOr</option>
      <option value="RdBu">RdBu</option>
      <option value="RdGy">RdGy</option>
      <option value="RdYlBu">RdYlBu</option>
      <option value="RdYlGn" selected>RdYlGn</option>
      <option value="Spectral">Spectral</option>
    </optgroup>
  </select>
</div>

<div id="calendar"></div>

<?php
  // drupal_add_library('system', 'ui.slider');  

  $path = drupal_get_path('module', 'pharmtrac');
  
  drupal_add_js($path . '/js/d3.v3.min.js', 'file');
  drupal_add_css($path . '/css/calendar.css');
  drupal_add_js($path . '/js/calendar.js');
  
  // grab some data from service views
  module_load_include('inc','services_views','services_views.resource');
  $items = services_views_retrieve('report_calendar', 'services_1');
  
  drupal_add_js(array('pharmtrac' => array('data' => $items)), 'setting');
?>

