<?php
/**
 * @file
 * pharmtrac.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function pharmtrac_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'import_location';
  $feeds_importer->config = array(
    'name' => 'Import Location',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv xml opml',
        'direct' => FALSE,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsCSVParser',
      'config' => array(
        'delimiter' => ',',
        'no_headers' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'expire' => '-1',
        'author' => '1',
        'authorize' => 1,
        'mappings' => array(
          0 => array(
            'source' => 'locationnumber',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'shortname',
            'target' => 'field_location_shortname',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'longname',
            'target' => 'field_location_longname',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'rooms',
            'target' => 'field_location_rooms:label',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '0',
        'input_format' => 'plain_text',
        'skip_hash_check' => 0,
        'bundle' => 'location',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['import_location'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'import_med';
  $feeds_importer->config = array(
    'name' => 'Import Med',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv xml opml',
        'direct' => FALSE,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsCSVParser',
      'config' => array(
        'delimiter' => ',',
        'no_headers' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'expire' => '-1',
        'author' => 0,
        'authorize' => 1,
        'mappings' => array(
          0 => array(
            'source' => 'identifier',
            'target' => 'guid',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'identifier',
            'target' => 'title',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'codingsystem',
            'target' => 'field_med_codingsystem',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'shortname',
            'target' => 'field_med_shortname',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'longname',
            'target' => 'field_med_longname',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'description',
            'target' => 'body',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'medtype',
            'target' => 'field_med_medtype',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'amt',
            'target' => 'field_med_amt',
            'unique' => FALSE,
          ),
          8 => array(
            'source' => 'amtunit',
            'target' => 'field_med_amtunits',
            'unique' => FALSE,
          ),
          9 => array(
            'source' => 'ndc',
            'target' => 'field_med_ndc',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '0',
        'input_format' => 'plain_text',
        'skip_hash_check' => 0,
        'bundle' => 'med',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['import_med'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'import_user';
  $feeds_importer->config = array(
    'name' => 'Import User',
    'description' => '',
    'fetcher' => array(
      'plugin_key' => 'FeedsFileFetcher',
      'config' => array(
        'allowed_extensions' => 'txt csv tsv xml opml',
        'direct' => FALSE,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsCSVParser',
      'config' => array(
        'delimiter' => ',',
        'no_headers' => 0,
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsUserProcessor',
      'config' => array(
        'mappings' => array(
          0 => array(
            'source' => 'UserName',
            'target' => 'name',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'Barcode',
            'target' => 'field_user_barcode',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'FirstName',
            'target' => 'field_user_firstname',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'LastName',
            'target' => 'field_user_lastname',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'EmailAddress',
            'target' => 'mail',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'Password',
            'target' => 'pass',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'MobileRole',
            'target' => 'field_user_mobilerole:label',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '0',
        'input_format' => 'plain_text',
        'skip_hash_check' => 0,
        'roles' => array(
          5 => '5',
          4 => 0,
        ),
        'status' => '1',
        'defuse_mail' => 0,
        'bundle' => 'user',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['import_user'] = $feeds_importer;

  return $export;
}
