<?php

/**
 * @file
 * This file serves as base API functionalities.
 */

include_once 'pharmtrac.drupal.inc';

/**
 * Define wallboard color options
 */
function plusdelta_wallboardsettings_colors() {
  return array('red' => t('Red'), 'orange' => t('Orange'), 'grey' => t('Grey'), 'green' => t('Green'), 'blue' => t('Blue'));
}

/**
 * Define wallboard column options
 */
function plusdelta_api_getWallboardColumns() {
  $cols = array(
    'patientid' => 'Patient Barcode',
    'patientName' => 'Patient Name',
    'dispense' => 'Dispense',
    'order' => 'Order',
    'treatment' => 'Treatment',
    'medBarcode' => 'Med',
    'medShortName' => 'Med Name',
    'room' => 'Room',
    // 'dispenseUser' => 'User',
    'requestUser' => 'User',
    'postedDispense' => 'Dispense Timestamp', 
    'postedRequest' => 'Request Timestamp',  
    'dueDispense' => 'Due Timestamp',  
    'stateTime' => 'State Time',
    'totalTime' => 'Total Time',
    'dueTime' => 'Due Time',
    'latestRequest' => 'Last State',
    'nextRequest' => 'State',
    'prepareLocation' => 'Prepare Location',
    'dropoffLocation' => 'Dropoff Location'
  );
  return $cols;
}

/**
 * Get all request type
 * Depends on views: request_types -> dipslay: services_1
 */
function plusdelta_api_getRequestTypeArray() {
  // use service views to pull data
  module_load_include('inc','services_views','services_views.resource');
  $items = services_views_retrieve('request_types', 'services_1');
  $options = array();
  foreach ($items as $value) {
    $options[$value->nid] = $value->title;
  }
  
  return $options;
}

/**
 * Create a message with status, msg, data and category info
 */
function plusdelta_returnMsg($status='', $msg='', $data=array(), $category='') {
  return array('status'=>$status, 'message'=>$msg, 'data'=>$data, 'category'=>$category);
}

/**
* Centralize log for API and HL7
* $result array('message', 'status', 'category')
*/
function plusdelta_api_watchdog($result) {
  $severity= ($result['status']=='failure') ? WATCHDOG_ERROR : WATCHDOG_INFO;
  $category = 'Pharmtrac';
  if (getSet($result['category'])) $category = $category . '.' . $result['category'];
  watchdog($category, $result['message'], array(), $severity, current_path());
}

/**
 * Return of API call
 * support browser and non-browser API mode, default: non-browser
 * For non-browser mode, 
 * depending on if api return 400 option is turned on, 
 * either json message or 400 error will be returned
 */
function plusdelta_api_return($result, $browser = FALSE) {
  plusdelta_api_watchdog($result);
  
  if ($browser) {
    $type = ($result['status'] == 'success')?'status':'error';
    // dsm($result['data']);
    drupal_set_message($result['message'], $type);
    drupal_goto('messages');
  }
  
  $msg =!(($result['status'] == 'failure') & (variable_get('plusdelta_api_return_err')));
  if ($msg) {
    echo drupal_json_output($result);
  } else {
    // drupal_access_denied();
    drupal_add_http_header('Content-Type', 'text/html; charset=utf-8');
    global $language;
    drupal_add_http_header('Content-Language', $language->language);
    $code = isset($result['message'])?$result['message']:'';
    drupal_add_http_header('Status', '400 ' . $code);
    // watchdog('plusdelta', $type, array($result), WATCHDOG_INFO, $_GET['q']);
  }
  
  drupal_exit();
}

/**
 * API [GET][CREATE] Add HL7 message
 * * Add a HL7 message with json formatted string in body, message_type = HL7 in json.
 * * Pre-existing order, patient, med will be updated. 
 * * Example: process
 *
 * @param order_number_orc2_1
 *   Specify order barcode
 * @param patient_acct_num_pid18
 *   Specify patient barcode
 */
function plusdelta_hl7_add_msg($json, $msg_id = null) {
  
  if(!isset($json->order_number_orc2_1)||!isset($json->patient_acct_num_pid18))
    return plusdelta_returnMsg('failure','hl7 missing order or patient id');
  
  // parse json message
//  $json = json_decode($msg);
  // print_r($json); return;
  
  $result = array();
  
  $na ="N/A";
  
  if ($oid = plusdelta_raw_get(array('type' => 'patient', 'title' => $json->patient_acct_num_pid18))) {
    $node = node_load($oid);
  } else {
    $node = new StdClass();
    $node->type = 'patient';
    node_object_prepare($node);
    $node->language = 'und';
  }
  
  // creat a patient
  $node->title = $json->patient_acct_num_pid18;

  $node->field_patient_lastname[$node->language][0]['value'] = getSet($json->patient_last_name_pid5_1);
  $node->field_patient_firstname[$node->language][0]['value'] = getSet($json->patient_first_name_pid5_2);
  $node->field_patient_middlename[$node->language][0]['value'] = getSet($json->patient_first_middle_name_pid5_3);
  $node->field_patient_suffix[$node->language][0]['value'] = getSet($json->patient_suffix_pid5_4);
  $node->field_patient_prefix[$node->language][0]['value'] = getSet($json->patient_prefix_pid5_5);
  $node->field_patient_dob[$node->language][0]['value'] = getSet($json->patient_dob_pid7);
  $node->field_patient_sex[$node->language][0]['value'] = getSet($json->patient_sex_pid8);
  $node->field_patient_race[$node->language][0]['value'] = getSet($json->patient_race_pid10);
  $node->field_patient_address1[$node->language][0]['value'] = getSet($json->patient_address1_pid11_1);
  $node->field_patient_address2[$node->language][0]['value'] = getSet($json->patient_address2_pid11_2);
  $node->field_patient_city[$node->language][0]['value'] = getSet($json->patient_city_pid11_3);
  $node->field_patient_state[$node->language][0]['value'] = getSet($json->patient_state_pid11_4);
  $node->field_patient_zip[$node->language][0]['value'] = getSet($json->patient_zip_pid11_5);
  $node->field_patient_country[$node->language][0]['value'] = getSet($json->patient_country_pid11_6);

  if ($node = node_submit($node)) {
    node_save($node);
    $result['patient_id'] = $node->nid;
    $patient_id = $node->nid;
  } else {
    return plusdelta_returnMsg('failure','hl7 saving patient failed');
  }
  
  $result['patient_id'] = $patient_id;


  // creat an order referencing to patient
  // find new order if any
  if ($oid = plusdelta_raw_get(array('type' => 'order', 'title' => $json->order_number_orc2_1))) {
    $node = node_load($oid);
  } else {
    $node = new StdClass();
    $node->type = 'order';
    node_object_prepare($node);
    $node->language = 'und';
  }

  $node->title = $json->order_number_orc2_1;
  $node->field_order_treatment[$node->language][0]['value'] = $json->order_description_rxe1_8;

  $node->field_order_patient[$node->language][0]['target_id'] = $patient_id;
  if ($msg_id)
    $node->field_order_message[$node->language][0]['target_id'] = $msg_id;

  $node->field_order_messagetype[$node->language][0]['value'] = getSet($json->message_type_msh9);
  $node->field_order_messagectrlid[$node->language][0]['value'] = getSet($json->message_ctlID_msh10);
  $node->field_order_messageverid[$node->language][0]['value'] = getSet($json->message_verID_msh12);
  $node->field_order_patientclass[$node->language][0]['value'] = getSet($json->patient_class_pv2);
  $node->field_order_patientunit[$node->language][0]['value'] = getSet($json->order_patient_unit_pv3_1);
  $node->field_order_patientroom[$node->language][0]['value'] = getSet($json->order_patient_room_pv3_2);
  $node->field_order_patientbed[$node->language][0]['value'] = getSet($json->order_patient_bed_pv3_3);
  $node->field_order_readid[$node->language][0]['value'] = getSet($json->readmission_id_pv13);
  $node->field_order_adsource[$node->language][0]['value'] = getSet($json->admit_source_pv14);
  $node->field_order_visitnum[$node->language][0]['value'] = getSet($json->visit_num_pv19);
  $node->field_order_adts[$node->language][0]['value'] = getSet($json->admit_ts_pv44);

  $node->field_order_control[$node->language][0]['value'] = getSet($json->order_control_orc1);
  $node->field_order_sys[$node->language][0]['value'] = getSet($json->order_sys_orc2_2);
  $node->field_order_enteredts[$node->language][0]['value'] = getSet($json->order_entered_ts_orc9);

  $node->field_order_enterid[$node->language][0]['value'] = getSet($json->enter_staff_id_orc10_1);
  $node->field_order_enterlastname[$node->language][0]['value'] = getSet($json->enter_staff_last_name_orc10_2);
  $node->field_order_enterfirstname[$node->language][0]['value'] = getSet($json->enter_staff_first_name_orc10_3);

  $node->field_order_verid[$node->language][0]['value'] = getSet($json->verify_staff_id_orc11_1);
  $node->field_order_verlastname[$node->language][0]['value'] = getSet($json->verify_staff_last_name_orc11_2);
  $node->field_order_verfirstname[$node->language][0]['value'] = getSet($json->verify_staff_first_name_orc11_3);

  $node->field_order_orderbyid[$node->language][0]['value'] = getSet($json->orderby_staff_id_orc12_1);
  $node->field_order_orderbylastname[$node->language][0]['value'] = getSet($json->orderby_staff_last_name_orc12_2);
  $node->field_order_orderbyfirstname[$node->language][0]['value'] = getSet($json->orderby_staff_first_name_orc12_3);

  $node->field_order_orderorg[$node->language][0]['value'] = getSet($json->ordering_org_orc21);
  $node->field_order_qty[$node->language][0]['value'] = getSet($json->order_qty_rxe1_1);
  $node->field_order_interval[$node->language][0]['value'] = getSet($json->order_interval_rxe1_2);
  $node->field_order_duration[$node->language][0]['value'] = getSet($json->order_duration_rxe1_3);
  $node->field_order_startts[$node->language][0]['value'] = getSet($json->order_start_ts_rxe1_4);
  $node->field_order_endts[$node->language][0]['value'] = getSet($json->order_end_ts_rxe1_5);
  $node->field_order_priority[$node->language][0]['value'] = getSet($json->order_priority_rxe1_6);
  $node->field_order_condition[$node->language][0]['value'] = getSet($json->order_condition_rxe1_7);
  $node->field_order_secondary[$node->language][0]['value'] = getSet($json->order_secondary_rxe1_9);
  $node->field_order_seq[$node->language][0]['value'] = getSet($json->order_seq_rxe1_10);
  $node->field_order_amt[$node->language][0]['value'] = getSet($json->order_amt_rxe3);
  $node->field_order_giveamt[$node->language][0]['value'] = getSet($json->order_give_amt_rxe5_1);
  $node->field_order_givelabel[$node->language][0]['value'] = getSet($json->order_give_label_rxe5_2);
  $node->field_order_admininstruct[$node->language][0]['value'] = getSet($json->administer_instr_rxe7);

  $node->field_order_dispamt[$node->language][0]['value'] = getSet($json->order_disp_amount_rxe10);
  $node->field_order_dispunit[$node->language][0]['value'] = getSet($json->order_disp_unit_rxe11);
  $node->field_order_verifyid[$node->language][0]['value'] = getSet($json->order_verify_id_rxe14_1);
  $node->field_order_verifylastname[$node->language][0]['value'] = getSet($json->order_verify_last_name_rxe14_2);
  $node->field_order_verifyfirstname[$node->language][0]['value'] = getSet($json->order_verify_first_name_rxe14_3);

  $node->field_order_dispinstruct[$node->language][0]['value'] = getSet($json->rx_disp_instructions_rxe21);

  $node->field_order_ivgiverateamount[$node->language][0]['value'] = getSet($json->iv_give_rate_amt_rxe23);
  $node->field_order_ivgiverateunits[$node->language][0]['value'] = getSet($json->iv_give_rate_units_rxe24);

  $node->field_order_disptype[$node->language][0]['value'] = getSet($json->dispense_type_rxe32);

  $node->field_order_routecode[$node->language][0]['value'] = getSet($json->route_rxr1_1);
  $node->field_order_routedescrip[$node->language][0]['value'] = getSet($json->route_description_rxr1_2);

  $node->field_order_interfacemessageid[$node->language][0]['value'] = getSet($json->interface_message_id);
  $node->field_order_interfacemessagegets[$node->language][0]['value'] = getSet($json->interface_message_ts);

  $node->field_order_pdorderid[$node->language][0]['value'] = getSet($json->pd_order_id);
  $node->field_order_pdpatientid[$node->language][0]['value'] = getSet($json->pd_patient_id);
  $node->field_order_pdorderts[$node->language][0]['value'] = getSet($json->pd_order_ts);
  $node->field_order_pdmedid[$node->language][0]['value'] = getSet($json->pd_med_id);
  
  $pop = (array)$json->meds;
  $meds = array();
  foreach($pop as $key => $value){
    $num = plusdelta_hl7_add_med($value);
    $node->field_order_meds[$node->language][$key]['target_id'] = $num['data']['med_id'];
    // save med quantity info
    $meds[] = array(
      'id' => $num['data']['med_id'],
      'quantity' => getSet($value->med_amt),
      'unit' => getSet($value->med_amt_units)
    );
  }
  
  if ($node = node_submit($node)) {
    node_save($node);
    $result['order_id'] = (int)$node->nid;
    // dsm($node);
    // update med quantity with order id
    plusdelta_hl7_add_med_quantity($meds,$node->nid);
  } else 
    return plusdelta_returnMsg('failure','save hl7 failed');
  
  
  return plusdelta_returnMsg('success','hl7 added succesfully', $result);
}

/**
 * API [GET][CREATE] Add print stream
 * * Add a print stream with json formatted string in body, message_type = PS in json.
 * * Example: process
 *
 * @param order_number_orc2_1
 *   Specify order barcode
 * @param disp_location_ps
 *   Specify location barcode
 * @param disp_roomname
 *   Specify room name
 */
function plusdelta_hl7_add_stream($json, $msg_id = null) {
  if(!isset($json->order_number_orc2_1)||(!isset($json->disp_location_ps))||(!isset($json->disp_roomname)))
    return plusdelta_returnMsg('failure','print stream missing order, location or room barcode');
  
  // make sure dispense is unique
  if (getEntityValue(array('type' => 'dispense', 'title' => getSet($json->disp_barcode_id_ps))))
    return plusdelta_returnMsg('failure','print stream barcode not unique');
  
  $node = new StdClass();
  $node->type = 'dispense';
  node_object_prepare($node);
  $node->language = 'und';
  
  //connect to order
  if($nn = plusdelta_api_getNodeByTitle("order", $json->order_number_orc2_1)){
    // make sure the order is not discontinued
    if ($nn->field_order_control['und'][0]['value'] == 'DC') return;
  }
  else {
    // requires a default order and patient
    if ($nn = node_load(variable_get('plusdelta_default_order_id'))) {
      unset($nn->nid);
      unset($nn->vid);
      $nn->title = $json->order_number_orc2_1;
      $nn->field_order_control['und'][0]['value'] = 'MS';
      node_save($nn);
    } else 
      return plusdelta_returnMsg('failure','print stream wrong order barcode');
  }
  $node->field_dispense_order[$node->language][0]['nid'] = $nn->nid;

  // add location
  if($nn = plusdelta_api_getNodeByTitle("location", $json->disp_location_ps)){
    $node->field_dispense_location[$node->language][0]['target_id'] = $nn->nid;
  }
  else{
    $node->field_dispense_location[$node->language][0]['target_id'] = plusdelta_api_createLocation($json)->nid;
  }

  // add next request type
  $request_type = isset($json->disp_next_request)?$json->disp_next_request:'print';
  $type = plusdelta_api_getNodeByTitle('requesttype',$request_type);
  if ($type) {
    $node->field_dispense_nextrequesttype[$node->language][0]['target_id'] = $type->nid;
  }

  // add room
  if($nn = plusdelta_api_getNodeByTitle("room", $json->disp_roomname)){
    $node->field_dispense_room[$node->language][0]['target_id'] = $nn->nid;
  }
  else{
    $node->field_dispense_room[$node->language][0]['target_id'] = plusdelta_api_createRoom($json)->nid;
  }
  
  // add priority
  if (!empty($json->disp_priority_level)) {
    $props = array('type' => 'priority_level', 'title' => $json->disp_priority_level);
    if ($nn = getEntityValue($props)){
      $node->field_dispense_prioritylevel[$node->language][0]['target_id'] = $nn;
    }
    else{
      $priority = createEntity($props);
      $node->field_dispense_prioritylevel[$node->language][0]['target_id'] = $priority->nid;
    }
  }
  
  $node->title = getSet($json->disp_barcode_id_ps);
  $node->field_dispense_patientid[$node->language][0]['value'] = getSet($json->patient_id_ps);
  $node->field_dispense_patientunit[$node->language][0]['value'] = getSet($json->patient_unit_ps);
  $node->field_dispense_patientroom[$node->language][0]['value'] = getSet($json->patient_room_ps);
  $node->field_dispense_action[$node->language][0]['value'] = getSet($json->disp_action_ps);
  $node->field_dispense_receivets[$node->language][0]['value'] = getSet($json->disp_receive_ts_ps);
  $node->field_dispense_duets[$node->language][0]['value'] = getSet($json->disp_due_ts_ps);

  if ($node = node_submit($node)) {
    node_save($node);
    plusdelta_raw_updateDispense($node->nid);
    $result['dispense_id'] = (int)$node->nid;
  }
  
  return plusdelta_returnMsg('success','print stream added successfully.', $result);
}

/**
 * API [GET][CREATE] Add request
 * * Add a request with json formatted string in body, message_type = RQ in json.
 * * Example: process
 *
 * @param disp_barcode_id_ps
 *   Specify dispense barcode
 * @param request_user
 *   Specify user barcode
 * @param request_location
 *   Specify location barcode
 * @param request_type
 *   Specify request type string
 * @param request_device
 *   [OPTIONAL] Specify device barcode
 * @param note
 *   [OPTIONAL] Specify request note
 */
function plusdelta_hl7_add_request($json, $msg_id = NULL) {
  if(!isset($json->disp_barcode_id_ps)||
    !isset($json->request_user)||
    !isset($json->request_location)||
    !isset($json->request_type) ||
    !isset($json->request_device)) 
    return plusdelta_returnMsg('failure','request missing barcode');
  
  $g = array(
    'override' => getSet($json->override),
    'barcode' => $json->disp_barcode_id_ps,
    'user' => $json->request_user,
    'location' => $json->request_location,
    'type' => $json->request_type,
    'device'=> $json->request_device,
    'note' => getSet($json->note),
    'user2' => getSet($json->request_delivery_user),
    
    'ndc_override' => 0,
    'ndc' => getSet($json->ndc),
    
    'bundle' => getSet($json->is_bundle)
  );
  
  return plusdelta_api_createRequest($g);
  
  // return plusdelta_api_createRequest(
  //   $json->disp_barcode_id_ps, 
  //   $json->request_user, 
  //   $json->request_location, 
  //   $json->request_type, 
  //   getSet($json->request_device), 
  //   getSet($json->note), 
  //   getSet($json->request_delivery_user),
  //   getSet($json->ndc)
  // );
}

/**
 * Add med 
 */
function plusdelta_hl7_add_med($json, $msg_id = null){
  $result = array();
  if ($oid = plusdelta_raw_get(array('type' => 'med', 'title' => $json->med_id_RX_CE2_1))) {
    $node = node_load($oid);
  } else {
    $node = new StdClass();
    $node->type = 'med';
    node_object_prepare($node);
    $node->language = 'und';
  }

  $node->title = $json->med_id_RX_CE2_1;
  $node->body[$node->language][0]['value'] = getSet($json->med_id_RX_CE2_2);
  $node->field_med_codingsystem[$node->language][0]['value'] = getSet($json->med_id_RX_CE2_3);
  $node->field_med_shortname[$node->language][0]['value'] = getSet($json->pd_med_short_name);
  $node->field_med_longname[$node->language][0]['value'] = getSet($json->pd_med_long_name);

  $features = isset($json->med_features)?(array)$json->med_features:array();
  foreach ($features as $key => $value) {
    $type = plusdelta_api_getMedFeatureType($value);
    if (isset($type['data']['fid']))
      $node->field_med_medfeature[$node->language][0]['target_id'] = $type['data']['fid'];
  }

  if ($node = node_submit($node)) {
    node_save($node);
    $result['med_id'] = $node->nid;
  }
  return plusdelta_returnMsg('failure','med added successfully.', $result);
}

/**
 * Add med quantity
 */
function plusdelta_hl7_add_med_quantity($meds, $order_id) {
  foreach ($meds as $med) {
    $node = new StdClass();
    $node->type = 'quantity';
    node_object_prepare($node);
    $node->language = 'und';
    
    $node->title = 'quantity';
    $node->field_quantity_order['und'][0]['target_id'] = $order_id;
    $node->field_quantity_med['und'][0]['target_id'] = $med['id'];
    $node->field_quantity_quantity['und'][0]['value'] = $med['quantity'];
    $node->field_quantity_unit['und'][0]['value'] = $med['unit'];
    if ($node = node_submit($node)) {
      node_save($node);
    }
  }
}

/**
 * Add location
 */
function plusdelta_api_createLocation($json) {
  $node = new StdClass();
  $node->type = 'location';
  node_object_prepare($node);
  $node->language = 'und';
  
  $node->title=getSet($json->disp_location_ps);
  $node->field_location_longname[$node->language][0]['value']=getSet($json->disp_location_longname);
  $node->field_location_shortname[$node->language][0]['value']=getSet($json->disp_location_shortname);
    
  if ($node = node_submit($node)) {
    node_save($node);
  }
  return $node;
}

/**
 * Add room
 */
function plusdelta_api_createRoom($json) {
  $node = new StdClass();
  $node->type = 'room';
  node_object_prepare($node);
  $node->language = 'und';
  
  $node->title=getSet($json->disp_roomname);
  
  if ($node = node_submit($node)) {
    node_save($node);
  }
  return $node;
}

/**
 * Get patients
 */
function plusdelta_api_getPatients($location=null, $name=null) {
  module_load_include('inc','services_views','services_views.resource');
  $items = services_views_retrieve('services_patients_api', 'services_1');

  $res = array();
  foreach ($items as $node) {
    $re = plusdelta_api_getPatient('',$node->nid);
    $res[] = $re['data'];
  }
  
  return plusdelta_returnMsg('success','ok',$res);
}

/**
 * API [GET][INDEX]: Get Dispenses 
 * * Get dispense index, assembled by services views
 * * Order which has been auto-closed are excluded
 * * Return maximum 25 records
 * * Example: api?command=getDispenses&location=8000
 *
 * @param location
 *   Specify location barcode which the dispense can be delivered to.
 *   The location has to match the room of this dispense destination. [Optional] 
 * @param user
 *   Specify user barcode who issued the last request. [Optional] 
 * @param request_location
 *   Specify location barcode where last request was issued. [Optional] 
 * @param order
 *   Specify order barcode of the dispense. [Optional] 
 * @param request
 *   Specify future request type which the dispense is going to. [Optional] 
 * @param request2
 *   Specify extra future request type which the dispense is going to. [Optional] 
 * @param currequest
 *   Specify request type which the dispense has just finished. [Optional] 
 * @param currequest2
 *   Specify extra request type which the dispense has just finished. [Optional] 
 */
function plusdelta_api_getDispenses() {
  module_load_include('inc','services_views','services_views.resource');
  $items = services_views_retrieve('services_dispenses_api', 'services_1', array(), 0, 25);
  $res = array();
  foreach ($items as $node) {
    $re = plusdelta_api_getDispense('',$node->did);
    $res[] = $re['data'];
  }
  
  return plusdelta_returnMsg('success','ok',$res);
}

/**
 * Get shift
 */
function plusdelta_api_getShift($id) {
  $node = node_load($id);
  if ($node) {
    $re['sid'] = $node->nid;
    $re['barcode'] = $node->title;
    // $re['range'] = $timestamp = $node->field_shift_timerange['und'][0]['value'];
    $re['from'] = date('H:i',strtotime($node->field_shift_timerange['und'][0]['value']));
    $re['to'] = date('H:i',strtotime($node->field_shift_timerange['und'][0]['value2']));
    
    $res = plusdelta_returnMsg('success','ok',$re);
  }
  else {
    $res = plusdelta_returnMsg('failure','location does not exist');
  }
  return $res;
}

/**
 * Get shifts
 */
function plusdelta_api_getShifts() {
  $query = new EntityFieldQuery();
  $entities = $query->entityCondition('entity_type', 'node')
        ->propertyCondition('type', 'shift')
        ->execute();
  $result = array();
  if ($entities) {
    $ids = array_keys($entities['node']);
    foreach($ids as $id) {
      $result_location = plusdelta_api_getShift($id);
      $result[] = $result_location['data'];
    }
    $res = plusdelta_returnMsg('success','ok', $result);
  }
  else{
    $res = plusdelta_returnMsg('success','no result');
  }
  return $res;
}

/**
 * Get current shifts
 */
function plusdelta_api_getCurrentShifts($time = null) {
  // 2 - 4,  4 - 2 
  // 9-12, 21-1, 21-23, 0-4, 12-24, 20-0,
  // $time = time();
  if($time == null) {
    $time = time();
  }
  $timetmp = explode(':', date('H:i', $time));
  $time = $timetmp[0]*60 + $timetmp[1];
  
  $shifts = plusdelta_api_getShifts();
  $entities = $shifts['data'];
        
  if($entities) {
    foreach($entities as $shift) {
      $fromtmp = explode(':',$shift['from']);
      $totmp = explode(':',$shift['to']);
      $from = $fromtmp[0]*60 + $fromtmp[1];
      $to = $totmp[0]*60 + $totmp[1];
            
      if ($from < $to) {
        if (($time >= $from) && ($time <= $to)){
          $result[] = $shift;
        }
      } else {
        if (($time >= $from) || ($time <= $to)){
          $result[] = $shift;
        }
      }
    }
    if($result){
      $res = plusdelta_returnMsg('success','ok', $result);
    }
    else {
      $res = plusdelta_returnMsg('success','no result');
    }
  }
  else{
    $res = plusdelta_returnMsg('success','no result');
  }
  return $res;
}

/**
 * Get priority levels
 */
function plusdelta_api_getPriorityLevels() {
  $query = new EntityFieldQuery();
  $entities = $query->entityCondition('entity_type', 'node')
                    ->propertyCondition('type', 'priority_level')
                    ->execute();
  $result = array();
  if ($entities) {
    $ids = array_keys($entities['node']);
    foreach($ids as $id) {
      $node = node_load($id);
      $result[] = array(
        'pid' => $node->nid,
        'barcode' => $node->title,
        'indicator' => getSet($node->field_priority_indicator['und'][0]['value']),
      );
    }
    $res = plusdelta_returnMsg('success','ok', $result);
  }
  else{
    $res = plusdelta_returnMsg('success','no result');
  }
  return $res;
}

/**
 * Get reject types
 */
function plusdelta_api_getRejectTypes() {
  $query = new EntityFieldQuery();
  $entities = $query->entityCondition('entity_type', 'node')
                    ->propertyCondition('type', 'reject_type')
                    ->execute();
  $result = array();
  if ($entities) {
    $ids = array_keys($entities['node']);
    foreach($ids as $id) {
      $node = node_load($id);
      $result[] = array(
        'rid' => $node->nid,
        'barcode' => $node->title,
        'description' => getSet($node->body['und'][0]['value'])
      );
    }
    $res = plusdelta_returnMsg('success','list reject types succesfully', $result);
  }
  else{
    $res = plusdelta_returnMsg('success','no result');
  }
  return $res;
}

/**
 * Get application settings
 * Example: api?command=getAppSettings
 */
function plusdelta_api_getAppSettings(){
  $seqs = array_values(variable_get('plusdelta_request_sequence'));
  foreach ($seqs as &$value) {
    $value = node_load($value)->title;
  }
  $rejs = plusdelta_api_getRejectTypes();
  $pris = plusdelta_api_getPriorityLevels();
  
  $result = array(
    'application_key'     => variable_get('plusdelta_application_key'),
    // 'enable_interface'    => variable_get('plusdelta_enable_interface'),
    'barcode_patterns'    => array(
      'order' => variable_get('plusdelta_barcode_validation'),
      'user' => variable_get('pharmtrac_user_barcode_validation'),
      'location' => variable_get('pharmtrac_location_barcode_validation'),
      'dispense' => variable_get('pharmtrac_dispense_barcode_validation'),
    ),
    'require_password'    => variable_get('plusdelta_require_authentication'),
    'api_version'         => variable_get('plusdelta_api_version'),
    // 'enable_print_action' => variable_get('plusdelta_enable_print_action'),
    'api_return_err'      => variable_get('plusdelta_api_return_err'),
    'request_sequence'    => $seqs,
    // 'use_deviceid'     => variable_get('plusdelta_use_deviceid'),
    'max_licenses'        => variable_get('plusdelta_license_total'),
    'sort_priority_duedate'   => variable_get('plusdelta_dispense_sortorder'),
    'special_rules'       => array_keys(variable_get('plusdelta_special_rules')),
    'mobile_idle'         => variable_get('plusdelta_mobile_idle'),
    'reject_types'        => getMappedArray('barcode', getSet($rejs['data'])),
    'priority_levels'     => getSet($pris['data']),
  );

  // figure out all installed and turned on service
  $services = array('bundle', 'ndc');
  $result['services'] = array();
  foreach ($services as $value) {
    if (module_exists('pharmtrac_' . $value) && 
      variable_get_value('pharmtrac_' . $value . '_enabled')) 
        $result['services'][] = $value;
  }
  
  if (module_exists('pharmtrac_ndc'))
    $result['barcode_patterns']['ndc'] = variable_get_value('pharmtrac_ndc_pattern_validation');

  return plusdelta_returnMsg("success",'app settings information',$result);
}

/**
 * Authenticate by user barcode and password 
 */
function plusdelta_api_barcode_login($barcode, $password) {
  $username = getEntityValue(array('field_user_barcode' => $barcode), 'name', 'user');
  return plusdelta_api_user_login($username, $password);
}
 
/**
 * Drupal authenticate by user name and password
 */
function plusdelta_api_user_login($username, $password) {
  if(user_authenticate($username, $password)) {
    $user_obj = user_load_by_name($username);
    $form_state = array();
    $form_state['uid'] = $user_obj->uid;      
    user_login_submit(array(), $form_state);
    return true;
  }
  return false;
}

/**
 * Drupal authenticate for user id
 */
function plusdelta_api_login_other($uid) {
  global $user;
  if ($account = user_load($uid)) {
    $user->uid = $uid; 
    $user->name = $account->name;
    $user->timezone = $account->timezone;
    user_login_finalize();
    return true;
  }
  return false;
}

/**
 * Drupal de-authenticate
 */
function plusdelta_api_userLogout() {
  global $user;
  // if ($user && $user->name) watchdog('user', 'Session closed for %name.', array('%name' => $user->name));

  module_invoke_all('user_logout', $user);

  session_start();    // add this to make sure session is started, works.
  session_destroy();
}

/**
 * Get user info
 * Example: api?command=getUser&barcode=SE23-231
 */
function plusdelta_api_getUser($barcode, $uid = NULL){
  if (!$uid) {
    $query = new EntityFieldQuery();
    $entities = $query->entityCondition('entity_type', 'user')
                      ->fieldCondition('field_user_barcode', 'value', $barcode)
                      ->execute();
    if ($entities) {
      $ids = array_keys($entities['user']);
      $uid = $ids[0];
    }
  }
  if ($uid) {
    $result = array();
    $user = user_load($uid);
    $result['authorized'] = 'true';
    $result['username'] = $user->name;
    $result['uid'] = $user->uid;
    $result['email'] = $user->mail;
    $result['barcode'] = getSet($user->field_user_barcode['und'][0]['value']);
    $result['firstName'] = getSet($user->field_user_firstname['und'][0]['value']);
    $result['lastName'] = getSet($user->field_user_lastname['und'][0]['value']);
    $result['defaultLocation'] = isset($user->field_user_location['und'][0]['target_id'])?
            node_load($user->field_user_location['und'][0]['target_id'])->title:'';
    $result['mobileRole'] = array();

    if (isset($user->field_user_mobilerole['und'])) {
      $node = node_load($user->field_user_mobilerole['und'][0]['target_id']);
      $result['mobileRole']['title'] = $node->title;
      $result['mobileRole']['features'] = array();
      if (isset($node->field_mobilerole_features['und'])) {
        foreach ($node->field_mobilerole_features['und'] as $value) {
          $result['mobileRole']['features'][] = isset($value)?node_load($value['target_id'])->title:'';
        }
      }
    }

    // if (isset($user->field_user_portalrole['und'])) {
    //   $node = node_load($user->field_user_portalrole['und'][0]['target_id']);
    //   $result['portalRole']['title'] = $node->title;
    //   $result['portalRole']['features'] = array();
    //   if (isset($node->field_portalrole_features['und'])) {
    //     foreach ($node->field_portalrole_features['und'] as $value) {
    //       $result['portalRole']['features'][] = isset($value)?node_load($value['target_id'])->title:'';
    //     }
    //   }
    // }
    
    $res = plusdelta_returnMsg('success','ok',$result);
  } else {
    $res = plusdelta_returnMsg('failure','user does not exist');
  }

  return $res;
}

/**
 * Get locations
 * Example: api?command=getLocations
 */
function plusdelta_api_getLocations($type=NULL, $room=NULL){
  $query = new EntityFieldQuery();
  $entities = array();
  if ((!$type) && (!$room)) {
    $entities = $query->entityCondition('entity_type', 'node')
          ->propertyCondition('type', 'location')
          ->execute();
  } else {
    if ($type) {
      if ($node = plusdelta_api_getNodeByTitle('location_properties',$type)) {
        $entities = $query->entityCondition('entity_type', 'node')
              ->propertyCondition('type', 'location')
              ->fieldCondition('field_location_properties', 'target_id', $node->nid) 
              ->execute();
      }
    } 
    if ($room) {
      if ($node = plusdelta_api_getNodeByTitle('room',$room)) {
        $entities = $query->entityCondition('entity_type', 'node')
              ->propertyCondition('type', 'location')
              ->fieldCondition('field_location_rooms', 'target_id', $node->nid) 
              ->execute();
      }
    }
    
  }
  
  $result = array();
  if ($entities) {
    $ids = array_keys($entities['node']);
    foreach($ids as $id) {
      $result_location = plusdelta_api_getLocation('',$id);
      $result[] = $result_location['data'];
    }
    $res = plusdelta_returnMsg('success','ok',$result);
  }
  else{
    $res = plusdelta_returnMsg('success','no result');
  }
  return $res;
}

/**
 * Get location by location barcode
 * Example: api?command=getLocation&barcode=MC13AE
 */
function plusdelta_api_getLocation($barcode,$nid=null){
  if ($nid)
    $node = node_load($nid);
  else
    $node = plusdelta_api_getNodeByTitle('location',$barcode);
  
  $re = array();
  if ($node) {

    $re['lid'] = $node->nid;
    $re['barcode'] = $node->title;
    $re['shortName'] = isset($node->field_location_shortname['und'])?
                      $node->field_location_shortname['und'][0]['value']:'';
    $re['longName'] = isset($node->field_location_longname['und'][0]['value'])?
                      $node->field_location_longname['und'][0]['value']:'';
    $re['properties'] = array();
    if (isset($node->field_location_properties['und']))
      foreach ($node->field_location_properties['und'] as $value) {
        $re['properties'][] = node_load($value['target_id'])->title;
      }
    $re['rooms'] = array();
    if (isset($node->field_location_rooms['und']))
      foreach ($node->field_location_rooms['und'] as $value) {
        $re['rooms'][] = node_load($value['target_id'])->title;
      }

    $res = plusdelta_returnMsg('success','ok',$re);
  }
  else {
    $res = plusdelta_returnMsg('failure','location does not exist');
  }
  return $res;
}

/**
 * Get patient
 * Example: api?command=getPatiente&barcode=accountID
 */
function plusdelta_api_getPatient($barcode,$nid=null) {
  if ($nid)
    $node = node_load($nid);
  else
    $node = plusdelta_api_getNodeByTitle('patient',$barcode);

  $re = array();
  if ($node) {
    $re['pid'] = $node->nid;
    $re['barcode'] = $node->title;
    $re['firstname'] = isset($node->field_patient_firstname['und'][0]['value'])?$node->field_patient_firstname['und'][0]['value']:'';
    $re['lastname'] = isset($node->field_patient_lastname['und'][0]['value'])?$node->field_patient_lastname['und'][0]['value']:'';
    $re['name'] = (isset($re['lastname'])?$re['lastname'] . ', ':'') . (isset($re['firstname'])?$re['firstname']:'');
    $re['dob'] = isset($node->field_patient_dob['und'])?date('y-m-d',$node->field_patient_dob['und'][0]['value']):'';
    $re['sex'] = isset($node->field_patient_sex['und'][0]['value'])?$node->field_patient_sex['und'][0]['value']:'';
    $re['race'] = isset($node->field_patient_race['und'][0]['value'])?$node->field_patient_race['und'][0]['value']:'';
    $res = plusdelta_returnMsg('success','ok',$re);
  } else {
    $res = plusdelta_returnMsg('failure','patient does not exist'); 
  }
  return $res;
}

/**
 * Get order by order barcode
 * Example: api?command=getOrder&barcode=DISP_BARCODE
 */
function plusdelta_api_getOrder($barcode, $nid = NULL){
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
        ->propertyCondition('type', 'order');
  if ($nid) 
    $query->propertyCondition('nid', $nid);
  else
    $query->propertyCondition('title', $barcode);
  $entities = $query->execute();
  if ($entities) {
    $ids = array_keys($entities['node']);
    $node = node_load(end($ids));
    
    // print_r($node); die;
    
    $result['oid'] = $node->nid;
    $result['barcode'] = $node->title;
    $result['timestamp'] = $node->created;
    $result['treatCommonName'] = getSet($node->field_order_givelabel['und'][0]['value']);
    $result['treatStartStamp'] = getSet($node->field_order_startts['und'][0]['value']);
    $result['treatGiveText'] = getSet($node->field_order_treatment['und'][0]['value']);
    $result['treatQuantity'] = getSet($node->field_order_qty['und'][0]['value']);
    $result['treatInterval'] = getSet($node->field_order_interval['und'][0]['value']);
    $result['locationUnit'] = getSet($node->field_order_patientunit['und'][0]['value']);
    $result['locationRoom'] = getSet($node->field_order_patientroom['und'][0]['value']);
    $result['field_order_orderorg'] = getSet($node->field_order_orderorg['und'][0]['value']);
    $result['field_order_qty'] = getSet($node->field_order_qty['und'][0]['value']);
    $result['field_order_interval'] = getSet($node->field_order_interval['und'][0]['value']);
    $result['field_order_duration'] = getSet($node->field_order_duration['und'][0]['value']);
    $result['field_order_startts'] = getSet($node->field_order_startts['und'][0]['value']);
    $result['field_order_endts'] = getSet($node->field_order_endts['und'][0]['value']);
    $result['field_order_priority'] = getSet($node->field_order_priority['und'][0]['value']);
    $result['field_order_condition'] = getSet($node->field_order_condition['und'][0]['value']);
    $result['field_order_treatment'] = getSet($node->field_order_treatment['und'][0]['value']);
    $result['field_order_secondary'] = getSet($node->field_order_secondary['und'][0]['value']);
    $result['field_order_seq'] = getSet($node->field_order_seq['und'][0]['value']);
    $result['field_order_amt'] = getSet($node->field_order_amt['und'][0]['value']);
    $result['field_order_givelabel'] = getSet($node->field_order_givelabel['und'][0]['value']);
    $result['field_order_admininstruct'] = getSet($node->field_order_admininstruct['und'][0]['value']);
    $result['field_order_dispamt'] = getSet($node->field_order_dispamt['und'][0]['value']);
    $result['field_order_dispunit'] = getSet($node->field_order_dispunit['und'][0]['value']);
    $result['field_order_verifyid'] = getSet($node->field_order_verifyid['und'][0]['value']);
    $result['field_order_verifylastname'] = getSet($node->field_order_verifylastname['und'][0]['value']);
    $result['field_order_verifyfirstname'] = getSet($node->field_order_verifyfirstname['und'][0]['value']);
    $result['field_order_dispinstruct'] = getSet($node->field_order_dispinstruct['und'][0]['value']);
    $result['field_order_ivgiverateamount'] = getSet($node->field_order_ivgiverateamount['und'][0]['value']);
    $result['field_order_ivgiverateunits'] = getSet($node->field_order_ivgiverateunits['und'][0]['value']);
    $result['field_order_disptype'] = getSet($node->field_order_disptype['und'][0]['value']);
    $result['field_order_patientclass'] = getSet($node->field_order_patientclass['und'][0]['value']);
    $result['field_order_patientunit'] = getSet($node->field_order_patientunit['und'][0]['value']);
    $result['field_order_patientroom'] = getSet($node->field_order_patientroom['und'][0]['value']);
    $result['field_order_patientbed'] = getSet($node->field_order_patientbed['und'][0]['value']);
    $result['field_order_readid'] = getSet($node->field_order_readid['und'][0]['value']);
    $result['field_order_adsource'] = getSet($node->field_order_adsource['und'][0]['value']);
    $result['field_order_visitnum'] = getSet($node->field_order_visitnum['und'][0]['value']);
    $result['field_order_adts'] = getSet($node->field_order_adts['und'][0]['value']);
    $result['field_order_messagetype'] = getSet($node->field_order_messagetype['und'][0]['value']);
    $result['field_order_messagectrlid'] = getSet($node->field_order_messagectrlid['und'][0]['value']);
    $result['field_order_messageverid'] = getSet($node->field_order_messageverid['und'][0]['value']);
    $result['field_order_interfacemessageid'] = getSet($node->field_order_interfacemessageid['und'][0]['value']);
    $result['field_order_interfacemessagets'] = getSet($node->field_order_interfacemessagets['und'][0]['value']);
    $result['field_order_control'] = getSet($node->field_order_control['und'][0]['value']);
    $result['field_order_sys'] = getSet($node->field_order_sys['und'][0]['value']);
    $result['field_order_enterid'] = getSet($node->field_order_enterid['und'][0]['value']);
    $result['field_order_enterlastname'] = getSet($node->field_order_enterlastname['und'][0]['value']);
    $result['field_order_enterfirstname'] = getSet($node->field_order_enterfirstname['und'][0]['value']);
    $result['field_order_verid'] = getSet($node->field_order_verid['und'][0]['value']);
    $result['field_order_verlastname'] = getSet($node->field_order_verlastname['und'][0]['value']);
    $result['field_order_verfirstname'] = getSet($node->field_order_verfirstname['und'][0]['value']);
    $result['field_order_orderbyid'] = getSet($node->field_order_orderbyid['und'][0]['value']);
    $result['field_order_orderbylastname'] = getSet($node->field_order_orderbylastname['und'][0]['value']);
    $result['field_order_orderbyfirstname'] = getSet($node->field_order_orderbyfirstname['und'][0]['value']);
    $result['field_order_routecode'] = getSet($node->field_order_routecode['und'][0]['value']);
    $result['field_order_routedescrip'] = getSet($node->field_order_routedescrip['und'][0]['value']);
    $result['field_order_enteredts'] = getSet($node->field_order_enteredts['und'][0]['value']);
    
    $result['tabs'] = array(
      'tab1' => array(
        'title' => 'Message Info',
        'data' => array(
          'field_order_messagetype' => 'Message Type',
          'field_order_messagectrlid' => 'Message Control ID'
        )
      ),
      'tab2' => array(
        'title' => 'Patient Location',
        'data' => array(
          'field_order_patientroom' => 'Patient Room',
          'field_order_patientunit' => 'Patient Unit'
        )
      )
    );
    
    // fetch dispenses
    $meds = array();
    if (isset($node->field_order_meds['und']))
      foreach ($node->field_order_meds['und'] as $value) {
        $meds[] = node_load($value['target_id'])->title;
      }
    $result['meds'] = $meds;
    
    $res = plusdelta_returnMsg('success','ok',$result);
  }
  else {
    $res = plusdelta_returnMsg('failure','order does not exist');
  }
  return $res;
}

/**
 * Get orders
 */
function plusdelta_api_getOrders($patient_id) {
  if (!$patient_id) return plusdelta_returnMsg('failure','patient does not exist');
  
  $query = new EntityFieldQuery();
  $entities = $query->entityCondition('entity_type', 'node')
                    ->propertyCondition('type', 'order')
                    ->fieldCondition('field_order_patient', 'target_id', $patient_id)
                    ->execute();
  $orders = array();
  if ($entities) {
    $ids = array_keys($entities['node']);
    foreach ($ids as $nid) {
      $order_result = plusdelta_api_getOrder('',$nid);
      $orders[] = $order_result['data'];
    }
  }
  
  return plusdelta_returnMsg('success','ok',$orders);
}

/**
 * Get from sequence pattern to current pattern
 *  find out what should be the next sequence
 */
function plusdelta_api_getNextRequestType($seqs,$curr) {
  $result = "";
  
  if (!$curr) return $curr;
  if (!$curr) return $curr;
  if (!$seqs) return $result;
  
  // assume current sequence is in reverse order
  $last = array_search($curr[0],$seqs);
  $total = count($seqs);
  if ($last>=$total) return $result;
  
  return $seqs[$last+1];
}

/**
 * Get dispense by dispense barcode
 * Example: api?command=getDispense&barcode=DISP_BARCODE
 */
function plusdelta_api_getDispense($barcode,$nid=null){
  if ($nid)
    $node = node_load($nid);
  else
    $node = plusdelta_api_getNodeByTitle('dispense',$barcode);

  if ($node) {
    $result['did'] = $node->nid;
    $result['barcode'] = $node->title;
    $result['originateUser'] = plusdelta_user_string($node->uid);
    $result['originateTimestamp'] = $node->created;
    $result['dueTimeStamp'] = $node->field_dispense_duets['und'][0]['value'];
    $result['priorityLevel'] = isset($node->field_dispense_prioritylevel['und'])?
      node_load($node->field_dispense_prioritylevel['und'][0]['target_id'])->title:'';
      
    // order information
    $result['features'] = array();
    $result['meds'] = array();
    if (isset($node->field_dispense_order['und'][0]['nid'])) {
      $order = node_load($node->field_dispense_order['und'][0]['nid']);
      if ($order) {
        $orderTitle = $order->title;
        $result['order'] = $orderTitle;
        $result['patient'] = '';
        $result['patientName'] = '';
        if (isset($order->field_order_patient['und'])) {
          if ($patient = node_load($order->field_order_patient['und'][0]['target_id'])) {
            $result['patient'] = $patient->title;
            $result['patientName'] = $patient->field_patient_lastname['und'][0]['value'] . ', ' .  $patient->field_patient_firstname['und'][0]['value'];
          }
        }

        $result['treatment'] = getSet($order->field_order_treatment['und'][0]['value']);
        $result['features'] = plusdelta_api_getOrderFeatures($orderTitle);
        $result['defaultSequence'] = array_values(plusdelta_api_getOrderSequence($orderTitle));
      
        // meds information
        $result['meds'] = array();
        // collect med features
        if (isset($order->field_order_meds['und'])) {
          foreach ($order->field_order_meds['und'] as $med) {
            $result_med = plusdelta_api_getMed('', $med['target_id']);
            if (isset($result_med['data'])) {
              $result['meds'][] = $result_med['data'];
            }
          }
        }
      }
    }
    
    $result['room'] = isset($node->field_dispense_room['und'][0]['target_id'])?
      node_load($node->field_dispense_room['und'][0]['target_id'])->title:'';

    if (isset($node->field_dispense_preparelocation['und'][0]['target_id']))
      $result['prepare'] = node_load($node->field_dispense_preparelocation['und'][0]['target_id'])->title;
    if (isset($node->field_dispense_dropofflocation['und'][0]['target_id']))
      $result['dropoff'] = node_load($node->field_dispense_dropofflocation['und'][0]['target_id'])->title;
    $result['latestRequest'] = '';  
    if (isset($node->field_dispense_lastrequest['und'][0]['target_id']))
      if ($abc = plusdelta_api_getRequest($node->field_dispense_lastrequest['und'][0]['target_id'])) {
        $result['latestRequest'] = $abc['data'];
      }
    $result['currentSequence'] = isset($node->field_dispense_sequence['und'][0]['value'])?unserialize($node->field_dispense_sequence['und'][0]['value']):'';
    
    $result['nextRequest']['type'] = isset($node->field_dispense_nextrequesttype['und'][0]['target_id'])? node_load($node->field_dispense_nextrequesttype['und'][0]['target_id'])->title:'';
    
    $res = plusdelta_returnMsg('success','ok',$result);
    
  }
  else {
    $res = plusdelta_returnMsg('failure','dispense does not exist');
  }
  return $res;
}

/**
 * Get request type by request type name
 * Example: api?command=getRequestType&barcode=print
 */
function plusdelta_api_getRequestType($barcode) {
  $query = new EntityFieldQuery();
  $entities = $query->entityCondition('entity_type', 'node')
        ->propertyCondition('type', 'requesttype')
        ->propertyCondition('title', $barcode)
        ->execute();
  $result = array();
  if ($entities) {
    $ids = array_keys($entities['node']);
    $node = node_load(end($ids));
    
    $result['tid'] = $node->nid;
    $result['title'] = $node->title;
    $res = plusdelta_returnMsg('success','ok',$result);
  } else {
    $res = plusdelta_returnMsg('failure','request type does not exist');
  }
  return $res;
}

/**
 * Get location property by property type name
 * Example: api?command=getLocationProperty&barcode=print
 */
function plusdelta_api_getLocationProperty($barcode) {
  $node = plusdelta_api_getNodeByTitle('location_properties',$barcode);
  if ($node) {
    $result['type'] = 'location property';
    $result['nid'] = $node->nid;
    $result['title'] = $node->title;
    $res = plusdelta_returnMsg('success','ok',$result);
  } else {
    $res = plusdelta_returnMsg('failure','location property does not exist');
  }
  return $res;
}

/**
 * Get mobile role by role name
 * Example: api?command=getMobileRole&barcode=Provider
 */
function plusdelta_api_getMobileRole($barcode) {
  $query = new EntityFieldQuery();
  $entities = $query->entityCondition('entity_type', 'node')
        ->propertyCondition('type', 'mobilerole')
        ->propertyCondition('title', $barcode)
        ->execute();
  $result = array();
  if ($entities) {
    $ids = array_keys($entities['node']);
    $node = node_load(end($ids));
    
    $result['tid'] = $node->nid;
    $result['title'] = $node->title;
    $result['features'] = array();
    if (isset($node->field_mobilerole_features['und']))
      foreach ($node->field_mobilerole_features['und'] as $value) {
        $result['features'][] = node_load($value['target_id'])->title;
      }
    $res = plusdelta_returnMsg('success','ok',$result);
  } else {
    $res = plusdelta_returnMsg('failure','mobile role does not exist');
  }
  return $res;
}

/**
 * Get med feature type by feature type name
 * Example: api?command=getMedFeatureType&barcode=Do Not Tube
 */
function plusdelta_api_getMedFeatureType($barcode) {
  $query = new EntityFieldQuery();
  $entities = $query->entityCondition('entity_type', 'node')
        ->propertyCondition('type', 'medfeature')
        ->propertyCondition('title', $barcode)
        ->execute();
  $result = array();
  if ($entities) {
    $ids = array_keys($entities['node']);
    $node = node_load(end($ids));
    
    $result['fid'] = $node->nid;
    $result['title'] = $node->title;
    $res = plusdelta_returnMsg('success','ok',$result);
  } else {
    $res = plusdelta_returnMsg('failure','med feature type does not exist');
  }
  return $res;
}

/**
 * Get med by med barcode
 * Example: api?command=getMed&barcode=14795
 */
function plusdelta_api_getMed($barcode, $nid=null) {
  if ($nid)
    $node = node_load($nid);
  else
    $node = plusdelta_api_getNodeByTitle('med',$barcode);
  
  if ($node) {
    $result['fid'] = $node->nid;
    $result['title'] = $node->title;
    //$result['shortName'] = $node->field_med_shortname['und'][0]['value'];
    //$result['longName'] = $node->field_med_longname['und'][0]['value'];
    
    // get med features
    $features = array();
    foreach (drupal_getNodeFieldValues($node,'field_med_medfeature','target_id') as $value) {
      $features[] = node_load($value['target_id'])->title;
    }
    $result['features'] = $features;

    // get med sequence
    $processes = array();
    if (isset($node->field_med_processflow['und']))
      foreach ($node->field_med_processflow['und'] as $value) {
        $processes[] = node_load($value['target_id'])->title;
      }
    $result['processes'] = $processes;
    
    // get med ndc
    if (module_exists('pharmtrac_ndc')) {
      // $result['ndc'] = '';
      // if ($ndc_field = getEntityValue(array('type' => 'ndc', 'title' => $node->title), 'field_ndc_ndc')) 
      //   $result['ndc'] = $ndc_field['und'][0]['value'];
      $result['ndc'] = getSet($node->field_med_ndc['und'][0]['value']);
    }
    
    $res = plusdelta_returnMsg('success','ok',$result);
  } else {
    $res = plusdelta_returnMsg('failure','med does not exist');
  }
  return $res;
}

/**
 * Get room at location
 */
function plusdelta_api_roomAtLocation($room, $location) {
  if (!($location_id = plusdelta_raw_get(array('type' => 'location', 'title' => $location)))) return false;
  
  if (!($room_id = plusdelta_raw_get(array('type' => 'room', 'title' => $room)))) return false;
  
  $query = new EntityFieldQuery();
  $entities = $query->entityCondition('entity_type', 'node')
                    ->propertyCondition('type', 'location')
                    ->propertyCondition('nid', $location_id)
                    ->fieldCondition('field_location_rooms', 'target_id', array($room_id), 'IN')
                    ->execute();
  return (!empty($entities));
}

/**
 * Check to see if the request can be allowed
 * Base check without special rules applied
 */
function plusdelta_api_requestAllowedBase($dispense, $type) {
  //regular request checking logic...
  $requests = $dispense['currentSequence'];

  $sequence = plusdelta_api_getOrderSequence($dispense['order']);

  // get request order array, needs to be override by meds in the future
  if (!$sequence)
    $sequence = plusdelta_api_getRequestTypeArray();

  // add reject type into this sequence if not there
  if (array_search('reject',$sequence) === false) {
    $sequence['-1'] = 'reject';
  }
  $types = array_values($sequence);

  // get the reject sequence order id
  $rejectType = array_search('reject',$types);

  // get the current request sequence order id
  $currType = array_search($type,$types);

  // get the latest request sequence order id
  //if (isset($requests[0])) {
  //  $latestType = array_search($requests[0],$types);
  //} else {
  //  $latestType = '';
  //}
  
  $latestType = isset($requests[0]) ? array_search($requests[0],$types) : '';

  $result['requests'] = $requests;
  $result['sequence'] = $types;
  $result['rejectType'] = $rejectType;
  $result['requestType'] = $currType;
  $result['latestType'] = $latestType;

  // if there's no request, add it as a starting request 
  //if (!$requests) return ($currType === 0);
  if (!isset($requests[0])) return ($currType === 0);
  
  // for latest step is reject
  if ($latestType == $rejectType) {
    // when reject is issued, always fall back to reject next type
    $rejectNextType = 1;
    if ($rt = variable_get('plusdelta_reject_nextrequest')) {
      $rejectNextType = array_search($rt, $types);
    }
    $allowed = ($currType == $rejectNextType);
  } else {
    // either reject is issued, or next step is issued
    $allowed = ($rejectType == $currType) || ($latestType+1 == $currType);
  }

  return $allowed;
}

/**
 * Check to see if the request can be allowed
 *  to add in the current request sequence
 */
// function plusdelta_api_requestAllowed($result_dispense, $result_user, $result_location, $type, $user2, $ndc) {
function plusdelta_api_requestAllowed($g) {
  $array = variable_get('plusdelta_special_rules');
  $type = $g['type']['title'];
  
  // Refrigeration
  if (!empty($array['Refrigerator']) && 
    ($type == 'dropoff') && 
    in_array('Refrigeration', $g['dispense']['features']) && 
    !in_array('refrigerator', $g['location']['properties'])
  ) return 'refrigerator';
  
  // Tube
  if (!empty($array['Tube']) && 
    ($type == 'dropoff') && 
    in_array('Do Not Tube', $g['dispense']['features']) && 
    in_array('tube', $g['location']['properties'])
  ) return 'tube';

  // Dropoff location
  if (!empty($array['Dropoff Location']) && 
    ($type == 'dropoff') && 
    !plusdelta_api_roomAtLocation(
      $g['dispense']['room'], 
      $g['location']['barcode']
    )
  ) return 'dropoff location';

  // Hand delivery
  if (!empty($array['Hand delivery']) && 
    ($type == 'dropoff') && 
    in_array('Hand Deliver', $g['dispense']['features']) && 
    empty($user2)
  ) return 'hand delivery';
  
  // NDC
  if (module_exists('pharmtrac_ndc') &&
    variable_get_value('pharmtrac_ndc_enabled') &&
    !$g['ndc_override'] &&
    in_array($type, pharmtrac_ndc_requesttypes()) &&
    !empty($g['dispense']['meds'])
  ) {
    // only work with one med for now
    $ndc_array = explode(',', $g['ndc']);
    if (count($ndc_array) < 1) return 'ndc-missing';
    
    // go through each med and each ndc
    foreach ($g['dispense']['meds'] as $id => $med) {
      $ndc_check = 0;
      if (empty($med['ndc'])) {
        plusdelta_api_watchdog(array(
          'status' => 'failure',
          'category' => 'ndc', 
          'message' => 'NDC missing for med - ' . $med['title'],
        ));
        return 'ndc-missing';
      }
      $med_ndc_array = explode(',', $med['ndc']);
      foreach ($ndc_array as $nd) {
        if (in_array($nd, $med_ndc_array)) $ndc_check = 1;
      }
      if (!$ndc_check) return 'ndc-mismatch|'.$med['title'];
    }
  } 
  
  // Fallback to the base check
  return plusdelta_api_requestAllowedBase($g['dispense'], $type);
  
  //OR logic...if these fail, thats ok..keep on rolling...
  // if (!empty($array['Tube'])) {
  //   if (plusdelta_api_tubeAllowed($type,$result_dispense,$result_location)) {
  //     $result_type = plusdelta_api_getRequestType('pickup');
  //     $node = plusdelta_raw_createRequest(
  //         $result_dispense['data']['did'],
  //         $result_user['data']['uid'],
  //         $result_location['data']['lid'],
  //         $result_type['data']['tid'],
  //         array('override'=>getVars('override'))
  //     );
  //     //reload dispense to refresh latest request and next request
  //     $result_dispense = plusdelta_api_getDispense($result_dispense['data']['barcode']);    
  //   }   
  // }
  
}

/**
 * Check to see if refrigerator is allowed
 */
function plusdelta_api_refrigeratorAllowed($type,$result_dispense,$result_location) {
  $result = true;

  if ($type != 'pickup') return $result;

  if (in_array("Refrigeration", $result_dispense['data']['features'])) {
    $result = in_array("refrigerator", $result_location['data']['properties']);
  }

  return $result;
}

/**
 * Check  to see if tube is allowed
 */
function plusdelta_api_tubeAllowed($type,$result_dispense,$result_location) {

  if (!isset($result_dispense['data']['nextRequest'])) return false;
  
  if ($result_dispense['data']['nextRequest']['type'] != 'pickup') return false;
  
  if ($type != 'dropoff') return false;
  
  if (!in_array("tube", $result_location['data']['properties'])) return false;
  
  return true;  
}

/**
 * Find combined sequence for an order
 *           in the order of default sequence order
 */
function plusdelta_api_getOrderSequence($barcode) {
  // fetch order
  $result_order = plusdelta_api_getOrder($barcode);

  // get default requet sequence
  $all_types = plusdelta_api_getRequestTypeArray();
  $ty = variable_get('plusdelta_request_sequence');
  $types = array();
  foreach ($ty as $value) {
    $types[$value] = $all_types[$value];
  }

  // fetch all med sequence
  $pr = $types; 
  if (!empty($result_order['data']['meds'])) {
    foreach ($result_order['data']['meds'] as $value) {
      $med = plusdelta_api_getMed($value);
      $seq = $med['data']['processes'];
      if (!$seq) 
        $seq = $types;
      $pr = array_merge($pr,$seq);
    }
  }
  
  $diff = array_diff($all_types, $pr);
  $combine = array_diff($all_types, $diff);
  return $combine;
}

/**
 * Get combined features for an order
 */
function plusdelta_api_getOrderFeatures($barcode) {
  // fetch order
  $result_order = plusdelta_api_getOrder($barcode);
    
  // fetch all processes
  $pr = array();
  foreach ($result_order['data']['meds'] as $value) {
    $med = plusdelta_api_getMed($value);
    $seq = $med['data']['features'];
    $diff = array_diff($seq, $pr);
    $pr = array_merge($pr, $diff);
  }

  return $pr;
}

/**
 * Update dispense info after createRequest call
 */
function plusdelta_raw_updateDispense($dispense_id) {
  $dis = node_load($dispense_id);
  
  $reqs = plusdelta_api_getRequests($dis->title);
  
  $orderTitle = node_load($dis->field_dispense_order['und'][0]['nid'])->title;
  
  $res['defaultSequence'] = array_values(plusdelta_api_getOrderSequence($orderTitle));
  $res['currentSequence'] = '';
  $seqs = array();
  if (isset($reqs['data'][0])) {
    // assmeble a simple current sequence
    foreach ($reqs['data'] as $value) {
      $seqs[] = $value['type'];
    }
    $res['currentSequence'] = $seqs;
    $res['latestRequest'] = $reqs['data'][0];
    $res['nextRequest']['type'] = plusdelta_api_getNextRequestType(
      $res['defaultSequence'], $res['currentSequence']
    );
    // print_r($res); die;
  }

  // current sequence
  $dis->field_dispense_sequence['und'][0]['value'] = serialize($seqs);
   
  // if newly dispense
  if (!$res['currentSequence']) {
    // take care init handling, ex. shift
    $shifts = plusdelta_api_getCurrentShifts();
    if (isset($shifts['data']) && ($shifts['data']))
      $dis->field_dispense_shift['und'][0]['target_id'] = $shifts['data'][0]['sid'];
    // $res['nextRequest']['type'] = 'print';
  }
  
  // update the latest request id
  if (isset($res['latestRequest']['rid'])) {
    $dis->field_dispense_lastrequest['und'][0]['target_id'] = $res['latestRequest']['rid'];
    // handle prepare
    if ($res['latestRequest']['type'] == 'print') {
      $dis->field_dispense_preparelocation['und'][0]['target_id'] = $res['latestRequest']['lid'];
    }
    // handle dropoff
    if ($res['latestRequest']['type'] == 'dropoff') {
      $dis->field_dispense_isdropoff['und'][0]['value'] = 1;
      $dis->field_dispense_dropofflocation['und'][0]['target_id'] = $res['latestRequest']['lid'];
      $dis->field_dispense_dropoffts['und'][0]['value'] = $res['latestRequest']['timestamp'];
      // handle hand delivery
      if (isset($res['latestRequest']['user2'])) {
        $dis->field_dispense_handed['und'][0]['value'] = 1;
      }
      // handle ontime check here
      $offset = variable_get('plusdelta_ontime_offset_time')*60;
      if ($res['latestRequest']['timestamp'] + $offset <= $dis->field_dispense_duets['und'][0]['value']) {
        $dis->field_dispense_isontime['und'][0]['value'] = 1;
      }
      $res['nextRequest']['type'] = 'done';
    }
    // handle auto-close
    if ($res['latestRequest']['type'] == 'close') {
      $dis->field_dispense_isclose['und'][0]['value'] = 1;
      $res['nextRequest']['type'] = 'done';
    }
    // handle reject
    if ($res['latestRequest']['type'] == 'reject') {
      $res['nextRequest']['type'] = variable_get('plusdelta_reject_nextrequest', 'prepare');
    }
  }
    
  if (isset($res['nextRequest']['type'])) 
    if ($node = plusdelta_api_getNodeByTitle('requesttype',$res['nextRequest']['type']))
      $dis->field_dispense_nextrequesttype['und'][0]['target_id'] = $node->nid;
      

  node_save($dis);
}

/**
 * Raw add request with dispense, user, location, and type id
 */
// function plusdelta_raw_createRequest($dispense_id, $user_id, $location_id, $type_id, $device_id = NULL, $note = "", $delivery_user_id = 0, $options = array()) {
function plusdelta_raw_createRequest($op) {
  $node = new StdClass();
  $node->type = 'request';
  $node->language = 'und';
  node_object_prepare($node);
  $node->title = 'abc';
  $node->uid = $op['user']['uid'];
  $node->field_request_dispense['und'][0]['nid'] = $op['dispense']['did'];
  $node->field_request_location['und'][0]['target_id'] = $op['location']['lid'];
  $node->field_request_requesttype['und'][0]['target_id'] = $op['type']['tid'];
  $node->field_request_device['und'][0]['target_id'] = $op['device']['did'];
  $node->body[$node->language][0]['value'] = $op['note'];
  if (!empty($op['user2']['uid']))
    $node->field_request_deliveryuser['und'][0]['target_id'] = $op['user2']['uid'];

  if (isset($op['override'])) {
    $node->field_request_isoverride[$node->language][0]['value']  = intval($op['override']);
  }
  
  // handle picture attachment here
  if (!empty($_FILES['files']['name']['image'])) {
    $file = file_save_upload('image', array(), 'private://requests');
    $file->status = 1;
    file_save($file);
    $node->field_request_picture['und'][0] = (array)$file;
  }

  if ($node = node_submit($node)) { 
    node_save($node);
    plusdelta_raw_updateDispense($op['dispense']['did']);
  }

  return $node;
}

/**
 * Update upon adding request
 */
function plusdelta_raw_finishRequest($rid, $timestamp) {
  $node = node_load($rid);
  $node->field_request_duration[$node->language][0]['value'] = $timestamp - $node->created;
  node_save($node);
}

/**
 * Create missing dispense
 */
function plusdelta_raw_createDispense($barcode) {
  $did = variable_get('plusdelta_default_dispense_id');
  if (empty($did)) return 0;
  
  if ($nn = node_load($did)) {
    unset($nn->nid);
    unset($nn->vid);
    node_object_prepare($nn);
    $nn->title = $barcode;
    $nn->field_dispense_isclosed['und'][0]['value'] = 0;
    $nn->field_dispense_isdropoff['und'][0]['value'] = 0;
    $nn->field_dispense_sequence['und'][0]['value'] = '';
    
    // for validation rule, we need to apply order id
    // if not found, we fall back to template version
    $options = variable_get('plusdelta_special_rules');
    if (isset($options['Dispense creation'])) {
      $pattern = variable_get('plusdelta_barcode_validation');
      preg_match_all('/' . $pattern . '/', $barcode, $matches);
      $order_barcode = isset($matches[0][0]) ? $matches[0][0] : '';
      if ($oid = getEntityValue(array('type' => 'order', 'title' => $order_barcode)))
        $nn->field_dispense_order['und'][0]['nid'] = $oid;
    }
    
    // save and return dispense id
    node_save($nn);
    
    // update 
    return $nn->nid;
  }
  
  return '';
}

/**
 * API [POST][CREATE] Add request
 * * Example: api?command=createRequest&barcode=DISP_BARCODE&user=SE23-231&location=MC13AE&type=pickup
 * * Example: api?command=createRequest&barcode=JZ01234560-ABCD&user=SE23-231&location=2000&type=verify$device=DE333&note=123
 * @param barcode
 *   Specify dispense barcode
 * @param user
 *   Specify user barcode
 * @param location
 *   Specify location barcode
 * @param type
 *   Specify request type string
 * @param device
 *   [OPTIONAL] Specify device barcode
 * @param note
 *   [OPTIONAL] Specify request note
 * @param user2
 *   [OPTIONAL] Specify a user barcode for hand delivery
 * @param files[image]
 *   [OPTIONAL] Upload a picture file through $_FILES
 */
// function plusdelta_api_createRequest($barcode, $user, $location, $type, $device="", $note="", $user2 = "", $ndc = "", $override = 0){
function plusdelta_api_createRequest($op){
  // handle bundle case
  if (module_exists('pharmtrac_bundle') && 
    variable_get_value('pharmtrac_bundle_enabled')) { 
    module_load_include('inc', 'pharmtrac_bundle', 'pharmtrac_bundle.api');
    
    if (plusdelta_api_isBundle($op['barcode'])) {
      $res = plusdelta_api_createBundleRequest($op);
      return $res;
    }
  }
  
  $result = array();
  $g = array();
  $result_dispense = plusdelta_api_getDispense($op['barcode']);
  if ($result_dispense['status']=='failure') {
    // requires a default order and patient
    if ($nid = plusdelta_raw_createDispense($op['barcode'])) {
      plusdelta_raw_updateDispense($nid);
      $result_dispense = plusdelta_api_getDispense(NULL,$nid);
    } else
      return plusdelta_returnMsg('failure', 'auto dispense creation failed');
  }
  $g['dispense'] = $result_dispense['data'];

  // make sure the order isn't discountinued
  $control = plusdelta_raw_get(array('type'=>'order', 'title'=>$result_dispense['data']['order']), 'field_order_control');
  if ($control['und'][0]['value'] == 'DC') {
    $res = plusdelta_returnMsg('failure', 'order is discountinued, no request is allowed');   
    return $res;
  }

  // get user info
  $result_user = plusdelta_api_getUser($op['user']);
  if ($result_user['status']=='failure') return $result_user;
  $g['user'] = $result_user['data'];

  // get location info
  $result_location = plusdelta_api_getLocation($op['location']);
  if ($result_location['status']=='failure') return $result_location;
  $g['location'] = $result_location['data'];

  // get request type info
  $result_type = plusdelta_api_getRequestType($op['type']);
  if ($result_type['status']=='failure') return $result_type;
  $g['type'] = $result_type['data'];
  
  // get device id
  $device_id = getEntityValue(array('type' => 'device', 'title' => $op['device']));
  if (!$device_id) return plusdelta_returnMsg('failure', 'no matched device');
  $g['device'] = array('did' => $device_id, 'barcode' => $op['device']);

  // get delivery user id, need to put in module 
  $g['user2'] = array();
  if (!empty($op['user2'])) {
    if ($delivery_user_id = getEntityValue(array('field_user_barcode' => $op['user2']), 'uid', 'user')) {
      $uu = user_load($delivery_user_id);
      if (!(pharmtrac_check_user_mobile('hand', $uu)))
        return plusdelta_returnMsg('failure', 'hand delivery is not authenrized for the user');
    }
    $g['user2'] = array('uid' => $delivery_user_id, 'barcode' => $op['user2']);
  }
  
  $g['note'] = $op['note'];
  $g['override'] = $op['override'];
  
  if (module_exists('pharmtrac_ndc') &&
    variable_get_value('pharmtrac_ndc_enabled')) {
    $g['ndc'] = $op['ndc'];
    $g['ndc_override'] = $op['ndc_override'];
  }
  
  if ($g['override'] || (($return = plusdelta_api_requestAllowed($g)) > 0)) {
    // //refresh in case a new request was added for a special
    //      $result_dispense = plusdelta_api_getDispense($barcode); 
    $node = plusdelta_raw_createRequest($g);
    $result['rid'] = $node->nid;
    
    // need to finish the previous request as well
    if (isset($result_dispense['data']['latestRequest']['rid']))
      plusdelta_raw_finishRequest($result_dispense['data']['latestRequest']['rid'],$node->created);
    
    $res = plusdelta_returnMsg('success','request added successfully.',$result);
  }  else {
    $re_array = explode('|', $return);
    switch ($re_array[0]) {
      case 'refrigerator':
        $res = plusdelta_returnMsg('failure', 'invalid delivery location for medication - refrigeration needed');
        break;
      case 'dropoff location': 
        $res = plusdelta_returnMsg('failure', 'invalid dropoff location for medication');    
        break;
      case 'hand delivery':
        $res = plusdelta_returnMsg('failure', 'meds require hand delivery');    
        break;
      case 'tube':
        $res = plusdelta_returnMsg('failure', 'invalid delivery location for medication - do not tube');    
        break;
      case 'ndc-missing':
        $res = plusdelta_returnMsg('failure', 'missing med ndc code');    
        break;
      case 'ndc-mismatch':
        $err = "med NDC code mismatch";
        if (!empty($re_array[1])) $err .= " for med " . $re_array[1];
        $res = plusdelta_returnMsg('failure', $err);
        break;
      
      default:
        // improve error message
        $msg = 'Invalid Scan - ';
        if (isset($result_dispense['data']['latestRequest'])) 
          $msg .= $result_dispense['data']['latestRequest']['type'] . ' completed';
        else $msg .= 'init completed';
        if (isset($result_dispense['data']['nextRequest'])) {
          if ($result_dispense['data']['nextRequest']['type'] != 'done')
            $msg .= ', expecting ' . $result_dispense['data']['nextRequest']['type'];
          else $msg .= '.';
        }
        $res = plusdelta_returnMsg('failure', $msg);
        break;
    }
  }
  return $res;
}

/**
 * Find out mobile features for user
 */
function pharmtrac_check_user_mobile($feature, &$user) {
  $role_id = getSet($user->field_user_mobilerole['und'][0]['target_id']);
  if (empty($role_id)) return false;
  
  $feature_id = getEntityValue(array('type'=> 'mobilefeature', 'title'=> $feature));
  if (empty($feature_id)) return false;
  
  $role = node_load($role_id);
  if (empty($role->field_mobilerole_features['und'])) return false;
  foreach ($role->field_mobilerole_features['und'] as $value) {
    if ($value['target_id'] == $feature_id) return true;
  }
  
  return false;
}

/**
 * Get request by dispense barcode
 * Example: api?command=getRequests&barcode=DISP_BARCODE
 */
function plusdelta_api_getRequest($nid) {
  if ($node = node_load($nid)) {
    // print_r($node); die;
    $re['rid'] = $node->nid;
    $re['type'] = node_load($node->field_request_requesttype['und'][0]['target_id'])->title;
    $re['timestamp'] = $node->created;
    $re['user'] = user_load($node->uid)->field_user_barcode['und'][0]['value'];
    $re['verified'] = $node->field_request_isverified['und'][0]['value'];
    // $re['handoff'] = $node->field_request_handoff['und'][0]['value'];
    $re['override'] = $node->field_request_isoverride['und'][0]['value'];
    $re['lid'] = $node->field_request_location['und'][0]['target_id'];
    $re['location'] = node_load($node->field_request_location['und'][0]['target_id'])->title;
    $re['duration'] = getSet($node->field_request_duration['und']['0']['value']);
    $re['note'] = getSet($node->body['und']['0']['value']);
    if (isset($node->field_request_deliveryuser['und'][0]['target_id']))
      $re['user2'] = user_load($node->field_request_deliveryuser['und'][0]['target_id'])->field_user_barcode['und'][0]['value'];
   
    $res = plusdelta_returnMsg('success','ok',$re);
  } else {
    $res = plusdelta_returnMsg('failure','','');
  }
  
  return $res;
}

/**
 * Get request by dispense barcode
 * Example: api?command=getRequests&barcode=DISP_BARCODE
 */
function plusdelta_api_getRequests($barcode){
  $query = new EntityFieldQuery();
  $entities = $query->entityCondition('entity_type', 'node')
        ->propertyCondition('type', 'dispense')
        ->propertyCondition('title', $barcode)
        ->execute();
  $result = array();
  // if dispense exist
  if ($entities) {
    $ids = array_keys($entities['node']);
    $did = end($ids);

    $query = new EntityFieldQuery();
    $entities = $query->entityCondition('entity_type', 'node')
      ->propertyCondition('type', 'request')
      ->fieldCondition('field_request_dispense', 'nid', $did)
      // ->propertyOrderBy('created', 'DESC')  
      ->propertyOrderBy('nid', 'DESC')  
      ->execute();
    if ($entities) {
      $ids = array_keys($entities['node']);
      $result = array();
      foreach($ids as $id) {
        $node = node_load($id);
        $re['rid'] = $node->nid;
        $re['type'] = node_load($node->field_request_requesttype['und'][0]['target_id'])->title;
        $re['timestamp'] = $node->created;
        $re['user'] = user_load($node->uid)->field_user_barcode['und'][0]['value'];
        $re['verified'] = $node->field_request_isverified['und'][0]['value'];
        // $re['handoff'] = $node->field_request_handoff['und'][0]['value'];
        $re['override'] = $node->field_request_isoverride['und'][0]['value'];
        $re['lid'] = $node->field_request_location['und'][0]['target_id'];
        $re['location'] = node_load($node->field_request_location['und'][0]['target_id'])->title;
        $re['duration'] = getSet($node->field_request_duration['und']['0']['value']);
        $re['note'] = getSet($node->body['und']['0']['value']);
        if (isset($node->field_request_deliveryuser['und'][0]['target_id']))
          $re['user2'] = user_load($node->field_request_deliveryuser['und'][0]['target_id'])->field_user_barcode['und'][0]['value'];
        
        $result[] = $re;
        //$result[$id] = $re;
      }
      $res = plusdelta_returnMsg('success','ok',$result);
    } else {
      $res = plusdelta_returnMsg('success','no requests');
    }
  } else {
    $res = plusdelta_returnMsg('failure','dispense does not exist');
  }

  return $res;
}

/**
 * Get request count
 */
function plusdelta_api_getRequestCount($type) {
  $c = 0;
  if ($node = plusdelta_api_getNodeByTitle('requesttype',$type)) {
    $query = new EntityFieldQuery();
    $c = $query->entityCondition('entity_type', 'node')
       ->propertyCondition('type', 'request')
       ->fieldCondition('field_request_requesttype', 'target_id', $node->nid)
       ->count()
       ->execute();
  }
  
  return $c;
}

/**
 * Get dispense count
 */
function plusdelta_api_getDispenseCount($location_id = NULL, $nextrequesttype_id = NULL,
  $from = NULL, $to = NULL, $autoclose = NULL, $dropoff = NULL, 
  $duetime = NULL, $ontime = NULL) 
{
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node');
  $query->propertyCondition('type', 'dispense');
  if (!$from) $from = 974222952;
  if (!$to) $to = 32531131752;
  $query->propertyCondition('created', array($from, $to), 'BETWEEN');
  if (!is_null($location_id))
    $query->fieldCondition('field_dispense_preparelocation' , 'target_id', $location_id);
  if (!is_null($autoclose)) 
    $query->fieldCondition('field_dispense_isclosed', 'value', $autoclose);
  if (!is_null($dropoff)) 
    $query->fieldCondition('field_dispense_isdropoff', 'value', $dropoff);
  if ($nextrequesttype_id) 
    $query->fieldCondition('field_dispense_nextrequesttype' , 'target_id', $nextrequesttype_id);
  if (!is_null($ontime)) 
    $query->fieldCondition('field_dispense_isontime', 'value', $ontime);
  if (!is_null($duetime)) 
    $query->fieldCondition('field_dispense_duets', 'value', $duetime, '<');

  return $query->count()->execute();
} 

/**
 * Get request count color
 */
function plusdelta_api_getRequestCountColor($setting, $count) {
  if ($count > intval($setting['thres2'])) {
    return $setting['footer3'];
  }
  if ($count > intval($setting['thres1'])) {
    return $setting['footer2'];
  }
  return $setting['footer1'];
}

/**
 * Get wallboard counts
 */
function plusdelta_api_getWallboardCounts($settings, $location_id = NULL) {
  $duetime = time() + variable_get('plusdelta_wallboard_duetime_offset')*60;  //30 minutes from now
  
  // for now let's make the shift to be defined by created
  $from = strtotime('today'); // $from = NULL;

  // the defination of ontime is that all dispenses who got a dropoff status 
  // and the dropoff status time is before dispense due time :) 
  $reqs = plusdelta_api_getRequestTypeArray();
  $countArray = array(
    'print' => array(
      'label' => 'Prep Queue',
      'count' => plusdelta_api_getDispenseCount($location_id,
        array_search('prepare',$reqs),
        $from, NULL, False
      ),
      'color' => 'red',
    ),
    'prepare'  => array( 
      'label' => 'Check Queue',
      'count' => plusdelta_api_getDispenseCount($location_id,
        array_search('verify',$reqs),
        $from, NULL, False
      ),
      'color' => 'green',
    ),
    'pickup'     => array( 
      'label' => 'Depart Queue',
      'count' => plusdelta_api_getDispenseCount($location_id,
        array_search('pickup',$reqs),
        $from, NULL, False
      ),
      'color' => 'blue',
    ),
    'dropoff'     => array( 
      'label' => 'Delivered',
      'count' => plusdelta_api_getDispenseCount($location_id,
        NULL,
        $from, NULL, NULL, TRUE, NULL, NULL
      ),
      'color' => 'blue',
    ),
    'closed'   => array( 
      'label' => 'Autoclosed',
      'count' => plusdelta_api_getDispenseCount($location_id,
        NULL,
        $from, NULL, TRUE, NULL, NULL, NULL
      ),
      'color' => 'orange',
    ),
    'due'     => array(
      'label' => 'Due',
      'count' => plusdelta_api_getDispenseCount($location_id,
        NULL,
        $from, NULL, False, 0, $duetime, NULL
      ),
      'color' => 'grey',
    ),
    'ontime'  => array(
      'label' => 'OnTime',
      'count' => plusdelta_api_getDispenseCount($location_id,
        NULL,
        $from, NULL, NULL, NULL, NULL, TRUE
      ),
      'color' => 'white',
    ),
  );
// dsm($countArray);  
  // update total and last percentage
  $total = $countArray['dropoff']['count'] + $countArray['closed']['count'];
  $countArray['ontime']['count'] = $total? round(floatval($countArray['ontime']['count']) / $total, 3):0;
  
  // fix color
  foreach ($countArray as $request => &$value) {
    if (array_key_exists($request,$settings)) 
      $value['color'] = plusdelta_api_getRequestCountColor($settings[$request],$value['count']);
  }
  
  return $countArray;
}

/**
 * Format interval hours
 */
function format_interval_hours($interval) {
  $digits = array();
  $secs = array(3600,60,1);
  foreach ($secs as $value) {
    $digits[] = str_pad(floor($interval / $value), 2, "0", STR_PAD_LEFT);
    $interval %= $value;
  }
  return implode(':',$digits);
}

/**
 * Get wallboard dispense
 */
function plusdelta_api_getWallboardDispense($nid, $cols = array()) {
  $dis = plusdelta_api_getDispense('',$nid);
  
  // fix latest request timestamp issue
  if (empty($dis['data']['latestRequest'])) {
    $dis['data']['latestRequest']['timestamp'] = $dis['data']['originateTimestamp'];
  }
  
  // hold the container first
  // $re = plusdelta_api_getWallboardColumns();
  $data = &$dis['data'];
  $re = array(
    'patientid'       => getSet($data['patient']),
    'patientName'     => getSet($data['patientName']),
    'dispense'        => getSet($data['barcode']),
    'order'           => getSet($data['order']),
    'treatment'       => getSet($data['treatment']),
    'room'            => getSet($data['room']),
    'requestUser'     => getSet($data['latestRequest']['user']),
    'latestRequest'   => getSet($data['latestRequest']['type']),
    'nextRequest'     => getSet($data['nextRequest']['type']),
    'postedDispense'  => getSet($data['originateTimestamp']),
    'postedRequest'   => getSet($data['latestRequest']['timestamp']),
    'dueDispense'     => getSet($data['dueTimeStamp']),
    'prepareLocation' => getSet($data['prepare']),
    'dropoffLocation' => getSet($data['dropoff']),
    'totalTime'       => format_interval_hours(REQUEST_TIME-$dis['data']['originateTimestamp']),
    'stateTime'       => format_interval_hours(REQUEST_TIME-$dis['data']['latestRequest']['timestamp']),
    'dueTime'         => (REQUEST_TIME-$dis['data']['dueTimeStamp'] < 0 ? '' : '- ') . format_interval_hours(abs(REQUEST_TIME-$dis['data']['dueTimeStamp']))
    // 'totalTime'       => format_interval(REQUEST_TIME-$dis['data']['originateTimestamp'],1),
    // 'stateTime'       => format_interval(REQUEST_TIME-$dis['data']['latestRequest']['timestamp'],1),
    // 'dueTime'         => (REQUEST_TIME-$dis['data']['dueTimeStamp'] < 0 ? '' : '- ') . format_interval(abs(REQUEST_TIME-$dis['data']['dueTimeStamp']),1)
  );
  
  if ($data['meds']) {
    $re['medBarcode'] = $data['meds'][0]['barcode'];
    $re['medShortName'] = $data['meds'][0]['short'];
  }
  
  $re = array_intersect_key($re, $cols);
  
  return $re;
}

/**
 * Auto close dispenses
 */
function plusdelta_api_autoCloseDispenses($total = 10) {
  $time = time() - (60*variable_get('plusdelta_autoclosed_time'));   //time is minute
       
  $query = new EntityFieldQuery();
  $entities = $query->entityCondition('entity_type', 'node')
        ->propertyCondition('type', 'dispense')
        // ->propertyCondition('changed', $time, '<')
        ->fieldCondition('field_dispense_isclosed', 'value', '0')
        ->propertyCondition('created', $time, '<')
        // ->fieldCondition('field_dispense_receivets', 'value', $time, '<')
        ->propertyOrderBy('nid', 'DESC')
        ->pager($total)
        ->execute();

  $count = 0;
  if ($entities) {
    $ids = array_keys($entities['node']);
    foreach($ids as $id) {
      $node = node_load($id);
      $node->field_dispense_isclosed['und'][0]['value'] = 1;
      node_save($node);
    }
    $count = count($entities['node']);
  }
  
  watchdog('Pharmtrac','Autoclose ' . $count . ' dispenses.');
  return $count;
}

/**
 * Fast auto close dispenses
 */
function plusdelta_api_autoCloseFast($total = 1) {
  $time = time() - (60*variable_get('plusdelta_autoclosed_time'));   //time is minute
  // $time = time();
  // find dispense that needs to be autoclosed
  $query = "
SELECT node.nid FROM node INNER JOIN field_data_field_dispense_isclosed on field_data_field_dispense_isclosed.entity_id = node.nid WHERE ((field_data_field_dispense_isclosed.field_dispense_isclosed_value=0) AND (node.created < {$time})) ORDER BY node.nid DESC LIMIT {$total}
  ";
  $result = db_query($query);
    
  $ids = array();
  foreach ($result as $value) {
    $ids[] = $value->nid;
  }
  $id_array = implode(', ', $ids);
  // dsm($id_array);
  
  if ($id_array) {
    // auto close found dispenses
    $query = "
      UPDATE field_data_field_dispense_isclosed SET field_data_field_dispense_isclosed.field_dispense_isclosed_value = 1 WHERE field_data_field_dispense_isclosed.entity_id IN ({$id_array})
    ";
    $result = db_query($query);
    
    // clear field cache
    $cids = array();
    foreach ($ids as $value) {
      $cids[] = "'field:node:" . $value . "'";
    }
    $cache_array = implode(", ", $cids);
    // dsm($cache_array);
    $query = "
      DELETE FROM cache_field WHERE cid IN ({$cache_array})
    ";
    $result = db_query($query);
    
    watchdog('Pharmtrac','Autoclose ' . count($ids) . ' dispenses.');
  }
}

/**
 * Get wallboar settings
 */
function plusdelta_api_getWallboardSettings($barcode) {
  if (!$barcode) 
    return plusdelta_returnMsg('failed','wallboard does not exist');
  // find wallboard
  $node = plusdelta_api_getNodeByTitle('wallboard', $barcode);
  
  // find wallboard
  $result_locations = plusdelta_api_getLocations();
  $result_location = plusdelta_api_getLocation('',$node->field_wallboard_location['und']['0']['target_id']);
  
  $res = array(
    'refresh' => variable_get('plusdelta_wallboard_refreshrate'),
    'location' => $result_location['data'],
    'function' => $node->field_wallboard_options['und']['0']['value'],
    'colors' => variable_get('plusdelta_color_settings'),
    'locations' => $result_locations['data'],
  );
  
  return plusdelta_returnMsg('success','ok',$res);
}

/**
 * Set wallboard setting
 */
function plusdelta_api_setWallboardSettings($barcode) {
  $res = $_POST;
  
  $node = plusdelta_api_getNodeByTitle('wallboard', $barcode);
  if ($location = plusdelta_api_getNodeByTitle('location',$_POST['location'])) {
    $node->field_wallboard_location['und'][0]['target_id'] = $location->nid;
  }
  $node->field_wallboard_options['und'][0]['value'] = $_POST['function-radio'];
  node_save($node);
  
  return plusdelta_returnMsg('success','ok',$res);
}

// API CALL : return a json which includes
// 1) settings all this wallboard
// 2) dispenses associatd with this setting
function plusdelta_api_getWallboard2($barcode){
  // find wallboard
  $node = plusdelta_api_getNodeByTitle('wallboard', $barcode);

  if (!($node)) {
    return plusdelta_returnMsg('failure', $barcode . ' does not exist');
  }

  // load wallboard setting
  // currently its coming from the global setting
  // might later migrate into each wallboard setting
  // $colors = plusdelta_api_getWallboardColors();
  $allCols = plusdelta_api_getWallboardColumns(); 
  // $cols = array_intersect_key(array_flip(),$allCols);
  $cols = array();
  foreach (variable_get('plusdelta_wallboard_column_order') as $value) {
    $cols[$value] = $allCols[$value];
  }
  // print_r($cols); die;

  // load services wallboard dispenses api 
  $lid = $node->field_wallboard_location['und'][0]['target_id'];
  $option = $node->field_wallboard_options['und'][0]['value'];
  module_load_include('inc','services_views','services_views.resource');
  $service = 'wallboard_services';
  $filter = array($option => $lid);
  if ($option == 'dropoff') 
    $service = $service . '_dropoff';
  else 
    $filter = array();  // currently everything will show up in prepare wallboard
  $items = services_views_retrieve($service, 'services_1',
                array(), 0, 10, false, $filter);

  foreach ($items as &$item) {
    $item = plusdelta_api_getWallboardDispense($item->did, $cols);
  }

  $settings = variable_get('plusdelta_color_settings');
  // return the json
  $res = array(
    'columns' => $cols,
    'items' => $items,
    'counts' => plusdelta_api_getWallboardCounts($settings),
    'settings' => $settings,
    'refresh' => variable_get('plusdelta_wallboard_refreshrate'),
    'type' =>  getSet($node->field_wallboard_options['und'][0]['value']),
  );

  
  return plusdelta_returnMsg('success','wallboard ' . $barcode . ' updated',$res);
}

/**
 * Get report
 */
function plusdelta_api_getReport($barcode,$interval=null,$from=null,$to=null) {
  // db query
  // $query = new EntityFieldQuery();
  // $entities = $query->entityCondition('entity_type', 'node')
  //                   ->propertyCondition('type', 'dispense')
  //                   // ->fieldCondition('field_dispense_isdropoff','value','1')
  //                   // ->propertyCondition('created',array(1345146124,1351623929),'BETWEEN')
  //                   ->execute();
  // $res = node_load_multiple(array_keys($entities['node']));
  
  // post execute does nothing to datasource
  $view = views_get_view('report_ontime_delivery');
  if ($interval) {
    $date_field = $view->get_item('page_1','field','created');
    $date_field['date_format'] = 'wallboard_' . $interval;
    $view->set_item('page_1','field','created',$date_field);
  }
  if ($from || $to) {
    $date_filter = $view->get_item('page_1','filter','created');
    $date_filter['value']['min'] = $from?date('Y-m-d',$from):'2010-01-01';
    $date_filter['value']['max'] = $to?date('Y-m-d',$to):'2020-01-01';
    $view->set_item('page_1','filter','created',$date_filter);
  }
  $view->set_display('page_1');
  $view->pre_execute();
  $view->execute();
  $res = $view->render('page_1');
  // print_r(); die;
  // print_r($res); die;
  
  // Currently I can't render a view into a array
  // So I just override a template to make it dumping json format only
  // $res = views_get_view_result('report_ontime_delivery','default');
  // $res = views_embed_view('report_ontime_delivery', 'page_1');
  $res = json_decode($res);
  // $res = html_entity_decode($res);
  
  // strip out the ones which are similar
  $seriesX = array();
  $seriesY = array();
  $seriesY2 = array();
  $old = '';
  $total = plusdelta_api_getDispenseCount();
  foreach ($res as $key => $value) {
    $new = $value->Date;
    if ($old != $new) {
      $seriesX[] = $value->Date;
      $seriesY[] = intval($value->OnTime);
      // $seriesY2[] = ((float)$value->Count)/$total*100.0;
      $seriesY2[] = (float)$value->OnTime / $value->Count *100.0; 
    }
    $old = $new;
  }
  $res = array(
    'X' => $seriesX,
    'Y' => $seriesY,
    'Y2' => $seriesY2
  );
  
  return plusdelta_returnMsg('success','ok',$res);
  // return plusdelta_returnMsg('success','ok',$date_filter);
  
}

/**
 * Get report
 * Ditch views PHP and use EFQ 
 */
function plusdelta_api_getReport2($barcode,$interval=NULL,$from=NULL,$to=NULL,
            $priority=NULL,$prepare=NULL,$dropoff=NULL,$shift=NULL,$dueoffset=NULL) {
  // handle interval format
  if (!$interval) $interval = 'month';
  $date_format = 'wallboard_'.$interval;

  // handle date range
  if (!$from) $from = 974222952;
  if (!$to) $to = 32531131752;
  
  // handle priority list
  if ($priority) {
    $priority = explode(',', $priority);
  }

  // build query
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
        ->propertyCondition('type', 'dispense')
        ->propertyCondition('created', array($from, $to), 'BETWEEN');
  if ($prepare) 
    $query->fieldCondition('field_dispense_preparelocation','target_id',array($prepare),'IN');
  if ($dropoff) 
    $query->fieldCondition('field_dispense_dropofflocation','target_id',array($dropoff),'IN');
  if ($priority)
    $query->fieldCondition('field_dispense_prioritylevel','target_id',array($priority),'IN');
  if ($shift)
    $query->fieldCondition('field_dispense_shift','target_id',array($shift),'IN');
  $query->propertyOrderBy('created', 'ASC');
  $entities = $query->execute();
  // calculate data
  $data = array();
  if ($entities) {
    $ids = array_keys($entities['node']);
    foreach($ids as $id) {
      $node = node_load($id);
      $created = format_date($node->created, $date_format);
      
      // create date key
      if (!isset($data[$created])) 
        $data[$created] = array(
          'total' => 0,
          'ontime' => 0,
          'closed' => 0,
          'dropoff' => 0,
        );
      
      // calculate total
      $item = &$data[$created];
      $item['total'] = $item['total'] + 1;
      
      // calculate ontime
      if (!$dueoffset) {
        // default has been calculated and stored
        if ($node->field_dispense_isontime['und'][0]['value']) 
          $item['ontime'] = $item['ontime'] + 1;
      } else {
        // recalculate it
        if ($node->changed + $dueoffset*60 <= $node->field_dispense_duets[$node->language][0]['value'])
          $item['ontime'] = $item['ontime'] + 1;
      }
      
      // calculate closed
      if ($node->field_dispense_isclosed['und'][0]['value']) 
        $item['closed'] = $item['closed'] + 1;
      
      // calculate closed
      if ($node->field_dispense_isdropoff['und'][0]['value']) 
        $item['dropoff'] = $item['dropoff'] + 1;
    }
  }
  
  // prepare series
  $keys = array('total','ontime','closed','dropoff');
  $res = array();
  foreach ($data as $d => $item) {
    $res['date'][] = $d;
    foreach ($keys as $k) {
      $res[$k][] = $item[$k]; 
    }
  }
  
  return plusdelta_returnMsg('success','ok',$res);
} 

/**
 * Get report request
 * report for request now
 */
function plusdelta_api_getReportRequest($interval=NULL,$from=NULL,$to=NULL) {
  // we need to finalize the histogram time format handling
  if (!$interval) $interval = 'month';
  $date_format = 'wallboard_'.$interval;

  // handle date range
  if (!$from) $from = 974222952;
  if (!$to) $to = 32531131752;
  
  // get list of request type
  $requests = plusdelta_api_getRequestTypeArray();
  $rdata = array_fill_keys(array_keys($requests),array(
    'total' => 0,
    'average' => 0
  ));
  
  // build query
  $query = new EntityFieldQuery();
  $entities = $query->entityCondition('entity_type', 'node')
        ->propertyCondition('type', 'request')
        ->propertyCondition('created', array($from, $to), 'BETWEEN')
        ->propertyOrderBy('created', 'ASC')
        ->execute();

  // get histogram, D8 has groupby but not D7, so we're still need to build it
  $data = array();
  if ($entities) {
    $ids = array_keys($entities['node']);
    foreach($ids as $id) {
      $node = node_load($id);
      $created = format_date($node->created, $date_format);
      
      // create date key
      if (!isset($data[$created])) 
        $data[$created] = $rdata;
      
      // print_r($node); die;
      if ($requesttype = drupal_getNodeFieldValue($node,'field_request_requesttype','target_id')) {
        $data[$created][$requesttype]['total'] = $data[$created][$requesttype]['total'] + 1;
        $data[$created][$requesttype]['average'] += drupal_getNodeFieldValue($node,'field_request_duration');
      }
    }
  }
  
  // prepare series
  $res = array();
  foreach ($data as $d => $rvalue) {
    $res['date'][] = $d;
  }
  foreach ($rdata as $key => $value) {
    $res[$key] = array();
    $res[$key]['type'] = $requests[$key];
    $res[$key]['total'] = array();
    $res[$key]['average'] = array();
    foreach ($data as $d => $rvalue) {
      $res[$key]['total'][] = $rvalue[$key]['total'];
      $res[$key]['average'][] = ($rvalue[$key]['total'])?floatval($rvalue[$key]['average'])/$rvalue[$key]['total']:0;
    }
  }

  return plusdelta_returnMsg('success','ok',$res);
}


/**
 * Get report settings
 */
function plusdelta_api_getReportSettings() {
  $locations = plusdelta_api_getLocations();
  $shifts = plusdelta_api_getShifts();
  $priorities = plusdelta_api_getPriorityLevels();
  
  $res = array(
    'locations' => $locations['data'],
    'shifts' => $shifts['data'],
    'priorities' => $priorities['data'],
  );
  
  return plusdelta_returnMsg('success','ok',$res);
}

/**
 * API [GET][AUTH]: Authenticate user
 * * Two ways to authenticate based on authentication settings
 * * Example: auth?barcode=SE24-197&pass=admin (require_authentication on)
 * * Example: auth?app=plusdelta123&barcode=SE24-197&device=DE834 (require_authentication off)
 * * Supported success message
 * * - current logged in user info
 * * - logged out successfully
 * * - user authenticated
 * * 
 * * Supported failure message
 * * - device blocked
 * * - device run out of license
 * * - require device info
 * * - authentication failed
 *
 * @param app
 *   Specify application name for app login.
 * @param barcode
 *   Specify user barcode for app login.
 * @param pass
 *   [OPTIONAL] Specify password for user login when authentication by password is turned on.
 * @param device
 *   Specify device name for app login.
 * @param note
 *   [OPTIONAL] Specify device note on first attemp  for app login.
 * @param status
 *   [OPTIONAL] If specified, returns current logged in user info, no authentication is performed.
 * @param logout
 *   [OPTIONAL] If specified, logs out current user.
 * @param desktop
 *   [OPTIONAL] If specified, login accepts user/pass combo.
 *
 */
function plusdelta_api_authenticate(){
  // add status flag support
  if (getVars('status')) {
    global $user;
    $res = 0;
    if ($user->uid) {
      $res = ($user->uid)?plusdelta_api_getUser('',$user->uid):0;
      $res = $res['data'];
    }
    $result = plusdelta_returnMsg('success','current logged in user info.',$res,'Auth');
    plusdelta_api_return($result);
  }
  // add status flag logout 
  if (getVars('logout')) {
    plusdelta_api_userLogout();
    $result = plusdelta_returnMsg('success','logged out successfully.');
    plusdelta_api_return($result);
  }
  
  $desktop = getVars('desktop');
  $authenticated = false;

  if (!$desktop) { 
    // handle device license authentication
    if ($device = getVars('device')) {
      $body = getVars('note');
      $returnCode = plusdelta_license_authenticate($device, $body);
      if ($returnCode < 0) {
        plusdelta_api_userLogout();
        if ($returnCode == -1) {
          $result = plusdelta_returnMsg('failure','device blocked');
        } else {        
          $result = plusdelta_returnMsg('failure','device run out of license');
        }
        plusdelta_api_return($result);
      }
    } else {
      plusdelta_api_userLogout();
      $result = plusdelta_returnMsg('failure','require device info');
      plusdelta_api_return($result);
    }
  }

  // handle regular authentication
  if (variable_get('plusdelta_require_authentication') or $desktop) {
    // $g = getVars(array('user','pass'));
    // $uid =  plusdelta_api_user_login($g['user'], $g['pass']);
    $g = getVars(array('barcode','pass'));
    $uid =  plusdelta_api_barcode_login($g['barcode'], $g['pass']);
    $authenticated = ($uid>0);
  } else {
    $g = getVars(array('app','barcode'));
    // check app key
    if ($g['app'] == variable_get('plusdelta_application_key')) {
      // check user barcode
      $result = plusdelta_api_getUser($g['barcode']);
      if ($result['status'] == 'success') {
        $authenticated = plusdelta_api_login_other($result['data']['uid']);
      }
    }
  }
  if ($authenticated) {
    $result = plusdelta_returnMsg('success','user authenticated.',array('session'=>session_id()));
    plusdelta_api_return($result);
  } else {
    plusdelta_api_userLogout();
      $result = plusdelta_returnMsg('failure','authentication failed');
    plusdelta_api_return($result);
  }
}

/**
 * Get app info
 */
function plusdelta_api_info() {
  $result = plusdelta_api_getAppSettings();
  $result['category'] = 'API';
  plusdelta_api_return($result);
}

function plusdelta_api_getContent($type, $barcode) {
  $node = plusdelta_api_getNodeByTitle($type,$barcode);
  if ($node) {
    $result['type'] = $type;
    $result['nid'] = $node->nid;
    $result['title'] = $node->title;
    $res = plusdelta_returnMsg('success','ok',$result);
  } else {
    $res = plusdelta_returnMsg('failure',$type . ' does not exist');
  }
  return $res;
}

/**
 * Reset the system data
 */
function plusdelta_api_reset() {
  watchdog('Pharmtrac','Database reset.');
  drupal_set_title('Reset contents');
  
  // delete some contents
  $types = array('med', 'patient', 'dispense', 'order', 'request');
  
  $query = new EntityFieldQuery();
  $res = $query->entityCondition('entity_type', 'node')
               ->propertyCondition('type', $types, 'IN')
               ->range(0,1000)
               ->execute();
  if (!empty($res['node'])) {
    node_delete_multiple(array_keys($res['node']));
    return count($res['node']). ' nodes deleted';
  }
    
  return 'nothing is deleted';
}

/**
 * Get device
 */
function plusdelta_api_getDevice($barcode,$nid =null) {
  if ($nid)
    $node = node_load($nid);
  else
    $node = plusdelta_api_getNodeByTitle('device',$barcode);

  if ($node) {
    $result['did'] = $node->nid;
    $result['title'] = $node->title;
    $result['status'] = getSet($node->field_device_status['und'][0]['value']);
    $res = plusdelta_returnMsg('success','ok',$result);
  } else {
    $res = plusdelta_returnMsg('failure','device does not exist');
  }
  return $res;
}

/**
 * Check if license can be used
 */
function plusdelta_license_authenticate($barcode, $note='') {
  //-1 = blocked device, -2 = out of licenses, 1 = good device
  // take a look if we have this device used in the past
  $can = plusdelta_license_canLicense();
  $node = plusdelta_api_getNodeByTitle('device',$barcode);
  $allowed = false;
  
  // let's figure out can we do it first
  if ($node) {
    $active = getSet($node->field_device_status['und'][0]['value']);
  
    switch ($active) {
      case 0: //inactive
        $allowed = ($can>1);
        break;
        
      case 1: //active
        $allowed = ($can>0);
        if (!$allowed) {
          $node->field_device_status['und'][0]['value'] = 0;
          node_save($node);
        }
        break;
      
      default://blocked
        return -1;
        break;
    }
  } else {
    $allowed = ($can>1);
  }
  
  // run out of license
  if (!$allowed) return -2;
  
  // for allowed license
  if ($node) {
    if (!$active) {
      $node->field_device_status['und'][0]['value'] = 1;
      node_save($node);
    }
  } else {
    $node = plusdelta_api_addNodeWithTitle('device',$barcode);
    $node->field_device_status['und'][0]['value'] = 1;
    $node->body['und'][0]['value'] = $note;
    node_save($node);
  }
  
  return 1;
}

/**
 * Check if license number does not run out
 */
function plusdelta_license_canLicense() {
  $query = new EntityFieldQuery();
  $curr = $query->entityCondition('entity_type', 'node')
      ->propertyCondition('type', 'device')
      ->fieldCondition('field_device_status', 'value', 1)
      ->count()
      ->execute();
  $total = variable_get('plusdelta_license_total');
  
  if ($curr < $total) return 2;  // 2 stands for more license
  return ($curr == $total); // 1 stands for enough license currently
}

/**
 * Rest API : Dispense
 * * api?command=dispense&id=barcode
 * * {"barcode":"123", "shortname": "haha"}
 *
 * @param closed
 *   [OPTIONAL] Specify order status is_closed
 *
 */
function pharmtrac_rest_dispense() {
  return pharmtrac_rest_api();
}

/**
 * Rest API : Request
 * * api?command=dispense&id=barcode
 * * {"barcode":"123", "shortname": "haha"}
 *
 * @param user
 *   [OPTIONAL] Specify barcode of user who issued the request
 * @param picture
 *   [OPTIONAL] Specify picture file id regex, use . for valid picture
 *
 */
function pharmtrac_rest_request() {
  return pharmtrac_rest_api();
}

/**
 * Rest API : Location
 * * api?command=location&id=barcode
 * * {"barcode":"123", "shortname": "haha"}
 *
 * @param barcode
 *   Specify barcode of location
 * @param shortname
 *   [OPTIONAL] Specify location shortname
 * @param property
 *   [OPTIONAL] Specify properties, ex. 'print, pickup'
 * @param room
 *   [OPTIONAL] Specify rooms, ex. 'R520, R543'
 *
 */
function pharmtrac_rest_location() {
  return pharmtrac_rest_api();
}

/**
 * Rest API : User
 * * api?command=user&id=name
 * * [POST] {"name": "fjin", "pass":"abc", "pass2":"abc", "mail":"abc@gmail.com", "barcode": "35ddtred" }
 * * [PUT]  {"name": "fjinb", "barcode": "35ddtreddxx", "mobilerole":"Pharmacist", "portalrole":"Pharmacy", "location":"2000" }
 *
 * @param type
 *   [OPTIONAL] Specify type to be 'user'
 * @param name
 *   Specify username
 * @param pass
 *   Specify password
 * @param pass2
 *   Specify confirmation password
 * @param mail
 *   Specify email
 * @param barcode
 *   Specify user barcode
 *
 */
function pharmtrac_rest_user() {
  return pharmtrac_rest_api();
}

/**
 * Rest API : Wallboard
 * * api?command=wallboard
 *
 */
function pharmtrac_rest_wallboard() {
  return pharmtrac_rest_api();
}
