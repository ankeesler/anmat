Object('heap.o', 'src/heap.c', CPPPATH=['inc', '.'], OBJPREFIX='build/')
Object('util.o', 'src/util.c', CPPPATH=['inc', '.'], OBJPREFIX='build/')
Object('heap-test.o', 'tst/heap-test.c', CPPPATH=['inc', '.'], OBJPREFIX='build/')
Program('build/heap_test',
        ['build/heap.o', 'build/util.o', 'build/heap-test.o'],
		CPPPATH=['inc', '.'],
		LIBS=['mcgoo'])