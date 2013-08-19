<?php
// $Id$
/**
 * @file
 * Plugin Theme File
 */
function template_preprocess_views_highcharts(&$vars) {
  $options = $vars['options'];
  module_load_include("module", "libraries", "libraries");
  if ($path = libraries_get_path("highcharts")) {
    drupal_add_js($path . '/js/highcharts.js');
  }
  drupal_add_js(drupal_get_path('module', 'views_highcharts') . '/js/views_highcharts.js');
  module_load_include('inc', 'uuid', 'uuid');
  $chart_id = "views-highcharts-" . uuid_generate();
  $view = $vars['view'];
  $fields = $view->get_items("field");
  $data = array();
  $type = ($options['format']['chart_type'] == "pie") ? "pie" : "bar";
  $highcharts_config = json_decode(file_get_contents(drupal_get_path("module", "views_highcharts") . "/defaults/bar-basic.json"));
  $highcharts_config->chart->defaultSeriesType = $options['format']['chart_type'];
  $highcharts_config->title->text = $options['format']['title'];
  $highcharts_config->subtitle->text = $options['format']['subtitle'];

  $highcharts_config->legend->enabled = (strtolower($options['format']['legend_enabled']) == "yes")?true:false;
	if ($options['format']['chart_type']!= "pie") {
		$highcharts_config->yAxis[0]->title->text = $options['y_axis']['title'];
	  $highcharts_config->yAxis[0]->title->align = $options['y_axis']['title_align'];
		$highcharts_config->yAxis[1]->title->text = $options['y2_axis']['title'];
	  $highcharts_config->yAxis[1]->title->align = $options['y2_axis']['title_align'];
    
	  if ($options['x_axis']["reversed"] != false) {
		$highcharts_config->xAxis->reversed = true;
	  }
	  if ($options['y_axis']["reversed"] != false) {
		$highcharts_config->yAxis[0]->reversed = true;
	  }
	  if ($options['format']['swap_axes'] != false) {
		$highcharts_config->chart->inverted = true;
	  }
	}

  
  $highcharts_config->chart->renderTo = $chart_id;
  $highcharts_config->series = array();
  $highcharts_config->xAxis->categories = array();
  if (is_array($options)
    && is_array($options['x_axis']['dataset_data'])
    && is_array($fields)
  ) {
    foreach ($options['x_axis']['dataset_data'] as $key) {
      if ($key != FALSE) {
        $vars['fields'][$key] = $fields[$key];
        $vars['fields'][$key]['axis'][] = 0;
      }
    }
  }
  
  foreach ($options['y2_axis']['dataset_data'] as $key) {
    if (!empty($key)) {
      $vars['fields'][$key] = $fields[$key];
      $vars['fields'][$key]['axis'][] = 1;
    }
  }

  $highcharts_config->xAxis->categories = array();
  foreach ($view->style_plugin->render_tokens as $result_index => $row) {
    foreach ($row as $field_name => $field) {
    // $f = str_replace(array('[',']'), "", $field_name);
    // if ($options['x_axis']['dataset_data'][$f]) {
    //  $data[$f][] = (float)$field;
    // }
      $check = str_split($field_name);
      if ($check[0] !==  '%' && $check[0] !== '!') {
        $f = str_replace(array('[',']'), '', $field_name);
        if ($options['x_axis']['dataset_data'][$f]) {
           $data[$f][] = (float)$field;
        } else if (isset($options['y2_axis']['dataset_data'][$f])) {
           $data[$f][] = (float)$field;
        }
      }    
    }
    if (!empty($options['x_axis']['dataset_label'])) {
      $highcharts_config->xAxis->categories[] = $row["[".$options['x_axis']['dataset_label']."]"];		
    }
  }


	if (function_exists("highcharts_series_".$options['format']['chart_type'])) {
		//if there's a specialized data writer, return data from data writer
		$highcharts_config->series = array(call_user_func("highcharts_series_".$options['format']['chart_type'], $data, $fields, $options));
	} else {
		//else get a standard series
		$highcharts_config->series = highcharts_series($data, $vars['fields']);
	}

  //   $highcharts_config->tooltip->formatter = 'eval(function() {
  // return ' . $options['format']['tooltip_formatter'] . '; 
  // })';

	

  //print_r($options);
  drupal_add_js(array("views_highcharts" => array($chart_id => $highcharts_config)), "setting");
  $vars['chart_id'] = $chart_id;

//print_r($highcharts_config);
//exit();

}

function highcharts_series($data, $fields) {
	$toReturn = array();
	foreach ($fields as $key => $field) {
    foreach ($field['axis'] as $ax) {
	    $item = new stdClass;
	    $item->name = (array_key_exists("label", $field))?$field['label']:$field['id'];
	    $item->data = $data[$key];
      $item->yAxis = $ax;
	    $toReturn[] = $item;
    }
  }
	return $toReturn;
	
}

function highcharts_series_pie($data, $fields, $options) {
	$toReturn = new StdClass();
	$toReturn->type = $options['format']['chart_type'];
	$toReturn->name = $options['format']['title'];
	$toReturn->data = array();
	
	foreach ($fields as $key => $field) {
		$pie = 0;
		if (array_key_exists($key, $data)) {
			foreach($data[$key] as $value) {
				if (is_numeric($value)) {
					$pie += $value;
				}
			}
	    $toReturn->data[] = array((array_key_exists("label", $field))?$field['label']:$field['id'], $pie);
		}
		
	  }
	return $toReturn;
}
