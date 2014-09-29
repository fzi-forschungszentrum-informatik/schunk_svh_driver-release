﻿// this is for emacs file handling -*- mode: c++; indent-tabs-mode: nil -*-

// -- BEGIN LICENSE BLOCK ----------------------------------------------
// This file is part of the SCHUNK SVH Driver suite.
//
// This program is free software licensed under the LGPL
// (GNU LESSER GENERAL PUBLIC LICENSE Version 3).
// You can find a copy of this license in LICENSE folder in the top
// directory of the source code.
//
// © Copyright 2014 SCHUNK Mobile Greifsysteme GmbH, Lauffen/Neckar Germany
// © Copyright 2014 FZI Forschungszentrum Informatik, Karlsruhe, Germany
//
// -- END LICENSE BLOCK ------------------------------------------------

//----------------------------------------------------------------------
/*!\file
 *
 * \author  Georg Heppner
 * \date    2014-09-23
 *
 * This file contains a very very simple test node that will generate a
 * sin movement for slected fingers of the Schunk Five finger hand.
 * It is only meant to be used as a quick test and demo program to use the
 * Schunk five finger hand and test its operation.
 */
//----------------------------------------------------------------------

// ROS includes.
#include <ros/ros.h>

// Messages
#include <std_msgs/Int8.h>
#include <std_msgs/Empty.h>
#include <sensor_msgs/JointState.h>

#include <icl_core/EnumHelper.h>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


// Consts
// Loop Rate (i.e Frequency) of the ROS node -> 50 = 50HZ
const double loop_rate = 50;
// Time of a half Sin. i.e. 10 = In 10 Seconds the selected fingers will perform a close and open (Sin to 1PI)
const double sin_duration = 10;


// Local Vars
bool running = true;
/*--------------------------------------------------------------------
 * Callback functions
 *------------------------------------------------------------------*/
// Toggle on off
void runCallback(const std_msgs::Empty&)
{
  running = !running;
}
/*--------------------------------------------------------------------
 * main()
 * Main function to set up ROS node.
 *------------------------------------------------------------------*/

int main(int argc, char **argv)
{
  // Set up ROS.
  ros::init(argc, argv, "svh_sine_test");
  ros::NodeHandle nh("~");

  // Subscribe connect topic (Empty)
  ros::Subscriber run_sub = nh.subscribe("toggle_run", 1, runCallback);

  // Publish current target positions
  ros::Publisher channel_pos_pub = nh.advertise<sensor_msgs::JointState>("channel_targets", 1);

  // joint state message template
  sensor_msgs::JointState channel_pos;
  channel_pos.name.resize(9);
  channel_pos.position.resize(9, 0.0);

  // Pre Fill the Names of the Finger
  channel_pos.name[0] = "Thumb_Flexion";
  channel_pos.name[1] = "Thumb_Opposition";
  channel_pos.name[2] = "Index_Finger_Distal";
  channel_pos.name[3] = "Index_Finger_Proximal";
  channel_pos.name[4] = "Middle_Finger_Distal";
  channel_pos.name[5] = "Middle_Finger_Proximal";
  channel_pos.name[6] = "Ring_Finger";
  channel_pos.name[7] = "Pinky";
  channel_pos.name[8] = "Finger_Spread";

  // Set up the normalized time
  ros::Time counter = ros::Time::now();
  double normalized_time = 0;

  // Init the Random
  srand (time(NULL));

  // Tell ROS how fast to run this node. (100 = 100 Hz = 10 ms)
  ros::Rate rate(50);

  // Main loop.
  while (nh.ok())
  {
    // Only when toggled on (std empty message)
    if (running)
    {
      // Get a normalized time
      if ((ros::Time::now() - counter) > ros::Duration(sin_duration))
      {
        counter = ros::Time::now();
      }
      else
      {
        normalized_time = (ros::Time::now() - counter).toSec() / sin_duration;
      }

      // Publish channel positions in RAD.
      // Everything is 0 by default
      for (size_t channel = 0; channel < 9; ++channel)
      {

        double cur_pos = 0.0;

        channel_pos.position[channel] = cur_pos;
      }

      // Set the Spread to 0.5 (to avoid any collisions)
      channel_pos.position[8] = 0.5;
      // Calculate a halfe sin for the fingers
      double cur_pos = sin(normalized_time*3.14);
      // Set the 2 Test fingers to the sin value
      channel_pos.position[7] = cur_pos; //Pinky
      channel_pos.position[2] = cur_pos; //Index Distal
      channel_pos.position[3] = cur_pos; //Index proximal
      channel_pos.position[4] = cur_pos; //Middle Distal
      channel_pos.position[5] = cur_pos; //Middle proximal
      channel_pos.position[6] = cur_pos; //Ring Finger

      // Publish
      channel_pos_pub.publish(channel_pos);
    }
    rate.sleep();
    ros::spinOnce();

    // TO INDTRODUCE A VARIIING TIME RATE (in This case 50 - 100 HZ ) Uncomment this (discouraged! Will result in strange things obiously)
    // Was meant to test jitter in the trajectory generation
    //rate = 50+(rand() % 10 )*5;
  }

  return 0;
} // end main()
