#ifndef DAS_TEMPLATES_H
#define DAS_TEMPLATES_H

// TODO more and better

const char * simple_template =R""""(
[export]
def _ready()
    print("It's ready!")

[export]
def _enter_tree()
    print("It's enter tree!")
)"""";


#endif // TEMPLATES_H
