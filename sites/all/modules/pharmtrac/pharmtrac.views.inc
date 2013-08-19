<?php
/**
 * @file
 * pharmtrac.views.inc
 */

/**
 * Implets hook_views_data_alter() 
 */
function pharmtrac_views_data_alter(&$data) {
  $data['field_data_body']['body']['field']['add fields to query'] = 1;
  // device 
  $data['field_data_field_device_status']['field_device_status']['field']['add fields to query'] = 1;
  // location
  $data['field_data_field_location_shortname']['field_location_shortname']['field']['add fields to query'] = 1;
  $data['field_data_field_location_longname']['field_location_longname']['field']['add fields to query'] = 1;
  // $data['field_data_field_location_properties']['field_location_properties']['field']['add fields to query'] = 1;
  // $data['field_data_field_location_rooms']['field_location_rooms']['field']['add fields to query'] = 1;
 
  // med
  $data['field_data_field_med_shortname']['field_med_shortname']['field']['add fields to query'] = 1;
  // $data['field_data_field_med_medfeature']['field_med_medfeature']['field']['add fields to query'] = 1;
  
  // quantity
  $data['field_data_field_quantity_quantity']['field_quantity_quantity']['field']['add fields to query'] = 1;
  $data['field_data_field_quantity_unit']['field_quantity_unit']['field']['add fields to query'] = 1;
  // order
  $data['field_data_field_patient_lastname']['field_patient_lastname']['field']['add fields to query'] = 1;
  $data['field_data_field_patient_firstname']['field_patient_firstname']['field']['add fields to query'] = 1;
  $data['field_data_field_order_patientbed']['field_order_patientbed']['field']['add fields to query'] = 1;
  $data['field_data_field_order_patientroom']['field_order_patientroom']['field']['add fields to query'] = 1;
  $data['field_data_field_order_priority']['field_order_priority']['field']['add fields to query'] = 1;
  // dispense
  $data['field_data_field_dispense_duets']['field_dispense_duets']['field']['add fields to query'] = 1;
  $data['field_data_field_dispense_preparelocation']['field_dispense_preparelocation']['field']['add fields to query'] = 1;
  $data['field_data_field_dispense_order']['field_dispense_order']['field']['add fields to query'] = 1;
  
  // user
  $data['field_data_field_user_barcode']['field_user_barcode']['field']['add fields to query'] = 1;
  $data['field_data_field_user_firstname']['field_user_firstname']['field']['add fields to query'] = 1;
  $data['field_data_field_user_lastname']['field_user_lastname']['field']['add fields to query'] = 1;
  $data['field_data_field_user_location']['field_user_location']['field']['add fields to query'] = 1;
  $data['field_data_field_user_mobilerole']['field_user_mobilerole']['field']['add fields to query'] = 1;
  
  
  // patient
  $data['field_data_field_patient_firstname']['field_patient_firstname']['field']['add fields to query'] = 1;
  $data['field_data_field_patient_lastname']['field_patient_lastname']['field']['add fields to query'] = 1;
  $data['field_data_field_patient_dob']['field_patient_dob']['field']['add fields to query'] = 1;
  $data['field_data_field_patient_sex']['field_patient_sex']['field']['add fields to query'] = 1;
  // request
  $data['field_data_field_request_requesttype']['field_request_requesttype']['field']['add fields to query'] = 1;
  $data['field_data_field_request_picture']['field_request_picture']['field']['add fields to query'] = 1;
  $data['field_data_field_request_dispense']['field_request_dispense']['field']['add fields to query'] = 1;
  $data['field_data_field_request_device']['field_request_device']['field']['add fields to query'] = 1;
  //wallboard
  // $data['field_data_field_wallboard_requesttype']['field_wallboard_requesttype']['field']['add fields to query'] = 1;
  $data['field_data_field_wallboard_options']['field_wallboard_options']['field']['add fields to query'] = 1;
  //shift
  $data['field_data_field_shift_timerange']['field_shift_timerange']['field']['add fields to query'] = 1;
}

/**
 * Implements hook_views_query_alter().
 */
// function pharmtrac_views_query_alter(&$view, &$query) {
// }

/**
 * Implements hook_views_pre_execute().
 */
// function pharmtrac_views_pre_execute(&$view) {
// }

/**
 * Implements hook_views_data().
 */
// function pharmtrac_views_data() {
//   // ----------------------------------------------------------------
//   // node type table
// 
//   $data['node_type']['table']['group'] = t('Content Type');
// 
//   // Advertise this table as a possible base table
//   $data['node_type']['table']['base'] = array(
//     'field' => 'type',
//     'title' => t('Content Type'),
//     'weight' => -10,
//     'defaults' => array(
//       'field' => 'type',
//     ),
//   );
// 
//   // type
//   $data['node_type']['type'] = array(
//     'title' => t('Type'),
//     'help' => t('The machine-readable name of this type.'), 
//     'field' => array(
//       'handler' => 'views_handler_field',
//       'click sortable' => TRUE,
//     ),
//     'sort' => array(
//       'handler' => 'views_handler_sort',
//     ),
//     'filter' => array(
//       'handler' => 'views_handler_filter',
//     ),
//     'argument' => array(
//       'handler' => 'views_handler_argument',
//     ),
//   );
// 
//   // name
//   $data['node_type']['name'] = array(
//     'title' => t('Name'),
//     'help' => t('The human-readable name of this type.'),
// 
//     'field' => array(
//       'handler' => 'views_handler_field',
//       'click sortable' => TRUE,
//      ),
//      'filter' => array(
//       'handler' => 'views_handler_filter_string',
//      ),
//      'sort' => array(
//       'handler' => 'views_handler_sort',
//      ),
//      'argument' => array(
//       'handler' => 'views_handler_argument_string',
//      ),
//   );
// 
//   // description
//   $data['node_type']['description'] = array(
//     'title' => t('Description'),
//     'help' => t('A brief description of this type.'),
// 
//     'field' => array(
//       'handler' => 'views_handler_field',
//       'click sortable' => TRUE,
//      ),
//      'filter' => array(
//       'handler' => 'views_handler_filter_string',
//      ),
//      'sort' => array(
//       'handler' => 'views_handler_sort',
//      ),
//      'argument' => array(
//       'handler' => 'views_handler_argument_string',
//      ),
//   );
// 
//   // help
//   $data['node_type']['help'] = array(
//     'title' => t('Help'),
//     'help' => t('Help information shown to the user when creating a node of this type.'),
// 
//     'field' => array(
//       'handler' => 'views_handler_field',
//       'click sortable' => TRUE,
//      ),
//      'filter' => array(
//       'handler' => 'views_handler_filter_string',
//      ),
//      'sort' => array(
//       'handler' => 'views_handler_sort',
//      ),
//      'argument' => array(
//       'handler' => 'views_handler_argument_string',
//      ),
//   );
//   
//   // has_title
//   $data['node_type']['has_title'] = array(
//     'title' => t('Has Title'),
//     'help' => t('Boolean indicating whether this type uses the node title field.'),
//     'field' => array(
//       'handler' => 'views_handler_field_boolean',
//       'click sortable' => TRUE,
//       'output formats' => array(
//         'promoted-notpromoted' => array(t('Yes'), t('No')),
//       ),
//     ),
//     'filter' => array(
//       'handler' => 'views_handler_filter_boolean_operator',
//       'label' => t('Defined in module'),
//       'type' => 'yes-no',
//     ),
//     'sort' => array(
//       'handler' => 'views_handler_sort',
//     ),
//   );
// 
//   // title_label
//   $data['node_type']['title_label'] = array(
//     'title' => t('Title Label'),
//     'help' => t('The label displaed for the title field on the edit form.'),
// 
//     'field' => array(
//       'handler' => 'views_handler_field',
//       'click sortable' => TRUE,
//      ),
//      'filter' => array(
//       'handler' => 'views_handler_filter_string',
//      ),
//      'sort' => array(
//       'handler' => 'views_handler_sort',
//      ),
//      'argument' => array(
//       'handler' => 'views_handler_argument_string',
//      ),
//   );
//   
//   // module
//   $data['node_type']['module'] = array(
//     'title' => t('Module'),
//     'help' => t('The module defining this node type.'),
// 
//     'field' => array(
//       'handler' => 'views_handler_field',
//       'click sortable' => TRUE,
//      ),
//      'filter' => array(
//       'handler' => 'views_handler_filter_string',
//      ),
//      'sort' => array(
//       'handler' => 'views_handler_sort',
//      ),
//      'argument' => array(
//       'handler' => 'views_handler_argument_string',
//      ),
//   );
// 
//   // custom - stored in module
//   $data['node_type']['custom'] = array(
//     'title' => t('Stored in database'),
//     'help' => t('A boolean indicating whether this type is defined by a module (FALSE) or by a user via Add content type (TRUE).'),
//     'field' => array(
//       'handler' => 'views_handler_field_boolean',
//       'click sortable' => TRUE,
//       'output formats' => array(
//         'promoted-notpromoted' => array(t('Yes'), t('No')),
//       ),
//     ),
//     'filter' => array(
//       'handler' => 'views_handler_filter_boolean_operator',
//       'label' => t('Defined in module'),
//       'type' => 'yes-no',
//     ),
//     'sort' => array(
//       'handler' => 'views_handler_sort',
//     ),
//   );
//   
//   
//   
//   return $data;
// }
