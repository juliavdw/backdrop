<?php
// $Id$

/**
 * @file
 * Defines a field type and its formatters and widgets.
 */

/**
 * Implements hook_field_info().
 */
function field_test_field_info() {
  return array(
    'test_field' => array(
      'label' => t('Test Field'),
      'description' => t('Dummy field type used for tests.'),
      'settings' => array(
        'test_field_setting' => 'dummy test string',
        'changeable' => 'a changeable field setting',
        'unchangeable' => 'an unchangeable field setting',
      ),
      'instance_settings' => array(
        'test_instance_setting' => 'dummy test string',
        'test_hook_field_load' => FALSE,
      ),
      'default_widget' => 'test_field_widget',
      'default_formatter' => 'field_test_default',
    ),
  );
}

/**
 * Implements hook_field_schema().
 */
function field_test_field_schema($field) {
  return array(
    'columns' => array(
      'value' => array(
        'type' => 'int',
        'size' => 'tiny',
        'not null' => FALSE,
      ),
    ),
    'indexes' => array(
      'value' => array('value'),
    ),
  );
}

/**
 * Implements hook_field_update_forbid().
 */
function field_test_field_update_forbid($field, $prior_field, $has_data) {
  if ($field['type'] == 'test_field' && $field['settings']['unchangeable'] != $prior_field['settings']['unchangeable']) {
    throw new FieldException("field_test 'unchangeable' setting cannot be changed'");
  }
}

/**
 * Implements hook_field_load().
 */
function field_test_field_load($obj_type, $objects, $field, $instances, $langcode, &$items, $age) {
  foreach ($items as $id => $item) {
    // To keep the test non-intrusive, only act for instances with the
    // test_hook_field_load setting explicitly set to TRUE.
    if ($instances[$id]['settings']['test_hook_field_load']) {
      foreach ($item as $delta => $value) {
        // Don't add anything on empty values.
        if ($value) {
          $items[$id][$delta]['additional_key'] = 'additional_value';
        }
      }
    }
  }
}

/**
 * Implements hook_field_validate().
 *
 * Possible error codes:
 * - 'field_test_invalid': The value is invalid.
 */
function field_test_field_validate($obj_type, $object, $field, $instance, $langcode, $items, &$errors) {
  foreach ($items as $delta => $item) {
    if ($item['value'] == -1) {
      $errors[$field['field_name']][$langcode][$delta][] = array(
        'error' => 'field_test_invalid',
        'message' => t('%name does not accept the value -1.', array('%name' => $instance['label'])),
      );
    }
  }
}

/**
 * Implements hook_field_is_empty().
 */
function field_test_field_is_empty($item, $field) {
  return empty($item['value']);
}

/**
 * Implements hook_field_settings_form().
 */
function field_test_field_settings_form($field, $instance, $has_data) {
  $settings = $field['settings'];

  $form['test_field_setting'] = array(
    '#type' => 'textfield',
    '#title' => t('Field test field setting'),
    '#default_value' => $settings['test_field_setting'],
    '#required' => FALSE,
    '#description' => t('A dummy form element to simulate field setting.'),
  );

  return $form;
}

/**
 * Implements hook_field_instance_settings_form().
 */
function field_test_field_instance_settings_form($field, $instance) {
  $settings = $instance['settings'];

  $form['test_instance_setting'] = array(
    '#type' => 'textfield',
    '#title' => t('Field test field instance setting'),
    '#default_value' => $settings['test_instance_setting'],
    '#required' => FALSE,
    '#description' => t('A dummy form element to simulate field instance setting.'),
  );

  return $form;
}

/**
 * Implements hook_field_widget_info().
 */
function field_test_field_widget_info() {
  return array(
    'test_field_widget' => array(
      'label' => t('Test field'),
      'field types' => array('test_field'),
      'settings' => array('test_widget_setting' => 'dummy test string'),
    ),
    'test_field_widget_multiple' => array(
      'label' => t('Test field 1'),
      'field types' => array('test_field'),
      'settings' => array('test_widget_setting_multiple' => 'dummy test string'),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget().
 */
function field_test_field_widget(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $element = array(
    'value' => $element + array(
      '#type' => 'textfield',
      '#default_value' => isset($items[$delta]['value']) ? $items[$delta]['value'] : '',
    ),
  );
  return $element;
}

/**
 * Implements hook_field_widget_error().
 */
function field_test_field_widget_error($element, $error) {
  form_error($element['value'], $error['message']);
}

/**
 * Implements hook_field_widget_settings_form().
 */
function field_test_field_widget_settings_form($field, $instance) {
  $widget = $instance['widget'];
  $settings = $widget['settings'];

  $form['test_widget_setting'] = array(
    '#type' => 'textfield',
    '#title' => t('Field test field widget setting'),
    '#default_value' => $settings['test_widget_setting'],
    '#required' => FALSE,
    '#description' => t('A dummy form element to simulate field widget setting.'),
  );

  return $form;
}

/**
 * Implements hook_field_formatter_info().
 */
function field_test_field_formatter_info() {
  return array(
    'field_test_default' => array(
      'label' => t('Default'),
      'description' => t('Default formatter'),
      'field types' => array('test_field'),
      'settings' => array(
        'test_formatter_setting' => 'dummy test string',
      ),
    ),
    'field_test_multiple' => array(
      'label' => t('Multiple'),
      'description' => t('Multiple formatter'),
      'field types' => array('test_field'),
      'settings' => array(
        'test_formatter_setting_multiple' => 'dummy test string',
      ),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
      ),
    ),
    'field_test_needs_additional_data' => array(
      'label' => t('Tests hook_field_formatter_prepare_view()'),
      'field types' => array('test_field'),
      'settings' => array(
        'test_formatter_setting_additional' => 'dummy test string',
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_prepare_view().
 */
function field_test_field_formatter_prepare_view($obj_type, $objects, $field, $instances, $langcode, &$items, $displays) {
  foreach ($items as $id => $item) {
    // To keep the test non-intrusive, only act on the
    // 'field_test_needs_additional_data' formatter.
    if ($displays[$id]['type'] == 'field_test_needs_additional_data') {
      foreach ($item as $delta => $value) {
        // Don't add anything on empty values.
        if ($value) {
          $items[$id][$delta]['additional_formatter_value'] = $value['value'] + 1;
        }
      }
    }
  }
}

/**
 * Implements hook_field_formatter().
 */
function field_test_field_formatter($object_type, $object, $field, $instance, $langcode, $display, $items, $delta) {
  $settings = $display['settings'];

  switch ($display['type']) {
    case 'field_test_default':
      $item = $items[$delta];
      $output = $settings['test_formatter_setting'] . '|' . $item['value'];
      break;

    case 'field_test_needs_additional_data':
      $item = $items[$delta];
      $output = $settings['test_formatter_setting_additional'] . '|' . $item['value'] . '|' . $item['additional_formatter_value'];
      break;

    case 'field_test_multiple':
      $array = array();
      foreach ($items as $delta => $item) {
        $array[] = $delta .':'. $item['value'];
      }
      $output = $settings['test_formatter_setting_multiple'] . '|' . implode('|', $array);
      break;
  }

  return array(
    '#markup' => $output,
  );
}

/**
 * Sample 'default value' callback.
 */
function field_test_default_value($obj_type, $object, $field, $instance) {
  return array(array('value' => 99));
}

/**
 * Implements hook_field_access().
 */
function field_test_field_access($op, $field, $obj_type, $object, $account) {
  if ($field['field_name'] == "field_no_{$op}_access") {
    return FALSE;
  }
  return TRUE;
}