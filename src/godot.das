require godot

class Node2D
    native: Node2DNative?

    def rotate(radians: float): void
        *native |> _Node2D_rotate(radians)

    def translate(offset: Vector2): void
        *native |> _Node2D_translate(offset)

    def set_position(position: Vector2): void
        *native |> _Node2D_set_position(position)

    def get_position(): Vector2
        return *native |> _Node2D_get_position()
