<?php

/**
 * Form used by pilots to find cadets who want to fly on a given day, from one of the pilot's
 * preferred airports
 */

function form_email_cadets($form, &$form_state) {
global $user;


$mm = print_r($form_state, TRUE);
$fh = fopen("/tmp/form_state.log", "w");
fwrite($fh, $mm);
fclose($fh);

// Generate list of recipients
$airport_tid = $form_state['input']['airport_tid'];
$recordAry   = $form_state['input']['record_id'];
$recipientList = "";
   foreach($recordAry as $recipient){
   $recipAry = explode(":", $recipient);
   $flightDate = $recipAry[0];
   $capid = $recipAry[1];
   $result = db_query("SELECT field_data_field_first_name.field_first_name_value FNAME, 
                       field_data_field_last_name.field_last_name_value   LNAME,
                       users.mail EMAIL, field_data_field_capid.entity_id 
                       FROM field_data_field_capid 
                       LEFT JOIN users on field_data_field_capid.entity_id=users.uid
                       LEFT JOIN field_data_field_first_name ON field_data_field_first_name.entity_id=users.uid
                       LEFT JOIN field_data_field_last_name ON field_data_field_last_name.entity_id=users.uid
                       WHERE field_data_field_capid.field_capid_value= :capid", array(':capid' => $capid));
      foreach ($result as $record){
      $recipientList .= $record->FNAME . " " . $record->LNAME . "<" . $record->EMAIL . ">,";  
      }
   }
$recipientList = rtrim($recipientList, ",");
drupal_set_message($recipientList);
watchdog('CPJ', $recipientList);
$user_data = user_load($user->uid);
 $mm = "<pre>" . print_r($user_data, TRUE) . "</pre>";
watchdog('CAP', $mm);
$lastName  = (isset($user_data->field_last_name['und'][0]['safe_value'])  ? $user_data->field_last_name['und'][0]['safe_value']  : "<i>No Last Name Set</i>");
$firstName = (isset($user_data->field_first_name['und'][0]['safe_value']) ? $user_data->field_first_name['und'][0]['safe_value'] : "<i>No First Name Set</i>");
$capid     = (isset($user_data->field_capid['und'][0]['safe_value'])      ? $user_data->field_capid['und'][0]['safe_value']      : "<i>No CAPID Set</i>");
$form_description="<h3>Pilot: " . $lastName . ", " . $firstName . "<br>
CAPID: $capid</h3>";

  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Use this form to send email to the selected cadets.'),
    '#description' => t($form_description),
  );
/**
 *  $form['name'] = array(
 *   '#type' => 'fieldset',
 *   '#title' => t('Name'),
 *   '#collapsible' => TRUE,
 *   '#collapsed' => FALSE,
 * );
 *
 *  $form['name']['first'] = array(
 *   '#type' => 'textfield',
 *   '#title' => t('First name'),
 *   '#required' => TRUE,
 *   '#default_value' => "First name",
 *   '#description' => "Please enter your first name.",
 *   '#size' => 20,
 *   '#maxlength' => 20,
 * );
 * $form['name']['last'] = array(
 *   '#type' => 'textfield',
 *   '#title' => t('Last name'),
 *   '#required' => TRUE,
 * );
 *
 */


$today = date("Y-m-d H:i:s", strtotime("+1 Saturday"));
      $form['date'] = array(
        '#type' => 'date_popup',
        '#default_value' => $today,
        '#date_type' => DATE_DATETIME,
        '#date_timezone' => date_default_timezone(),
        '#date_format' => 'D, d-M-Y',
        '#date_increment' => 1,
        '#date_year_range' => '0:+1',
        '#attributes'      => array('class' => array('datefields')),
      );

  $myAirports =array();
     if (isset($user_data->field_airports['und'])){
       foreach($user_data->field_airports['und'] as $airportId){
         $tid = $airportId['tid'];
         $term = taxonomy_term_load($tid);
         $term_name = $term->name;
         $myAirports[$tid] = $term_name;
       }
       $preferred_airport_description = "If you don't see your preferred airport, <a href=\"/user/" . $user->uid . "/edit\" target=\"_blank\" >click here</a> to add it to your profile.";
     } else {
       $preferred_airport_description = "No airports found in your profile. <a href=\"/user/" . $user->uid . "/edit\" target=\"_blank\" >Click here</a> to add some.";
     }

  $form['selected'] = array(
       '#type' => 'select',
       '#title' => t('Airport'),
       '#options' => $myAirports,
 //      '#default_value' => $category['selected'],
       '#description' => t($preferred_airport_description),
   );

//  $form['#action'] = '/pilots/your-cadets/' . $user->uid;
  $form['#action'] = '/pilots/show-cadets';
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Find Cadets',
  );

//  $form['#submit'][]='find_cadets_events_arguments';
  return $form;
}


