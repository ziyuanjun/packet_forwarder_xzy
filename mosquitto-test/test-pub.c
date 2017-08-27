/*
 * test-sub.c
 * Part of the mosquito-test demonstration application
 * Publishes a fixed number of simple messages to a topic 
 * Copyright (c)2016 Kevin Boone. Distributed under the terms of the 
 *  GPL v3.0 
 */
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>

// Server connection parameters
#define MQTT_HOSTNAME "192.168.21.102" 
#define MQTT_PORT 1883
#define MQTT_USERNAME "admin"
#define MQTT_PASSWORD "admin"
#define MQTT_TOPIC "test"

/*
 * Start here
 */
int main (int argc, char **argv)
  {
  struct mosquitto *mosq = NULL;
 
  // Initialize the Mosquitto library
  mosquitto_lib_init();

  // Create a new Mosquito runtime instance with a random client ID,
  //  and no application-specific callback data.  
  mosq = mosquitto_new (NULL, true, NULL);
  if (!mosq)
    {
    fprintf (stderr, "Can't initialize Mosquitto library\n");
    exit (-1);
    }

//  mosquitto_username_pw_set (mosq, MQTT_USERNAME, MQTT_PASSWORD);

  // Establish a connection to the MQTT server. Do not use a keep-alive ping
  //int ret = mosquitto_connect (mosq, MQTT_HOSTNAME, MQTT_PORT, 0);
  int ret = mosquitto_connect (mosq, argv[1], MQTT_PORT, 0);
  if (ret)
    {
    fprintf (stderr, "Can't connect to Mosquitto server\n");
    exit (-1);
    }

  int i;
  char text[20];
  for (i = 0; i < 10; i++)
    {
    sprintf (text, "Hello, World %d", i);
    // Publish the message to the topic
    ret = mosquitto_publish (mosq, NULL, MQTT_TOPIC, 
      strlen (text), text, 0, false);
    if (ret)
      {
      fprintf (stderr, "Can't publish to Mosquitto server\n");
      exit (-1);
      }
    }

  // We need a short delay here, to prevent the Mosquito library being
  //  torn down by the operating system before all the network operations
  //  are finished.
  sleep (1);

  // Tidy up
  mosquitto_disconnect (mosq);
  mosquitto_destroy (mosq);
  mosquitto_lib_cleanup();
  
  return 0;
  }

