from abc import ABCMeta, abstractmethod


class Object(metaclass=ABCMeta):

    @abstractmethod
    def update(self):
        pass

    @abstractmethod
    def draw(self):
        pass
