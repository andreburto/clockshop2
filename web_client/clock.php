<?php

// Manually input data
$DEVICEID = "";
$ACCESS_TOKEN = "";

// Accepted JSON
$json_input = json_decode(file_get_contents('php://input'), true);

function outputData($data) {
    header("Content-Type: text/plain");
    echo($data);
}

function errMsg($msg=null) {
    return sprintf("{\"status\":0,\"msg\":\"%s\"}", $msg);
}

function setTimeManual() {
    global $ACCESS_TOKEN, $json_input;
    // Set the hour
    $data = sprintf("access_token=%s&args=h%s", $ACCESS_TOKEN, strval($json_input['h']));
    $res = json_decode(postRequest($data, "settime"), true);
    if ($res['return_value'] != $json_input['h']) {
        throw new Exception("Could not set hour.");
    }
    // Set the minute
    $data = sprintf("access_token=%s&args=m%s", $ACCESS_TOKEN, strval($json_input['m']));
    $res = json_decode(postRequest($data, "settime"), true);
    if ($res['return_value'] != $json_input['m']) {
        throw new Exception("Could not set minute.");
    }
    // Success
    outputData(json_encode(array('status'=>1,'msg'=>'Time has been set.')));
}

function setTimeAuto() {
    global $ACCESS_TOKEN, $json_input;
    $data = sprintf("access_token=%s&args=%s", $ACCESS_TOKEN, strval($json_input['mode']));
    $res = postRequest($data, "setmanual");
    $json = json_decode($res, true);
    outputData(json_encode(array('status'=>1,'autoset'=>$json['return_value'])));
}

function playStopMusic() {
    global $ACCESS_TOKEN;
    $data = sprintf("access_token=%s", $ACCESS_TOKEN);
    $res = postRequest($data, "playmusic");
    $json = json_decode($res, true);
    outputData(json_encode(array('status'=>1,'playstop'=>$json['return_value'])));}

function checkMode() {
    getVariable("autoset");
}

function checkMusic() {
    getVariable("playstop");
}

function getVariable($varname) {
    global $DEVICEID, $ACCESS_TOKEN;
    $url = "https://api.particle.io/v1/devices/".$DEVICEID."/".$varname."?access_token=".$ACCESS_TOKEN;
    $ch = curl_init($url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_HEADER, false);
    curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
    $response = curl_exec($ch);
    $sc = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    if ($sc != 200) { throw new Exception(sprintf("HTTP CODE: %s", $sc)); }
    $json = json_decode($response, true);
    outputData(json_encode(array('status'=>1,$json['name']=>$json['result'])));
}

function postRequest($data=null, $function=null) {
    global $DEVICEID;
    $url = "https://api.particle.io/v1/devices/".$DEVICEID."/".$function;
    $ch = curl_init($url);
    curl_setopt($ch, CURLOPT_POST, true);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $data);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_HEADER, false);
    curl_setopt($ch, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
    $response = curl_exec($ch);
    $sc = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    if ($sc != 200) { throw new Exception(sprintf("HTTP CODE: %s", $sc)); }
    return $response;
}

// The action router
try {
    switch($json_input['action']) {
        case "manual":
            setTimeManual();
            break;
        case "auto":
            setTimeAuto();
            break;
        case "music":
            playStopMusic();
            break;
        case "checkmusic":
            checkMusic();
            break;
        default:
            checkMode();
    }
} catch(Exception $e) {
    outputData(errMsg($e->getMessage()));
}

// Exit the script
exit;

?>