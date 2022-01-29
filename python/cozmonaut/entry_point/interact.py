#
# Cozmonaut
# Copyright 2019 The Cozmonaut Contributors
#

import asyncio

import cv2

from cozmonaut.entry_point import EntryPoint


import core

class EntryPointInteract(EntryPoint):
    """
    An entry point for actual robot interaction.
    """

    def __init__(self):
        self.stop = False

    async def demo_video(self):
        """
        This coroutine grabs video frames. It's job is to go as fast as it can.
        When we start integrating Cozmos, we will have one coroutine per Cozmo
        robot, and we will not need to grab frames ourselves. Instead, the SDK
        will give us frames via its frame callback. Not a big change at all.
        """

        # Open the first video device
        cap = cv2.VideoCapture(0)

        while not self.stop:
            # Read a video frame
            ret, frame = cap.read()

            # Show the frame
            cv2.imshow('Output', frame)

            # Update window and stop on Q key down
            if cv2.waitKey(1) == ord('q'):
                self.stop = True

            # Yield control
            await asyncio.sleep(0)

    async def demo_faces(self):
        """
        This coroutine is designed to take its time handling faces. It will not
        bring the video coroutine above down in this demo, and it likewise will
        not bring Cozmo down in production.
        """

        while not self.stop:
            # Yield control
            await asyncio.sleep(0)

    def main(self) -> int:
        """
        The main method.
        """

        # Get event loop for this thread
        loop = asyncio.get_event_loop()

        # Call our demo coroutines and set them up for running on the loop
        future_demo_video = asyncio.ensure_future(self.demo_video(), loop=loop)
        future_demo_faces = asyncio.ensure_future(self.demo_faces(), loop=loop)

        # Bundle the coroutines together so we can treat them like one
        future_demo = asyncio.gather(future_demo_video, future_demo_faces)

        # Run the loop until the demo is done
        # This blocks on the main thread of the program
        loop.run_until_complete(future_demo)
        return 0
