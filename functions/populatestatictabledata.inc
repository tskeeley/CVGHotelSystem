<?php
// Build tables, and populate the static/default content

if (!empty($NOLOOP)) {
	// ACK!  We are looping!!!  Trying to write to an undefined table!
	die("Static Data file is self-referring!  " . __FILE__ . " Failed.  ($query) from DB: " . mysql_error());
}
else {
	// Set this to make sure that what we are about to do doesn't loop us into infinity.
	$NOLOOP=TRUE;
}

// Ops DB Structure
$OPS_TABLES=array(
  // LOGSTABLE needs to be the first table created as the create process posts to it on success (MakeLog)
  $GLOBALS['SYSTEMLOGTABLE'] => "
    EntryID INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    LogEntry TEXT,
    Created TIMESTAMP DEFAULT CURRENT_TIMESTAMP
  ",
  $GLOBALS['LOGSTABLE'] => "
    MasterEntryID INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    Tags BIGINT UNSIGNED NOT NULL,
    InterestedDepartments BIGINT UNSIGNED,
    Active TINYINT(1),
    LASTUPDATE TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    KEY Tags (Tags)
  ",
  $GLOBALS['LOGENTRIESTABLE'] => "
    EntryID INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    MasterEntryID INT UNSIGNED NOT NULL,
    UserName INT NOT NULL,
    LogEntry TEXT NOT NULL,
    Created TIMESTAMP DEFAULT CURRENT_TIMESTAMP
  ",
  $GLOBALS['LOGTAGSTABLE'] => "
    Tag BIGINT UNSIGNED NOT NULL PRIMARY KEY,
    Label VARCHAR(50) NOT NULL,
    SecureView TINYINT(1),
    RoleBasedEdit TINYINT(1),
    DisplayGroup TINYINT(2) NOT NULL
  ",
  $GLOBALS['DEPARTMENTSTABLE'] => "
    Tag BIGINT UNSIGNED NOT NULL PRIMARY KEY,
    Department VARCHAR(50) NOT NULL,
    DisplayGroup TINYINT(2) NOT NULL
  ",
  $GLOBALS['AUTHENTICATIONTABLE'] => "
    ATTENDEE_ID INT NOT NULL PRIMARY KEY,
    LoginName VARCHAR(20) NOT NULL UNIQUE,
    Password VARCHAR(100) NOT NULL,
    LastChanged TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
  ",
  $GLOBALS['DUTYTABLE'] => "
    Area TINYINT(2) UNSIGNED NOT NULL,
    Shift VARCHAR(40) NOT NULL,
    Name VARCHAR(40),
    Note VARCHAR(70),
    LastChanged TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    PRIMARY KEY (AREA,SHIFT)
  ",
  $GLOBALS['EQUIPMENTTABLE'] => "
    EquipmentID INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    TITLE VARCHAR(15) NOT NULL,
    TYPE TINYINT UNSIGNED NOT NULL,
    ID VARCHAR(30),
    NOTE VARCHAR(50),
    BATTERY SMALLINT UNSIGNED,
    PERSONAL TINYINT(1),
    CHECKEDOUT MEDIUMINT UNSIGNED,
    LASTUPDATE TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    KEY PERSONAL (PERSONAL),
    KEY CHECKEDOUT (CHECKEDOUT),
    KEY TYPE (TYPE)
  ",
  $GLOBALS['LOSTFOUNDTABLE'] => "
    ItemID INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
    Lost TINYINT(1),
    Type VARCHAR(3) NOT NULL,
    ShortDesc VARCHAR(50) NOT NULL,
    Description TEXT,
    KEY LOST (LOST),
    KEY TYPE (TYPE)
  ",
  $GLOBALS['LOSTFOUNDTYPESTABLE'] => "
    Type VARCHAR(3) NOT NULL PRIMARY KEY,
    Description VARCHAR(20)
  ",
  $GLOBALS['USERSKILLSTABLE'] => "
    ATTENDEE_ID INT NOT NULL PRIMARY KEY,
    Basic VARCHAR(4),
    Gopher VARCHAR(4),
    Triage VARCHAR(4),
    Comm_Officer VARCHAR(4),
    Dispatch_Officer VARCHAR(4),
    Wandering_Host VARCHAR(4),
    XO VARCHAR(4),
    Ops_SubHead VARCHAR(4),
    Ops_Head VARCHAR(4)
  "
);


// Registration DB Structure
$REG_TABLES=array(
  $GLOBALS['ADDRESSTYPETABLE'] => "
    ADDRESS_TYPE_ID INT(11) NOT NULL AUTO_INCREMENT PRIMARY KEY,
    ADDRESS_TYPE VARCHAR(50) default NULL
  ",
  $GLOBALS['ATTENDEETABLE'] => "
    ATTENDEE_ID int(11) NOT NULL auto_increment PRIMARY KEY,
    FIRST_NAME varchar(50) NOT NULL,
    MIDDLE_NAME varchar(50) default NULL,
    LAST_NAME varchar(50) NOT NULL,
    DATE_OF_BIRTH datetime default NULL,
    COMMENTS varchar(50) default NULL,
    EMAIL varchar(255) default NULL,
    BAD_EMAIL_IND tinyint(4) default 0,
    HOME_PHONE varchar(15) default NULL,
    WORK_PHONE varchar(15) default NULL,
    OTHER_PHONE varchar(15) default NULL,
    ADDRESS_LINE_1 varchar(50) default NULL,
    ADDRESS_LINE_2 varchar(50) default NULL,
    ADDRESS_LINE_3 varchar(50) default NULL,
    ADDRESS_CITY varchar(50) default NULL,
    ADDRESS_STATE_CODE varchar(2) default 'MN',
    ADDRESS_ZIP varchar(20) default NULL,
    FOREIGN_POSTAL_CODE varchar(12) default NULL,
    BAD_ADDRESS_IND tinyint(4) default 0,
    BAD_ADDRESS_DATE datetime default NULL,
    COUNTRY_CODE varchar(3) default 'USA',
    NO_SEND_IND tinyint(4) default 0,
    ADDRESS_TYPE_ID int(11) default 0,
    KEY ADDRESS_LINE_1 (ADDRESS_LINE_1),
    KEY ADDRESS_LINE_2 (ADDRESS_LINE_2),
    KEY ADDRESS_LINE_3 (ADDRESS_LINE_3),
    KEY ADDRESS_STATE_CODE (ADDRESS_STATE_CODE),
    KEY ADDRESS_TYPE_ID (ADDRESS_TYPE_ID),
    KEY COUNTRY_CODE (COUNTRY_CODE),
    KEY DATE_OF_BIRTH (DATE_OF_BIRTH),
    KEY FOREIGN_POSTAL_CODE (FOREIGN_POSTAL_CODE),
    KEY LAST_NAME (LAST_NAME,FIRST_NAME),
    KEY ADDRESS_ZIP (ADDRESS_ZIP)
  ",
  $GLOBALS['BADGETABLE'] => "
    ATTENDEE_ID int(11) default 0,
    YEAR int(11) NOT NULL,
    BADGE_NUMBER int(11) NOT NULL,
    BADGE_NAME varchar(32) NOT NULL,
    BADGE_PRINTED_IND tinyint(4) default 0,
    PRIMARY KEY  (YEAR,BADGE_NUMBER),
    KEY ATTENDEE_ID (ATTENDEE_ID),
    KEY YEAR (YEAR)
  ",
  $GLOBALS['BADGENUMBERTABLE'] => "
    BADGE_NUMBER int(11) NOT NULL default 0,
    PRIMARY KEY  (BADGE_NUMBER)
  ",
  $GLOBALS['BANTABLE'] => "
    ATTENDEE_ID int(11) NOT NULL default 0,
    YEAR int(11) NOT NULL default 0,
    DATE_CREATED datetime default NULL,
    ACTIVE_IND tinyint(4) default 0,
    COMMENTS varchar(50) default NULL,
    PRIMARY KEY  (ATTENDEE_ID,YEAR),
    KEY ATTENDEE_ID (ATTENDEE_ID)
  ",
  $GLOBALS['COUNTRYTABLE'] => "
    COUNTRY_CODE varchar(3) NOT NULL,
    COUNTRY varchar(48) NOT NULL,
    PRIMARY KEY  (COUNTRY_CODE),
    UNIQUE KEY COUNTRY (COUNTRY)
  ",
  $GLOBALS['STATETABLE'] => "
    STATE_CODE varchar(3) NOT NULL,
    STATE varchar(32) default NULL,
    PRIMARY KEY  (STATE_CODE),
    UNIQUE KEY STATE (STATE)
  "
);
// End Registration Tables

// Combine all table structures
$DBTABLES=$OPS_TABLES + $REG_TABLES;

// Add all "static" content functions here (at least default vaules)

function PopulateStaticData ($table) {
  global $LOGTAGSTABLE, $LOSTFOUNDTYPESTABLE, $DEPARTMENTSTABLE;
  switch ($table) {
    case $LOGTAGSTABLE:
      // Consider moving OptionTypes to seperate relational DB, and referentially populate data
	    //$EntryFields="Type,Headline,Public,Page,HeadlineClass,ActiveLabel,InactiveLabel,OptionTypes";
	    $EntryFields="Tag,Label,SecureView,RoleBasedEdit,DisplayGroup";
      $EntryData=array("1,'Emergency',0,1,3",
        "2,'Medical',0,1,3",
        "4, 'Flagged',0,1,1",
        "8, 'Secure',1,1,2",
        "16,'Rejected',1,1,2",
        "32,'Note/Sticky',0,0,1",
        "64,'Comment',0,0,1",
        "128,'Post-Con Follow-up',0,0,1",
        "256,'Quote',0,0,1"
      );

    foreach($EntryData as $line) {
      $query="REPLACE INTO " . $LOGTAGSTABLE . " (" . $EntryFields . ") VALUES(" . $line . ");";
      run_query($query);
    }

    MakeLog("Populated Static/Default data into " . $table . ": " . mysql_error());
    break;
  case $LOSTFOUNDTYPESTABLE:
    $EntryFields="Type,Description";
    $EntryData=array("0,'Badge'",
      "1,'Clothing'",
      "2,'Jewlery'",
      "3,'Art'",
      "4,'Money'",
      "5,'Eye Wear'",
      "6,'Weapon'",
      "7,'Identification'",
      "8,'Wallet'",
      "9,'Media'",
      "255,'Other'");

    foreach($EntryData as $line) {
      $query="REPLACE INTO " . $LOSTFOUNDTYPESTABLE . " (" . $EntryFields . ") VALUES(" . $line . ");";
      run_query($query);
    }

    MakeLog("Populated Static/Default data into " . $table . ": " . mysql_error());
    break;
  case $DEPARTMENTSTABLE:
    $EntryFields="Tag,Department,DisplayGroup";
    $EntryData=array("1,'Hotel',1",
      "2,'ConSuite',1",
      "4,'Volunteers',1",
      "8,'Parties',1",
      "16,'Dealers Room',2",
      "32,'Registration',2",
      "64,'Art Show',2",
      "128,'Programming',2");
    foreach($EntryData as $line) {
      $query="REPLACE INTO " . $DEPARTMENTSTABLE . " (" . $EntryFields . ") VALUES(" . $line . ");";
      run_query($query);
    }

    MakeLog("Populated Static/Default data into " . $table . ": " . mysql_error());
    break;
  default:
    // ##==## Need static reg content
    //MakeLog("No Static/Default data defined for " . $table); // Debug to system log
    break;
  }
}

// Run check and populate as needed
$Tables=array();
$result=run_query("SHOW TABLES");
while ($row=mysql_fetch_array($result)) {
	array_push($Tables, strtolower($row[0]));
}

foreach($DBTABLES as $Table => $Fields) {
	if (!in_array(strtolower($Table), $Tables)) {
		$query="CREATE TABLE ". $Table ." (" . $Fields . ");";
		run_query($query);
		MakeLog("Created -" . $Table . "- in Database");
		PopulateStaticData($Table);
	}
}

?>