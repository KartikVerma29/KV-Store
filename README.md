# kvstore-cpp

A lightweight, embedded key-value store in C++17/20 — WAL-based durability, crash recovery, and reader-writer concurrency.

## Features

- Persistent storage via an append-only Write-Ahead Log
- Crash recovery — rebuilds the in-memory index by replaying the WAL on startup, safely discards any torn/mid-write entry
- Thread-safe concurrent reads/writes (`std::shared_mutex`)
- CLI REPL for interactive use
- Clean layered design — Facade, Decorator (logging), Factory, Strategy

## Build

```bash
mkdir build && cd build
cmake ..
make
```

## Usage

```bash
./demoClient
--- KV Store REPL ---
Commands: PUT <key> <value> | GET <key> | DEL <key> | EXIT
> PUT hello world
> GET hello
world
```

## Known Limitations

- WAL grows unbounded — no compaction yet (planned next)
- Single-node, embedded only — no network layer

## Design

See `docs/` for the full requirements and UML design reference.
