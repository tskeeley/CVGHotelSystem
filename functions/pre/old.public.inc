<?php

function UserAuthorized($user, $position) {
  // Check if user is allowed to use this position
  $PosTrim=explode("-", $position);
  $PosCheck=$PosTrim[0];

  $query="SELECT ATTENDEE_ID FROM " . $GLOBALS['AUTHENTICATIONTABLE'] . " WHERE LoginName='" . $user . "'";
  $result=run_query($query);
  $rows=mysql_fetch_assoc($result);

  $query="SELECT Basic," . $PosCheck . " FROM " . $GLOBALS['USERSKILLSTABLE'] . " WHERE ATTENDEE_ID='" . $rows['ATTENDEE_ID'] . "'";
  $result=run_query($query);

  if (mysql_num_rows($result) == 1) {
    $rows=mysql_fetch_assoc($result);
    return (!empty($rows[$PosCheck]) && !empty($rows['Basic']));
  } else {
    return(FALSE);
  }
}

function getCurrentUser() { //string(username), or False
  if (!empty($_SESSION['username'])) {
    if($_SESSION['username'] == $GLOBALS['ADMINUSER']) {
      // $ADMINUSER is allowed all postitions, Log 'em in!
      return $_SESSION['username'];
    } else {
      if (UserAuthorized($_SESSION['username'], $_SESSION['position'])) {
        return $_SESSION['username'];
      } else {
        return False;
      }
    }
  } else {
    return False;
  }
}

function doUserAndPasswordMatch($user,$password) {
  if ($user == $GLOBALS['ADMINUSER']) {
    return (crypt($password, $GLOBALS['ADMINPASS']) == $GLOBALS['ADMINPASS']);
  }

  global $AUTHENTICATIONTABLE;

  $query="SELECT Password FROM " . $AUTHENTICATIONTABLE . " WHERE LoginName='" . mysql_real_escape_string($user) . "'";
  $result=run_query($query);

  $count=mysql_num_rows($result);
  if($count > 1) {
    die("Database error.  Got " . $count . " responses from DB when asking for user login info.  Impossible.  Check for corruption!");
  } elseif ($count == 1) {
    $rows=mysql_fetch_assoc($result);
    return (crypt($password, $rows['Password']) == $rows['Password']);
  } elseif ($count == 0) {
    MakeLog("Failed login attempt of non-existant user");
    return False;
  } else {
    die("Unexpected impossible result from database call.  Aborting!");
  }
}

function CheckAdminStatus() {
  // Is user allowed to access the Administration Fuctions
  if($_SESSION['username'] == $GLOBALS['ADMINUSER']) {
    // Admin user is always allowed access
    return(TRUE);
  }

  $query="SELECT ATTENDEE_ID FROM " . $GLOBALS['AUTHENTICATIONTABLE'] . " WHERE LoginName='" . $_SESSION['username'] . "'";
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

// Clear any previous login info
if (isset($_COOKIE[session_name()])) {
  session_start();
  $_SESSION=array();
  session_destroy();
  setcookie(session_name(), '', time()-42000, '/');
}

// Check if there is a login attempt
if (!empty($_REQUEST['user'])) {
  if (doUserAndPasswordMatch($_REQUEST['user'],$_REQUEST['password'])) {
    // Log in the User
    session_start();
    $_SESSION['username'] = $_REQUEST['user'];
    $_SESSION['position'] = $_REQUEST['position'];
    MakeLog("Login Allowed");

    if (getCurrentUser()) {
      // Logged in successfully, Grab user's Real Name
      if($_SESSION['username'] == $GLOBALS['ADMINUSER']) {
        $_SESSION['FullName'] = "Admin User";
      } else {
        $query="SELECT ATTENDEE_ID FROM " . $AUTHENTICATIONTABLE . " WHERE LoginName='" . $_REQUEST['user'] . "'";
        $result=run_query($query);
        $rows=mysql_fetch_assoc($result);

        $query="SELECT FIRST_NAME,LAST_NAME FROM " . $ATTENDEETABLE . " WHERE ATTENDEE_ID='" . $rows['ATTENDEE_ID'] . "'";
        $result=run_query($query);
        $rows=mysql_fetch_assoc($result);

        $_SESSION['FullName'] = $rows['FIRST_NAME'] . " " . $rows['LAST_NAME'];
      }
      $_SESSION['administrator'] = CheckAdminStatus();
      goSite('/index.php?Function=main'); // goto default
    } else {
      MakeLog("User not authorized for " . $_REQUEST['position'] . ", logged out");
      $RETRY=2; // Not Authorized for requested position
      $_SESSION=array();
      session_destroy();
      setcookie(session_name(), '', time()-42000, '/');
    }
  } else {
    MakeLog("Failed Login Attempt");
    $RETRY=1; // Bad password
  }
}
?>
