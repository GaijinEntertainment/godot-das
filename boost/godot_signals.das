require godot_native


struct Signal
    name : string
    owner : Object?

// backup method
def get_signal(obj : Object?; name : string)
    return [[Signal name = name, owner = obj]]

// Connect to native signal
// timer |> get_timeout() |> connect(obj) <| "on_timeout"
// Connect to user signal
// sig |> connect(obj) <| "on_timeout"
// Connect to any signal
// obj |> get_signal("pls add mutual require??") |> connect(obj) <| "on_timeout"
def connect(signal : Signal; peer : Object?; func_name : string)
    connect_signal_to_func_str(signal.owner, signal.name, peer, func_name)
// TODO make in macro connect with bound self

// Emit custom signal
// sig |> emit()
// Emit native signal
// timer |> get_timeout() |> emit()
def emit(signal : Signal)
    emit_signal(signal.owner, signal.name)
