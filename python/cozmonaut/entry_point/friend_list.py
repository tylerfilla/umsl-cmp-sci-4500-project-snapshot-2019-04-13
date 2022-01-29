#
# Cozmonaut
# Copyright 2019 The Cozmonaut Contributors
#

from cozmonaut.entry_point import EntryPoint


class EntryPointFriendList(EntryPoint):
    """
    An entry point for listing friends.
    """

    def main(self) -> int:
        """
        The main method.
        """

        print('friend_list')
        return 0
