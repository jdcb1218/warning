<?php 

/**
 * Implements functions().
 */

/**
 * Function to Render Entity Form Config (Agents) 
 *
 * @param $type_error, $error,$uid,$user_wrapper,$empresa
 *   A boolean indicating whether colorbox should be active for the current
 *   URL or not.
 */

function warning_salesforce_config(){
	module_load_include('inc', 'entityform', 'entityform.admin');
	$entityform = entityform_load(variable_get('entity_id_config'), $reset = FALSE);
	return drupal_get_form($entityform->type . '_entityform_edit_form', $entityform, 'edit', 'page');
}

/**
 * Function to Gender Views List Full Reports (@module) (Pcolombia_cron_salesfoce)
 *
 * @param $type_error, $error,$uid,$user_wrapper,$empresa
 *   A boolean indicating whether colorbox should be active for the current
 *   URL or not.
 */

function warning_salesforce_list(){
  $items = array();
  $items['warning_sf_entity_id'] = array(
    '#type'=> 'item',
    '#title' => t('Id entityform'),
    '#markup' => l('Config Id Entityform', 'admin/config/warning_salesforce')
  );
  $items['warning_sf_entity_agents'] = array(
    '#type'=> 'item',
    '#title' => t('Agents'),    
    '#markup' => l('Config Agents', 'admin/config/system/warning_salesforce')
  );
  $items['warning_sf_send_mails'] = array(
    '#type'=> 'item',
    '#title' => t('Mails'),    
    '#markup' => l('Send Mail', 'admin/config/sendmail')
  );
  $items['warning_sf_list_records'] = array(
    '#type'=> 'item',
    '#title' => t('Records warning salesforce'),
    '#markup' => l('Views Full List', 'admin/config/warning/records')
  );
  return $items;
}


function warning_salesforce_list_records(){
    $header_table_edit = array(
      array('data' => t('Id') , 'width' => '20%', 'bgcolor' => '#0077c0'),  
      array('data' => t('Uid') , 'width' => '20%', 'bgcolor' => '#0077c0'),
      array('data' => t('User Email'), 'width' => '20%', 'bgcolor' => '#0077c0'),
      array('data' => t('Name User'), 'width' => '20%', 'bgcolor' => '#0077c0'),
      array('data' => t('Nit Company'), 'width' => '20%', 'bgcolor' => '#0077c0'),
      array('data' => t('Type Error'), 'width' => '20%', 'bgcolor' => '#0077c0'),
      array('data' => t('Log  Php'), 'width' => '20%', 'bgcolor' => '#0077c0'),
      array('data' => t('Log Salesforce'), 'width' => '20%', 'bgcolor' => '#0077c0'),
      array('data' => t('Day Created'), 'width' => '20%', 'bgcolor' => '#0077c0'),
      array('data' => t('Created'), 'width' => '20%', 'bgcolor' => '#0077c0'),
    ); 

    $query = db_select('warning_salesforce_reports', 'e');
    $query->fields('e');
    $query->orderBy('created', 'DESC');
    $result= $query->execute();

    while($record_edit_table = $result->fetchAssoc()) {
      $rows_table_edit[] =  
        array(  
          array(   
            'data' => $record_edit_table['id'], 'width' => '20%', 'bgcolor' => '#f0f0f0',
          ),
          array(  
             'data' => $record_edit_table['uid'] , 'width' => '20%', 'bgcolor' => '#f0f0f0',
          ),
          array(  
             'data' => $record_edit_table['user_email'], 'width' => '20%', 'bgcolor' => '#f0f0f0',
          ),
          array(  
             'data' => $record_edit_table['name_user'], 'width' => '20%', 'bgcolor' => '#f0f0f0',
          ),
          array(  
             'data' => $record_edit_table['nit_company'], 'width' => '20%', 'bgcolor' => '#f0f0f0',
          ),
          array(  
             'data' => $record_edit_table['type_error'], 'width' => '20%', 'bgcolor' => '#f0f0f0',
          ),
          array(  
             'data' => $record_edit_table['log_php'], 'width' => '20%', 'bgcolor' => '#f0f0f0',
          ),
          array(  
             'data' => $record_edit_table['log_web_service'], 'width' => '20%', 'bgcolor' => '#f0f0f0',
          ),          
          array(  
             'data' => $record_edit_table['day_created'], 'width' => '20%', 'bgcolor' => '#f0f0f0',
          ),
          array(  
             'data' => $record_edit_table['created'], 'width' => '20%', 'bgcolor' => '#f0f0f0',
          ),          
      );
    } 


    $caption_table_edit = t('Table for views information peoples');
    $per_page = 5;
    $current_page = pager_default_initialize(count($rows_table_edit), $per_page);
    // Split your list into page sized chunks
    $chunks = array_chunk($rows_table_edit, $per_page, TRUE);
    // Show the appropriate items from the list
    $output = theme('table', array('header' => $header_table_edit, 'rows' => $chunks[$current_page], 'caption'=> $caption_table_edit));
    // Show the pager
    $output .= theme('pager', array('quantity',count($rows_table_edit)));
    return $output;   
}



/**
 * Function to Get data by cront Job (@module) (Pcolombia_cron_salesfoce)
 *
 * @param $type_error, $error,$uid,$user_wrapper,$empresa
 *   A boolean indicating whether colorbox should be active for the current
 *   URL or not.
 */


function warning_salesforce_getdata($type_error,$error,$uid,$user,$empresa){
  switch ($type_error) {
    case 'ws_connection_error':
      ws_crud_connection_error($type_error,$error,$uid,$user,$empresa);
      break;
    case 'impossible_create_account_sf':
      ws_crud_create_warning($type_error,$error,$uid,$user,$empresa);
      break; 
    case 'oportunitie_disabled':
      ws_crud_disabled_oportunitie($type_error,$error,$uid,$user,$empresa);
      break;
    case 'done_create_account_sf':
      ws_crud_create_warning($type_error,$error,$uid,$user,$empresa);
      break;
    case 'done_update_contact_sf':
      ws_crud_create_warning($type_error,$error,$uid,$user,$empresa);
      break;      
  }
}

/**
 * Function By Insert info of Conexion by Day.
 *
 * @param type_error,error,uid,user,empresa
 *   A boolean indicating whether colorbox should be active for the current
 *   URL or not.
 */

function ws_crud_connection_error($type_error,$error,$uid,$user,$empresa){
  $data = db_insert('warning_salesforce_reports')
    ->fields(array('log_web_service', 'type_error','day_created','created'))
    ->values(array(
      'log_web_service' => $error->getMessage(),
      'type_error' => $type_error,
      'day_created' => date('Y-m-d'),
      'created' => REQUEST_TIME,
    ))
    ->execute();
  return  drupal_set_message(t($error->getMessage()), 'error');
}

/**
 * Function to add failed account in salesforce.
 *
 * @param $type_error, $error,$uid,$user_wrapper,$empresa
 *   A boolean indicating whether colorbox should be active for the current
 *   URL or not.
 */

function ws_crud_create_warning($type_error, $error,$uid,$user_wrapper,$empresa){
  // Check if issset register today
    $result = db_select('warning_salesforce_reports', 'n')
      ->fields('n')
      ->condition('day_created', date('Y-m-d'),'=')
      ->condition('nit_company', $user_wrapper->field_nit->value(),'=')
      ->execute()
      ->fetchAssoc();

    if (empty($result)) {
      $data = db_insert('warning_salesforce_reports')
      ->fields(array('uid','user_email','name_user','nit_company',
                    'name_company','type_error','log_php','log_web_service','day_created','created'))
      ->values(array(
        'uid' => $uid,
        'user_email' => $user_wrapper->mail->value(),
        'name_user' => $user_wrapper->name->value(),
        'nit_company' => $user_wrapper->field_nit->value(),
        'name_company' => $user_wrapper->field_nombre_de_la_empresa->value(),
        'type_error' => $type_error,
        'log_php' => '',
        'log_web_service' => '',
        'day_created' => date('Y-m-d'),
        'created' => REQUEST_TIME,
      ))
      ->execute();
    }
    return drupal_set_message(t('Update records'), 'status', TRUE);
}

/**
 * Function to add failed account in salesforce.
 *
 * @param $type_error, $error,$uid,$user_wrapper,$empresa
 *   A boolean indicating whether colorbox should be active for the current
 *   URL or not.
 */


function ws_crud_disabled_oportunitie($type_error,$error,$uid,$offer,$empresa){
  if ($empresa->Numero_de_Identificacion__c) {
    // Check User Asc Company
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'user')
      ->fieldCondition('field_nit', 'value', $empresa->Numero_de_Identificacion__c, '=');
    $results = $query->execute();
    // Load Entity @User
    $user = reset($results['user']);
    $account = user_load($user->uid);
    // Load Wrapper Entity @User 
    $user_wrapper = entity_metadata_wrapper('user', $account);
    // Check if issset register today
    $result = db_select('warning_salesforce_reports', 'n')
      ->fields('n')
      ->condition('day_created', date('Y-m-d'),'=')
      ->condition('nit_company', $user_wrapper->field_nit->value(),'=')
      ->execute()
      ->fetchAssoc();
    if (empty($result)) {      
      $data = db_insert('warning_salesforce_reports')
        ->fields(array('uid','user_email','name_user','nit_company',
                      'name_company','type_error','log_php','log_web_service','day_created','created'))
        ->values(array(
          'uid' => $user->uid,
          'user_email' => $user_wrapper->mail->value(),
          'name_user' => $user_wrapper->name->value(),
          'nit_company' => $user_wrapper->field_nit->value(),
          'name_company' => $user_wrapper->field_nombre_de_la_empresa->value(),
          'type_error' => $type_error,
          'log_php' => '',
          'log_web_service' => '',
          'day_created' => date('Y-m-d'),
          'created' => REQUEST_TIME,
        ))
        ->execute();
    }  
  }
}

/**
 * Function to Send Mail Every Day.
 *
 * @param $type_error, $error,$uid,$user_wrapper,$empresa
 *   A boolean indicating whether colorbox should be active for the current
 *   URL or not.
 */


function sendmail_page(){
    $emails = '';
  	module_load_include('inc', 'entityform', 'entityform.admin');
  	$entityform = entityform_load(variable_get('entity_id_config'), $reset = FALSE);
  	foreach ($entityform->field_agente['und'] as $key => $value) {
  	  $entity_load =  entity_load('paragraphs_item', array($value['value']));
    	$paragraphs = reset($entity_load);
  		$wrapper = entity_metadata_wrapper('paragraphs_item', $paragraphs);
  		if ($wrapper->field_activar_agente->value() == TRUE) {
  			$emails .= $wrapper->field_email_agente->value() .',';
  		}
  	}

    $header_table_edit = array(
      array('data' => t('Id') , 'width' => '20%', 'bgcolor' => '#0077c0'),  
      array('data' => t('Uid') , 'width' => '20%', 'bgcolor' => '#0077c0'),
      array('data' => t('User Email'), 'width' => '20%', 'bgcolor' => '#0077c0'),
      array('data' => t('Name User'), 'width' => '20%', 'bgcolor' => '#0077c0'),
      array('data' => t('Nit Company'), 'width' => '20%', 'bgcolor' => '#0077c0'),
      array('data' => t('Type Error'), 'width' => '20%', 'bgcolor' => '#0077c0'),
      array('data' => t('Log  Php'), 'width' => '20%', 'bgcolor' => '#0077c0'),
      array('data' => t('Log Salesforce'), 'width' => '20%', 'bgcolor' => '#0077c0'),
      array('data' => t('Day Created'), 'width' => '20%', 'bgcolor' => '#0077c0'),
      array('data' => t('Created'), 'width' => '20%', 'bgcolor' => '#0077c0'),
    ); 

    $query = db_select('warning_salesforce_reports', 'e');
    $query->fields('e');
    $query->condition('day_created',date('Y-m-d'),'=');
    $result= $query->execute();

    while($record_edit_table = $result->fetchAssoc()) {
      $rows_table_edit[] =  
        array(  
          array(   
            'data' => $record_edit_table['id'], 'width' => '20%', 'bgcolor' => '#f0f0f0',
          ),
          array(  
             'data' => $record_edit_table['uid'] , 'width' => '20%', 'bgcolor' => '#f0f0f0',
          ),
          array(  
             'data' => $record_edit_table['user_email'], 'width' => '20%', 'bgcolor' => '#f0f0f0',
          ),
          array(  
             'data' => $record_edit_table['name_user'], 'width' => '20%', 'bgcolor' => '#f0f0f0',
          ),
          array(  
             'data' => $record_edit_table['nit_company'], 'width' => '20%', 'bgcolor' => '#f0f0f0',
          ),
          array(  
             'data' => $record_edit_table['type_error'], 'width' => '20%', 'bgcolor' => '#f0f0f0',
          ),
          array(  
             'data' => $record_edit_table['log_php'], 'width' => '20%', 'bgcolor' => '#f0f0f0',
          ),
          array(  
             'data' => $record_edit_table['log_web_service'], 'width' => '20%', 'bgcolor' => '#f0f0f0',
          ),          
          array(  
             'data' => $record_edit_table['day_created'], 'width' => '20%', 'bgcolor' => '#f0f0f0',
          ),
          array(  
             'data' => $record_edit_table['created'], 'width' => '20%', 'bgcolor' => '#f0f0f0',
          ),          
      );
    } 
    $output = theme('table', array('header' => $header_table_edit, 'rows' => $rows_table_edit)); 
    drupal_mail('warning_salesforce', 'send_mail', $emails, language_default(), 
    $output, variable_get('site_mail') , TRUE);
    drupal_set_message(t('Send Mail'), 'status', TRUE);
    return $output;
}