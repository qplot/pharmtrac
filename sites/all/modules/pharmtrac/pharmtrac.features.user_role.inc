<?php
/**
 * @file
 * pharmtrac.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function pharmtrac_user_default_roles() {
  $roles = array();

  // Exported role: device.
  $roles['device'] = array(
    'name' => 'device',
    'weight' => '4',
  );

  // Exported role: hospital admin.
  $roles['hospital admin'] = array(
    'name' => 'hospital admin',
    'weight' => '5',
  );

  // Exported role: hospital manager.
  $roles['hospital manager'] = array(
    'name' => 'hospital manager',
    'weight' => '6',
  );

  // Exported role: hospital staff.
  $roles['hospital staff'] = array(
    'name' => 'hospital staff',
    'weight' => '7',
  );

  return $roles;
}
