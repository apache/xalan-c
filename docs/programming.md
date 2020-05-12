# Programming tips

## Introduction

This section was created to guide users on how to use some of the new
features going into the Xalan source code base.  Some of the features
discussed in this section are based on feedback and questions posted on
the xalan-c-users newsgroup.  This section will cover the benefits of
certain features and provide users with programming hints on how to
utilize the features in their applications.

## Pluggable Memory Management

Pluggable memory management was added as a new feature in Xalan-C++
Version 1.8.  This feature introduces an object called `MemoryManager`
which allows applications with stricter memory management requirements
to utilize a more efficient allocation method.  This `MemoryManager`
object can be applied to each processor instance, thus recovery
procedures from memory leaks or processor crashes will be applied to
the associated instance only.

The memory management model is similar to the memory management feature
provided by the Xerces-C++ XML Parser.

### How To Use This Feature

To apply memory management to your application, the `MemoryManager`
object needs to be specified in two stages:

* At initialization phase.  The purpose of specifying a `MemoryManager`
  object during initialization is to create a separate memory manager
  for the overall application.  Example of how this can be done is
  shown in the example below:

```c++
// Initialization step
static void XalanTransformer::initialize(MemoryManager* initMemoryManager=0);
```

* Creation of a transformer instance.  This creates a unique memory
  manager for the instance of the processor.  This step is optional.
  If no memory manager is provided, the global heap is used as the
  memory source.  Example of this is shown below:

```c++
// Create instance of XalanTransformer
MemoryManager      memMgrA;                // memory manager object
XalanTransformer   transformerA(&memMgrA);

MemoryManager      memMgrB;
XalanTransformer   transformerB(&memMgrB);
XalanTransformer   transformerC(&memMgrB);  // Uses same memory manager object as transformerB
XalanTransformer   transformerD;           // Uses default static memory manager
```

The above method demonstrates how users can apply the basic pluggable
memory management feature.  Users also have the option of implementing
their own memory manager.  This can be done by simply writing methods
for:

```c++
// Method for allocating memory
void* allocate(size_t size);
```

and

```c++
// Method for deallocating memory
void deallocate(void *p);
```

For an example of how to use this feature, please see the
[SimpleTransform](samples.md#simpletransform)
sample that has been provided in the binary distributions.

## More Topics

Please feel free to give us feedback on what topics you would like to see.

Send comments to the Xalan Development Mailing List.
