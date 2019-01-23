#!/usr/bin/php
<?php
/* * * *
 * * gnomeNibblesLevelsToNibbersLevels.phps
 * *
 * * adjust $sIn, $sOut and run at own risk.
 * *
 * * script to convert gnome-nibbles source level files to
 * * binary files as used in QtSssSNibblers
 * *
 * * gnome-nibbles: git clone https://gitlab.gnome.org/GNOME/gnome-nibbles.git
 * * QtSssSNibblers: git clone git://???/QtSssSNibblers.git (you probably
 * * have it as this file is packaged with it.
 * *
 * * @version Jan 04 2019 (CC) Luke JZ aka SwissalpS
 * * * */
error_reporting(E_ALL); // 0); //
define('NL', chr(10));

$sIn = '/home/git/gnome-nibbles/data/levels/';
$sOut = '/home/git/QtSssSNibblers/Levels/';

$iCount = 0;
$hD = openDir($sIn);
if (!$hD) {
	echo 'Could not open dir: ' . $sIn . NL;
	return;
} // if not able to open

while (false !== ($sFile = readDir($hD))) {

	// check hidden
	if ('.' === $sFile{0}) continue;
	//echo $sFile . NL;

	// check directories
	$sPF = $sIn . $sFile;
	if (is_dir($sPF)) {
		//echo 'Dir found: ' . $sPF . NL;
		continue;
	} // if is dir
	
	if ('level' != substr($sFile, 0, 5)) continue;
	
	$sNum = substr($sFile, 6, 2);
	if ('0' == $sNum{0}) $sNum = substr($sNum, 1);
	$sPFout = $sOut . 'Level_' . $sNum;
	
	echo $sPF . NL;
	echo $sPFout . NL;
	doLevel($sPF, $sPFout);
	
} // loop all level files

exit(0);


function doLevel($sPFin, $sPFout) {

	$sOut = '';
	$aIn = @file($sPFin);
	
	if (false === $aIn) {
	
		echo 'KO:Can not open file: ' . $sPFin . NL;
		exit(2);
		
	} // if failed to read file

	foreach($aIn as $sLine) {

		$sLine = trim($sLine);
	
		if (empty($sLine)) continue;
		
		for($i = 0; $i < strlen($sLine); $i++) {
			
			$sChar = $sLine{$i};
			
			$sOut .= translateChar($sChar);
			
		} // loop each char

	} // loop each line
	
	//echo $sOut . NL;

	file_put_contents($sPFout, $sOut);
	
} // doLevel


function translateChar($sChar) {

	switch (ord($sChar)) {
	
		// R teleporter entrance
		case 82: return chr(220);
		// S teleporter entrance
		case 83: return chr(222);
		// T teleporter entrance
		case 84: return chr(224);
		// U teleporter entrance
		case 85: return chr(226);
		// V teleporter entrance
		case 86: return chr(228);
		// W teleporter entrance
		case 87: return chr(230);
		
		// a empty space
		case 97: return chr(0);
		
		// b wall vertical
		case 98: return chr(200);
		// c wall horizontal
		case 99: return chr(201);
		// d wall corner sw
		case 100: return chr(202);
		// e wall corner se
		case 101: return chr(203);
		// f wall corner nw
		case 102: return chr(204);
		// g wall corner ne
		case 103: return chr(205);
		// h wall T north
		case 104: return chr(206);
		// i wall T east
		case 105: return chr(207);
		// j wall T west
		case 106: return chr(208);
		// k wall T south
		case 107: return chr(209);
		// l wall cross
		case 108: return chr(210);
		
		// m spawn point headed north
		case 109: return chr(90);
		// n spawn point headed west
		case 110: return chr(91);
		// o spawn point headed south
		case 111: return chr(92);
		// p spawn point headed east
		case 112: return chr(93);
		
		// r teleporter exit
		case 114: return chr(221);
		// s teleporter exit
		case 115: return chr(223);
		// t teleporter exit
		case 116: return chr(225);
		// u teleporter exit
		case 117: return chr(227);
		// v teleporter exit
		case 118: return chr(229);
		// w teleporter exit
		case 119: return chr(231);
		
		default:
			echo 'Found unknown char "' . $sChar . '" ' . ord($sChar) . NL;
			exit(3);
		break;
	
	} // switch
	
	return $sChar;

} // translateChar

/* * * *\ The_Header (CC) Luke JZ aka SwissalpS /* * * */
?>
