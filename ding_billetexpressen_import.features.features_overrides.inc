<?php
/**
 * @file
 * ding_billetexpressen_import.features.features_overrides.inc
 */

/**
 * Implements hook_features_override_default_overrides().
 */
function ding_billetexpressen_import_features_override_default_overrides() {
  // This code is only used for UI in features. Exported alters hooks do the magic.
  $overrides = array();

  // Exported overrides for: field_base
  $overrides["field_base.field_ding_event_price.foreign keys"]["DELETED"] = TRUE;
  $overrides["field_base.field_ding_event_price.indexes"]["DELETED"] = TRUE;
  $overrides["field_base.field_ding_event_price.module"] = 'text';
  $overrides["field_base.field_ding_event_price.type"] = 'text';

  // Exported overrides for: field_instance
  $overrides["field_instance.node-ding_event-field_ding_event_price.settings|text_processing"] = 0;

 return $overrides;
}
