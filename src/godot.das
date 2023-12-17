require godot

class Node2D
    native: Node2DNative?

    def rotate(radians: float): void
        _Node2D_rotate(*native, radians)

    def translate(offset: Vector2): void
        _Node2D_translate(*native, offset)