<?php
// Various Authentication needed routines - Relies on NeonCRM

function LogoutUser() {
  $_SESSION=[];
  if (isset($_COOKIE[session_name()])) {
    setcookie(session_name(), '', time()-42000, '/');
  }
  session_destroy();
}

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

function getCurrentUser($user=FALSE, $position) { //string(username), or False
  if ($user) {
    if($user == $GLOBALS['ADMINUSER']) {
      // $ADMINUSER is allowed all postitions, Log 'em in!
      return $user;
    } else {
      if (UserAuthorized($user, $position)) {
        return $user;
      } else {
        return False;
      }
    }
  } else {
    return False;
  }
}

function doUserAndPasswordMatch($user, $password) {
  if ($user == $GLOBALS['ADMINUSER']) {
    return (crypt($password, $GLOBALS['ADMINPASS']) == $GLOBALS['ADMINPASS']);
  }

  $query="SELECT Password FROM " . $$GLOBALS['AUTHENTICATIONTABLE'] . " WHERE LoginName='" . mysql_real_escape_string($user) . "'";
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

function CheckAdminStatus($user) {
  // Is user allowed to access the Administration Fuctions
  if($user == $GLOBALS['ADMINUSER']) {
    // Admin user is always allowed access
    return(TRUE);
  }

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

function LoginUser($user, $password, $position) {
  if (doUserAndPasswordMatch($user, $password)) {
    // Log in the User
    session_start();
    $_SESSION['username'] = $user;
    $_SESSION['position'] = $position;
    MakeLog("Login Allowed");

    if (getCurrentUser($user, $position)) {
      // Logged in successfully, Grab user's Real Name
      if($user == $GLOBALS['ADMINUSER']) {
        $_SESSION['FullName'] = "Admin User";
      } else {
        $query="SELECT ATTENDEE_ID FROM " . $AUTHENTICATIONTABLE . " WHERE LoginName='" . $user . "'";
        $result=run_query($query);
        $rows=mysql_fetch_assoc($result);

        $query="SELECT FIRST_NAME,LAST_NAME FROM " . $ATTENDEETABLE . " WHERE ATTENDEE_ID='" . $rows['ATTENDEE_ID'] . "'";
        $result=run_query($query);
        $rows=mysql_fetch_assoc($result);

        $_SESSION['FullName'] = $rows['FIRST_NAME'] . " " . $rows['LAST_NAME'];
      }
      $_SESSION['administrator'] = CheckAdminStatus($user);
      goSite('/index.php?Function=main'); // goto default
    } else {
      LogoutUser();
      MakeLog("User not authorized for " . $position . ", logged out");
      $RETRY=2; // Not Authorized for requested position
    }
  } else {
    MakeLog("Failed Login Attempt");
    $RETRY=1; // Bad password
  }
}
?>
