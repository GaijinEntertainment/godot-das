require godot_native

def iterate_children(node: Node?)
    return <- generator<Node?> () <|
        let child_count = node |> get_child_count()
        for i in range(0, child_count)
            yield node |> get_child(i)
        return false
