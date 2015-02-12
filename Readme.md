Video Remote
===============

My solution for: https://slack-files.com/T02U9E44U-F03EA6NKJ-cf62714632
Needs Qt >= 5.2.

Build it
---
      cd VideoRecorder
      mkdir build
      cd build
      qmake -r ..
      make


Problem statement
---

Recruitment Assignment Android - 1.2
Scenario

Our customer has a video screen at an exhibition that can be remotely controlled on a local network. In front of the video screen is an Android device that visitors at the exhibition use to control whats playing.

The customer needs an Android application that can run on a phone or tablet and control the machine. They are in a fix and need the app as soon as possible.
Specifications

Write a simple Android application, running on 4.1 or later. The exhibition visitor should be able to activate three different video sequences through the app.

For each video sequence an administrator using the app should be able to customize what is being sent to to the video screen device.

The protocol is based on sequences of UDP packages with customizable text payloads, i.e. commands, and customizable time delay between each message.

The administrator should also be able to change the target host and port.

The message format should be a plain text string terminated with \n.

Here is an example for what might be sent if the exhibition visitor initiates a video sequence.

      start video1

a 500 ms pause

      stop video1
      start video2

a 1200 ms pause

      start fade

a 200 ms pause

      stop fade
      stop video2

Optional Requirements

    Broadcasted UDP messages option.
    A unit test or two.
