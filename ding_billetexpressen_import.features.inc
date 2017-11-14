<?php
/**
 * @file
 * ding_billetexpressen_import.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ding_billetexpressen_import_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_field_default_field_bases_alter().
 */
function ding_billetexpressen_import_field_default_field_bases_alter(&$data) {
  if (isset($data['field_ding_event_price'])) {
    $data['field_ding_event_price']['module'] = 'text'; /* WAS: 'number' */
    $data['field_ding_event_price']['type'] = 'text'; /* WAS: 'number_integer' */
    unset($data['field_ding_event_price']['foreign keys']);
    unset($data['field_ding_event_price']['indexes']);
  }
}

/**
 * Implements hook_field_default_field_instances_alter().
 */
function ding_billetexpressen_import_field_default_field_instances_alter(&$data) {
  if (isset($data['node-ding_event-field_ding_event_price'])) {
    $data['node-ding_event-field_ding_event_price']['settings']['text_processing'] = 0; /* WAS: '' */
  }
}
