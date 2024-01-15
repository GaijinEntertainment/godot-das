require godot_native

def get_children(node: Node?)
    let child_count = node |> get_child_count(false)
    var children: array<Node?>
    children |> reserve(child_count)
    for i in range(0, child_count)
        children |> push(node |> get_child(i, false))
    return <- children

def iterate_children(node: Node?)
    return <- generator<Node?> () <|
        let child_count = node |> get_child_count(false)
        for i in range(0, child_count)
            yield node |> get_child(i, false)
        return false
