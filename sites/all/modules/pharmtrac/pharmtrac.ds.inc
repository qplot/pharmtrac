<?php
/**
 * @file
 * pharmtrac.ds.inc
 */

/**
 * Implements hook_ds_field_settings_info().
 */
function pharmtrac_ds_field_settings_info() {
  $export = array();

  $ds_fieldsetting = new stdClass();
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|dispense|teaser';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'dispense';
  $ds_fieldsetting->view_mode = 'teaser';
  $ds_fieldsetting->settings = array(
    'title' => array(
      'weight' => '0',
      'label' => 'hidden',
      'format' => 'default',
    ),
  );
  $export['node|dispense|teaser'] = $ds_fieldsetting;

  return $export;
}

/**
 * Implements hook_ds_layout_settings_info().
 */
function pharmtrac_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|dispense|teaser';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'dispense';
  $ds_layout->view_mode = 'teaser';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'title',
        1 => 'field_dispense_nextrequesttype',
        2 => 'field_dispense_dropofflocation',
        3 => 'field_dispense_prioritylevel',
        4 => 'field_dispense_requests',
      ),
    ),
    'fields' => array(
      'title' => 'ds_content',
      'field_dispense_nextrequesttype' => 'ds_content',
      'field_dispense_dropofflocation' => 'ds_content',
      'field_dispense_prioritylevel' => 'ds_content',
      'field_dispense_requests' => 'ds_content',
    ),
    'classes' => array(),
    'wrappers' => array(
      'ds_content' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
  );
  $export['node|dispense|teaser'] = $ds_layout;

  $ds_layout = new stdClass();
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|order|teaser';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'order';
  $ds_layout->view_mode = 'teaser';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'regions' => array(
      'ds_content' => array(
        0 => 'field_order_dispenses',
      ),
    ),
    'fields' => array(
      'field_order_dispenses' => 'ds_content',
    ),
    'classes' => array(),
    'wrappers' => array(
      'ds_content' => 'div',
    ),
    'layout_wrapper' => 'div',
    'layout_attributes' => '',
    'layout_attributes_merge' => 1,
  );
  $export['node|order|teaser'] = $ds_layout;

  return $export;
}
