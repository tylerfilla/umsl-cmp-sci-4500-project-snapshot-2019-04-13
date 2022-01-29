#
# Cozmonaut
# Copyright 2019 The Cozmonaut Contributors
#

from cozmonaut.entry_point import EntryPoint


class EntryPointFriendRemove(EntryPoint):
    """
    An entry point for removing friends.
    """

    def main(self) -> int:
        """
        The main method.
        """

        print('friend_remove')
        return 0
