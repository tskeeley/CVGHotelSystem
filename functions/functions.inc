<?php

// Main setup function

// Discover Self Location
$FUNCTIONDIR=dirname(__FILE__);
$BASEDIR=dirname($FUNCTIONDIR);
$WEBSERVER=$_SERVER['SERVER_NAME'];
$BASEURL="http://" . $WEBSERVER;

// Load Site Specific info
require_once($BASEDIR . "/.ht_ciab_config.php");

// Static Variables
$BUILD=explode(" ", '$Rev: 476 $');
$VERSION="6.0 Build: " . $BUILD[1];
$SITESUPPORTDIR=$BASEDIR . "/sitesupport";
$PAGESDIR=$BASEDIR . "/pages";

// Set a Date stamp
$DS=uniqid();

// Basic site redirector
function goSite($Site=FALSE) {
	if($Site != FALSE) {
		header ("Location: " . $Site);
	} else {
		header ("Location: /index.php?Function=home");
	}
	exit(); // Dump this run, assume the new page will start from scratch
}

// Call the DB and funtions, we always need these
require_once($FUNCTIONDIR . "/dbfunctions.inc");

// Start a session and check for login info
session_start();
?>