<?php
/**
 * @file
 * Code for the Pharmtrac admin feature.
 */

 /**
  * Variables
  */
function pharmtrac_variable_info() {
  $variables['pharmtrac_user_barcode_validation'] = array(
    'title' => t('User validation'),
    'description' => t('Specify the user barcode validation string'),
    'default' => '123',
    'type' => 'string',
  );
  $variables['pharmtrac_location_barcode_validation'] = array(
    'title' => t('Location validation'),
    'description' => t('Specify the location barcode validation string'),
    'default' => '',
    'type' => 'string',
  );
  $variables['pharmtrac_dispense_barcode_validation'] = array(
    'title' => t('Dispense validation'),
    'description' => t('Specify the dispense barcode validation string'),
    'default' => '',
    'type' => 'string',
  );
  return $variables;
}

/**
 * Menu iitems
 */
function pharmtrac_admin_menu() {
  $items = array();
  
  $items['settings'] = array(
    'title' => 'General',
    'description' => 'General application settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('pharmtrac_applicationsettings_form'),
    'access callback' => 'user_access',
    'access arguments' => array('administer pharmtrac'),
    'type' => MENU_CALLBACK,
  );
  $items['settings/general'] = array(
    'title' => 'General',
    'description' => 'General application settings',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 1
  );
  $items['settings/workflow'] = array(
    'title' => 'Workflow',
    'description' => 'Workflow settings for requests',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('pharmtrac_workflowsettings_form'),
    'access callback' => 'user_access',
    'access arguments' => array('administer pharmtrac'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2
  );
  $items['settings/wallboard'] = array(
    'title' => 'Wallboard',
    'description' => 'Wallboard settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('pharmtrac_wallboardsettings_form'),
    'access callback' => 'user_access',
    'access arguments' => array('administer pharmtrac'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 3
  );  
  

  return $items;
}

/**
 * General setting form page
 */
function pharmtrac_applicationsettings_form($form, &$form_state) {
  $options = array('No','Yes');
  $form['plusdelta_application_key'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('plusdelta_application_key', 'plusdelta123'),
    '#title' => t('Application Key'),
    '#description' => t('This field sets the application setting.'),
    '#size' => 40,
    '#maxlength' => 40,
    '#required' => TRUE,
  );
  $form['plusdelta_require_authentication'] = array(
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => variable_get('plusdelta_require_authentication', 0),
    '#title' => t('Require Password Authentication ?'),
    '#description' => t('When enabled, requires users to provide their portal password after scanning their badge to authenticate on a handheld device.'),
  );
  $form['plusdelta_api_version'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('plusdelta_api_version', '2.0'),
    '#title' => t('Pharmtrac API version'),
    '#description' => t('The version number of the current support pharmtrac api.'),
    '#size' => 10,
    '#maxlength' =>10,
  );
  $form['plusdelta_license_total'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('plusdelta_license_total', 10),
    '#title' => t('Total number of licenses'),
    '#description' => t('This will reject use of this application if number of licenses associated with device runs out.'),
    '#size' => 10,
    '#maxlength' =>10,
  );
  $form['plusdelta_barcode_validation'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('plusdelta_barcode_validation', '^(.*)$ 78'),
    '#title' => t('Order barcode validation pattern'),
    '#description' => t('This pattern will be used as a regular expression to validate the scanned barcode before creating a new order. A subgroup identifying the order id must be specified for extraction purposes.'),
    '#size' => 40,
    '#maxlength' => 40,
  );
  $form['pharmtrac_user_barcode_validation'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get_value('pharmtrac_user_barcode_validation'),
    '#title' => t('User barcode validation pattern'),
    '#description' => t('This pattern will be used as a regular expression to validate the scanned barcode before accepting user.'),
    '#size' => 40,
    '#maxlength' => 40,
  );
  $form['pharmtrac_location_barcode_validation'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get_value('pharmtrac_location_barcode_validation'),
    '#title' => t('Location barcode validation pattern'),
    '#description' => t('This pattern will be used as a regular expression to validate the scanned barcode before accepting location.'),
    '#size' => 40,
    '#maxlength' => 40,
  );
  $form['pharmtrac_dispense_barcode_validation'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get_value('pharmtrac_dispense_barcode_validation'),
    '#title' => t('Dispense barcode validation pattern'),
    '#description' => t('This pattern will be used as a regular expression to validate the scanned barcode before accepting dispense.'),
    '#size' => 40,
    '#maxlength' => 40,
  );
  
  $form['plusdelta_api_return_err'] = array(
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => variable_get('plusdelta_api_return_err', 0),
    '#title' => t('Enable 400 return status on API error ?'),
    '#description' => t('When enabled, the return message will be replaced with 400 status error.'),
  );
  $form['plusdelta_mobile_idle'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('plusdelta_mobile_idle', 10),
    '#title' => t('Mobile idle (mins)'),
    '#description' => t('This field sets the duration between mobile device goes idle.'),
    '#size' => 40,
    '#maxlength' => 40,
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );
  return $form;
}

/**
 * Workflow setting form page
 */
function pharmtrac_workflowsettings_form($form, &$form_state) {
  module_load_include('inc', 'pharmtrac', 'pharmtrac.api');
  $requests = plusdelta_api_getRequestTypeArray();
  $form['plusdelta_request_sequence'] = array(
    '#type' => 'multiselect',
    '#default_value' => variable_get('plusdelta_request_sequence', array()),
    '#title' => t('Default order of the request sequence'),
    '#description' => t('This order sequence will be used as default order when meds entered the system. To overwrite this order, use med edit page to change it.'),
    '#options' => $requests,
    '#multiple' => true,
    '#attributes' => array('size' => 10),
  );
  $form['plusdelta_reject_nextrequest'] = array(
    '#type' => 'select',
    '#default_value' => variable_get('plusdelta_reject_nextrequest', ''),
    '#title' => t('Next request type after reject'),
    '#description' => t('The next request type that the dispense is expecting after reject is issued. If not specified, the next request type after reject is set as the second request type in the request sequnece list.'),
    '#options' => drupal_map_assoc(array_values($requests)),
  );
  $sortOptions = array('Sort dispenses by Due date','Sort dispenses by Priority level, Due date');  
  $form['plusdelta_dispense_sortorder'] = array(
    '#type' => 'radios',
    '#options' => $sortOptions,
    '#default_value' => variable_get('plusdelta_dispense_sortorder',0),
    '#title' => t('Dispense sort order'),
    '#description' => t('This determines if dispenses sort by due date only or priority level then due date.'),
  );
  $specialOptions = array(t('Tube'), t('Refrigerator'), t('Dropoff Location'), t('Allow to transfer'), t('Dispense creation'), t('Hand delivery'), t('NDC'));
  $form['plusdelta_special_rules'] = array(
    '#type' => 'checkboxes',
    '#default_value' => variable_get('plusdelta_special_rules', array()),
    '#title' => t('Allow special rules'),
    '#options' => drupal_map_assoc($specialOptions),    
    '#description' => t('This allows for special rules to be applied.'),
  );
  $form['plusdelta_autoclosed_time'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('plusdelta_autoclosed_time', 30),
    '#title' => t('Dispense time to elapse before auto closing dispense (in minutes)'),
    '#description' => t('This field sets the amount of time to expire before dispenses get auto-closed.'),
    '#size' => 5,
  );
  $form['plusdelta_ontime_offset_time'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('plusdelta_ontime_offset_time', 2),
    '#title' => t('Ontime offset (in minutes)'),
    '#description' => t('This offset will be used when determining of orders are ontime.'),
    '#size' => 4,
  );
  $form['plusdelta_default_order_id'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('plusdelta_default_order_id', 0),
    '#title' => t('Default order node id'),
    '#description' => t('Default order template to use when order information is missing from print stream'),
    '#size' => 4,
  );
  $form['plusdelta_default_dispense_id'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('plusdelta_default_dispense_id', 0),
    '#title' => t('Default dispense node id'),
    '#description' => t('Default dispense template to use when dispense information is missing from request'),
    '#size' => 4,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );
  return $form;
}

/**
 * Wallboard setting page form
 */
function pharmtrac_wallboardsettings_form($form, &$form_state) {
  module_load_include('inc', 'pharmtrac', 'pharmtrac.api');  
  $form['plusdelta_wallboard_refreshrate'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('plusdelta_wallboard_refreshrate',5),
    '#title' => t('Auto refresh rate (s)'),
    '#description' => t('This field sets rate of wallboard auto refresh, meansured in seconds.'),
    '#size' => 5,
  );
  
  $form['plusdelta_wallboard_duetime_offset'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('plusdelta_wallboard_duetime_offset',30),
    '#title' => t('Due time offset (in minutes)'),
    '#description' => t('This offset will be used when determining if dispense is due.'),
    '#size' => 5,
  );

  //button color defaults
  //example line - ( [print] => orange ) 
  $colors = variable_get('plusdelta_color_settings', array());

  $form['plusdelta_color_settings'] = array(
    '#prefix' => '<div id="plusdelta_color_settings"><b>Queue thresholds and colors</b>',
    '#suffix' => '<small>Settings for wallboard thesholds and colors for each request type supported.</small></div>',
    '#tree' => TRUE,
    '#theme' => 'table',
    // '#header' => array(t('Color settings for request types'), ' '),
    '#header' => array(
      array('data' => 'label'),
      array('data' => 'button'),
      array('data' => 'footer1'),
      array('data' => 'thres1'),
      array('data' => 'footer2'),
      array('data' => 'thres2'),
      array('data' => 'footer3'),
    ),
    '#rows' => array(),
  );
  $color_options = plusdelta_wallboardsettings_colors();
  $row_template = array(
    // label of request
    'label' => array(
      '#type' => 'item',
      // '#title' => $request,
    ),
    // button color
    'button' => array(
      '#type' => 'select',
      '#options' => $color_options,
    ),
    // footer color 1
    'footer1' => array(
      '#type' => 'select',
      '#options' => $color_options,
    ),
    // footer thres 1
    'thres1' => array(
      '#type' => 'textfield',
      '#size' => '3',
    ),
    // footer color 2
    'footer2' => array(
      '#type' => 'select',
      '#options' => $color_options,
    ),
    // footer thres 2
    'thres2' => array(
      '#type' => 'textfield',
      '#size' => '3',
    ),
    // footer color 3
    'footer3' => array(
      '#type' => 'select',
      '#options' => $color_options,
    ),
  );

  $requestArray = plusdelta_api_getRequestTypeArray();
  foreach ($requestArray as $key => $request) {
    $row = $row_template;
    $row['label']['#title'] = $request;
    
    foreach ($form['plusdelta_color_settings']['#header'] as $value) {
      $row[$value['data']]['#default_value'] =  isset($colors[$request][$value['data']])?$colors[$request][$value['data']]:'';
      $rdata[] = array('data' => &$row[$value['data']]);
    }

    $form['plusdelta_color_settings'][$request] = &$row;
    $form['plusdelta_color_settings']['#rows'][] = &$rdata;
    
    unset($row);
    unset($rdata);
  }	
	
  $form['plusdelta_wallboard_column_order'] = array(
    '#type' => 'multiselect',
    '#default_value' => variable_get('plusdelta_wallboard_column_order'),
    '#title' => t('Default order of the columns'),
    '#description' => t('This order sequence will be used to determine the order of the columns on the wallboard.'),
    '#options' => plusdelta_api_getWallboardColumns(),
    '#multiple' => true,
    '#attributes' => array('size' => 10),
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );
  
	return $form;
}

/**
 * General setting form handle
 */
function pharmtrac_applicationsettings_form_submit($form, &$form_state) {
  variable_set('plusdelta_application_key',$form_state['values']['plusdelta_application_key']);
  variable_set('plusdelta_require_authentication',$form_state['values']['plusdelta_require_authentication']);
  variable_set('plusdelta_api_version',$form_state['values']['plusdelta_api_version']);
  variable_set('plusdelta_license_total',$form_state['values']['plusdelta_license_total']);
  variable_set('plusdelta_barcode_validation',$form_state['values']['plusdelta_barcode_validation']);
  variable_set_value('pharmtrac_user_barcode_validation',$form_state['values']['pharmtrac_user_barcode_validation']);
  variable_set_value('pharmtrac_location_barcode_validation',$form_state['values']['pharmtrac_location_barcode_validation']);
  variable_set_value('pharmtrac_dispense_barcode_validation',$form_state['values']['pharmtrac_dispense_barcode_validation']);
  variable_set('plusdelta_api_return_err',$form_state['values']['plusdelta_api_return_err']);
  variable_set('plusdelta_mobile_idle',$form_state['values']['plusdelta_mobile_idle']);
  drupal_set_message(t('Settings have been saved.'));
}

/**
 * Workflow setting form handle
 */
function pharmtrac_workflowsettings_form_submit($form, &$form_state) {
  variable_set('plusdelta_request_sequence',$form_state['values']['plusdelta_request_sequence']);
  variable_set('plusdelta_reject_nextrequest',$form_state['values']['plusdelta_reject_nextrequest']);
  variable_set('plusdelta_dispense_sortorder',$form_state['values']['plusdelta_dispense_sortorder']); 
  variable_set('plusdelta_special_rules',$form_state['values']['plusdelta_special_rules']);
  variable_set('plusdelta_autoclosed_time',$form_state['values']['plusdelta_autoclosed_time']); 
  variable_set('plusdelta_ontime_offset_time',$form_state['values']['plusdelta_ontime_offset_time']);
  variable_set('plusdelta_default_order_id',$form_state['values']['plusdelta_default_order_id']);
  variable_set('plusdelta_default_dispense_id',$form_state['values']['plusdelta_default_dispense_id']);
  drupal_set_message(t('Settings have been saved.'));
}

/**
 * Wallboard setting form handle
 */
function pharmtrac_wallboardsettings_form_submit($form, &$form_state) {
  variable_set('plusdelta_wallboard_refreshrate',$form_state['values']['plusdelta_wallboard_refreshrate']);
  variable_set('plusdelta_wallboard_duetime_offset',$form_state['values']['plusdelta_wallboard_duetime_offset']);
  variable_set('plusdelta_color_settings',$form_state['values']['plusdelta_color_settings']);
  variable_set('plusdelta_wallboard_column_order',$form_state['values']['plusdelta_wallboard_column_order']);
  drupal_set_message(t('The settings have been saved.'));
}



