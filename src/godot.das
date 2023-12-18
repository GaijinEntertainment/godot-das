require godot

class Node2D
    native: Node2DNative?

    def rotate(radians: float): void
        *native |> _Node2D_rotate(radians)

    def translate(offset: Vector2): void
        *native |> _Node2D_translate(offset)