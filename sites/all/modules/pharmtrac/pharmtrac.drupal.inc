<?php
/**
 * @file
 * Code for the Drupal Core Related API.
 */

/**
 * Menu iitems
 */
function pharmtrac_drupal_menu() {
  $items = array();
  
  $items['ajax_node_display/%'] = array(
    'title' => 'AJAX Node Display',
    'description' => 'Use ajax approach to display node view',
    'page callback' => 'pharmtrac_ajax_node_view',
    'page arguments' => array(1),
    'access callback' => 'user_access',
    'access arguments' => array('administer pharmtrac'),
    'type' => MENU_CALLBACK,
  );
  
  return $items;
}

/**
 * Ajax node view callback
 * * returns node teaser view through ajax
 *
 * @param nid
 *   Specify node id
 * @param type
 *   Ajax drupal notation
 * @param wrapper
 *   Ajax callback html wrapper id, ex. $('#id')
 */
function pharmtrac_ajax_node_view($nid, $type = 'ajax', $wrapper = 'ajax-node-display') {
  // if ($type == 'ajax') {
    $node = node_load($nid);
    $output['ajax-node-callback'] = array(
      '#type' => 'fieldset',
      '#title' => t(ucfirst(getSet($node->type)) . ' Info'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#attributes' => array('id' => $wrapper),
    );
    $output['ajax-node-callback']['teaser'] = node_view($node, 'teaser');
    $output = drupal_render_children($output);
    
    $commands = array();
    $commands[] = ajax_command_replace('#' . $wrapper, $output);
    $commands[] = ajax_command_invoke('a[rel=tooltip]', 'tooltip');   // temperory fix
    $page = array('#type' => 'ajax', '#commands' => $commands);
    ajax_deliver($page);
  // }
  // else {
  //   drupal_goto('node/' . $nid);
  // }
}

/**
 * Extract array value
 */
function getMappedArray($keyname, $arr) {
  return array_map(create_function('$value', 'return $value["' . $keyname . '"];'), $arr);  
}

/**
 * Unset array value
 */
function unsetMappedArray($keyname, $arr) {
  return array_map(create_function('&$value', 'if (isset($value["'. $keyname . '"])) unset($value["'. $keyname . '"]);'));
}

/**
 * Variation of isset
 */
function getSet(&$v, $default = '') {
  return isset($v) ? $v : $default;
}

/**
 * Check all variable in $_GET
 */
function getVars($vars = array()){
	if (!is_array($vars)) {
		// return isset($_GET[$vars]) ? check_plain($_GET[$vars]) : 0;
		return filter_input(INPUT_GET, $vars, 
			FILTER_SANITIZE_SPECIAL_CHARS, FILTER_NULL_ON_FAILURE);
	}
	$result = array();
	foreach ($vars as $value) {
		// $result[$value] = isset($_GET[$value]) ? check_plain($_GET[$value]) : 0;
		$result[$value] = filter_input(INPUT_GET, $value, 
			FILTER_SANITIZE_SPECIAL_CHARS, FILTER_NULL_ON_FAILURE);
	}
	return $result;
}

/**
 * Get cleaned input variables
 */
function getFilteredInput($vars, $type = INPUT_GET, $filter = FILTER_SANITIZE_ENCODED) {
  $v = array();
  foreach ($vars as $value) {
    $v[$value] = $type;
  }
  return filter_input_array($type, $v);
}

/**
 * Access control for content type node
 */
function access_node_content_type($node, $type) {
  if ($node->type == $type) 
    return NODE_ACCESS_ALLOW;
  else 
    return NODE_ACCESS_IGNORE;
}

/**
 * Get entities query, helper function
 *
 * @param prop_conditions
 *   Specify condition array, array('title'=>'123')
 * @param field_name
 *   Specify field name of the return record
 * @param entity_type
 *   Specify entity type, ex. 'node'
 */
function getEntitiesQuery($prop_conditions, $entity_type = 'node') {
  $query = new EntityFieldQuery;
  $query = $query->entityCondition('entity_type', $entity_type);
  foreach ($prop_conditions as $key => $value) {
    // use field condition if _ detected
    if (strpos($key,'_') !== false)
      $query->fieldCondition($key, 'value', $value);
    else
      $query->propertyCondition($key,$value);
  }
  return $query;
}

/**
 * Get entities list
 *
 * @param prop_conditions
 *   Specify condition array, array('title'=>'123')
 * @param field_name
 *   Specify field name of the return record
 * @param entity_type
 *   Specify entity type, ex. 'node'
 * @return a list of associated query object with key set to be object id
 */
function getEntities($prop_conditions, $entity_type = 'node') {
  $query = getEntitiesQuery($prop_conditions, $entity_type);
  $entities = $query->execute();
  
  return (!empty($entities[$entity_type]))?$entities[$entity_type]:array();
}

/**
 * Get entities count
 *
 * @param prop_conditions
 *   Specify condition array, array('title'=>'123')
 * @param entity_type
 *   Specify entity type, ex. 'node'
 * @return the total number of entities
 *
 */
function getEntitiesCount($prop_conditions, $entity_type = 'node') {
  $query = getEntitiesQuery($prop_conditions, $entity_type);
  return $query->count()->execute();
}

function getCount($prop_conditions, $entity_type = 'node') {
  $query = getEntitiesQuery($prop_conditions, $entity_type);
  return $query->count()->execute();
}


/**
 * Get single entity object
 *
 * @param prop_conditions
 *   Specify condition array, array('title'=>'123')
 * @param field_name
 *   Specify field name of the return record
 * @param entity_type
 *   Specify entity type, ex. 'node'
 * @return a entity object
 */
function getEntity($prop_conditions, $entity_type = 'node') {
  $query = getEntitiesQuery($prop_conditions, $entity_type);
  $entities = $query->range(0,1)->execute();

  if (empty($entities[$entity_type])) return array();
  
  $ids = array_keys($entities[$entity_type]);
  $id = array_shift($ids);
  $entities = entity_load($entity_type, array($id));
  
  return array_shift($entities);
}

/**
 * Get entity property value
 * * getEntityValue(array('nid'=> 1019), 'field_location_shortname');
 * * getEntityValue(array('nid'=> 1019), 'field_location_shortname:');
 * * getEntityValue(array('nid'=> 1019), 'field_location_shortname:und/0/value');
 * * new version of entity value wrapper call
 * * replace getField, plusdelta_raw_get and drupal_getNodeFieldValue
 *
 * @param prop_conditions
 *   Specify condition array, array('title'=>'123')
 * @param field_name
 *   Specify field name of the return record
 * @param entity_type
 *   Specify entity type, ex. 'node'
 * @return an entity property value
 */
function getEntityValue($prop_conditions, $field_name = 'nid', $entity_type = 'node') {
  $query = getEntitiesQuery($prop_conditions, $entity_type);
  $entities = $query->range(0,1)->execute();

  if (empty($entities[$entity_type])) return array();
  
  list($id) = array_keys($entities[$entity_type]);
  $entities = entity_load($entity_type, array($id));
  
  if (strpos($field_name, ':') === false)
    return $entities[$id]->$field_name;
  else {
    $v = explode(':', $field_name);
    if (isset($v[0])) {
      $value = $entities[$id]->$v[0];
      $parent = array('und', 0, 'value');
      if (!empty($v[1])) {
        $parent = explode('/', $v[1]);
      }
      return drupal_array_get_nested_value($value, $parent);
    }
  }
  return '';
}

/**
 * Create node with title
 */
function createEntity($prop_conditions, $entity_type = 'node') {
  $node = new StdClass();
  $node->type = $prop_conditions['type'];
  node_object_prepare($node);
  $node->language = 'und';
  
  $node->title=$prop_conditions['title'];
  
  if ($node = node_submit($node)) {
    node_save($node);
  }
  return $node;
}

/**
 * Get entity field
 *
 * @param prop_conditions
 *   Specify condition array, array('title'=>'123')
 * @param field_name
 *   Specify field name of the return record
 * @param entity_type
 *   Specify entity type, ex. 'node'
 */
function getField($prop_conditions, $field_name = 'id', $entity_type = 'node') {
  return plusdelta_raw_get($prop_conditions, $field_name, $entity_type);
}

/**
 * Get entity field value, limited to only one field
 * ex. plusdelta_raw_get(array('type' => 'location', 'title' => $location))
 */
function plusdelta_raw_get($prop_conditions, $return = 'id', $entity_type = 'node') {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', $entity_type);
  foreach ($prop_conditions as $key => $value) {
    // use field condition if _ detected
    if (strpos($key,'_') !== false)
      $query->fieldCondition($key, 'value', $value);
    else
      $query->propertyCondition($key,$value);
  }
  $entities = $query->execute();

  // no records
  if (!$entities) return false;
  
  // fetch first record id
  list($id) = array_keys($entities[$entity_type]);
  if ($return == 'id') return $id;
  
  // fetch other field
  if ($entity_type == 'node')
    $ent = node_load($id);
  elseif ($entity_type == 'user')
    $ent = user_load($id);
    
  return $ent->$return;
}

/**
 * Get node by node title
 */
function plusdelta_api_getNodeByTitle($type, $title) {
  $query = new EntityFieldQuery();
  $entities = $query->entityCondition('entity_type', 'node')
        ->propertyCondition('type', $type)
        ->propertyCondition('title', $title)
        ->execute();
        
  if ($entities) {
    $ids = array_keys($entities['node']);
    $node = node_load(end($ids));
    return $node;
  }
  
  return null;
}

/**
 * Add node by type and title
 */
function plusdelta_api_addNodeWithTitle($type, $title) {
  $node = new StdClass();
  $node->type = $type;
  node_object_prepare($node);
  $node->language = 'und';
  $node->title = $title;
  if ($node = node_submit($node)) {
    return $node;
  } else {
    return null;
  }
}

/**
 * Retreve field value from node 
 */
function drupal_getNodeFieldValue(&$node, $field_name, $property_name='value') {
  if (!$node) return '';
  $tmp = &$node->$field_name;
  if (!isset($tmp[$node->language][0][$property_name])) return '';
  return $tmp[$node->language][0][$property_name];
}

/**
 * Retreve field values (array) from node 
 */
function drupal_getNodeFieldValues(&$node, $field_name, $property_name='value') {
  if (!$node) return array();
  $tmp = &$node->$field_name;
  if (!isset($tmp[$node->language])) return array();
  $result = array();
  foreach ($tmp[$node->language] as $value) {
    if (isset($value[$property_name]))
      $result[] = $value;
  }
  return $result;
}

/**
 * Retrieve user string
 */
function plusdelta_user_string($uid) {
  $user = user_load($uid);
  $result = array();
  //$result['id'] = $user->uid;
  //$result['name'] = $user->name;
  $result = $user->name;
  //$result['mail'] = $user->mail;
  return $result;
}