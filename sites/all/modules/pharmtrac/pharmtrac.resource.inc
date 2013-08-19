<?php

/**
 * @file
 * This file serves as base API functionalities.
 */

/**
 * Set php value by path
 */
function set_by_path(&$data, $path, $value) {
  $temp = &$data;
  $exploded = explode(".", $path);
  foreach ($exploded as $key) {
    $temp = &$temp[$key];
  }
  $temp = $value;
  unset($temp);
}

/**
 * Find entity id by keys 
 */
function get_entity_id($type, $keys) {
  $id = 0;
  switch ($type) {
    case 'user':
      $id = getEntityValue($keys, 'uid', 'user');
      break;
    
    default:
      $id = getEntityValue(array_merge(array('type' => $type), $keys));
      break;
  }

  return $id;
}

/**
 * Prehandle the name mapping
 */
function pharmtrac_services_mapping(&$node) {
  $pharmtrac_mapping = array(
    '0' => array(
      'barcode' => 'title',
      'note' => 'body.und.0.value'
    ),
    'room' => array(
    ),
    'location' => array(
      'shortname' => 'field_location_shortname.und.0.value',
      'property' => 'field_location_properties.und',
      'room' => 'field_location_rooms.und',
      // 'room' => 'field_location_rooms.und.0.target_id'
    ),
    'user' => array(
      'barcode' => 'field_user_barcode.value',
      'firstname' => 'field_user_firstname.value',
      'lastname' => 'field_user_lastname.value',
      'mobilerole' => 'field_user_mobilerole.target_id',
      // 'portalrole' => 'field_user_portalrole.und',
      // 'location' => 'field_user_location.und',
    ),
    'dispense' => array(
      'closed' => 'field_dispense_isclosed.und.0.value',
    )
  );
  $maps = array('0');
  if (isset($node['type']))
    if (isset($pharmtrac_mapping[$node['type']]))
      $maps[] = $node['type'];
  foreach ($node as $key => $value) {
    // $key - raw field name
    // $value - raw field value(s)
    
    // find map key
    $map_key = '';
    foreach ($maps as $map) {
      if (array_key_exists($key, $pharmtrac_mapping[$map]))
        $map_key = $pharmtrac_mapping[$map][$key];
    }
    
    // assign values
    if ($map_key) {
      // delete the old defination
      unset($node[$key]);
        
      // setup the key/values pair for field
      $keys = explode('.', $pharmtrac_mapping[$map][$key]);
      $field_name = $keys[0];
      $key_name = $keys[1];
      $values_raw = explode(',', $value);
      $values = array();
        
      foreach ($values_raw as $value) {
        switch ($key_name) {
          // entity reference 
          case 'target_id':
            $field = field_info_field($field_name);
            $handler = entityreference_get_selection_handler($field);
            if ($entities = $handler->getReferencableEntities($value)) {
              foreach ($entities as $bundle) {
                foreach ($bundle as $id => $item) {
                  // skip target_id key for checkbox ?
                  // $values[] = array($key_name => $id);
                  $values[] = $id;
                }
              }
            }
            break;
          
          default:    // value
            $values[] = array($key_name => $value);
            break;
        }
      }
      // print_r($values);  
      set_by_path($node, $field_name . '.und', $values);
        
      // if (is_array($value)) {
      //   // split the map string
      //   // separate into two parts, field name/language name and value name
      //   $keys = explode('.0.',$pharmtrac_mapping[$map][$key]);
      //   $values = array();
      //   if (count($keys) > 1) {
      //     foreach ($value as $v) {
      //       $values[] = array($keys[1] => $v);
      //     }
      //     set_by_path($node, $keys[0], $values);
      //   }
      // } else
      //   set_by_path($node, $pharmtrac_mapping[$map][$key], $value);
    }
  }
}


/**
 * Based on key value of reference field
 * return 
 * currently this is not a general approach
 * most of the reference and field referenced are hard-coded
 */
function pharmtrac_services_getReferenceValue($key, $value, $fieldname = 'title') {
  $result = $value;
  switch ($key) {
    case 'target_id':
    case 'nid':
      $result = getEntityValue(array('nid' => $value), $fieldname);
      break;

    case 'uid':
    case 'target_id[uid]':
      // hard code user for now
      $user = getEntityValue(array('uid' => $value), 'field_user_barcode', 'user');
      if (isset($user['und'][0]['value'])) 
        $result = $user['und'][0]['value'];
      break;
  }
  return $result;
}

/**
 * Given fieldkey
 * return fieldname and keyname
 */
function getFieldKeyName($fieldkey) {
  $keys = explode('.', $fieldkey);
  switch(count($keys)) {
    case 1:
      return array($keys[0], '');
      break;
    case 2:
      return $keys;
  }
  return $keys;
}

/**
 * Based on field_name.field_key combo
 * return values
 */
function pharmtrac_services_getFieldValue($node, $fieldkey) {
  list($field_name, $key_name) = getFieldKeyName($fieldkey);

  $values = array();
  if (empty($key_name)) {
    if (isset($node->{$field_name})) 
      $values[] = pharmtrac_services_getReferenceValue($field_name, $node->{$field_name});
  } else {
    // find out if there's a value for that field
    if (isset($node->{$field_name}['und'][0][$key_name])) {
      foreach ($node->{$field_name}['und'] as $fvalue) {
        $values[] = pharmtrac_services_getReferenceValue($key_name, $fvalue[$key_name]);
      }
    }
  }
  return implode(',', $values);
}

/**
 * Posthandle the name mapping
 * Syntax:
 * field_name.field_key>target_fieldname.target_fieldkey
 */
function pharmtrac_services_rmapping($node) {
  $pharmtrac_mapping = array(
    '0' => array(
      'barcode' => 'title',
      'note' => 'body.und.0.value'
    ),
    'room' => array(
    ),
    'location' => array(
      'shortname' => 'field_location_shortname.und.0.value',
      'property' => 'field_location_properties.und',
      'room' => 'field_location_rooms.und',
      // 'room' => 'field_location_rooms.und.0.target_id'
    ),
    'user' => array(
      'barcode' => 'field_user_barcode.value',
      'firstname' => 'field_user_firstname.value',
      'lastname' => 'field_user_lastname.value',
      'mobilerole' => 'field_user_mobilerole.target_id',
      'location' => 'field_user_location.target_id',
    ),
    'dispense' => array(
      'closed' => 'field_dispense_isclosed.und.0.value',
    ),
    'request' => array(
      'dispense' => 'field_request_dispense.nid',
      'device' => 'field_request_device.target_id',
      'user' => 'uid',
      'requestType' => 'field_request_requesttype.target_id',
      'deliveryUser' => 'field_request_deliveryuser.target_id',
      'location' => 'field_request_location.target_id',
      'isOverride' => 'field_request_isoverride.value',
      'isVerified' => 'field_request_isverified.value',
      'attempt' => 'field_request_attempt.value',
      'duration' => 'field_request_duration.value',
      'picture' => 'field_request_picture.uri',
      'note' => 'body.value',
    )
  );
  
  $nn = array();
  $map = $pharmtrac_mapping[0];
  if (!empty($node->type))
    if (isset($pharmtrac_mapping[$node->type]))
      $map = $pharmtrac_mapping[$node->type];
  $stored_keys = array(); 
  foreach ($map as $key => $value) {
    $nn[$key] = pharmtrac_services_getFieldValue($node, $value);
  }
  return $nn;
}

/**
 * Implement hook_services_resources()
 */
function pharmtrac_process_resources() {
  $resources['pharmtrac'] = array();

  $resources['pharmtrac']['index'] = array(
    'help' => 'Get content list.',
    'file' => array('type' => 'inc', 'module' => 'pharmtrac', 'name' => 'pharmtrac.resource'),
    'callback' => 'pharmtrac_services_index',
    'access callback' => 'user_access',
    'access arguments' => array('access pharmtrac'),
    'access arguments append' => FALSE,
    'args' => array(
      array(
        'name' => 'type',
        'optional' => FALSE,
        'type' => 'string',
        'description' => 'The type of the content to get',
        'default value' => '*',
        'source' => array('path' => 0),
      ),
      array(
        'name' => 'page',
        'optional' => TRUE,
        'type' => 'int',
        'description' => 'The zero-based index of the page to get, defaults to 0.',
        'default value' => 0,
        'source' => array('param' => 'page'),
      ),
      array(
        'name' => 'fields',
        'optional' => TRUE,
        'type' => 'string',
        'description' => 'The fields to get.',
        'default value' => '*',
        'source' => array('param' => 'fields'),
      ),
      array(
        'name' => 'parameters',
        'optional' => TRUE,
        'type' => 'array',
        'description' => 'Parameters array',
        'default value' => array(),
        'source' => array('param' => 'parameters'),
      ),
      array(
        'name' => 'pagesize',
        'optional' => TRUE,
        'type' => 'int',
        'description' => 'Number of records to get per page.',
        'default value' => 20,
        'source' => array('param' => 'pagesize'),
      ),
    ),
  );
  $resources['pharmtrac']['retrieve'] = array(
    'help' => 'Fetch a content.',
    'file' => array('type' => 'inc', 'module' => 'pharmtrac', 'name' => 'pharmtrac.resource'),
    'callback' => 'pharmtrac_services_retrieve',
    'access callback' => 'user_access',
    'access arguments' => array('access pharmtrac'),
    'access arguments append' => FALSE,
    'args' => array(
      array(
        'name' => 'type',
        'optional' => FALSE,
        'type' => 'string',
        'description' => 'The type of the content to get',
        'default value' => '*',
        'source' => array('path' => 0),
      ),
      array(
        'name' => 'id',
        'optional' => FALSE,
        'source' => array('path' => 1),
        'type' => 'string',
        'default value' => '',
        'description' => 'The entity id or entity key value to get',
      ),
      array(
        'name' => 'key',
        'optional' => TRUE,
        'source' => array('path' => 2),
        'description' => 'The entity key which identifies the entity',
        'type' => 'string',
        'default value' => 'id',
      ),
    ),
  );
  $resources['pharmtrac']['create'] = array(
    'help' => 'Create a content.',
    'file' => array('type' => 'inc', 'module' => 'pharmtrac', 'name' => 'pharmtrac.resource'),
    'callback' => 'pharmtrac_services_create',
    'access callback' => 'user_access',
    'access arguments' => array('access pharmtrac'),
    'access arguments append' => FALSE,
    'args' => array(
      array(
        'name' => 'type',
        'optional' => FALSE,
        'type' => 'string',
        'description' => 'The type of the content to get',
        'default value' => '*',
        'source' => array('path' => 0),
      ),
      array(
        'name' => 'entity',
        'optional' => FALSE,
        'source' => 'data',
        'description' => 'The entity data to create',
        'type' => 'array',
      ),
    ),
  );
  $resources['pharmtrac']['update'] = array(
    'help' => 'Update a content.',
    'file' => array('type' => 'inc', 'module' => 'pharmtrac', 'name' => 'pharmtrac.resource'),
    'callback' => 'pharmtrac_services_update',
    'access callback' => 'user_access',
    'access arguments' => array('access pharmtrac'),
    'access arguments append' => FALSE,
    'args' => array(
      array(
        'name' => 'type',
        'optional' => FALSE,
        'type' => 'string',
        'description' => 'The type of the content to get',
        'default value' => '*',
        'source' => array('path' => 0),
      ),
      array(
        'name' => 'id',
        'optional' => FALSE,
        'source' => array('path' => 1),
        'type' => 'string',
        'default value' => '',
        'description' => 'The entity id or barcode to get',
      ),
      array(
        'name' => 'entity',
        'optional' => FALSE,
        'source' => 'data',
        'description' => 'The entity data to update',
        'type' => 'array',
      ),
    ),
  );
  
  return $resources;
}

/**
 * node?fields=nid,vid&parameters[nid]=7&parameters[uid]=1
 */
function pharmtrac_services_index($type, $page, $fields, $parameters, $page_size) {
  // services views are used here to facilliate the work
  $service = 'services_index_' . $type;
  if (!views_get_view($service)) 
    return services_error(t('Content index does not exist'), 406);
  
  module_load_include('inc','services_views','services_views.resource');
  $items = services_views_retrieve($service, 'services_1', array(), 0, 25);
  $result = array();
  foreach ($items as $node) {
    $result[] = pharmtrac_services_retrieve($type, $node->id, 'id');
  }
    
  return $result;
  
  // $p = array_merge($parameters, array('type' => $type));
  // switch ($type) {
  //   case 'user':
  //     module_load_include('inc', 'services', 'resources/user_resource');
  //     return _user_resource_index($page, $fields, $parameters, $page_size);
  //     break;
  //   default:
  //     module_load_include('inc', 'services', 'resources/node_resource');
  //     $result = array();
  //     $list = _node_resource_index($page, 'nid', $p, $page_size);
  //     foreach ($list as $node) {
  //       $result[] = pharmtrac_services_retrieve($type, $node->nid, 'id');
  //     }
  //     return $result;
  //     break;
  // }
}

/**
 * Handle service retrieve action
 * * /rest/pharmtrac/{type}/{id}.json
 * * /rest/pharmtrac/{type}/{value}/{key}.json
 * * /api?command={type}&id={id}&key={key}
 */ 
function pharmtrac_services_retrieve($type, $id, $key) {
  // if key is specified other than id
  if ($key != 'id') {
    $keys = array($key => $id);
    pharmtrac_services_mapping($keys);
    if (!($id = get_entity_id($type, $keys)))
      return services_error(t('Content does not exist'), 406);
  }

  $entity = array();
  switch ($type) {
    case 'user':
      module_load_include('inc', 'services', 'resources/user_resource');
      $entity = _user_resource_retrieve($id);
      $entity->type = 'user';
      break;
    
    default:
      module_load_include('inc', 'services', 'resources/node_resource');
      $entity = _node_resource_retrieve($id);
      break;
  }
  
  return pharmtrac_services_rmapping($entity);
}

/**
 * Handle service create action
 * * /rest/pharmtrac/{type}
 * * /api?command={type}
 */ 
function pharmtrac_services_create($type, $entity) {
  if (!isset($entity['type'])) $entity['type'] = $type;
  pharmtrac_services_mapping($entity);

  switch ($type) {
    case 'user':
      module_load_include('inc', 'services', 'resources/user_resource');
      return _user_resource_create($entity);
      break;
    
    default:
      module_load_include('inc', 'services', 'resources/node_resource');
      return _node_resource_create($entity);
      break;
  }
}

/**
 * Handle service update operation
 * * /rest/pharmtrac/{type}/{id/key}
 * * /api?command={type}
 */ 
function pharmtrac_services_update($type, $id, $entity) {
  // clean $id a bit for complex barcode
  if (!isset($entity['type'])) $entity['type'] = $type;
  
  // find out key and value if $id is not available
  if (!is_numeric($id)) {
    $key = $id;
    if (empty($entity[$key])) 
      return services_error(t('Missing content id'), 406);
    
    $keys = array($key => $entity[$key]);
    pharmtrac_services_mapping($keys);

    switch ($type) {
      case 'user':
        if (!($id = getEntityValue($keys, 'uid', 'user')))
          return services_error(t('User does not exist'), 406);
        break;
      
      default:
        if (!($id = getEntityValue(array_merge(array('type' => $type), $keys))))
          return services_error(t('Content does not exist'), 406);
        break;
    }
  }
  
  pharmtrac_services_mapping($entity); 

  switch ($type) {
    case 'user':
      module_load_include('inc', 'services', 'resources/user_resource');
      return _user_resource_update($id, $entity);
      break;
    
    default:
      module_load_include('inc', 'services', 'resources/node_resource');
      return _node_resource_update($id, $entity);
      break;
  }
}

