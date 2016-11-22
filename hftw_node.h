// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_NODE_H)

#define LinkedList(type) \
typedef struct Node_##type_ { type Value; struct Node_##type_ *Next; } Node_##type; \
\
inline Node_##type *\
NewNode_##type(type Value){ \
    Node_##type *Item; \
    Item = (Node_##type *)PlatformMemAlloc(sizeof(Node_##type)); \
    Assert(Item); \
    Item->Value = Value; \
    Item->Next = 0; \
    return(Item); \
} \
\
inline Node_##type *\
AddNode_##type(Node_##type *List, type Value){ \
    Node_##type *NewNode = NewNode_##type(Value); \
    List->Next = NewNode; \
    return(NewNode); \
} \

#define HFTW_NODE_H
#endif