<?php
// Various Authentication needed routines - Relies on NeonCRM

function LogoutUser() {
  $_SESSION=[];
  if (isset($_COOKIE[session_name()])) {
    setcookie(session_name(), '', time()-42000, '/');
  }
  session_destroy();
}

function doUserAndPasswordMatch($user, $password) {
  if (!empty($GLOBALS['ADMINUSER']) && $user == $GLOBALS['ADMINUSER']) {
    if (crypt($password, $GLOBALS['ADMINPASS']) == $GLOBALS['ADMINPASS']) {
      $name = "Admin User";
      return $name;
    }
  } else {

  $request = [
    'method' => 'common/authenticateUser', 
    'parameters' => [
      'username' => $user,
      'password' => $password,
    ],
  ];
  $result = $neon->go($request);

  if ($response['operationResult'] == 'SUCCESS') {
    return $response;
  } else {
    return FALSE;
  }
}

function CheckInteractiveStatus($user) {
  // Is user allowed to access this system
  if(!empty($GLOBALS['ADMINUSER']) && $user == $GLOBALS['ADMINUSER']) {
    // Admin user is always allowed access
    return(TRUE);
  }

die("Poll Neon user to check if this user is allowed in the hotel system"); // ##==##
  $query="SELECT ATTENDEE_ID FROM " . $GLOBALS['AUTHENTICATIONTABLE'] . " WHERE LoginName='" . $user . "'";
  $result=run_query($query);
  $rows=mysql_fetch_assoc($result);

  // Need to make this shift name agnostic
  $query="SELECT Ops_SubHead,Ops_Head FROM " . $GLOBALS['USERSKILLSTABLE'] . " WHERE ATTENDEE_ID='" . $rows['ATTENDEE_ID'] . "'";
  $result=run_query($query);

  if (mysql_num_rows($result) == 1) {
    $rows=mysql_fetch_assoc($result);
    return (!empty($rows['Ops_SubHead']) || !empty($rows['Ops_Head']));
  } else {
    return(FALSE);
  }
}

function LoginUser($user, $password) {
  $result = doUserAndPasswordMatch($user, $password);
  if ($result) {
    if (CheckInteractiveStatus($result['accountId'])) {
      // Log in the User - Session started in Functions
      $_SESSION['username'] = $user;
      $_SESSION['FullName'] = $result['name'];
      MakeLog("Login Allowed");
      return(TRUE);
    } else {
      MakeLog($user . " not authorized for interaction session on this system, login denied");
      LogoutUser();
      return(FALSE);
    }
  } else {
    MakeLog("Failed Login Attempt by " . $user);
    return(FALSE);
  }
}
?>
