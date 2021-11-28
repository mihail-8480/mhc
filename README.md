# libmhc

A C convenience library.

## Goals

### Full Async Support
- [x] Add cooperative multi-tasking.
- [ ] Add timers and semaphores.
- [ ] Add asynchronous IO.
- [ ] Make all the async code thread safe and use an N:M model for the cooperative multi-tasking.
- [ ] Write a web server using async IO.
- [ ] Full HTTP/2 support for the web server.
- [ ] Full HTTP/3 support for the web server.

### Other Concurrency Stuff
- [ ] Add Generator functions.
- [ ] Add Windows and FreeBSD support.
- [ ] Make the code more memory safe.

### Abstractions
- [ ] Implement interfaces.
- [ ] Make every API be more extensible by using interfaces.
- [ ] Make our own implementation of streams.
- [ ] Implement some common data types and algorithms.
- [ ] Implement proper error reporting.
- [ ] Implement unit testing and write tests for everything.

## Naming
All code must follow this naming convention: 

| Type | Naming |
| ---- | ------ |
| `struct` | `namespace_snake_case`
| `typedef struct,enum,union` | `namespace_snake_case_t`
| `typedef struct,enum,union *` | `CamelCase`
| `function` | `namespace_snake_case`
| `typedef function` | `namespace_snake_case_func`
| `typedef function *` | `FCamelCase`
| `constant` | `NAMESPACE_UPPER_CASE_UNDERSCORE`

## Versioning
* The version gets updated before a release.
* When a version number gets changed, the lower levels get set to `0`.

| Level | Update Condition |
| ----- | ---------------- |
| Major | Any changes or deletions to the public API that break source compatibility.
| Minor | Any additions to the public API.
| Patch | Any changes done that break binary compatibility (changes of typedefs, structs, private APIs, etc.).
| Tweak | Any minor changes that don't break binary compatibility.
