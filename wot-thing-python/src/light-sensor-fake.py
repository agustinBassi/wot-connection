#!/usr/bin/python
###############################################################################
# Author: Agustin Bassi
# Date: June 2020
# Licence: GPLV3
# Brief: TODO comment here
###############################################################################

#########[ Imports ]########################################################### 

from __future__ import division, print_function

import logging
import random
import time
import uuid

import tornado.ioloop
from webthing import (Action, Event, MultipleThings, Property, SingleThing,
                      Thing, Value, WebThingServer)

#########[ Settings & Data ]###################################################

#########[ Specific module code ]##############################################

class FakeSensor(Thing):
    """A sensor which updates its measurement every few seconds."""

    def __init__(self, id, description, data_to_measure, measures_tick):
        # Create the device
        Thing.__init__(
            self,
            'urn:dev:ops:' + id,
            description,
            ['MultiLevelSensor'],
            'A web connected sensor'
        )
        # create a variable that will be update in each measure
        self.level = Value(0.0)
        # add level property to the thing
        self.add_property(
            Property(
                self,
                'level',
                self.level,
                metadata = {
                    '@type': 'LevelProperty',
                    'title': data_to_measure,
                    'type': 'number',
                    'description': 'The current ' + data_to_measure + ' in %',
                    'minimum': 0,
                    'maximum': 100,
                    'unit': 'percent',
                    'readOnly': True,
                }))
        # create a timer that will call update_level periodically
        logging.debug('starting the sensor update looping task')
        self.timer = tornado.ioloop.PeriodicCallback(
            self.update_level, measures_tick)
        self.timer.start()

    def update_level(self):
        # emulate read from a real sensor
        new_measure = random.randint(0, 100)
        logging.debug('New value read from sensor is: %s', new_measure)
        self.level.notify_of_external_update(new_measure)

    def cancel_update_level_task(self):
        self.timer.stop()


#########[ Module main code ]##################################################

def run_server():
    # Sensor settings
    SENSOR_ID = "light-sensor-01"
    SENSOR_DESCRIPTION = "Garden Light Sensor"
    DATA_TO_MEASURE = "Light"
    MEASURES_TICK = 10000
    # Create a thing that represents a humidity sensor
    fake_sensor = FakeSensor(
        SENSOR_ID,
        SENSOR_DESCRIPTION,
        DATA_TO_MEASURE,
        MEASURES_TICK)
    # Add sensor to server
    server = WebThingServer(SingleThing(fake_sensor), port=8888)
    # Starts the server and stay in server loop
    try:
        logging.info('Starting the WebThing server')
        server.start()
    except KeyboardInterrupt:
        logging.debug('Canceling the sensor update looping task')
        fake_sensor.cancel_update_level_task()
        logging.info('Stopping the WebThing server')
        server.stop()
        logging.info('Exit!')

if __name__ == '__main__':
    logging.basicConfig(
        level=10,
        format="%(asctime)s %(filename)s:%(lineno)s %(levelname)s %(message)s"
    )
    run_server()

#########[ Enf of file ]#######################################################
