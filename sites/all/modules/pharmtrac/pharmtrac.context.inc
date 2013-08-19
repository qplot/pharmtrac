<?php
/**
 * @file
 * pharmtrac.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function pharmtrac_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'admin_role';
  $context->description = 'Roles for sys admin';
  $context->tag = '';
  $context->conditions = array(
    'user' => array(
      'values' => array(
        'administrator' => 'administrator',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-features' => array(
          'module' => 'menu',
          'delta' => 'features',
          'region' => 'navigation',
          'weight' => '-10',
          'title' => '<none>',
        ),
        'menu_block-2' => array(
          'module' => 'menu_block',
          'delta' => '2',
          'region' => 'sidebar_first',
          'weight' => '-10',
          'title' => '<none>',
        ),
      ),
    ),
    'region' => array(
      'mockup' => array(
        'disable' => array(
          'sidebar_second' => 'sidebar_second',
          'navigation' => 0,
          'header' => 0,
          'featured' => 0,
          'highlighted' => 0,
          'help' => 0,
          'content' => 0,
          'sidebar_first' => 0,
          'footer' => 0,
          'footer_bottom' => 0,
          'dashboard_main' => 0,
          'dashboard_sidebar' => 0,
          'dashboard_inactive' => 0,
        ),
      ),
      'seven' => array(
        'disable' => array(
          'content' => 0,
          'help' => 0,
          'page_top' => 0,
          'page_bottom' => 0,
          'sidebar_first' => 0,
          'dashboard_main' => 0,
          'dashboard_sidebar' => 0,
          'dashboard_inactive' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Roles for sys admin');
  $export['admin_role'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'hospital_role';
  $context->description = 'Roles for hospital staff/manager/admin';
  $context->tag = '';
  $context->conditions = array(
    'user' => array(
      'values' => array(
        'hospital admin' => 'hospital admin',
        'hospital manager' => 'hospital manager',
        'hospital staff' => 'hospital staff',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'menu-features' => array(
          'module' => 'menu',
          'delta' => 'features',
          'region' => 'navigation',
          'weight' => '-10',
          'title' => '<none>',
        ),
      ),
    ),
    'region' => array(
      'mockup' => array(
        'disable' => array(
          'sidebar_first' => 'sidebar_first',
          'sidebar_second' => 'sidebar_second',
          'navigation' => 0,
          'header' => 0,
          'featured' => 0,
          'highlighted' => 0,
          'help' => 0,
          'content' => 0,
          'footer' => 0,
          'footer_bottom' => 0,
          'dashboard_main' => 0,
          'dashboard_sidebar' => 0,
          'dashboard_inactive' => 0,
        ),
      ),
      'seven' => array(
        'disable' => array(
          'content' => 0,
          'help' => 0,
          'page_top' => 0,
          'page_bottom' => 0,
          'sidebar_first' => 0,
          'dashboard_main' => 0,
          'dashboard_sidebar' => 0,
          'dashboard_inactive' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 1;

  // Translatables
  // Included for use with string extractors like potx.
  t('Roles for hospital staff/manager/admin');
  $export['hospital_role'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'wallboard';
  $context->description = '';
  $context->tag = '';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'wallboard/*' => 'wallboard/*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'system-main' => array(
          'module' => 'system',
          'delta' => 'main',
          'region' => 'content_full',
          'weight' => '-10',
          'title' => '<none>',
        ),
      ),
    ),
    'region' => array(
      'mockup' => array(
        'disable' => array(
          'help' => 'help',
          'content' => 'content',
          'sidebar_first' => 'sidebar_first',
          'sidebar_second' => 'sidebar_second',
          'footer' => 'footer',
          'navigation' => 0,
          'header' => 0,
          'featured' => 0,
          'highlighted' => 0,
          'content_full' => 0,
          'footer_bottom' => 0,
          'dashboard_main' => 0,
          'dashboard_sidebar' => 0,
          'dashboard_inactive' => 0,
        ),
      ),
      'seven' => array(
        'disable' => array(
          'content' => 0,
          'help' => 0,
          'page_top' => 0,
          'page_bottom' => 0,
          'sidebar_first' => 0,
          'dashboard_main' => 0,
          'dashboard_sidebar' => 0,
          'dashboard_inactive' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;
  $export['wallboard'] = $context;

  return $export;
}
