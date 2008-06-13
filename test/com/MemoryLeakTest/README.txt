The MemoryLeakTest application is intended to find regressions with
memory leaks in the AAF SDK.

A real-world sample file, exported from a popular editor, was found to
cause memory leaks in the AAF SDK using the simplest possible use of
the AAF SDK API, namely AAFFileOpenExistingRead() followed by calling
the Close and Release methods on the AAFFile object.  This only was
enough to cause a memory leak in AAF SDK v1.1.2 and up to at least CVS
snapshot 2008-05-12.

When future memory leaks are found, this test can be used to add the
simplest code which triggers the leak so it can be more readily isolated
and fixed.
