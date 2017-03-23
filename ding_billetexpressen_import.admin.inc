<?php
/**
 * @file
 * Admin form definitions.
 */

/**
 * Form renders mapped organizer.
 *
 * @see ding_billetexpressen_import_menu().
 */
function ding_billetexpressen_import_admin_form($form, &$form_state) {
  $form['#tree'] = TRUE;

  $form['ding_billetexpressen_import_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Billetexpressen XML source'),
    '#required' => TRUE,
    '#default_value' => variable_get('ding_billetexpressen_import_url', ''),
    '#element_validate' => array('_ding_billetexpressen_import_add_mapping_validate'),
  );

  // Define library mapping table.
  if (!$form_state['submitted']) {
    $form_state['libraries_mappings'] = variable_get('ding_billetexpressen_import_libraries', array());
  }
  $form['ding_billetexpressen_import_libraries'] = array(
    '#theme' => 'ding_billetexpressen_libraries_table',
    '#prefix' => '<div id="ding-billetexpressen-libraries-mapping">',
    '#suffix' => '</div>'
  );
  $form['ding_billetexpressen_import_libraries']['mapping'] = array(
    '#type' => 'invisible',
    '#value' => $form_state['libraries_mappings'],
  );
  $form['ding_billetexpressen_import_libraries']['libraries'] = array(
    '#type' => 'invisible',
    '#value' => ding_billetexpressen_import_get_libraries(),
  );
  $form['ding_billetexpressen_import_libraries']['add']['mapping'] = array(
    '#type' => 'textfield',
    '#default_value' => '',
  );
  $form['ding_billetexpressen_import_libraries']['add']['library'] = array(
    '#type' => 'select',
    '#options' => ding_billetexpressen_import_get_not_mapped_libraries(),
  );
  $form['ding_billetexpressen_import_libraries']['add']['op'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#name' => 'add_library',
    '#submit' => array('ding_billetexpressen_import_map_library'),
    '#ajax' => array(
      'callback' => 'ding_billetexpressen_import_library_ajax_callback',
      'wrapper' => 'ding-billetexpressen-libraries-mapping',
    ),
  );

  // Define category mapping table.
  if (!$form_state['submitted']) {
    $form_state['categories_mappings'] = variable_get('ding_billetexpressen_import_categories', array());
  }
  $form['ding_billetexpressen_import_categories'] = array(
    '#theme' => 'ding_billetexpressen_categories_table',
    '#prefix' => '<div id="ding-billetexpressen-categories-mapping">',
    '#suffix' => '</div>'
  );
  $form['ding_billetexpressen_import_categories']['mapping'] = array(
    '#type' => 'invisible',
    '#value' => $form_state['categories_mappings'],
  );
  $form['ding_billetexpressen_import_categories']['categories'] = array(
    '#type' => 'invisible',
    '#value' => ding_billetexpressen_import_get_categories(),
  );
  $form['ding_billetexpressen_import_categories']['add']['mapping'] = array(
    '#type' => 'textfield',
    '#default_value' => '',
  );
  $form['ding_billetexpressen_import_categories']['add']['category'] = array(
    '#type' => 'select',
    '#options' => ding_billetexpressen_import_get_not_mapped_categories(),
  );
  $form['ding_billetexpressen_import_categories']['add']['op'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#name' => 'add_category',
    '#submit' => array('ding_billetexpressen_import_map_category'),
    '#ajax' => array(
      'callback' => 'ding_billetexpressen_import_category_ajax_callback',
      'wrapper' => 'ding-billetexpressen-categories-mapping',
    ),
  );

  // Node status options.
  $form['ding_billetexpressen_import_status'] = array(
    '#type' => 'radios',
    '#options' => array(0 => t('Unpublished'), 1 => t('Published')),
    '#title' => t('Imported nodes status'),
    '#default_value' => variable_get('ding_billetexpressen_import_status', 1),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => array('ding_billetexpressen_import_admin_form_submit'),
  );

  return $form;
}

/**
 * Save configuration form.
 */
function ding_billetexpressen_import_admin_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $ding_billetexpressen_import_libraries = $form_state['libraries_mappings'];

  $libraries_mapping = $values['ding_billetexpressen_import_libraries']['add'];
  if (!empty($libraries_mapping['mapping']) && !empty($libraries_mapping['library'])) {
    unset($libraries_mapping['op']);
    $ding_billetexpressen_import_libraries[] = $libraries_mapping;
  }

  variable_set('ding_billetexpressen_import_libraries', $ding_billetexpressen_import_libraries);

  $ding_billetexpressen_import_categories = $form_state['categories_mappings'];

  $categories_mapping = $values['ding_billetexpressen_import_categories']['add'];
  if (!empty($categories_mapping['mapping']) && !empty($categories_mapping['library'])) {
    unset($categories_mapping['op']);
    $ding_billetexpressen_import_categories[] = $categories_mapping;
  }

  variable_set('ding_billetexpressen_import_categories', $ding_billetexpressen_import_categories);

  variable_set('ding_billetexpressen_import_status', $values['ding_billetexpressen_import_status']);

  variable_set('ding_billetexpressen_import_url', $values['ding_billetexpressen_import_url']);
}

/**
 * Validate function for Billetexpressen source url.
 *
 * @see ding_billetexpressen_import_admin_form().
 */
function _ding_billetexpressen_import_add_mapping_validate($element, &$form_state) {
  if (!valid_url($element['#value'], TRUE)) {
    form_set_error('ding_billetexpressen_import', t('Invalid URI to Billetexpressen XML.'));
  }
}

/**
 * Get all library CT nodes.
 */
function ding_billetexpressen_import_get_libraries() {
  $query = db_select('node', 'n')
    ->fields('n', array('nid', 'title'))
    ->condition('n.type', 'ding_library');

  $result = $query
    ->execute()
    ->fetchAllKeyed();

  return $result;
}

/**
 * Get not mapped libraries.
 *
 * @return array $not_mapped
 *  key = id of library CT, value = library name.
 */
function ding_billetexpressen_import_get_not_mapped_libraries() {
  $all_libraries = ding_billetexpressen_import_get_libraries();
  $mapped_libraries = variable_get('ding_billetexpressen_import_mapped_libraries', array());

  $not_mapped = array_diff_key($all_libraries, $mapped_libraries);

  return $not_mapped;
}

/**
 * Get ding event categories.
 *
 * @return array
 *   Set of ding event terms.
 */
function ding_billetexpressen_import_get_categories() {
  $vocabulary = taxonomy_vocabulary_machine_name_load('event_category');
  $terms = entity_load('taxonomy_term', FALSE, array('vid' => $vocabulary->vid));

  $result = array_map(function ($term) {
    return $term->name;
  }, $terms);

  return $result;
}

/**
 * Get not mapped ding event categories.
 *
 * @return array
 *   Set of not mapped categories.
 */
function ding_billetexpressen_import_get_not_mapped_categories() {
  $categories = ding_billetexpressen_import_get_categories();
  $mapped_categories = variable_get('ding_billetexpressen_import_mapped_categories', array());

  $not_mapped = array_diff_key($categories, $mapped_categories);

  return $not_mapped;
}

/**
 * Theme library mapping table on the admin page.
 */
function theme_ding_billetexpressen_libraries_table($variables) {
  $form = $variables['form'];
  $header = array(t('Mapping'), t('Library'), t('Operation'));

  $rows = array();
  foreach ($form['mapping']['#value'] as $item) {
    $rows[$item['library']] = array(
      'mapping' => $item['mapping'],
      'library' => $form['libraries']['#value'][$item['library']],
      'add' => l(t('Delete'), "billetexpressen/library/" . $item['library'] . "/delete", array('attributes' => array('class' => 'use-ajax'))),
    );
  }

  $rows['add'] = array(
    'mapping' => render($form['add']['mapping']),
    'library' => render($form['add']['library']),
    'add' => render($form['add']['op']),
  );
  $output = theme('table', array('header' => $header, 'rows' => $rows));

  return $output . drupal_render_children($form);
}

/**
 * Theme category mapping table on the admin page.
 */
function theme_ding_billetexpressen_categories_table($variables) {
  $form = $variables['form'];
  $header = array(t('Mapping'), t('Category'), t('Operation'));

  $rows = array();
  foreach ($form['mapping']['#value'] as $item) {
    $rows[$item['category']] = array(
      'mapping' => $item['mapping'],
      'category' => $form['categories']['#value'][$item['category']],
      'add' => l(t('Delete'), "billetexpressen/category/" . $item['category'] . "/delete", array('attributes' => array('class' => 'use-ajax'))),
    );
  }

  $rows['add'] = array(
    'mapping' => render($form['add']['mapping']),
    'category' => render($form['add']['category']),
    'add' => render($form['add']['op']),
  );
  $output = theme('table', array('header' => $header, 'rows' => $rows));

  return $output . drupal_render_children($form);
}

/**
 * Map libraries based on user defined values.
 */
function ding_billetexpressen_import_map_library($form, &$form_state) {
  $values = $form_state['values']['ding_billetexpressen_import_libraries']['add'];

  if (!empty($values['mapping'] && !empty($values['library']))) {
    $libraries = $form['ding_billetexpressen_import_libraries']['libraries']['#value'];

    $mapped_libraries = variable_get('ding_billetexpressen_import_mapped_libraries', array());
    $mapped_libraries[$values['library']] = $libraries[$values['library']];
    variable_set('ding_billetexpressen_import_mapped_libraries', $mapped_libraries);

    $form_state['libraries_mappings'][$values['library']] = array(
      'mapping' => $values['mapping'],
      'library' => $values['library'],
    );
  }

  $form_state['rebuild'] = TRUE;
}

/**
 * Return mapping library table.
 */
function ding_billetexpressen_import_library_ajax_callback($form, &$form_state) {
  return $form['ding_billetexpressen_import_libraries'];
}

/**
 * Map categories based on user defined values.
 */
function ding_billetexpressen_import_map_category($form, &$form_state) {
  $values = $form_state['values']['ding_billetexpressen_import_categories']['add'];

  if (!empty($values['mapping'] && !empty($values['category']))) {
    $categories = $form['ding_billetexpressen_import_categories']['categories']['#value'];

    $mapped_categories = variable_get('ding_billetexpressen_import_mapped_categories', array());
    $mapped_categories[$values['category']] = $categories[$values['category']];
    variable_set('ding_billetexpressen_import_mapped_categories', $mapped_categories);

    $form_state['categories_mappings'][$values['category']] = array(
      'mapping' => $values['mapping'],
      'category' => $values['category'],
    );
  }

  $form_state['rebuild'] = TRUE;
}

/**
 * Return mapping library table.
 */
function ding_billetexpressen_import_category_ajax_callback($form, &$form_state) {
  return $form['ding_billetexpressen_import_categories'];
}

/**
 * Delete library mapping from the admin table.
 *
 * @param int $library
 *   Library id.
 */
function ding_billetexpressen_library_mapping_delete($library) {
  $mapped_libraries = variable_get('ding_billetexpressen_import_mapped_libraries', array());
  $ding_billetexpressen_import_libraries = variable_get('ding_billetexpressen_import_libraries', array());

  if (!empty($library)) {
    unset($mapped_libraries[$library]);
    unset($ding_billetexpressen_import_libraries[$library]);

    variable_set('ding_billetexpressen_import_libraries', $ding_billetexpressen_import_libraries);
    variable_set('ding_billetexpressen_import_mapped_libraries', $mapped_libraries);
  }
}

/**
 * Delete category mapping from the admin table.
 *
 * @param int $category
 *   Category id.
 */
function ding_billetexpressen_category_mapping_delete($category) {
  $mapped_categories = variable_get('ding_billetexpressen_import_mapped_categories', array());
  $ding_billetexpressen_import_categories = variable_get('ding_billetexpressen_import_categories', array());

  if (!empty($category)) {
    unset($mapped_categories[$category]);
    unset($ding_billetexpressen_import_categories[$category]);

    variable_set('ding_billetexpressen_import_categories', $ding_billetexpressen_import_categories);
    variable_set('ding_billetexpressen_import_mapped_categories', $mapped_categories);
  }
}
