<?php
/**
 * @file
 * Code for the Pharmtrac debug/dev features.
 */

/**
 * Menu iitems
 */
function pharmtrac_debug_menu() {
  $items = array();
  
  $items['fileupload'] = array(
    'title' => 'createRequest Test',
    'description' => 'createRequest attach files testing page',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('pharmtrac_upload_form'),
    // 'page callback' => 'pharmtrac_debug_fileupload',
    'access callback' => 'user_access',
    'access arguments' => array('administer pharmtrac'),
    // 'delivery callback' => 'ajax_deliver',
    'type' => MENU_CALLBACK,
  );

  $items['fileupload2'] = array(
    'title' => 'Fileupload',
    'description' => 'Fileupload test',
    'page callback' => 'pharmtrac_debug_fileupload',
    'access callback' => 'user_access',
    'access arguments' => array('administer pharmtrac'),
    // 'delivery callback' => 'ajax_deliver',
    'type' => MENU_CALLBACK,
  );
  
  $items['ajax_patients_view/%/%'] = array(
    'title' => 'Patient AJAX View',
    'description' => 'Use ajax approach to fetch patients view',
    'page callback' => 'pharmtrac_ajax_patients_view',
    'page arguments' => array(1,2),
    'access callback' => 'user_access',
    'access arguments' => array('administer pharmtrac'),
    'type' => MENU_CALLBACK,
  );
  
  $items['d3'] = array(
    'title' => 'D3 Testing',
    'description' => 'D3 testing with various graph type',
    'page callback' => 'pharmtrac_d3',
    'access callback' => 'user_access',
    'access arguments' => array('administer pharmtrac'),
    'type' => MENU_CALLBACK,
  );
  
  $items['d3/content'] = array(
    'title' => 'Content Types',
    'description' => 'Content type relationship',
    'page callback' => 'theme',
    'page arguments' => array('mf__content'),
    'access callback' => 'user_access',
    'access arguments' => array('administer pharmtrac'),
    'type' => MENU_CALLBACK,
  );
  
  $items['picture'] = array(
    'title' => 'Approve pictures',
    'description' => 'Approve pictures for verification request step',
    'page callback' => 'theme',
    'page arguments' => array('mf__request_picture'),
    'access callback' => 'user_access',
    'access arguments' => array('administer pharmtrac'),
    'type' => MENU_CALLBACK,
  );
  
  $items['calendar'] = array(
    'title' => 'Ontime Calendar',
    'description' => 'Ontime calendar view using d3',
    'page callback' => 'theme',
    'page arguments' => array('mf__ontime_calendar'),
    'access callback' => 'user_access',
    'access arguments' => array('administer pharmtrac'),
    'type' => MENU_CALLBACK,
  );
  
  $items['heartbeat'] = array(
    'title' => 'Heartbeat',
    'description' => 'Bi-directional heartbeat notification system using pusher',
    'page callback' => 'theme',
    'page arguments' => array('mf__heartbeat'),
    'access callback' => 'user_access',
    'access arguments' => array('administer pharmtrac'),
    'type' => MENU_CALLBACK,
  );
  
  $items['nodejs'] = array(
    'title' => 'Realtime Notification',
    'description' => 'Realtime notification through nodejs',
    'page callback' => 'theme',
    'page arguments' => array('mf__nodejs'),
    'access callback' => 'user_access',
    'access arguments' => array('administer pharmtrac'),
    'type' => MENU_CALLBACK,
  );
  
  return $items;
}

function pharmtrac_debug_fileupload() {
  // drupal_json_output($_REQUEST);
  drupal_json_output($_FILES);
  
  drupal_exit();
}

function pharmtrac_upload_form($form, &$form_state) {
  
  $form['#action'] = url('api', array('query' => array('command' => 'createRequest')));
  
  $form['barcode'] = array(
    '#type' => 'textfield',
    '#title' => 'Dispense barcode',
    '#default_value' => 'JZ01234560-ABCD',
    '#size' => 40,
  );
  $form['user'] = array(
    '#type' => 'textfield',
    '#title' => 'User barcode',
    '#default_value' => 'SE23-231',
    '#size' => 20,
  );
  $form['location'] = array(
    '#type' => 'textfield',
    '#title' => 'Location barcode',
    '#default_value' => '2000',
    '#size' => 20,
  );
  $form['type'] = array(
    '#type' => 'textfield',
    '#title' => 'Request type',
    '#default_value' => 'received',
    '#size' => 10,
  );
  $form['device'] = array(
    '#type' => 'textfield',
    '#title' => 'Device barcode',
    '#default_value' => 'DE834',
    '#size' => 10,
  );
  $form['note'] = array(
    '#type' => 'textfield',
    '#title' => 'Note',
    '#default_value' => '123',
    '#size' => 40,
  );
  $form['image'] = array(
    '#type' => 'file',
    '#title' => 'Upload file',
  );
  
  $form['submit'] = array(
    '#type' => 'submit', 
    '#value' => 'Submit'
  );
  
  return $form;
}

function pharmtrac_upload_form_submit($form, &$form_state) {
  dprint_r($_POST);
  dprint_r($_FILES); die;
  // $file = file_save_upload('fileupload', array(), 'private://requests');
  // $file->status = 1;
  // file_save($file);
  // $node = node_load(1706);
  // $node->field_request_picture['und'][0] = (array)$file;
  // node_save($node);
  // 
  // dsm($file);
}

function pharmtrac_ajax_patients_view($type = 'ajax', $nid = NULL) {
  if ($type == 'ajax') {
    // $output = t("This is some content delivered via AJAX");
    
    // $view = views_get_view('patients');
    // $view->set_arguments(array( 'argument1', 'argument2', 3 ));
    // $view->exposed_input['title'] = "012345605";
    // $view->execute();
    // $output = $view->render('page');
    $output['patient'] = array(
      '#type' => 'fieldset',
      '#title' => t('More Info'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#attributes' => array('id' => 'ajax-patients-view'),
    );
    $output['patient']['teaser'] = node_view(node_load($nid), 'teaser');
      
    // $output = dprint_r($output, TRUE);
    $output = drupal_render_children($output);
    
    $commands = array();
    $commands[] = ajax_command_replace('#ajax-patients-view', $output);
    $page = array('#type' => 'ajax', '#commands' => $commands);
    ajax_deliver($page);
  }
  else {
    $output = t("This is some content delivered via a page load.");
    return $output;
  }
}

function pharmtrac_d3_init() {
  drupal_add_css(drupal_get_path('module', 'pharmtrac') . '/css/colorbrewer.css');
  drupal_add_js(drupal_get_path('module', 'pharmtrac') . '/js/d3.v3.min.js');
  drupal_add_js(drupal_get_path('module', 'pharmtrac') . '/js/debug.js');
  drupal_add_js(array('relativePath' => $_GET['q']), 'setting');
}

function pharmtrac_d3() {
  pharmtrac_d3_init();
  
  // add sample data
  $a = array(
    array(
      'status' => 'Good Service',
      'name' => 'ACE',
      'count' => 300,
    ),
    array(
      'status' => 'Planned Work',
      'name' => 'JZ',
      'count' => 150,
    ),
    array(
      'status' => 'Good Service',
      'name' => 'Pharmtrac',
      'count' => 225,
    )
  );
  drupal_add_js(array('pharmtrac' => array('data' => $a)), 'setting');
  $output = '<div id="d3-graph"></div>';
  
  // $b = array();
  // foreach ($a as $value) {
  //   $b[] = array(
  //     'data' => $value,
  //     'class' => array('d3-row')
  //   );
  // }
  // dprint_r($b); die;
  // create the table
  $output .=  theme('table', array(
    'header' => array(
      'status', 'name', 'count'
    ),
    'rows' => $a,
    'attributes' => array(
      'id' => 'd3-table'
    )
  ));

  return $output;
}

function pharmtrac_d3_enter() {
  pharmtrac_d3_init();
}