<?php
/**
 * @file
 * Code for the Pharmtrac process, ex. hl7 and api.
 */

include_once 'pharmtrac.api.inc';

/**
 * Menu items for process
 */
function pharmtrac_process_menu() {
  $items = array();
  
  $items['process'] = array(
    'title' => 'Process HL7 Message',
    'description' => 'Process HL7 Message',
    'page callback' => 'plusdelta_hl7_process',
    'page arguments' => array(),
    'access callback' => TRUE,
    // 'access arguments' => array(1,'hl7'),
    'type' => MENU_LOCAL_TASK,
  );

  $items['node/%node/process'] = array(
    'title' => 'Process',
    'description' => 'Process HL7 Message',
    'page callback' => 'plusdelta_hl7_process',
    'page arguments' => array(1),
    'access callback' => 'access_node_content_type',
    'access arguments' => array(1,'hl7'),
    'type' => MENU_LOCAL_TASK,
  );
  
  $items['api'] = array(
    'title' => 'API Result',
    'description' => 'API Calls',
    'page callback' => 'plusdelta_api',
    'page arguments' => array(),
    'access callback' => 'user_access',
    'access arguments' => array('access pharmtrac'),
    'type' => MENU_LOCAL_TASK,
  );
  
  $items['auth'] = array(
    'title' => 'API Result',
    'description' => 'API Calls',
    'page callback' => 'plusdelta_api_authenticate',
    'page arguments' => array(),
    'access callback' => TRUE,
    'type' => MENU_LOCAL_TASK,
  );
  
  $items['info'] = array(
    'title' => 'API Result',
    'description' => 'API Calls',
    'page callback' => 'plusdelta_api_info',
    'page arguments' => array(),
    'access callback' => TRUE,
    'type' => MENU_LOCAL_TASK,
  );
  
  $items['reset'] = array(
    'title' => 'Reset the content',
    'description' => 'Reset patient, order, dispense, request contents',
    'page callback' => 'plusdelta_api_reset',
    'page arguments' => array(),
    'access callback' => 'user_access',
    'access arguments' => array('administer content'),
    'type' => MENU_LOCAL_TASK,
  );
  
  $items['autoclose'] = array(
    'title' => 'Autoclose dispenses',
    'description' => '',
    'page callback' => 'plusdelta_api_autoCloseFast',
    'page arguments' => array(),
    'access callback' => TRUE,
    // 'access callback' => 'user_access',
    // 'access arguments' => array('access pharmtrac'),
    'type' => MENU_LOCAL_TASK,
  );
  
  
  return $items;
}

/**
 * HL7 handling
 * supports two ways of recieving data
 * a) $_POST, b) content node $message
 */
function plusdelta_hl7_process($message=NULL) {
  // for content message
  if ($message) {
    $json = getSet($message->body['und'][0]['value']);
  } else {
    $json = file_get_contents("php://input");
  }
  $json = json_decode($json);

  if (!isset($json->message_type)) {
    $result = plusdelta_returnMsg('failure','missing hl7 message type.', NULL, 'HL7');
    plusdelta_api_return($result, !empty($message));
  }

  switch ($json->message_type) {
    case 'HL7':
      $result = plusdelta_hl7_add_msg($json);
      break;
    case 'PS':
      $result = plusdelta_hl7_add_stream($json);
      break;
    case 'RQ':
      $result = plusdelta_hl7_add_request($json);
      break;
    case 'MED':
      $result = plusdelta_hl7_add_med($json);
      break;
    case 'BD':
      if (module_exists('pharmtrac_bundle') && 
        variable_get_value('pharmtrac_bundle_enabled')) {
          module_load_include('inc', 'pharmtrac_bundle', 'pharmtrac_bundle.api');
          $result = plusdelta_hl7_add_bundle($json);
      } else
        $result = plusdelta_returnMsg('failure','please enable bundle module for pharmtrac.');
      break;
    default:
      $result = plusdelta_returnMsg('failure','hl7 message type not recognized.');
      break;
  }
  $result['category'] = 'HL7';
  plusdelta_api_return($result, !empty($message));
}

/**
 * API handling based on for api?command=
 */
function plusdelta_api() {
  switch (getVars('command')) {
    case 'getUser':
      $result = plusdelta_api_getUser(getVars('barcode'));
      break;
      
    case 'getMobileRole':
      $result = plusdelta_api_getMobileRole(getVars('barcode'));
      break;
      
    case 'authenticate':
      $result = plusdelta_api_authenticate();
      $result['category'] = 'API.Auth';
      break;
      
    case 'getAppSettings':
      $result = plusdelta_api_getAppSettings();
      break;
      
    case 'getPatient': 
      $result = plusdelta_api_getPatient(getVars('barcode'));
      break;
      
    case 'getPatients': 
      $g = getVars(array('barcode','name'));
      $result = plusdelta_api_getPatients($g['barcode'],$g['name']);
      break;
      
    case 'getLocation': 
      $result = plusdelta_api_getLocation(getVars('barcode'));
      break;
      
    case 'getLocations':
      $g = getVars(array('room','type'));
      $result = plusdelta_api_getLocations($g['type'],$g['room']);
      break;
      
    case 'getDispense':
      $g = getVars('barcode');
      if (module_exists('pharmtrac_bundle')) {
        module_load_include('inc', 'pharmtrac_bundle', 'pharmtrac_bundle.api');
        if (plusdelta_api_isBundle($g))
          $result = plusdelta_api_getBundle($g);
        else
          $result = plusdelta_api_getDispense($g);
      } else
        $result = plusdelta_api_getDispense($g);
      break;
      
    case 'getBundle':
      if (module_exists('pharmtrac_bundle')) {
        module_load_include('inc', 'pharmtrac_bundle', 'pharmtrac_bundle.api');
        $result = plusdelta_api_getBundle(getVars('barcode'));
      } else 
        $result = plusdelta_returnMsg('failure','please install bundle module for pharmtrac.');
      break;
      
    case 'getDispenses':
      // $g = getVars(array('location','user','request'));
      $result = plusdelta_api_getDispenses();
      if (module_exists('pharmtrac_bundle')) {
        module_load_include('inc', 'pharmtrac_bundle', 'pharmtrac_bundle.api');
        $result2 = plusdelta_api_getBundles();
        if ($result2['status'] == 'failure') {
          $result['status'] == 'failure';
          $result['message'] .= '; ' . $result2['message'];
        } else
          $result['data'] = array_merge($result2['data'], $result['data']);
      }
      break;
      
    case 'getBundles':
      if (module_exists('pharmtrac_bundle')) {
        module_load_include('inc', 'pharmtrac_bundle', 'pharmtrac_bundle.api');
        $result = plusdelta_api_getBundles();
      } else 
        $result = plusdelta_returnMsg('failure','please install bundle module for pharmtrac.');
      break;

    case 'getOrder':
      $result = plusdelta_api_getOrder(getVars('barcode'));
      break;
      
    case 'getOrders':
      $result = plusdelta_api_getOrders(getVars('patient'));
      break;
  
    case 'createRequest':
      $handled = 0;
      $g = getFilteredInput(array(
        'barcode','user','location','type',
        'device','note','user2','override'
      ), INPUT_POST);
      
      // handle ndc
      if (module_exists('pharmtrac_ndc')) 
        $g = array_merge($g, getFilteredInput(array(
          'ndc', 'ndc_override'
        ), INPUT_POST));
      
      // handle bundle
      if (module_exists('pharmtrac_bundle')) {
        $g = array_merge($g, getFilteredInput(array(
          'bundle'
        ), INPUT_POST));
      }
      
      $result = plusdelta_api_createRequest($g);
      break;
      
    case 'createBundle':
      if (module_exists('pharmtrac_bundle') && 
        variable_get_value('pharmtrac_bundle_enabled')) { 
        $g = getFilteredInput(array(
          'barcode','dispenses'
        ), INPUT_POST);
        module_load_include('inc', 'pharmtrac_bundle', 'pharmtrac_bundle.api');
        $result = plusdelta_api_createBundle($g);
      } else 
        $result = plusdelta_returnMsg('failure','please enable bundle module for pharmtrac.');
      break;
      
    case 'getRequests':
      $result = plusdelta_api_getRequests(getVars('barcode'));
      break;
    
    case 'getRequestType':
      $result = plusdelta_api_getRequestType(getVars('barcode'));
      break;
      
    case 'getLocationProperty':
      $result = plusdelta_api_getLocationProperty(getVars('barcode'));
      break;

    case 'getMedFeatureType':
      $result = plusdelta_api_getMedFeatureType(getVars('barcode'));
      break;

    case 'getMed':
        $result = plusdelta_api_getMed(getVars('barcode'));
      break;
      
    case 'getDevice':
      $result = plusdelta_api_getDevice(getVars('barcode'));
      break;
      
    case 'getContent':
      $g = getVars(array('barcode','type'));
      $result = plusdelta_api_getContent($g['type'],$g['barcode']);
      break;
      
    // case 'getWallboard':
    //   $result = plusdelta_api_getWallboard(getVars('barcode'));
    //   break;
  
    case 'getWallboard2':
      $result = plusdelta_api_getWallboard2(getVars('barcode'));
      $result['category'] = 'API.Wallboard';
      break;
      
    case 'getWallboardSettings':
      $result = plusdelta_api_getWallboardSettings(getVars('barcode'));
      break;
      
    case 'setWallboardSettings':
      $result = plusdelta_api_setWallboardSettings(getVars('barcode'));
      break;
      
    case 'getReport':
      $g = getVars(array('barcode','from','to','interval','priority','prepare','dropoff','shift'));
      $result = plusdelta_api_getReport2($g['barcode'],$g['interval'],$g['from'],
                  $g['to'],$g['priority'],$g['prepare'],$g['dropoff'],$g['shift']);
      break;
      
    case 'getReportRequest':
      $g = getVars(array('from','to','interval'));
      $result = plusdelta_api_getReportRequest($g['interval'],$g['from'],$g['to']);
      break;
      
    case 'getReportSettings':
      $result = plusdelta_api_getReportSettings();
      break;
    
    case 'getShifts':
      $result = plusdelta_api_getShifts();
      break;
    
    case 'getCurrentShifts':
      $result = plusdelta_api_getCurrentShifts();
      break;
      
    case 'getPriorityLevels':
      $result = plusdelta_api_getPriorityLevels();
      break;
      
    case 'getRejectTypes':
      $result = plusdelta_api_getRejectTypes();
      break;
      
    case 'closeDispense':
      $result = plsudelta_api_closeDispense(getVars('barcode'));
      break;
    
    // transition to rest api   
    case 'rest':
      $result = pharmtrac_rest_api();
      break;
      
    case 'dispense':
      $result = pharmtrac_rest_dispense();
      break;
    
    case 'request':
      $result = pharmtrac_rest_request();
      break;

    case 'location':
      $result = pharmtrac_rest_location();
      break;
      
    case 'user':
      $result = pharmtrac_rest_user();
      break;

    case 'wallboard':
      $result = pharmtrac_rest_wallboard();
      break;

    default:
      $result = plusdelta_returnMsg('failure','api call does not exist');
      break;
  }
  plusdelta_api_return($result);
} // end plus_delta api

/**
 * Pharmtrac rest server integration
 * * map /rest/pharmtrac/{type}/{id} to /api?command={type}
 * * if {id} is set to string 'key', key needs to be specified in data
 */
function pharmtrac_rest_api() {
  $_oldq = $_GET['q'];
  
  $_GET['q'] = 'rest/pharmtrac/' . $_GET['command'];

  $suc_msg = '';
  switch ($_SERVER['REQUEST_METHOD']) {
    case 'GET':   // retrieve or index
      if (isset($_GET['id'])) {
        $suc_msg = 'retrieve';
        // retrieve
        $_GET['q'] = $_GET['q'] . '/' . $_GET['id'];
      
        if (isset($_GET['key']))
          $_GET['q'] = $_GET['q'] . '/' . $_GET['key'];
      } else {
        $suc_msg = 'list';
        // index
      }
      break;

    case 'POST':  // create
      $suc_msg = 'create';
      break;

    case 'PUT':   // update
      if (empty($_GET['id'])) 
        return plusdelta_returnMsg('failure', 'missing id or key');
      $_GET['q'] = $_GET['q'] . '/' . $_GET['id'];
      $suc_msg = 'update';
      break;

    default:
      break;
  }
  $_GET['q'] = $_GET['q'] . '.php';
  $suc_msg = $suc_msg . ' ' . $_GET['command'] . ' succesfully.';

  $endpoint_name = 'rest';
  module_load_include('runtime.inc', 'services');
  $endpoint = services_endpoint_load($endpoint_name);
  $server = $endpoint->server;

  // call the server
  services_set_server_info_from_array(array(
    'module'        => $server,
    'endpoint'      => $endpoint_name,
    'endpoint_path' => $endpoint->path,
    'debug'         => $endpoint->debug,
    'settings'      => $endpoint->server_settings,
  ));
  
  try {
    $data = call_user_func($server . '_server');
  } catch (Exception $e) {
    return plusdelta_returnMsg('failure', $e->getMessage());
  };

  $_GET['q'] = $_oldq;
  return plusdelta_returnMsg('success', $suc_msg, unserialize($data));
  
}
