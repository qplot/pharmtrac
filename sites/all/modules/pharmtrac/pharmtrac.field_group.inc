<?php
/**
 * @file
 * pharmtrac.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function pharmtrac_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_order_details|node|order|default';
  $field_group->group_name = 'group_order_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'order';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Order Details',
    'weight' => '6',
    'children' => array(
      0 => 'field_order_control',
      1 => 'field_order_sys',
      2 => 'field_order_enteredts',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Order Details',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_order_details|node|order|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_order_details|node|order|form';
  $field_group->group_name = 'group_order_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'order';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Order Details',
    'weight' => '5',
    'children' => array(
      0 => 'field_order_control',
      1 => 'field_order_sys',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Order Details',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_order_details|node|order|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_order_disp_and_req|node|order|default';
  $field_group->group_name = 'group_order_disp_and_req';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'order';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Dispenses and Requests',
    'weight' => '3',
    'children' => array(
      0 => 'field_order_dispenses',
      1 => 'field_order_requests',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_order_disp_and_req|node|order|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_order_disp_and_req|node|order|form';
  $field_group->group_name = 'group_order_disp_and_req';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'order';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Dispenses and Requests',
    'weight' => '4',
    'children' => array(
      0 => 'field_order_dispenses',
      1 => 'field_order_requests',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Dispenses and Requests',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_order_disp_and_req|node|order|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_order_medicine|node|order|default';
  $field_group->group_name = 'group_order_medicine';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'order';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Medicine',
    'weight' => '7',
    'children' => array(
      0 => 'field_order_orderorg',
      1 => 'field_order_qty',
      2 => 'field_order_interval',
      3 => 'field_order_duration',
      4 => 'field_order_priority',
      5 => 'field_order_condition',
      6 => 'field_order_secondary',
      7 => 'field_order_seq',
      8 => 'field_order_amt',
      9 => 'field_order_giveamt',
      10 => 'field_order_givelabel',
      11 => 'field_order_admininstruct',
      12 => 'field_order_dispamt',
      13 => 'field_order_dispunit',
      14 => 'field_order_verifyid',
      15 => 'field_order_verifylastname',
      16 => 'field_order_verifyfirstname',
      17 => 'field_order_dispinstruct',
      18 => 'field_order_ivgiverateamount',
      19 => 'field_order_ivgiverateunits',
      20 => 'field_order_disptype',
      21 => 'field_order_startts',
      22 => 'field_order_endts',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Medicine',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_order_medicine|node|order|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_order_medicine|node|order|form';
  $field_group->group_name = 'group_order_medicine';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'order';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Medicine',
    'weight' => '1',
    'children' => array(
      0 => 'field_order_admininstruct',
      1 => 'field_order_amt',
      2 => 'field_order_condition',
      3 => 'field_order_dispamt',
      4 => 'field_order_dispinstruct',
      5 => 'field_order_disptype',
      6 => 'field_order_dispunit',
      7 => 'field_order_duration',
      8 => 'field_order_endts',
      9 => 'field_order_giveamt',
      10 => 'field_order_givelabel',
      11 => 'field_order_interval',
      12 => 'field_order_ivgiverateamount',
      13 => 'field_order_ivgiverateunits',
      14 => 'field_order_orderorg',
      15 => 'field_order_priority',
      16 => 'field_order_qty',
      17 => 'field_order_secondary',
      18 => 'field_order_seq',
      19 => 'field_order_startts',
      20 => 'field_order_treatment',
      21 => 'field_order_verifyfirstname',
      22 => 'field_order_verifyid',
      23 => 'field_order_verifylastname',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Medicine',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_order_medicine|node|order|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_order_messageinfo|node|order|default';
  $field_group->group_name = 'group_order_messageinfo';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'order';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Message Info',
    'weight' => '4',
    'children' => array(
      0 => 'field_order_message',
      1 => 'field_order_messagetype',
      2 => 'field_order_messagectrlid',
      3 => 'field_order_messageverid',
      4 => 'field_order_interfacemessageid',
      5 => 'field_order_interfacemessagets',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Message Info',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_order_messageinfo|node|order|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_order_messageinfo|node|order|form';
  $field_group->group_name = 'group_order_messageinfo';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'order';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Message Info',
    'weight' => '3',
    'children' => array(
      0 => 'field_order_interfacemessageid',
      1 => 'field_order_interfacemessagets',
      2 => 'field_order_message',
      3 => 'field_order_messagectrlid',
      4 => 'field_order_messagetype',
      5 => 'field_order_messageverid',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Message Info',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_order_messageinfo|node|order|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_order_route|node|order|default';
  $field_group->group_name = 'group_order_route';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'order';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Route',
    'weight' => '9',
    'children' => array(
      0 => 'field_order_routecode',
      1 => 'field_order_routedescrip',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Route',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_order_route|node|order|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_order_route|node|order|form';
  $field_group->group_name = 'group_order_route';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'order';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Route',
    'weight' => '7',
    'children' => array(
      0 => 'field_order_routecode',
      1 => 'field_order_routedescrip',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Route',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_order_route|node|order|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_order_staff|node|order|default';
  $field_group->group_name = 'group_order_staff';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'order';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Staff',
    'weight' => '8',
    'children' => array(
      0 => 'field_order_enterid',
      1 => 'field_order_enterlastname',
      2 => 'field_order_enterfirstname',
      3 => 'field_order_verid',
      4 => 'field_order_verlastname',
      5 => 'field_order_verfirstname',
      6 => 'field_order_orderbyid',
      7 => 'field_order_orderbylastname',
      8 => 'field_order_orderbyfirstname',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Staff',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_order_staff|node|order|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_order_staff|node|order|form';
  $field_group->group_name = 'group_order_staff';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'order';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Staff',
    'weight' => '6',
    'children' => array(
      0 => 'field_order_enterfirstname',
      1 => 'field_order_enterid',
      2 => 'field_order_enterlastname',
      3 => 'field_order_orderbyfirstname',
      4 => 'field_order_orderbyid',
      5 => 'field_order_orderbylastname',
      6 => 'field_order_verfirstname',
      7 => 'field_order_verid',
      8 => 'field_order_verlastname',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Staff',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_order_staff|node|order|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_patient_address|node|patient|default';
  $field_group->group_name = 'group_patient_address';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'patient';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Address',
    'weight' => '8',
    'children' => array(
      0 => 'field_patient_address1',
      1 => 'field_patient_address2',
      2 => 'field_patient_city',
      3 => 'field_patient_state',
      4 => 'field_patient_zip',
      5 => 'field_patient_country',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Address',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_patient_address|node|patient|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_patient_address|node|patient|form';
  $field_group->group_name = 'group_patient_address';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'patient';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Address',
    'weight' => '4',
    'children' => array(
      0 => 'field_patient_address1',
      1 => 'field_patient_address2',
      2 => 'field_patient_city',
      3 => 'field_patient_state',
      4 => 'field_patient_zip',
      5 => 'field_patient_country',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_patient_address|node|patient|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_patient_basic|node|patient|default';
  $field_group->group_name = 'group_patient_basic';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'patient';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Basic',
    'weight' => '1',
    'children' => array(
      0 => 'field_patient_firstname',
      1 => 'field_patient_lastname',
      2 => 'field_patient_dob',
      3 => 'field_patient_sex',
      4 => 'field_patient_middlename',
      5 => 'field_patient_prefix',
      6 => 'field_patient_suffix',
      7 => 'field_patient_race',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_patient_basic|node|patient|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_patient_basic|node|patient|form';
  $field_group->group_name = 'group_patient_basic';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'patient';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Basic',
    'weight' => '1',
    'children' => array(
      0 => 'field_patient_firstname',
      1 => 'field_patient_lastname',
      2 => 'field_patient_dob',
      3 => 'field_patient_sex',
      4 => 'field_patient_middlename',
      5 => 'field_patient_prefix',
      6 => 'field_patient_suffix',
      7 => 'field_patient_race',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_patient_basic|node|patient|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_patient_location|node|order|default';
  $field_group->group_name = 'group_patient_location';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'order';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Patient Location',
    'weight' => '5',
    'children' => array(
      0 => 'field_order_patientclass',
      1 => 'field_order_patientunit',
      2 => 'field_order_patientroom',
      3 => 'field_order_patientbed',
      4 => 'field_order_readid',
      5 => 'field_order_adsource',
      6 => 'field_order_visitnum',
      7 => 'field_order_adts',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Patient Location',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_patient_location|node|order|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_patient_location|node|order|form';
  $field_group->group_name = 'group_patient_location';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'order';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Patient Location',
    'weight' => '2',
    'children' => array(
      0 => 'field_order_adsource',
      1 => 'field_order_adts',
      2 => 'field_order_patient',
      3 => 'field_order_patientbed',
      4 => 'field_order_patientclass',
      5 => 'field_order_patientroom',
      6 => 'field_order_patientunit',
      7 => 'field_order_readid',
      8 => 'field_order_visitnum',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Patient Location',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_patient_location|node|order|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_patient_orders1|node|patient|default';
  $field_group->group_name = 'group_patient_orders1';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'patient';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Orders',
    'weight' => '2',
    'children' => array(
      0 => 'field_patient_orders',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Orders',
      'instance_settings' => array(
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_patient_orders1|node|patient|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_patient_orders1|node|patient|form';
  $field_group->group_name = 'group_patient_orders1';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'patient';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Orders',
    'weight' => '2',
    'children' => array(
      0 => 'field_patient_orders',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Orders',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsible',
    ),
  );
  $export['group_patient_orders1|node|patient|form'] = $field_group;

  return $export;
}
