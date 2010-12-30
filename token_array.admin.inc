<?php
// $Id$

/**
 * @file
 * Form builder and callbacks for editing merge style settings.
 */

/**
 * Form builder function for module settings.
 */
function token_array_settings_form() {
  // If neither CCK nor Taxonomy module is enabled, display a warning that tells
  // the user that Array Tokens won't do any good.
  if (!module_exists('content') && !module_exists('taxonomy')) {
    drupal_set_message(t('Array Tokens are only useful if your site has
      multiple-value CCK fields or taxonomy vocabularies. Please enable Content
      or Taxonomy module to make use of the settings below.'), 'warning');
  }


  // Get stored values, or default ones
  $merge_styles = variable_get('token_array_settings', token_array_merge_styles());

  // Set an array of descriptions, for future reference
  $descriptions = array(
    'name' => t('The human-readable name of the merge style.'),
    'prefix' => t('The string to insert before each item.'),
    'suffix' => t('The string to insert after each item.'),
    'infix' => t('The string to insert between items.'),
  );
  $example_items = array(t('item1'), t('item2'), t('item3'));

  // Some helpt text is always useful
  $form['help'] = array(
    '#value' => t('On this page you can edit, create and delete the Array Token
      %merge_styles. Each merge style consists of prefix, suffix and infix.
      Prefix are put before item in the array. Suffix are appended to each item.
      The infix is put between items when merging them together. (If there is
      only one item present, no infix will be used.) Note that it is possible to
      enter markup in merge style configuration – only trusted users should be
      allowed to configure this.',
      array('%merge_styles' => 'merge styles'))
  );

  // Print out a fieldset with settings for each merge style, including an
  // example of what the merge style looks like

  $example = array();
  foreach ($example_items as $example_item) {
    $example[] = $merge_style['prefix'] . $example_item . $merge_style['suffix'];
  }

  foreach ($merge_styles as $machine_name => $merge_style) {
    $form[$machine_name] = array(
      '#type' => 'fieldset',
      '#title' => t('Merge style: @style_name (@machine_name)', array('@style_name' => t($merge_style['name']), '@machine_name' => $machine_name)),
      '#name' => $machine_name . '-fieldset',
      '#tree' => TRUE,
      '#description' => t('Example token output: !example',
      array('!example' => implode($merge_style['infix'], $example))),
    );

    foreach ($merge_style as $property => $value) {
      $form[$machine_name][$property] = array(
        '#type' => 'textfield',
        '#title' => t($property),
        '#value' => $value,
        '#description' => $descriptions[$property],
        '#name' => $machine_name . '-' . $property,
        '#tree' => TRUE,
      );
    }

    // Add a save button for each merge style
    $form[$machine_name]['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save style \'@style_name\'', array('@style_name' => t($merge_style['name']))),
      '#submit' => array('token_array_save'),
      '#name' => $machine_name . '-save',
    );
    // Add a delete button for each merge style
    $form[$machine_name]['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete style \'@style_name\'', array('@style_name' => t($merge_style['name']))),
      '#submit' => array('token_array_delete'),
      '#name' => $machine_name,
    );

  }

  // Form components for saving and reverting merge styles
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
    '#name' => 'save',
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#name' => 'reset',
    '#submit' => array('token_array_reset'),
  );

  // Form components for adding a new merge style
  $form['add'] = array(
    '#title' => 'Add a new merge style',
    '#type' => 'fieldset',
    '#tree' => TRUE,
  );
  $form['add']['machine_name'] = array(
    '#title' => t('Machine-readable name'),
    '#type' => 'textfield',
    '#value' => '',
    '#name' => 'add-machine_name',
    '#maxlength' => '8',
    '#description' => t('Add a unique machine name for a new style, using lowercase alphanumeric.<br />
      Maximum 8 characters. The machine name cannot be changed once set.'),
  );
  $form['add']['human_name'] = array(
    '#title' => t('Human-readable name'),
    '#type' => 'textfield',
    '#value' => '',
    '#name' => 'add-human_name',
    '#maxlength' => '32',
    '#description' => t('Add the displayed name for a new style.'),
  );
  $form['add']['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add new merge style'),
    '#name' => 'new-style-button',
    '#submit' => array('token_array_add_style'),
  );

  return $form;
}

/**
 * Callback for saving merge style settings
 */
function token_array_settings_form_submit($form, $form_state) {
  // Get the previous settings
  $merge_styles = variable_get('token_array_settings', token_array_merge_styles());
  foreach ($merge_styles as $machine_name => $merge_style) {
    foreach ($merge_style as $property => $vaule) {
      // Change each style according to posted data
      $new_styles[$machine_name][$property] = $form['#post'][$machine_name . '-' . $property];
    }
  }

  variable_set('token_array_settings', $new_styles);
}

/**
 * Callback for saving a single merge style
 */
function token_array_save($form, $form_state) {
  // Get the previous settings
  $merge_styles = variable_get('token_array_settings', token_array_merge_styles());
  foreach ($merge_styles as $machine_name => $merge_style) {
    if ($form_state['clicked_button']['#name'] == $machine_name . '-save') {
      foreach ($merge_style as $property => $vaule) {
        // Change each style according to posted data
        $merge_styles[$machine_name][$property] = $form['#post'][$machine_name . '-' . $property];
      }
    }
  }

  variable_set('token_array_settings', $merge_styles);
}

/**
 * Callback for deleting a given merge style
 */
function token_array_delete($form, $form_state) {
  $merge_styles = variable_get('token_array_settings', token_array_merge_styles());
  unset($merge_styles[$form_state['clicked_button']['#name']]);
  variable_set('token_array_settings', $merge_styles);
}

/**
 * Callback for adding a given merge style and giving it default values
 *
 * Note that this function is not safe att all – no checks are made against
 * the input data, not even that a new merge style name exists. This should
 * of course be fixed.
 */
function token_array_add_style($form, $form_state) {

  $machine_name = $form_state['clicked_button']['#post']['add-machine_name'];

  // Check that the machine name is actually set
  if (strlen($machine_name) < 1) {
    form_set_error('add-machine_name', t('No machine name was entered.'));
    return;
  }
  // Check that the machine name has no disallowed characters
  if (!preg_match('/^[a-z][a-z0-9_]*$/', $machine_name)) {
    form_set_error('add-machine_name', t('Machine name must be alphanumeric and underscores only, and the first character must be a lower case letter.'));
    return;
  }
  // Check that the machine name is not too long
  // (This only happens if someone hacks the form.)
  if (strlen($machine_name) > 8) {
    form_set_error('add-machine_name', t('Maximum 8 characters for this machine name, like in the old days. (This is to prevent huge tokens.)'));
    return;
  }
  // Check that machine name is unique
  $merge_styles = variable_get('token_array_settings', token_array_merge_styles());
  if (is_array($merge_styles[$machine_name])) {
    form_set_error('add-machine_name', t('The machine name %machine_name is already taken.',
      array('%machine_name' => $machine_name)));
    return;
  }



  $merge_styles = variable_get('token_array_settings', token_array_merge_styles());
  $merge_styles[$machine_name]
    = array(
      'name' => $form_state['clicked_button']['#post']['add-human_name'],
      'prefix' => '',
      'suffix' => '',
      'infix' => ' ',
    );
  variable_set('token_array_settings', $merge_styles);
}

/**
 * Callback for resetting merge styles to values in code
 */
function token_array_reset($form, $form_state) {
  variable_del('token_array_settings');
  variable_set('token_array_settings', token_array_merge_styles());
}
