Helgrind detected a race condition at line 10:
```
==3120562== Possible data race during write of size 8 at 0x10C018 by thread #4
==3120562== Locks held: none
==3120562==    at 0x109215: square (race.c:10)
==3120562==    by 0x485396A: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
==3120562==    by 0x4902AC2: start_thread (pthread_create.c:442)
==3120562==    by 0x4993A03: clone (clone.S:100)
==3120562== 
==3120562== This conflicts with a previous write of size 8 by thread #3
==3120562== Locks held: none
==3120562==    at 0x109215: square (race.c:10)
==3120562==    by 0x485396A: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
==3120562==    by 0x4902AC2: start_thread (pthread_create.c:442)
==3120562==    by 0x4993A03: clone (clone.S:100)
==3120562==  Address 0x10c018 is 0 bytes inside data symbol "accum"
```

This happens because the `square` function is called by multiple threads, and 2 of them attempt to
write to the same memory location (`accum`) at the same.

We can fix this by adding a mutex or a guard.