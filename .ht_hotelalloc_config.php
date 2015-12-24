<?php

// CON-In-A-Box Site Config

// Site Specific Variables
$CONSITENAME="CONvergence Hotel Allocation";

// Admin User - Must have until users are set up.
// After that, you can add an astrix to the password which will make it unmatchable (IE: Locked)
$ADMINUSER="Admin"; // Do not leave this blank
$ADMINPASS="sWGjBbyIZsL7Q"; // Crypt hashed password (eg: obtained via htpasswd)

// DBConfig, passwords, etc
// Do not share this info
$DBHOST="localhost";
$DBUSER="hotelalloc-dev";
$DBNAME="hotelalloc_dev";
$DBPASS="test1"; // Plain text database user's password

// Timezone to use
date_default_timezone_set("America/Chicago");

?>
