require godot_core

def get_children(node: Node?)
    let child_count = node |> get_child_count()
    var children: array<Node?>
    children |> reserve(child_count)
    for i in range(0, child_count)
        children |> push(node |> get_child(i))
    return <- children

def iterate_children(node: Node?)
    return <- generator<Node?> () <|
        let child_count = node |> get_child_count()
        for i in range(0, child_count)
            yield node |> get_child(i)
        return false
