<?php

/**
 * @file
 * This file serves as base API functionalities.
 */

/**
 * check if the dispense is bundle
 */
function plusdelta_api_isBundle($barcode) {
  $nid = getEntityValue(array('type' => 'bundle', 'title' => $barcode));
  return !empty($nid);
}

/**
 * API for fetch bundle
 */
function plusdelta_api_getBundle($barcode, $nid=null) {
  if ($nid)
    $node = getEntity(array('type' => 'bundle', 'nid' => $nid));
  else
    $node = getEntity(array('type' => 'bundle', 'title' => $barcode));
  
  if (!empty($node)) {
    $res = array(
      'barcode' => $node->title,
      'bundle'=> 1,
      'bid' => $node->nid,
      'dueTimeStamp' => 0, 
      'priorityLevel' => '', 
      'dispenses' => array()
    );
    $res['dueTimeStamp'] = 100000000000;
    foreach ($node->field_bundle_dispenses['und'] as $value) {
      $re = plusdelta_api_getDispense('', $value['target_id']);
      $res['dispenses'][] = $re['data'];
      if (intval($re['data']['dueTimeStamp']) < $res['dueTimeStamp']) 
        $res['dueTimeStamp'] = intval($re['data']['dueTimeStamp']);
      if (isset($re['data']['priorityLevel']))
        $res['priorityLevel'] = $re['data']['priorityLevel'];
    }
    return plusdelta_returnMsg('success', 'ok', $res);
  } else
    return plusdelta_returnMsg('failure', 'bundle does not exist');
}

/**
 * API for create bundle
 */
function plusdelta_api_createBundle($op) {
  // make sure we have barcode and dispenses
  if (empty($op['barcode']) || empty($op['dispenses']))
    return plusdelta_returnMsg('failure', 'missing info creating bundle');

  // make sure the bundle does not exist
  if (getEntityValue(array('type' => 'bundle', 'title' => $op['barcode'])))
    return plusdelta_returnMsg('failure', 'bundle already exists');
  
  $node = new StdClass();
  $node->type = 'bundle';
  $node->language = 'und';
  node_object_prepare($node);
  $node->title = $op['barcode'];

  $dispenses = array_map('trim', explode(',', $op['dispenses']));
  foreach ($dispenses as $value) {
    if ($nid = getEntityValue(array(
      'type' => 'dispense',
      'title' => $value
    )))
      $node->field_bundle_dispenses['und'][] = array('target_id' => $nid);
    else
      return plusdelta_returnMsg('failure', 'dispense does not exist for bundle');
  }

  if ($node = node_submit($node)) { 
    node_save($node);
    return plusdelta_returnMsg('success', 'bundle created succesfully');
  }

  return plusdelta_returnMsg('failure','bundle save failed');
}

/**
 * API for creating bundle request
 */
function plusdelta_api_createBundleRequest($op) {
  // go through each dispense and make a requeset
  $op2 = $op;
  unset($op2['bundle']);
  
  if ($items = getEntityValue(array('type' => 'bundle',
    'title' => $op2['barcode']), 'field_bundle_dispenses')) {
    $res = array();
    foreach ($items['und'] as $value) {
      $op2['barcode'] = getEntityValue(array('type' => 'dispense',
        'nid' =>$value['target_id']), 'title');
      $re = plusdelta_api_createRequest($op2);
      $re['data'] = array_merge($re['data'], array('dispense' => $op2['barcode']));
      $res[] = $re;
    }
    return plusdelta_returnMsgs($res);
  } else 
    return plusdelta_returnMsg('failure','bundle does not exist');
}

/**
 * API for get bundles
 */
function plusdelta_api_getBundles() {
  $items = array();
  
  module_load_include('inc','services_views','services_views.resource');
  $items = services_views_retrieve('services_bundle_api', 'services_1', array(), 0, 25);
  $res = array();
  foreach ($items as $node) {
    $re = plusdelta_api_getBundle('',$node->nid);
    $res[] = $re['data'];
  }
  
  return plusdelta_returnMsg('success','ok',$res);
}

/**
 * Add hl7 bundle
 */
function plusdelta_hl7_add_bundle($json) {
  if(!isset($json->bundle_barcode_id_ps)||
    !isset($json->bundle_dispense_ids_ps))
    return plusdelta_returnMsg('failure','bundle missing barcode');
      
  $g = array(
    'barcode' => $json->bundle_barcode_id_ps,
    'dispenses' => $json->bundle_dispense_ids_ps
  );
  return plusdelta_api_createBundle($g);
}

/**
 * Assemble multiple return message
 */
function plusdelta_returnMsgs($msgs) {
  $result = plusdelta_returnMsg('success', '');
  foreach ($msgs as $msg) {
    if ($msg['status'] == 'failure') 
      $result['status'] = 'failure';
  }
  $result['data'] = $msgs;
  $result['category'] = 'bundle';
  return $result;
}
