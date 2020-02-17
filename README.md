![MRTAlib](https://user-images.githubusercontent.com/23238394/74689584-01054900-51dc-11ea-8eb7-a45691b68297.png)


MRTAlib is a C++ library that solves the problem of Multi-Robot Task Allocation for heterogeneous swarms of robots in a simple yet computationally effective way, mainly in Search & Rescue scenarios. 

## Library Overview
>The main functionality is encapsulated under the *TaskManager* class, which provides different public methods to allow different modes of operation.
>
> - One can opt for the high-level public methods that provide ready to use solutions for tasks completion or simulation.
> ![2](https://user-images.githubusercontent.com/23238394/74690729-84289e00-51e0-11ea-9454-efaa506de976.png)
> 
> - Alternatively, mid-level public methods can be used. They provide unit operations and allow the user to alternate his own code and modify the global behavior of the Allocation.

![1](https://user-images.githubusercontent.com/23238394/74690722-7f63ea00-51e0-11ea-9bbb-c55dc03b8197.png)

>The main algorithm running underneath the library for the allocation is based on the so called Market Based Approach (MBA), but a mechanism is provided in order to customize how the allocation is performed from the *bidding Lists*.

### Note:
*The present work is part of a Bachelor thesis. Further information will be provided when the publication becomes available at University Carlos III de Madrid open media platform.*
