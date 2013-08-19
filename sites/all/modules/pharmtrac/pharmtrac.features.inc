<?php
/**
 * @file
 * pharmtrac.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function pharmtrac_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  list($module, $api) = func_get_args();
  if ($module == "ds" && $api == "ds") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "services" && $api == "services") {
    return array("version" => "3");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function pharmtrac_views_api() {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function pharmtrac_node_info() {
  $items = array(
    'color' => array(
      'name' => t('Color'),
      'base' => 'node_content',
      'description' => t('Pharmtrac color options'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'device' => array(
      'name' => t('Device'),
      'base' => 'node_content',
      'description' => t('Use <em>Device</em> to track added iphone unit.'),
      'has_title' => '1',
      'title_label' => t('Device ID'),
      'help' => '',
    ),
    'dispense' => array(
      'name' => t('Dispense'),
      'base' => 'node_content',
      'description' => t('PharmTrac Dispense, track dispense information'),
      'has_title' => '1',
      'title_label' => t('Dispense number'),
      'help' => '',
    ),
    'hl7' => array(
      'name' => t('HL7'),
      'base' => 'node_content',
      'description' => t('PharmTrac HL7 message includes raw message and extracted info. '),
      'has_title' => '1',
      'title_label' => t('Message number'),
      'help' => '',
    ),
    'location' => array(
      'name' => t('Location'),
      'base' => 'node_content',
      'description' => t('PharmTrac Location, used for track different location of device scan'),
      'has_title' => '1',
      'title_label' => t('Location Number'),
      'help' => '',
    ),
    'location_properties' => array(
      'name' => t('Location Properties'),
      'base' => 'node_content',
      'description' => t('Location properties list various properties for each location.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'med' => array(
      'name' => t('Med'),
      'base' => 'node_content',
      'description' => t('Med stores all medications'),
      'has_title' => '1',
      'title_label' => t('Identifier'),
      'help' => '',
    ),
    'medfeature' => array(
      'name' => t('Med Feature'),
      'base' => 'node_content',
      'description' => t('Med feature lists all features for medications'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
    'mobilefeature' => array(
      'name' => t('Mobile Features'),
      'base' => 'node_content',
      'description' => t('Mobile feature list all features for mobile device'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
    'mobilerole' => array(
      'name' => t('Mobile Role'),
      'base' => 'node_content',
      'description' => t('Mobile role groups mobile features as a role'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'order' => array(
      'name' => t('Order'),
      'base' => 'node_content',
      'description' => t('PharmTrac Order, used for tracking order, patient'),
      'has_title' => '1',
      'title_label' => t('Order Number'),
      'help' => '',
    ),
    'patient' => array(
      'name' => t('Patient'),
      'base' => 'node_content',
      'description' => t('PharmTrac Patient, record patient account and his info.'),
      'has_title' => '1',
      'title_label' => t('Account Number'),
      'help' => t('PharmTrac Patient, record patient account and his info.'),
    ),
    'portal_feature' => array(
      'name' => t('Portal Feature'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'portal_role' => array(
      'name' => t('Portal Role'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'priority_level' => array(
      'name' => t('Priority Level'),
      'base' => 'node_content',
      'description' => t('List of priority levels'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'quantity' => array(
      'name' => t('Quantity'),
      'base' => 'node_content',
      'description' => t('Pharmtrac quantity which tracks the meds quantity and units for orders.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'reject_type' => array(
      'name' => t('Reject Type'),
      'base' => 'node_content',
      'description' => t('List of reject types'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'request' => array(
      'name' => t('Request'),
      'base' => 'node_content',
      'description' => t('PharmTracPD request content type, including preps, prints, verifys, pickups and dropoff'),
      'has_title' => '1',
      'title_label' => t('Request Number'),
      'help' => '',
    ),
    'requesttype' => array(
      'name' => t('Request Type'),
      'base' => 'node_content',
      'description' => t('List of request types'),
      'has_title' => '1',
      'title_label' => t('Type Name'),
      'help' => '',
    ),
    'room' => array(
      'name' => t('Room'),
      'base' => 'node_content',
      'description' => t('List of rooms hosted by the hospital'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'shift' => array(
      'name' => t('Shift'),
      'base' => 'node_content',
      'description' => t('List of shifts by nurses'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'wallboard' => array(
      'name' => t('Wallboard'),
      'base' => 'node_content',
      'description' => t('PharmTrac Order, used for store wallboard options, prepare locations etc.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'wallboard_color' => array(
      'name' => t('Wallboard Color'),
      'base' => 'node_content',
      'description' => t('Pharmtrac wallboard color setting, storing wallboard color and threshold settings '),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'wallboard_column' => array(
      'name' => t('Wallboard Column'),
      'base' => 'node_content',
      'description' => t('Pharmtrac wallboard columns for wallboard display.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'wallboard_footer_color' => array(
      'name' => t('Wallboard Footer Color'),
      'base' => 'node_content',
      'description' => t('Use <em>Wallboard Footer Color</em> to add footer color size threshold element to wallboard footer setting'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
