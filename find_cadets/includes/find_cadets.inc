<?php

/**
 * Return the requested values.
 */
function find_cadets_get_vals($which){

switch($which) {
   case "months":
      $retval = array('','January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December');
      break;
   case "monthAbbrev":
      $retval = array('','January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December');
      break;
   case "days":
      $retval = array('', 'Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday');
      break;
   default:
      $retval = "";
}

return($retval);
}
