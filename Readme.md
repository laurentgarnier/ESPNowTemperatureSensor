# Project description

## Origin
I have in mind to monitor a lots of things in my house with autonomuous sensors that are powering with a battery and linked to a IOT hub. 
To archieve this feature, I choose ESP microcontroller due to their cost, their form factors, the ability to manage deep sleep to save battery power. They are really ease to use with Arduino environment. 
The principle is really easy. The sensors are sleeping the main part of the time. Periodically, they wake up, measure the data they are made for, send an ESP-now message and go in deep sleep. As they are working with a battery (18650) they are systematically measure the battery charge and include it in the message.

The sensors are using ESP D1 mini microcontroller.

## This project
This project is the source code for an autonomuous temperature and humidity sensor by using DHT 22 sensor.

## Getting Started

This project is set to use platform.io with Visual studio code.

### Prerequisites

* Visual studio code installed
* platformio plugin installed
* ESP platform installed

## Authors

  - **Laurent Garnier** 