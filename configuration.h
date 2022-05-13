#pragma once

uint8_t LEDpin = D6;

//MQTT credentials
const char* device_id = "5A_PWM_CNTRL_1"; //used as mqtt_topic and hostname
const char* mqtt_server = "192.168.x.xxx";
int mqtt_port = 1883;
const char* mqtt_user = "homeassistant";
const char* mqtt_pass = "password";


// Wireless access point ssid password
#define WIFI_SSID               "ssid"
#define WIFI_PASSWD             "password"
