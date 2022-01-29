#
# Cozmonaut
# Copyright 2019 The Cozmonaut Contributors
#

from abc import ABC, abstractmethod


class EntryPoint(ABC):
    """
    An entry point for some operation.
    """

    @abstractmethod
    def main(self) -> int:
        """
        The main method.
        """
        raise NotImplementedError
