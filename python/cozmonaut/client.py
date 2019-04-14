#
# Cozmonaut
# Copyright 2019 The Cozmonaut Contributors
#

from typing import Dict, Tuple

from core import Server


class Client:
    """The cozmonaut client interface."""

    def __init__(self, server: Server):
        self.server = server

    #
    # Entry Points
    #

    def entry_friend_list(self, friend: int) -> bool:
        """
        The entry point for friend detail listing.

        :param friend: The ID of the friend to list (optional)
        :return: True on success, else false
        """

        # TODO: Not implemented
        print(f'entry_friend_list({friend})')
        return True

    def entry_friend_remove(self, friend: int) -> bool:
        """
        The entry point for friend removal.

        :param friend: The ID of the friend to remove
        :return: True on success, else false
        """

        # TODO: Not implemented
        print(f'entry_friend_remove({friend})')
        return True

    def entry_interact(self) -> bool:
        """
        The entry point for actually running the robots interactively.

        :return: True on success, else false
        """

        # TODO: Not implemented
        print('entry_interact()')
        return True

    #
    # Camera Interface
    #

    def get_num_cameras(self) -> int:
        """Called by the engine to query the available number of cameras."""

        # TODO: Make this equal the number of Cozmos
        # Do we need to be able to update this while running?
        return 1

    #
    # Face Interface
    #

    def load_faces(self, faces: Dict[int, Tuple[float, ...]]):
        """
        Called by the core to load all known faces.

        :param faces: The faces dictionary
        """
        pass

    def on_face_show(self, camera: int, face: int, placeholder: bool, bounds: Tuple[int, int, int, int]):
        """
        Called when a face shows up on a camera for the first time.

        The face ID provided by this function may not reflect the ID assigned
        via the load_faces function above, as this function may be called before
        the face is fully recognized. In that case, the face is assigned a sane
        placeholder value, and the placeholder parameter is set to True.

        :param camera: The camera index
        :param face: The face ID
        :param placeholder: True if the face ID is a placeholder
        :param bounds: The bounding rectangle of the face (x, y, width, height)
        """

        # TODO: Not implemented
        pass

    def on_face_update(self, camera: int, old: int, new: int, placeholder: bool, bounds: Tuple[int, int, int, int]):
        """
        Called to issue an ID correction for a visible face.

        This is called whenever the face recognizer realizes its internal
        worldview does not match reality. This is routinely called between the
        phases of detection and recognition, for instance, when a person's
        identity is updated from a placeholder value to the real thing.

        :param camera: The camera index
        :param old: The old (out of date) face ID
        :param new: The new (updated) face ID
        :param placeholder: True if the new face ID is a placeholder
        :param bounds: The bounding rectangle of the face (x, y, width, height)
        """

        # TODO: Not implemented
        pass

    def on_face_hide(self, camera: int, face: int, placeholder: bool, bounds: Tuple[int, int, int, int]):
        """
        Called when a face is no longer visible on a camera.

        :param camera: The camera index
        :param face: The face ID
        :param placeholder: True if the face ID is a placeholder
        :param bounds: The last bounding rectangle of the face (x, y, width, height)
        """

        # TODO: Not implemented
        pass

    def on_face_move(self, camera: int, face: int, placeholder: bool, bounds: Tuple[int, int, int, int]):
        """
        Called when a face moves across the visual field of a camera.

        :param camera: The camera index
        :param face: The face ID
        :param placeholder: True if the face ID is a placeholder
        :param bounds: The bounding rectangle of the face (x, y, width, height)
        """

        # TODO: Not implemented
        pass
