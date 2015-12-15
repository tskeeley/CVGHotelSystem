<?php
// Load in basic functions
require_once('functions/functions.inc');

// Divert to public page if no session is available
if(empty($_REQUEST['Function'])) {
  goSite('/index.php?Function=public');
} else {
  $pubtest = str_split($_REQUEST['Function'], 6);

  if($pubtest[0] == "public") {
    $noheader=TRUE; // public pages don't need statusbars or logmenus
  } elseif (empty($_SESSION['username'])) {
    // if no username is set and we are not calling a public page, redirect for login needs
    goSite('/index.php?Function=public');
  }

}

// Pre-header process  <process_preheader>
if (is_file($FUNCTIONDIR . '/pre/' . $_REQUEST['Function'] . '.inc')) {
  require($FUNCTIONDIR . '/pre/' . $_REQUEST['Function'] . '.inc');
}

// Header <process_header>
require($PAGESDIR . '/base/header_start.inc');
if (is_file($PAGESDIR . '/head/' . $_REQUEST['Function'] . '.inc')) {
  require($PAGESDIR . '/head/' . $_REQUEST['Function'] . '.inc');
}
require($PAGESDIR . '/base/header_end.inc');

// Page Body <process_body>
require($PAGESDIR . '/base/body_begin.inc');

if (empty($noheader) && empty($_REQUEST['NoHeader'])) {
  require($PAGESDIR . '/base/statusbar.inc');
  require($PAGESDIR . '/base/menubar.inc');
}

require($PAGESDIR . '/body/' . $_REQUEST['Function'] . '.inc');

// footer <process_footer>
require($PAGESDIR . '/base/footer.inc');
require($PAGESDIR . '/base/body_end.inc');
?>
