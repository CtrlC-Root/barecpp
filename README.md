# barecpp

[![Continuous Integration](https://github.com/CtrlC-Root/barecpp/actions/workflows/integration.yaml/badge.svg)](https://github.com/CtrlC-Root/barecpp/actions/workflows/integration.yaml)

BARE C++ Serialization Library

## Requirements

* C++ compiler w/ support for C++20
* Python 3 (to run `waf` build system)

## Quick Start

Update Git submodules:

```bash
git submodules update --recursive --init
```

Configure the build:

```bash
./waf configure
```

Build:

```bash
./waf build
```

Run tests:

```bash
./build/pkg/test/test
```

## References

* [BARE Message Encoding](https://baremessages.org/)
  * [RFC DRAFT](https://datatracker.ietf.org/doc/draft-devault-bare/)
* [Catch2](https://github.com/catchorg/Catch2)
* [cxxopts](https://github.com/jarro2783/cxxopts)
