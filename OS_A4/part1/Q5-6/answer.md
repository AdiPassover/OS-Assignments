The problem with the code is at each iteration, memory is allocated for `string` but it's never 
freed, which is why the valgrind output shows a memory leak:
```
==3110416== LEAK SUMMARY:
==3110416==    definitely lost: 42 bytes in 3 blocks
==3110416==    indirectly lost: 0 bytes in 0 blocks
==3110416==      possibly lost: 0 bytes in 0 blocks
==3110416==    still reachable: 0 bytes in 0 blocks
==3110416==         suppressed: 0 bytes in 0 blocks
```

To fix this, we can deep copy `string` into `string_so_far` and then free the memory allocated 
for `string` at the end of each iteration.

Another problem is that the code checks (string_so_far != (char *)0) before string_so_far is
initialized which triggers:
```
==3110416== Conditional jump or move depends on uninitialised value(s)
==3110416==    at 0x109234: main (hello.c:15)
==3110416==  Uninitialised value was created by a stack allocation
==3110416==    at 0x1091C9: main (hello.c:8)
```

To fix this, we can initialize 'string_so_far' to NULL at the beginning of the function.