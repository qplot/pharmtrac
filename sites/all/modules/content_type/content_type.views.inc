<?php
/**
 * @file content_type.views.inc
 * TODO: Enter file description here.
 */

/**
 * Implements hook_views_data().
 */
function content_type_views_data() {
  // ----------------------------------------------------------------
  // node type table

  $data['node_type']['table']['group'] = t('Content Type');

  // Advertise this table as a possible base table
  $data['node_type']['table']['base'] = array(
    'field' => 'type',
    'title' => t('Content Type'),
    'weight' => -10,
    'defaults' => array(
      'field' => 'type',
    ),
  );

  // type
  $data['node_type']['type'] = array(
    'title' => t('Type'),
    'help' => t('The machine-readable name of this type.'), 
    'field' => array(
      'handler' => 'views_handler_field_node_type',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_node_type',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_type',
    ),
  );

  // name
  $data['node_type']['name'] = array(
    'title' => t('Name'),
    'help' => t('The human-readable name of this type.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
     'filter' => array(
      'handler' => 'views_handler_filter_string',
     ),
     'sort' => array(
      'handler' => 'views_handler_sort',
     ),
     'argument' => array(
      'handler' => 'views_handler_argument_string',
     ),
  );

  // description
  $data['node_type']['description'] = array(
    'title' => t('Description'),
    'help' => t('A brief description of this type.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
     'filter' => array(
      'handler' => 'views_handler_filter_string',
     ),
     'sort' => array(
      'handler' => 'views_handler_sort',
     ),
     'argument' => array(
      'handler' => 'views_handler_argument_string',
     ),
  );

  // help
  $data['node_type']['help'] = array(
    'title' => t('Help'),
    'help' => t('Help information shown to the user when creating a node of this type.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
     'filter' => array(
      'handler' => 'views_handler_filter_string',
     ),
     'sort' => array(
      'handler' => 'views_handler_sort',
     ),
     'argument' => array(
      'handler' => 'views_handler_argument_string',
     ),
  );
  
  // has_title
  $data['node_type']['has_title'] = array(
    'title' => t('Has Title'),
    'help' => t('Boolean indicating whether this type uses the node title field.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'promoted-notpromoted' => array(t('Yes'), t('No')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Defined in module'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // title_label
  $data['node_type']['title_label'] = array(
    'title' => t('Title Label'),
    'help' => t('The label displaed for the title field on the edit form.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
     'filter' => array(
      'handler' => 'views_handler_filter_string',
     ),
     'sort' => array(
      'handler' => 'views_handler_sort',
     ),
     'argument' => array(
      'handler' => 'views_handler_argument_string',
     ),
  );
  
  // module
  $data['node_type']['module'] = array(
    'title' => t('Module'),
    'help' => t('The module defining this node type.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
     'filter' => array(
      'handler' => 'views_handler_filter_string',
     ),
     'sort' => array(
      'handler' => 'views_handler_sort',
     ),
     'argument' => array(
      'handler' => 'views_handler_argument_string',
     ),
  );

  // custom - stored in module
  $data['node_type']['custom'] = array(
    'title' => t('Stored in database'),
    'help' => t('A boolean indicating whether this type is defined by a module (FALSE) or by a user via Add content type (TRUE).'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'promoted-notpromoted' => array(t('Yes'), t('No')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Defined in module'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  
  
  return $data;
}
