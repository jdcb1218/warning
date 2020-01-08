<?php 
module_load_include('inc', 'entityform', 'entityform.admin');

/**
 * Implements hook_form().
 */

function warning_salesforce_config_entity($form, &$form_state) {
  $form['entity'] = array(
	'#type' => 'fieldset', 
	'#title' => t('Entity'), 
	'#collapsible' => TRUE, 
	'#collapsed' => FALSE,
  );
  $form['entity']['entity_id_config'] = array(
  	'#type' => 'textfield',
	'#attributes' => array('type' => 'number'),
	'#title' => t('Please add Id Entityform'),
	'#required' => true,
	'#maxlength' => 4,
	'#size' => 3,
	'#default_value' => variable_get('entity_id_config'),
  );
  $form['mail'] = array(
	'#type' => 'fieldset', 
	'#title' => t('Mail'), 
	'#collapsible' => TRUE, 
	'#collapsed' => FALSE,
  );
  $form['mail']['subject'] = array(
  	'#type' => 'textfield',
	'#required' => true,
	'#title' => t('Please add Subject E-mail'),
	'#size' => 100,
	'#default_value' => variable_get('subject'),
  );
  $form['submit'] = array(
	'#type' => 'submit',
	'#value' => t('Submit'),
  );	
  return $form;
}

/**
 * Implements hook_validate().
 */

function warning_salesforce_config_entity_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['entity_id_config'])) {
	form_set_error('entity_id_config', t('Please numerical field'));
  }	
}

/**
 * Implements hook_submit.
 */

function warning_salesforce_config_entity_submit($form, &$form_state) {
  foreach ($form_state['values'] as $key => $value) {
	variable_set($key, $value);
  }
  return drupal_set_message(t('Config save'), 'status', TRUE);
}