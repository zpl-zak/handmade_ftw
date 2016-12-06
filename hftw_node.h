// (c) 2016 ZaKlaus; All Rights Reserved

#if !defined(HFTW_NODE_H)

doc(LinkedList)
doc_cat(Linked list)
doc_string(Linked list macro generator.)
doc_string(Supports arbitrary type.)
doc_example(LinkedList(StructName))
doc_ret(Generates Linked-List code for type StructName,
        ie. Node_StructName, NewNode_StructName, AddNode_StructName)
#define LinkedList(type) \
typedef struct _Node_##type { type Value; struct _Node_##type *Next; } Node_##type ; \
\
inline Node_##type *\
NewNode_##type (type Value){ \
    Node_##type *Item; \
    Item = (Node_##type *)PlatformMemAlloc(sizeof(Node_##type )); \
    Assert(Item); \
    Item->Value = Value; \
    Item->Next = 0; \
    return(Item); \
} \
\
inline Node_##type *\
AddNode_##type(Node_##type *List, type Value){ \
    Node_##type *NewNode = NewNode_##type (Value); \
    List->Next = NewNode; \
    return(NewNode); \
} \

#define HFTW_NODE_H
#endif