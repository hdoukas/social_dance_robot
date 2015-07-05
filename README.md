# A social dancing robot

This is the source code for a [social dancing robot]. It's built using an Arduino Uno and a CC3000 WiFi shield. It was developed during the [WSO2 EU 2015] Conference Hackathon.

The logic behind is that you visit the webpage of the robot with your smartphone and you tilt (dance with) your phone. The webpage (HTML5) detects the movement and delivers a dancing command to the Arduino over MQTT (through a PHP script).

When more than 2 users send a dancing command, then the robot changes its dancing style. 

See the video here:

[![IMAGE ALT TEXT HERE](http://img.youtube.com/vi/_5-DTexgfaM/0.jpg)](http://www.youtube.com/watch?v=YOUTUBE_VIDEO_ID_HERE)

License
----

MIT


**Free Software, Hell Yeah!**

[social dancing robot]:http://blog.buildinginternetofthings.com/2015/07/05/a-social-dancing-robot-with-arduino-and-mqtt/
[WSO2 EU 2015]: http://eu15.wso2con.com/
